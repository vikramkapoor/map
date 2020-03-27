// <Pipe.hpp> -*- C++ -*-


/**
 * \file   Pipe.hpp
 *
 * \brief  Defines the Pipe class
 *
 */

#ifndef __PIPE_EXP_HPP__
#define __PIPE_EXP_HPP__

#include <cinttypes>
#include <memory>
#include <functional>
#include "sparta/simulation/Clock.hpp"
#include "sparta/ports/Port.hpp"
#include "sparta/utils/SpartaAssert.hpp"
#include "sparta/utils/MathUtils.hpp"
#include "sparta/collection/IterableCollector.hpp"
#include "sparta/utils/ValidValue.hpp"

namespace sparta
{

/**
 * \class Pipe
 * \brief A very simple pipe, not part of the DES paradigm
 *
 * This is a very simple pipe that supports pipeline collection,
 * present-state/next-state behavior and an unsafe "look anywhere" in
 * the pipe read.
 *
 * The user is expected to maintain this Pipe and it's forward
 * progress.  After appends/deletes, the user is expected to perform
 * an update on the Pipe at specified time determined only by the
 * user. For collection and pipeline behaviors, update() should be
 * called every time the pipe being collected has changed
 * state. Assuming this happens, the collector will manage writing
 * transactions to the database.
 */
template <typename DataT>
class Pipe
{

private:

    struct PipeEntry
    {
        template<typename U>
        explicit PipeEntry(U&& data) : data(std::forward<U>(data)){}

        sparta::utils::ValidValue<DataT> data;
    };

    uint32_t getPhysicalStage_ (int32_t stage) const
    {
        sparta_assert (stage < static_cast<int32_t>(num_entries_));
        return (tail_ + stage) & stage_mask_;
    }

    void initPipe_(uint32_t num_entries)
    {
        sparta_assert(num_entries > 0, "ERROR: sparta::Pipe '" << name_
                    << "' Cannot be created with 0 stages");
        num_entries_ = num_entries;
        physical_size_ = sparta::utils::pow2 (sparta::utils::ceil_log2 (num_entries + 1));
        stage_mask_ = physical_size_ - 1;
        pipe_.reset(static_cast<PipeEntry *>(malloc(sizeof(PipeEntry) * physical_size_)));
    }

public:

    //! A typedef for this type of pipe
    typedef DataT value_type;

    //! Typedef for size_type
    typedef uint32_t size_type;

    template <bool is_const_iterator = true>
    class PipeIterator : public std::iterator<std::forward_iterator_tag, value_type>
    {
        typedef typename std::conditional<is_const_iterator,
                                          const value_type &,
                                          value_type &>::type DataReferenceType;

        typedef typename std::conditional<is_const_iterator,
                                          const Pipe<DataT> *,
                                          Pipe<DataT> *>::type PipePointerType;
        /// Get access on a non-const iterator
        DataReferenceType getAccess_(std::false_type) {
            return pipe_->access(index_);
        }

        /// Get access on a const iterator
        DataReferenceType getAccess_(std::true_type) {
            return pipe_->read(index_);
        }

    public:

        /**
         * \brief construct.
         * \param pipe  a pointer to the underlaying pipe.
         */
        explicit PipeIterator(PipePointerType pipe) : pipe_(pipe) {}

        /**
         * \brief construct.
         * \param pipe  a pointer to the underlaying pipe.
         * \param index index to the pipe container
         */
        PipeIterator(PipePointerType pipe, uint32_t index) : pipe_(pipe), index_(index) {}

        ///Default copy constructor
        PipeIterator(const PipeIterator &) = default;

        /// Default move constructor
        PipeIterator(PipeIterator &&)      = default;

        ///Override derefrence operator
        DataReferenceType operator*() {
            sparta_assert(index_ != uint32_t(-1));
            sparta_assert(pipe_->isValid(index_));
            return getAccess_(std::integral_constant<bool, is_const_iterator>());
        }


        ///support -> operator
        DataReferenceType operator->()
        {
            return operator*();
        }

        /// override Pre-increment operator
        PipeIterator & operator++() {
            if(++index_ > pipe_->capacity()) {
                index_ = pipe_->capacity();
            }
            return *this;
        }

        /// override post-increment operator
        PipeIterator operator++(int) {
            PipeIterator it(*this);
            this->operator++();
            return it;
        }

        /// Equals comparision operator
        bool operator==(const PipeIterator & it) const {
            return ((pipe_ == it.pipe_) && (index_ == it.index_));
        }

        /// Not Equals comparision operator
        bool operator!=(const PipeIterator & it) const {
            return !operator==(it);
        }
        /// Checks validity of iterator
        bool isValid() const {
            return pipe_->isValid(index_);
        }

    private:
        PipePointerType pipe_;
        uint32_t index_{0};
    };

    ///Typedef for regular iterator
    typedef PipeIterator<false> iterator;

    ///Typedef for constant iterator
    typedef PipeIterator<true> const_iterator;

    /// \brief STL-like begin operation,
    /// \return iterator to the oldest element in Pipe
    iterator begin(){ return iterator(this);}

    /// \brief STL-like begin operation,
    /// \return iterator to the latest element in Pipe
    iterator end()  { return iterator(this, num_entries_);}

    /// \brief STL-like begin operation,
    /// \return const_iterator to the oldest element in Pipe
    const_iterator begin() const { return const_iterator(this);}

    /// \brief STL-like begin operation,
    /// \return const_iterator to the latest element in Pipe
    const_iterator end()   const { return const_iterator(this, num_entries_);}

    //! No copies
    Pipe (const Pipe <DataT> &) = delete;

    //! No copies
    Pipe & operator= (const Pipe <DataT> &) = delete;

    /**
     * \brief Construct a pipe, 0-size not supported
     * \param name The name of the pipe
     * \param num_entries The number of entries this pipe can hold
     * \param clk The clock this pipe belongs to
     *
     */
    Pipe (const std::string & name,
          uint32_t num_entries,
          const Clock * clk) :
          name_(name),
          ev_update_(&es_, name+"_pipe_update_event",
                     CREATE_SPARTA_HANDLER(Pipe, internalUpdate_), 1)
    {
        initPipe_(num_entries);
        ev_update_.setScheduleableClock(clk);
        ev_update_.setScheduler(clk->getScheduler());
        ev_update_.setContinuing(false);
    }

    ~Pipe(){
        clear();
    }

    /**
     * \brief Resize the pipe immediately after construction
     * \param new_size New size for the pipe.  Must be greater than zero
     *
     * \note You cannot call this function after simulation
     *       finialization nor after enabling pipeline collection.
     */
    void resize(uint32_t new_size) {
        sparta_assert(collector_ == nullptr);
        //sparta_assert(!getClock()->isFinalized());
        initPipe_(new_size);
    }

    //! Tell the pipe to do its own updates.  Should be called once at
    //! the beginning of simulation.
    void performOwnUpdates() {
        if(!perform_own_updates_ && isAnyValid()) {
            ev_update_.schedule();
        }
        perform_own_updates_ = true;
    }

    //! What is the capacity of the pipe?  I.e. Entry count
    size_type capacity () const {
        return num_entries_;
    }

    //! How many entries are valid?  This number may change between
    //! update() calls
    size_type numValid () const {
        return num_valid_;
    }

    //! Resturns numValid -- useful for STL iteration
    size_type size() const {
        return numValid();
    }

    //! Return whether the pipe is empty
    bool empty() const {
        return numValid() == 0;
    }

    //! Append data to the beginning of the Pipe
    void append (const DataT & data)
    {
        appendImpl_(data);
    }

    //! Append data to the beginning of the Pipe
    void append (DataT && data)
    {
        return appendImpl_(std::move(data));
    }

    //! Append data to the beginning of the Pipe
    void push_front (const DataT & data)
    {
        append(data);
    }

    //! Append data to the beginning of the Pipe
    void push_front (DataT && data)
    {
        append(std::move(data));
    }

    /**
     *
     * \brief Append data to the specified stage.  Will clobber what's
     * there.
     *
     * \param stage The stage to write data to immediately
     * \param data  The data to write
     */
    void writePS (uint32_t stage, const DataT & data)
    {
        writePSImpl_(stage, data);
    }

    /**
     *
     * \brief Append data to the specified stage.  Will clobber what's
     * there.
     *
     * \param stage The stage to write data to immediately
     * \param data  The data to write
     */
    void writePS (uint32_t stage, DataT && data)
    {
        writePSImpl_(stage, std::move(data));
    }

    //! Invalidate the data at the given stage RIGHT NOW.  Will throw
    //! if there is no data at the given stage
    void invalidatePS (uint32_t stage)
    {
        sparta_assert(stage != uint32_t(-1), "Do not refer to stage -1 directly, use flushAppend()");
        sparta_assert(isValid(stage), "ERROR: In sparta::Pipe '" << name_
                      << "' invalidatePS at stage " << stage << " is not valid");
        pipe_[getPhysicalStage_(stage)].~PipeEntry();
        physical_stage_set_.erase(getPhysicalStage_(stage));
        --num_valid_;
        if(perform_own_updates_) {
            ev_update_.schedule();
        }
    }

    /**
     * \brief Clear the pipe
     */
    void clear() {
        std::for_each(physical_stage_set_.begin(), physical_stage_set_.end(), [this](const auto stage){
            pipe_[stage].~PipeEntry();
        });
        physical_stage_set_.clear();
        num_valid_ = 0;
    }

    //! Invalidate the data at the given stage RIGHT NOW.  Will throw
    //! if there is no data at the given stage
    void invalidateLastPS () {
        invalidatePS(num_entries_ - 1);
    }

    //! Flush the item at the given stage, even if not valid
    void flushPS (uint32_t stage)
    {
        sparta_assert(stage != uint32_t(-1), "Do not refer to stage -1 directly, use flushAppend()");
        if(isValid(stage)){
            pipe_[getPhysicalStage_(stage)].~PipeEntry();
            physical_stage_set_.erase(getPhysicalStage_(stage));
            --num_valid_;
        }
    }

    //! Flush the item that was appended
    void flushAppend()
    {
        pipe_[getPhysicalStage_(-1)].~PipeEntry();
        physical_stage_set_.erase(getPhysicalStage_(-1));
    }

    //! Flush everything, RIGHT NOW
    void flushAll ()
    {
        clear();
    }

    /**
    * \brief Flush any item that matches the given criteria
    * \param criteria The criteria to compare; must respond to operator==
    *
    * This function does a raw '==' comparison between the
    * criteria and the stashed items in the pipe. If matched, the
    * item is flushed, even if not valid.
    */
    void flushIf(const DataT& criteria){
        for(auto it = physical_stage_set_.begin(); it != physical_stage_set_.end(); ++it){
            PipeEntry & pe = pipe_[*it];
            if(pe.data.getValue() == criteria){
                pe.~PipeEntry();
                it = physical_stage_set_.erase(it);
                --num_valid_;
            }
        }
    }

    /**
    * \brief Flush any item that matches the given function
    * \param compare The function comparator to use
    *
    * This function allows a user to define his/her own
    * comparison operation outside of a direct operator==
    * comparison.  See sparta::PhasedPayloadEvent::cancelIf for an
    * example.
    */
    void flushIf(std::function<bool(const DataT&)> compare){
        for(auto it = physical_stage_set_.begin(); it != physical_stage_set_.end(); ++it){
            PipeEntry & pe = pipe_[*it];
            if(compare(pe.data.getValue())){
                pe.~PipeEntry();
                it = physical_stage_set_.erase(it);
                --num_valid_;
            }
        }
    }

    //! See if there is something at the given stage
    bool isValid (uint32_t stage) const
    {
        return physical_stage_set_.find(getPhysicalStage_(stage)) != physical_stage_set_.end();
    }

    //! Are any entries valid, including ones appended THIS cycle
    bool isAnyValid () const
    {
        return (num_valid_ > 0) || isValid(uint32_t(-1));
    }

    //! Is the last entry valid?
    bool isLastValid () const
    {
        return physical_stage_set_.find(getPhysicalStage_(num_entries_ - 1)) != physical_stage_set_.end();
    }

    //! Read the entry at the given stage
    const DataT & read (uint32_t stage) const
    {
        sparta_assert(isValid(stage), "ERROR: In sparta::Pipe '" << name_
                      << "' read at stage " << stage << " is not valid");
        const PipeEntry & pe = pipe_[getPhysicalStage_(stage)];
        return pe.data.getValue();
    }

    //! Read the entry at the given stage (non-const)
    DataT & access(uint32_t stage)
    {
        sparta_assert(isValid(stage), "ERROR: In sparta::Pipe '" << name_
                      << "' read at stage " << stage << " is not valid");
        PipeEntry & pe = pipe_[getPhysicalStage_(stage)];
        return pe.data.getValue();
    }

    //! Read the last entry
    const DataT & readLast() const
    {
        return read(num_entries_ - 1);
    }

    //! Update the pipe -- shift data appended/invalidated
    void update () {
        sparta_assert(perform_own_updates_ == false,
                      "HEY! You said you wanted the pipe to do it's own updates.  Liar.");
        internalUpdate_();
    }

    //! Move data from append stage (must be valid) into stage 0 (must be empty)
    //!\return true if data moved
    bool shiftAppend()
    {
        if(!isValid(-1) || isValid(0)){
            return false;
        }
        PipeEntry & pe_neg1 = pipe_[getPhysicalStage_(-1)];
        writePS(0, pe_neg1.data.getValue());
        pe_neg1.~PipeEntry();
        physical_stage_set_.erase(getPhysicalStage_(-1));
        return true;
    }

    /**
     * \brief Request that this queue begin collecting its
     *        contents for pipeline collection.
     * \param parent A pointer to the parent treenode for which to add
     *               Collectable objects under.
     * \note This only sets the Queue up for
     *       collection. collection must be started with an
     *       instatiation of the PipelineCollector
     */
    template<sparta::SchedulingPhase phase = SchedulingPhase::Collection>
    void enableCollection(TreeNode * parent) {
        collector_.reset (new collection::IterableCollector<Pipe<DataT>, phase, true>
                          (parent, name_, *this, capacity()));
    }

private:
    struct DeleteToFree_{
        void operator()(void * x){
            free(x);
        }
    };

    size_type num_entries_   = 0; //!< The number of entries in the pipe
    size_type physical_size_ = 0; //!< The physical size of the pipe
    size_type stage_mask_    = 0; //!< The stage mask
    size_type num_valid_     = 0; //!< Number of valid entries
    size_type tail_          = 0; //!< The tail of the pipe

    std::unique_ptr<PipeEntry[], DeleteToFree_> pipe_ = nullptr;

    const std::string name_;

    //////////////////////////////////////////////////////////////////////
    // Internal use only

    // Process appends, invalidates, etc, timed only
    sparta::EventSet es_{nullptr};
    UniqueEvent<SchedulingPhase::Update>   ev_update_;
    bool perform_own_updates_ = false;

    void internalUpdate_() {
        // Remove the head object
        if(isValid(num_entries_ - 1)){
            PipeEntry & pe_head = pipe_[getPhysicalStage_(num_entries_ - 1)];
            pe_head.~PipeEntry();
            physical_stage_set_.erase(getPhysicalStage_(num_entries_ - 1));
            --num_valid_;
        }

        // Add the tail object
        if(isValid(-1)){
            // Shift the pipe
            tail_ = getPhysicalStage_(-1);
            ++num_valid_;
        }

        if((num_valid_ > 0) && perform_own_updates_) {
            ev_update_.schedule();
        }
    }

    template<typename U>
    void appendImpl_ (U && data)
    {
        sparta_assert(isValid(-1) == false, "ERROR: sparta::Pipe '" << name_
                      << "' Double append of data before update");
        new (pipe_.get() + getPhysicalStage_(-1)) PipeEntry(std::forward<U>(data));
        physical_stage_set_.insert(getPhysicalStage_(-1));
        if(perform_own_updates_) {
            ev_update_.schedule();
        }
    }

    template<typename U>
    void writePSImpl_ (uint32_t stage, U && data)
    {
        if(isValid(stage)){
            pipe_[getPhysicalStage_(stage)].~PipeEntry();
            physical_stage_set_.erase(getPhysicalStage_(stage));
            --num_valid_;
        }
        new (pipe_.get() + getPhysicalStage_(stage)) PipeEntry(std::forward<U>(data));
        physical_stage_set_.insert(getPhysicalStage_(stage));
        ++num_valid_;
        if(perform_own_updates_) {
            ev_update_.schedule();
        }
    }

    //////////////////////////////////////////////////////////////////////
    // Collectors
    std::unique_ptr<collection::CollectableTreeNode> collector_;

    std::unordered_set<uint32_t> physical_stage_set_;

};

}

#endif

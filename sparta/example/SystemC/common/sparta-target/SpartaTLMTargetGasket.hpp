#pragma once

#include <cinttypes>
#include "sparta/simulation/Unit.hpp"
#include "sparta/simulation/TreeNode.hpp"
#include "sparta/simulation/ParameterSet.hpp"
#include "sparta/events/PayloadEvent.hpp"
#include "sparta/ports/DataPort.hpp"
#include "MemoryRequest.hpp"
#include "tlm.h"
#include "tlm_utils/peq_with_get.h"                   // Payload event queue FIFO
#include "memory.h"


namespace sparta_target
{

    class SpartaTLMTargetGasket : public sparta::Unit,
                                  public sc_core::sc_module,
                                  public tlm::tlm_fw_transport_if<>
    {
    protected:
       static int nextID;
    
    public:
        static constexpr char name[] = "mem_tlm_gasket";
        static constexpr char scName[2][20] = {"mem_tlm_gasket0", "mem_tlm_gasket1"};
        int calculateSpartaOffset(int sysc_offset)
        {
            //
            // This is a transaction coming from SysC that is on SysC's
            // clock, not Sparta's.  Need to find the same tick cycle on
            // the Sparta clock and align the time for the transaction.
            // Keep in mind that Sparta's scheduler starts on tick 1, not
            // 0 like SysC.
            //
            // For example,
            //   - The Sparta's clock is at 7 ticks (6 from SysC POV, hence the - 1)
            //   - The SysC clock is at 10 ticks
            //   - The transaction's delay is 1 tick (to be fired at tick 11)
            //
            //   sysc_clock - sparta_clock + delay = 4 cycles on sparta clock (11)
            //

            // Send to memory with the given delay - NS -> clock cycles.
            // The Clock is on the same freq as the memory block
            auto current_sc_time = sc_core::sc_time_stamp().value();
            const auto current_tick = getClock()->currentTick() - 1;
            sparta_assert(sc_core::sc_time_stamp().value() >= current_tick);
            const auto final_relative_tick =
                current_sc_time - current_tick + sysc_offset;
            return final_relative_tick;
        }
        class SpartaTLMTargetGasketParams : public sparta::ParameterSet
        {
        public:
            explicit SpartaTLMTargetGasketParams(sparta::TreeNode * n) :
                sparta::ParameterSet(n)
            {}
        };

        /**
         * \brief Construction of the Sparta TLM gasket
         *
         * \param node   The Sparta tree node
         * \param params The Gasket parameters
         * \param module_name The module name -- must be a variable that lives through construction.
         *
         * Explanation of the third parameter "module_name":
         *
         *   SystemC uses a global stack to determine the latest
         *   module (based on its name) being constructed for the
         *   tlm_fw_transport_if.  If this variable is a temporary, it
         *   will destruct BEFORE initializing the tlm_fw_transport_if
         *   and nullify the "current module being constructed."
         *
         *   You'll get this cryptic message:
         *
         *   "Error: (E122) sc_export specified outside of module:
         *    export 'tlm_base_target_socket_0' (sc_object)
         *
         *   To get around this nonsense, the module name (as a
         *   sc_core::sc_module_name) must live throughout the
         *   construction of the module.
         */
        SC_HAS_PROCESS(SpartaTLMTargetGasket);
        SpartaTLMTargetGasket(sparta::TreeNode * node,
                              const SpartaTLMTargetGasketParams * params,
                              sc_core::sc_module_name module_name = scName[nextID]) :
            Unit(node),
            sc_module(module_name),
            m_ID (nextID),                  
            m_target_memory(    
                nextID
                , sc_core::sc_time(50, sc_core::SC_NS)  // read response delay
                , sc_core::sc_time(30, sc_core::SC_NS) // write response delay)
                , 4*1024                                // memory size (bytes)
                , 4                                     // memory width (bytes)
                ),
            m_accept_delay(sc_core::sc_time(0, sc_core::SC_NS))
        {
            setAutoPrecedence(false);
            // This confusing call binds this TLM socket's
            // tlm_fw_transport_if API to this class for
            // nb_transport_fw calls.  The nb_transport_bw call
            // remains unset.
            nextID++;
            m_memory_socket(*this);

            // Register the callback for finished transactions coming
            // from the Sparta Memory model
            in_memory_response_.registerConsumerHandler
                (CREATE_SPARTA_HANDLER_WITH_DATA(SpartaTLMTargetGasket,
                                                 forwardMemoryResponse_, MemoryRequest));                     
        }

        // Unfortunately, this has to be made public for the SysC
        // binding to look "clean." Encapsulation?  Pffftthh... who
        // needs that?
        tlm::tlm_target_socket<>  m_memory_socket;

    void setTreeNode(sparta::TreeNode *treeNodePtr);

    private:
        const unsigned int        m_ID;                   ///< target ID
        memory m_target_memory;
        sc_core::sc_time m_accept_delay;
        // Nothing should call this function directly.  Should be done
        // through the tlm::tlm_fw_transport_if<> pointer
        tlm::tlm_sync_enum nb_transport_fw (tlm::tlm_generic_payload &gp,
                                            tlm::tlm_phase           &phase ,
                                            sc_core::sc_time         &delay_time ) override final;

        sparta::DataInPort<MemoryRequest>  in_memory_response_ {getPortSet(), "in_memory_response"};
        sparta::DataOutPort<MemoryRequest> out_memory_request_ {getPortSet(), "out_memory_request"};
        void send_end_request_(const MemoryRequest &);
        void forwardMemoryResponse_(const MemoryRequest &);
        unsigned long       m_request_count;        ///< used to calc synch transactions  
        bool                m_nb_trans_fw_prev_warning;
        bool                m_begin_resp_method_prev_warning;
        bool                m_trans_dbg_prev_warning;
        bool                m_get_dm_ptr_prev_warning;
        //tlm_utils::peq_with_get<tlm::tlm_generic_payload> m_response_PEQ;  ///< response payload event queue
       // tlm_utils::peq_with_get<tlm::tlm_generic_payload> m_end_request_PEQ;  ///< end request payload event queue
       // sc_core::sc_event   m_end_resp_rcvd_event;
              // An event to be scheduled in the sparta::SchedulingPhase::Tick
        // phase if data is received
        sparta::PayloadEvent<MemoryRequest, sparta::SchedulingPhase::Tick> event_end_req_{getEventSet(), "end_req_event", CREATE_SPARTA_HANDLER_WITH_DATA(SpartaTLMTargetGasket, send_end_request_, MemoryRequest)};
 
        // Junk not needed
        /// b_transport() - Blocking Transport
        void b_transport(tlm::tlm_generic_payload &payload, sc_core::sc_time &delay_time) override { }
        /// Not implemented for this example but required by interface
        bool get_direct_mem_ptr(tlm::tlm_generic_payload &payload, tlm::tlm_dmi &dmi_data) override { return false; }
        /// Not implemented for this example but required by interface
        unsigned int transport_dbg(tlm::tlm_generic_payload &payload) override { return 0; }


 };
}

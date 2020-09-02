
#include "SpartaTLMTargetGasket.hpp"
#include "MemoryRequest.hpp"

namespace sparta_target
{
    tlm::tlm_sync_enum SpartaTLMTargetGasket::nb_transport_fw (tlm::tlm_generic_payload &gp,
                                                               tlm::tlm_phase           &phase ,
                                                               sc_core::sc_time         &delay_time )
    {
        // Assume completed.  In a real system, the gasket could keep
        // track of credits in the downstream component and the
        // initiator of the request.  In that case, the gasket would
        // either queue the requests or deny the forward
        tlm::tlm_sync_enum return_status = tlm::TLM_COMPLETED;

  
//-----------------------------------------------------------------------------
// decode phase argument 
//-----------------------------------------------------------------------------
  switch (phase)
  {
//=============================================================================
    case tlm::BEGIN_REQ: 	
      { 
        // Convert the tlm GP to a sparta-based type.  If the modeler
        // chooses to use Sparta components to handle SysC data types,
        // the modeler could just pass the payload through as a
        // pointer on the DataOutPort.
        MemoryRequest request = {
            (gp.get_command() == tlm::TLM_READ_COMMAND ?
             MemoryRequest::Command::READ : MemoryRequest::Command::WRITE),
            gp.get_address(),
            gp.get_data_length(),

            // Always scary pointing to memory owned by someone else...
            gp.get_data_ptr(),
            (void*)&gp};

        if(SPARTA_EXPECT_FALSE(info_logger_)) {
            info_logger_ << " sending to memory model: " << request;
        }

        // Send to memory with the given delay - NS -> clock cycles.
        // The Clock is on the same freq as the memory block
        out_memory_request_.send(request, getClock()->getCycle(delay_time.value()));

        phase = tlm::END_REQ;                       // advance txn state to end request     
        return_status = tlm::TLM_UPDATED;           // force synchronization 
      break;
    } // end BEGIN_REQ

//=============================================================================
    case tlm::END_RESP:
    {
      //m_end_resp_rcvd_event.notify (sc_core::SC_ZERO_TIME);
      return_status = tlm::TLM_COMPLETED;         // indicate end of transaction     
      break;
      
    }
    
//=============================================================================
    case tlm::END_REQ:
    case tlm::BEGIN_RESP:
    { 
      return_status = tlm::TLM_ACCEPTED;
      break;
    }
   
//=============================================================================
    default:
    { 
      return_status = tlm::TLM_ACCEPTED; 
      break;
    }
  }
  
  return return_status;  


    }

    void SpartaTLMTargetGasket::forwardMemoryResponse_(const MemoryRequest & req)
    {
        // non-const lvalues
        tlm::tlm_phase resp    = tlm::BEGIN_RESP;
        sc_core::sc_time delay = sc_core::SC_ZERO_TIME;

        if(SPARTA_EXPECT_FALSE(info_logger_)) {
            info_logger_ << " sending back to transactor: " << req;
        }


        auto & gp = *((tlm::tlm_generic_payload*)req.meta_data);
        gp.set_response_status(tlm::TLM_OK_RESPONSE);

        // Send back the response to the initiator
        auto status = m_memory_socket->nb_transport_bw(*((tlm::tlm_generic_payload*)req.meta_data),
                                                       resp, delay);
        (void)status;
    }

}

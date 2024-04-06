// copy Cpu.cpp from ex_6 and add new content
#include "Cpu.h"
#include "IoModule.h"
#include "reporting.h"

using namespace sc_core;
using namespace tlm;

void Cpu::processor_thread(void) {

	while(true) {
		
		if(packetReceived_interrupt != true)
			wait(packetReceived_interrupt.value_changed_event());
		
		// read new packet descriptor
		const std::size_t size_descriptor = sizeof(m_packet_descriptor);
        // cout << payload.get_response_status() << endl;
		MEASURE_TRANSFER_TIME(
		do{
			startTransaction(TLM_READ_COMMAND, PROCESSOR_QUEUE_ADDRESS, 
			reinterpret_cast<unsigned char*> (&m_packet_descriptor), size_descriptor);

			wait(transactionFinished_event);

			cout << "Get a new descriptor";
		} while (payload.get_response_status() != TLM_OK_RESPONSE); // alternatively, payload.is_response_error() == false
        
		cout << "\t Successfully!" << endl;
        
		// Fetch the packet header from memory based on contained packet address in descriptor
        const std::size_t size_packet_header = sizeof(sc_time) + sizeof(uint64_t) + IpPacket::MINIMAL_IP_HEADER_LENGTH;
        
		do{
			startTransaction(TLM_READ_COMMAND, m_packet_descriptor.baseAddress, 
			reinterpret_cast<unsigned char*> (&m_packet_header), size_packet_header);

			wait(transactionFinished_event);

			cout << "Get the header based on descriptor.address";
		}while (payload.get_response_status() != TLM_OK_RESPONSE); // alternatively, payload.is_response_error() == false
		
		cout << "\t Successfully!" << endl;
        )
		// Process the packet header
        unsigned int PORT_ID;
		if(!verifyHeaderIntegrity(m_packet_header)){
            MEASURE_PROCESSING_TIME(
            wait(CPU_VERIFY_HEADER_CYCLES * CLK_CYCLE_CPU);
            )
            // discard the packet_descriptor	
			MEASURE_TRANSFER_TIME(
            do{
				startTransaction(TLM_WRITE_COMMAND, DISCARD_QUEUE_ADDRESS,
				reinterpret_cast<unsigned char*>(&m_packet_descriptor), size_descriptor);

				wait(transactionFinished_event);
				cout << "Discard the descriptor";
			}while (payload.get_response_status() != TLM_OK_RESPONSE); // alternatively, payload.is_response_error() == false
			cout << "\t Successfully!" << endl;
            )
		} else {
            MEASURE_PROCESSING_TIME(
            wait(CPU_VERIFY_HEADER_CYCLES * CLK_CYCLE_CPU);
            )
			// Find the output Port
            if(use_accelerator) {
                m_lookup_request.destAddress = m_packet_header.getDestAddress();
                m_lookup_request.processorId = m_id;
                MEASURE_TRANSFER_TIME(
                do{
                    startTransaction(TLM_WRITE_COMMAND, ACCELERATOR_ADDRESS, reinterpret_cast<unsigned char*> (&m_lookup_request), sizeof(m_lookup_request));
                    wait(transactionFinished_event);
                    cout << "Write m_loopup_request to accelerator";
                } while(payload.get_response_status() != TLM_OK_RESPONSE);
                cout << "\t successfully" << endl;
                )
                MEASURE_PROCESSING_TIME(
                if(lookupReady_interrupt != true)
                    wait(lookupReady_interrupt.value_changed_event());
                )
                MEASURE_TRANSFER_TIME(
                do{
                    startTransaction(TLM_READ_COMMAND, ACCELERATOR_ADDRESS, reinterpret_cast<unsigned char*> (&PORT_ID), sizeof(unsigned int));
                    wait(transactionFinished_event);
                    cout << "Read result from the accelerator";
                } while(payload.get_response_status() != TLM_OK_RESPONSE);
                cout << "\t successfully" << endl;
                )
                
            } else {
                PORT_ID = makeNHLookup(m_packet_header);
                wait(CPU_IP_LOOKUP_CYCLES * CLK_CYCLE_CPU);
            }
			MEASURE_PROCESSING_TIME(
			unsigned int TTL = decrementTTL(m_packet_header);
            wait(CPU_DECREMENT_TTL_CYCLES * CLK_CYCLE_CPU);
            )
            if(TTL == 0){
				// discard
				MEASURE_TRANSFER_TIME(
				do{
					startTransaction(TLM_WRITE_COMMAND, DISCARD_QUEUE_ADDRESS,
					reinterpret_cast<unsigned char*>(&m_packet_descriptor), size_descriptor);

					wait(transactionFinished_event);
					cout << "Discard the descriptor";
				}while (payload.get_response_status() != TLM_OK_RESPONSE); // alternatively, payload.is_response_error() == false
				cout << "\t Successfully!" << endl;
                )
            }
            MEASURE_PROCESSING_TIME(
			updateChecksum(m_packet_header);
            wait(CPU_UPDATE_CHECKSUM_CYCLES * CLK_CYCLE_CPU);
            )
            // Write back the update to memory
			MEASURE_TRANSFER_TIME(
			do{
				startTransaction(TLM_WRITE_COMMAND, m_packet_descriptor.baseAddress,
				reinterpret_cast<unsigned char*>(&m_packet_header), size_packet_header);

				wait(transactionFinished_event);
				cout << "Update the header in mem";
			}while (payload.get_response_status() != TLM_OK_RESPONSE); // alternatively, payload.is_response_error() == false
			cout << "\t Successfully!" << endl;
            
			// write into the output port
			if(PORT_ID == 0){
				
				do{
					startTransaction(TLM_WRITE_COMMAND, OUTPUT_0_ADDRESS,
					reinterpret_cast<unsigned char*>(&m_packet_descriptor), size_descriptor);

					wait(transactionFinished_event);
					cout << "Write descriptor to output Port0";
				}while (payload.get_response_status() != TLM_OK_RESPONSE); // alternatively, payload.is_response_error() == false
				cout << "\t Successfully!" << endl;
			} else if(PORT_ID == 1){
				
				do{
					startTransaction(TLM_WRITE_COMMAND, OUTPUT_1_ADDRESS,
					reinterpret_cast<unsigned char*>(&m_packet_descriptor), size_descriptor);

					wait(transactionFinished_event);
					cout << "Write descriptor to output Port1";
				}while (payload.get_response_status() != TLM_OK_RESPONSE); // alternatively, payload.is_response_error() == false
				cout << "\t Successfully!" << endl;
			} else if(PORT_ID == 2){
				
				do{
					startTransaction(TLM_WRITE_COMMAND, OUTPUT_2_ADDRESS,
					reinterpret_cast<unsigned char*>(&m_packet_descriptor), size_descriptor);

					wait(transactionFinished_event);
					cout << "Write descriptor to output Port2";
				}while (payload.get_response_status() != TLM_OK_RESPONSE); // alternatively, payload.is_response_error() == false
				cout << "\t Successfully!" << endl;
			} else if(PORT_ID == 3){
				
				do{
					startTransaction(TLM_WRITE_COMMAND, OUTPUT_3_ADDRESS,
					reinterpret_cast<unsigned char*>(&m_packet_descriptor), size_descriptor);

					wait(transactionFinished_event);
					cout << "Write descriptor to output Port3";
				}while (payload.get_response_status() != TLM_OK_RESPONSE); // alternatively, payload.is_response_error() == false
				cout << "\t Successfully!" << endl;
			} else {
				cout << "Error: PORT_ID Invalid" << endl;
			}
            )
		}
	}
	output_load();
}

// ############# COMPLETE THE FOLLOWING SECTION ############# //
// startTransaction
	void Cpu::startTransaction(tlm_command command, soc_address_t address,
	unsigned char* data, unsigned int size) {
		cout << std::setw(9) << sc_time_stamp() << ": " << name()
			<< "'\t transaction starts"
			<< endl;
		payload.set_address(address);
		payload.set_data_length(size);
		payload.set_data_ptr(data);
		payload.set_command(command);
		payload.set_response_status(TLM_GENERIC_ERROR_RESPONSE);
		tlm_phase phase;
		sc_time delay;
		tlm_sync_enum tlm_resp;
		delay = SC_ZERO_TIME;
		phase = BEGIN_REQ;
		tlm_resp = initiator_socket -> nb_transport_fw(payload, phase, delay);
		if(tlm_resp != TLM_UPDATED || phase != END_REQ) {				
			cout << std::setw(12) << sc_time_stamp() << ":"
				<< "Protocol Error in fw"
				<< endl;
		}
		wait(delay);
	}
// ####################### UP TO HERE ####################### //

// nb_transport_bw: implementation of the backward path callback
tlm_sync_enum Cpu::nb_transport_bw(tlm_generic_payload& transaction,
		tlm_phase& phase, sc_time& delay_time) {
	if(phase != BEGIN_RESP) {
		cout << std::setw(12) << sc_time_stamp() << ": "
			<< "Protocol Error"
			<< endl;
	}
	delay_time = CLK_CYCLE_BUS;
	wait(delay_time);
	transactionFinished_event.notify();
	phase = END_RESP;
	return TLM_COMPLETED;
}

unsigned int Cpu::instances = 0;


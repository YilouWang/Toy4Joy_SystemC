#include "consumer.h"
#include "producer.h"
#include "fifo_2.h"
#include "systemc.h"

int sc_main(int argc, char *argv[]) {

	// ############# COMPLETE THE FOLLOWING SECTION ############# //
	// instantiate and connect the modules
	fifo_2 ff("fifo_2", 20);
	consumer c("consumer");
	producer p("producer");

	p.prod2fifo_port(ff);
	c.consum2fifo_port(ff);
	// ####################### UP TO HERE ####################### //

	// simulation duration may be modified by command line parameter
	sc_time sim_dur = sc_time(5000, SC_NS);
	if(argc != 2) {
		cout << "Default simulation time = " << sim_dur << endl;
	}
	else {
		sim_dur = sc_time(atoi(argv[1]), SC_NS);
		cout << "Simulation time = " << sim_dur << endl;
	}

	// start simulation
	sc_start(sim_dur);

	return (0);
}

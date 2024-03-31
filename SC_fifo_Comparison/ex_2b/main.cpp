#include "consumer.h"
#include "producer.h"
#include "fifo_1b.h"

int sc_main(int argc, char *argv[]) {
	// the following instruction generates a clock signal with clock named
	// "clock" with a period of 100 ns, a duty cycle of 50%, and a falling edge
	// after 50 ns
	sc_clock clk("clock", 100, SC_NS, 0.5, 50, SC_NS, false);

	// fill in the required commands to instantiate and connect producer, fifo,
	// and consumer

	sc_signal<bool> wr_en, rd_en, full, valid;
	sc_signal<unsigned char> d_in, d_out;
	fifo_1b f("fifo_1b", 5);
	producer p("producer");
	consumer c("consumer");

	c.clk(clk);
	c.valid(valid);
	c.d_in(d_out);
	c.rd_en(rd_en);

	f.wr_en(wr_en);
	f.rd_en(rd_en);
	f.full(full);
	f.valid(valid);
	f.d_in(d_in);
	f.d_out(d_out);
	f.clk(clk);

	p.clk(clk);
	p.d_out(d_in);
	p.full(full);
	p.wr_en(wr_en);

	// fill in code to generate traces that can be used to observe the
	// functionality of the model with the waveform viewer gtkwave

	sc_trace_file *tf = sc_create_vcd_trace_file("main");
	sc_trace(tf, clk, "clk");
	sc_trace(tf, valid, "valid");
	// sc_trace(tf, c.fetch, "consumer_fetch");
	sc_trace(tf, rd_en, "rd_en");
	sc_trace(tf, wr_en, "wr_en");
	sc_trace(tf, d_out, "consumer_d_in");


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

	sc_close_vcd_trace_file(tf);

	return 0;
}

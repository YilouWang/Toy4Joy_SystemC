#include "stimulus.h"
#include "counter.h"
#include "bcd_decoder.h"

int sc_main(int argc, char *argv[]) {

	sc_signal<bool> clock, reset_n;
	sc_signal<unsigned short int> count_val;
	sc_signal<char> v_hi, v_lo;

// 10 ns period clock cycle
	sc_set_time_resolution(1, SC_NS);

	bcd_decoder de("bcd_decoder");
	counter cn("counter");
	stimulus sti("stimulus");

	sti.clk(clock);
	sti.rst_n(reset_n);

	de.val(count_val);
	de.hi(v_hi);
	de.lo(v_lo);

	cn.rst_n(reset_n);
	cn.clk(clock);
	cn.cnt(count_val);

	sc_trace_file *tf = sc_create_vcd_trace_file("main");
	sc_trace(tf, count_val, "cnt");
	sc_trace(tf, sti.clk, "clock");
	sc_trace(tf, reset_n, "rst");
	sc_trace(tf, v_hi, "high");
	sc_trace(tf, v_lo, "low");

	int n_cycles;
	if(argc != 2) {
		cout << "default n_cycles = 2000" << endl;
		n_cycles = 2000;
	}
	else {
		n_cycles = atoi(argv[1]);
		cout << "n_cycles = " << n_cycles << endl;
	}

	sc_start(n_cycles, SC_NS);

	sc_close_vcd_trace_file(tf);

	return 0;
}

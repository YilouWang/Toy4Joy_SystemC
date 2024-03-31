#ifndef __FIFO_2
#define __FIFO_2

#include "systemc.h"
#include "fifo_if.h"

class fifo_2: public fifo_if, public sc_module {
private:
	const unsigned int fifo_size;
	unsigned char *fifo_data;
	unsigned int wr_ptr, rd_ptr;
	unsigned int fill_level;

	// declaration of helper function
	void output_fifo_status();

public:
	// declaration of the interface functions
	bool read_fifo(unsigned char *data, unsigned int &count);
	bool write_fifo(unsigned char *data, unsigned int &count);

	// constructor declaration
	SC_HAS_PROCESS(fifo_2);
	fifo_2(sc_module_name name, unsigned int fifo_size);

};

#endif // __FIFO_2

// A hierachical channel is a class derived from interface and sc_module.
// It has all properties of a module.(may contain sub-modules, ports, processes, signals...)
// It has to implement the interface functions
// A module implements interface functions is actually a hierachical channel.
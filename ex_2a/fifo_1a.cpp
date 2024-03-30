#include <iomanip>
#include "fifo_1a.h"

fifo_1a::fifo_1a(sc_module_name name, unsigned int fifo_size) : fifo_size(fifo_size) {
	// register processes

	SC_THREAD(write_fifo);
	sensitive << clk.pos();

	SC_THREAD(read_fifo);
	sensitive << clk.pos();

	SC_METHOD(set_flags);
	sensitive << rd_ptr << wr_ptr;

	// initialization of member variables/ports
	fifo_data = new unsigned char[fifo_size];
	rd_ptr.write(0);
	wr_ptr.write(0);
	fill_level = 0;

	// initialize output ports
	// full.write(false) is incorrect, which will cause the "port can not be bound" error!!
	full.initialize(false);
	empty.initialize(true);
	d_out.initialize(-1);
}

void fifo_1a::write_fifo() {
	while (true)
	{
		wait();
		unsigned int wr_ptr_tmp = wr_ptr.read();
		if(wr_en.read() == true && full.read() == false){
			fifo_data[wr_ptr_tmp] = d_in.read();
			wr_ptr_tmp += 1;
			wr_ptr_tmp %= fifo_size;
			wr_ptr.write(wr_ptr_tmp);
			fill_level += 1;
		}
	}
	
}

void fifo_1a::read_fifo() {
	while (true)
	{
		wait();
		unsigned int rd_ptr_tmp = rd_ptr.read();
		if(rd_en.read() == true && empty.read() == false){
			d_out.write(fifo_data[rd_ptr_tmp]);
			rd_ptr_tmp += 1;
			rd_ptr_tmp %= fifo_size;
			rd_ptr.write(rd_ptr_tmp);
			fill_level -= 1;
		}
	}
	
}

void fifo_1a::set_flags() {
	if(fill_level == fifo_size){
		full.write(true);
		empty.write(false);
	} else if(fill_level == 0){
		empty.write(true);
		full.write(false);
	} else{
		full.write(false);
		empty.write(false);
	}
}

#include <iomanip>
#include "fifo_1b.h"

fifo_1b::fifo_1b(sc_module_name name, unsigned int fifo_size) : fifo_size(fifo_size) {
	// register process
	SC_THREAD(read_write_fifo);
	sensitive << clk.pos();

	// initialization of member variables/ports
	fifo_data = new unsigned char[fifo_size];
	rd_ptr.write(0);
	wr_ptr.write(0);
	fill_level = 0;

	full.initialize(false);
	valid.initialize(false);
	d_out.initialize(-1);
}

void fifo_1b::read_write_fifo() {
	while (true)
	{
		wait();
		if(wr_en.read() == true && full.read() == false){
			if(valid.read() == false) {
				d_out.write(d_in.read());
				valid.write(true);
			} else {
				unsigned int wr_ptr_tmp = wr_ptr.read();
				fifo_data[wr_ptr_tmp] = d_in.read();
				wr_ptr_tmp += 1;
				wr_ptr_tmp %= fifo_size;
				wr_ptr.write(wr_ptr_tmp);
				fill_level += 1;
			}
		}
		if(rd_en.read() == true && valid.read() == true){
			if(fill_level != 0){
				unsigned int rd_ptr_tmp = rd_ptr.read();
				d_out.write(fifo_data[rd_ptr_tmp]);
				rd_ptr_tmp += 1;
				rd_ptr_tmp %= fifo_size;
				rd_ptr.write(rd_ptr_tmp);
				fill_level -= 1;
			} else{
				valid.write(false);
			}
		}
		if(fill_level == fifo_size){
			full.write(true);
		}
	}
	


}

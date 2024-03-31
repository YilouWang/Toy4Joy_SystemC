#include <iostream>
#include <iomanip>
#include "fifo_2.h"

fifo_2::fifo_2(sc_module_name name, unsigned int fifo_size) : fifo_size(fifo_size) {
	// initialization of member variables/ports
	fifo_data = new unsigned char[fifo_size];
	wr_ptr = 0;
	rd_ptr = 0;
	fill_level = 0;
}

bool fifo_2::write_fifo(unsigned char *data, unsigned int &count) {
	bool result = false;

	unsigned int len; // amount of data written
	unsigned char *ptr; // pointer where to put data
	ptr = data;
	sc_time delay;

	if(fill_level + (int)count > fifo_size) // not enough space for all data
		len = fifo_size - fill_level; 		// none or less data will be written
	else {
		len = count;
		result = true;
	}

	if((wr_ptr + len) <= fifo_size) {
		memcpy(&fifo_data[wr_ptr], ptr, len);
		wr_ptr += len;
		wr_ptr %= fifo_size;
	} else {
		unsigned int len1 = fifo_size - wr_ptr;
		memcpy(&fifo_data[wr_ptr], ptr, len1);
		unsigned int len2 = len - len1;
		memcpy(&fifo_data[0], &ptr[len1], len2);
		wr_ptr = len2;
	}
	fill_level += len;
	count = len;
	wait(delay);
	
	//if(fifo_size <= 50)
		//output_fifo_status();
	return result;
}

bool fifo_2::read_fifo(unsigned char *data, unsigned int &count) {
	bool result = false;

	unsigned int len;	// amount of data read
	unsigned char *ptr;	// pointer where to put data
	ptr = data;
	sc_time delay;

	if(fill_level < count)	// not enough data to read
		len = fill_level;	// none or less data will be read
	else {
		len = count;
		result = true;
	}

	if(len + rd_ptr <= fifo_size){
		memcpy(ptr, &fifo_data[rd_ptr], len);
		rd_ptr += len;
		rd_ptr %= fifo_size;
	}else {
		unsigned int len1 = fifo_size - rd_ptr;
		memcpy(ptr, &fifo_data[rd_ptr], len1);
		unsigned int len2 = len - len1;
		memcpy(&ptr[len1], &fifo_data[0], len2);
		rd_ptr = len2;
	}
	fill_level -= len;
	count = len;
	wait(delay);

	//if(fifo_size <= 50)
	    //output_fifo_status();
	return result;
}

// helper function to output content of FIFO
void fifo_2::output_fifo_status() {
    cout << "\tCurrent status of '" << name() << "': write address: "
			<< wr_ptr << ", read address: " << rd_ptr
			<< ", fill level: " << fill_level << endl;
	cout << "\t";
	cout << hex; // switch to hexadecimal mode;
    if(fill_level == 0) {
		for(unsigned int i = 0; i < fifo_size; i++)
			cout << "-- ";
	}
	else if(fill_level == fifo_size) {
		for(unsigned int i = 0; i < fifo_size; i++)
			cout << std::setw(2) << std::setfill('0') << (int)*(fifo_data + i)
					<< " ";
	}
	else if(wr_ptr > rd_ptr) {
		for(unsigned int i = 0; i < fifo_size; i++) {
			if((i >= rd_ptr) && (i < wr_ptr)) {
				cout << std::setw(2) << std::setfill('0')
						<< (int)*(fifo_data + i) << " ";
			}
			else
				cout << "-- ";
		}
	}
	else if(wr_ptr < rd_ptr) {
		for(unsigned int i = 0; i < fifo_size; i++) {
			if((i >= rd_ptr) || (i < wr_ptr)) {
				cout << std::setw(2) << std::setfill('0')
						<< (int)*(fifo_data + i) << " ";
			}
			else
				cout << "-- ";
		}
	}
	cout << dec << std::setfill(' ') << endl << endl;
}

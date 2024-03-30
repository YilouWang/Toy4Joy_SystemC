#include <iomanip>
// iomanip is a lib for format output, like std::setw(), setting width
#include "data_gen.h"

// modeling a network data dispatching, used for testing the performance of protocol
void data_gen::generate() {

	unsigned short int iat;
	short unsigned int bytes;

	iat = rand() % 10;

	dat.write(0);

	while(true) {
		wait(iat, SC_US);
		bytes = (64 + rand() % 1436);
		iat = (bytes + rand() % (6045 - bytes)) / 155;

		dat.write(bytes);
		cout << std::setw(9) << sc_time_stamp() << ": '" << name()
				<< "'\tnumber of bytes: " << std::setw(4) << bytes << endl;
	}
}

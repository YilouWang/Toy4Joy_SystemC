#include "bcd_decoder.h"
#include <iomanip>
 
void bcd_decoder::decode() {

		unsigned short int data_in = val.read();
		char high = data_in / 10;
		char low = data_in % 10;
		hi.write(high);
		lo.write(low);
		cout << std::setw(8) << sc_time_stamp() << ": '" << name()
			<< "'\tcurrent counter is: " << data_in
			<< ", high: " << static_cast<int>(high)
			<< ", low: " << static_cast<int>(low) << endl;
}

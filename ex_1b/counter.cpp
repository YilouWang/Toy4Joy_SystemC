#include "counter.h"
#include <iomanip>
void counter::count() {

	while (true){
		wait();
		if(!rst_n.read())
			cnt_int = 0;
		else
			cnt_int += 1;
		cnt_int = cnt_int % 100;
		cnt.write(cnt_int);
		// cout << std::setw(8) << sc_time_stamp() << ": '" << name()
		//		<< "'\tcurrent counter is: " << std::setw(8) << cnt_int << endl;
	}
}

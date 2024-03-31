#include <iomanip>
#include "policer.h"

// token bucket is an algorithm for traffice shaping and traffic policing
void policer::policer_proc() {

	unsigned short int d_amount;
	int dec;

	sc_time t;
	t = sc_time_stamp() - last_arrival_time;
// dec is the leaking token during this period
	dec = t.value() * r_pol;

	last_arrival_time = sc_time_stamp();
// check how many tokens are left in bucket
	counter = ((counter - dec) < 0) ? 0 : counter - dec;

	cout << std::setw(9) << last_arrival_time << ": '" << name()
			<< "'\tupdated counter: " << std::setw(4) << counter;
// check whether the data coming exceeds the size of bucket
	d_amount = size.read();

	if(counter + d_amount > max) {
		discard.write(true);
		cout << "\tnew counter: " << std::setw(4) << counter << "\tdiscard: "
				<< true << endl;
	}
	else {
		counter += d_amount;
		discard.write(false);
		cout << "\tnew counter: " << std::setw(4) << counter << "\tdiscard: "
				<< false << endl;
	}
}

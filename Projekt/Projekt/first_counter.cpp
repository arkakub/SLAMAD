#include "systemc.h"

SC_MODULE(first_counter) {
	sc_in_clk clock;
	sc_in<bool> reset;
	sc_in<bool> enable;
	sc_out<sc_uint<5> > hours_out;
	sc_out<sc_uint<6> > minutes_out;

	sc_uint<5> hours = 0;
	sc_uint<6> minutes = 0;

	void incr_count() {

		if ((reset.read() == 1) || (hours >= 23)) {
			hours = 0;
			minutes = 0;
		}
		else if (enable.read() == 1) {
			
			if (minutes < 59) {
				minutes++;
			} else {
				minutes = 0;
				hours++;
			}
		}
		hours_out.write(hours);
		minutes_out.write(minutes);
	}

	  // Constructor for the counter
	  // We trigger the below block with respect to positive
	  // edge of the clock and also when ever reset changes state
	SC_CTOR(first_counter) {
		SC_METHOD(incr_count);
		sensitive << reset;
		sensitive << clock.pos(); //CLK positive edge
	}
};
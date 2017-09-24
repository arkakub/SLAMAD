#include "systemc.h"

SC_MODULE(Clock_Module) {
	sc_in_clk clock;

	sc_out<sc_uint<5> > actual_hours_out;
	sc_out<sc_uint<6> > actual_minutes_out;

	sc_uint<5> actual_hours = 0;
	sc_uint<6> actual_minutes = 0;

	void incr_count() {
		
		if (actual_minutes < 59) {
			actual_minutes++;
		} else {
			actual_minutes = 0;
			actual_hours++;
			if (actual_hours > 23) {
				actual_hours = 0;
				actual_minutes = 0;
			}
		}
		
		actual_hours_out.write(actual_hours);
		actual_minutes_out.write(actual_minutes);
	}

	SC_CTOR(Clock_Module) {
		SC_METHOD(incr_count);
		sensitive << clock.pos();
	}
};
#include "systemc.h"

SC_MODULE(Electrical_Outlet_Module) {

	sc_in_clk clock;
	sc_in<sc_uint<5> > actual_hours;
	sc_in<sc_uint<6> > actual_minutes;

	sc_out<bool> electrical_outlet_state;

	sc_uint<5> start_hour = 12;
	sc_uint<6> start_minute = 30;

	sc_uint<5> end_hour = 14;
	sc_uint<6> end_minute = 35;

	void prepareOutletState() {
		if ( (actual_hours == start_hour) && (actual_minutes == start_minute) ) {
			electrical_outlet_state = 1;
		} else if ((actual_hours == end_hour) && (actual_minutes == end_minute)) {
			electrical_outlet_state = 0;
		}
	}

	SC_CTOR(Electrical_Outlet_Module) {
		SC_METHOD(prepareOutletState);
		sensitive << clock.neg();
	}
};
#include "systemc.h"

SC_MODULE(Electrical_Outlet_Module) {

	sc_in_clk clock;
	sc_in<sc_uint<5> > actual_hours;
	sc_in<sc_uint<6> > actual_minutes;

	sc_out<bool> electrical_outlet_state;

	sc_uint<5> event_hour = 12;
	sc_uint<6> event_minute = 30;	

	void prepareOutletState() {
		if ( (actual_hours == event_hour) && (actual_minutes == event_minute) ) {
			electrical_outlet_state = 1;
		} else {
			electrical_outlet_state = 0;
		}
	}

	SC_CTOR(Electrical_Outlet_Module) {
		SC_METHOD(prepareOutletState);
		sensitive << clock.neg();
	}
};
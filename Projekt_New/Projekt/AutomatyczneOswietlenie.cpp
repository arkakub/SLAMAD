#include "systemc.h"

SC_MODULE(AutomatyczneOswietlenie) {

	sc_in_clk clock;
	sc_in<sc_uint<5> > actual_hours;
	sc_in<sc_uint<6> > actual_minutes;

	sc_in<sc_uint<5> > start_hour;
	sc_in<sc_uint<6> > start_minute;

	sc_in<sc_uint<5> > end_hour;
	sc_in<sc_uint<6> > end_minute;
	sc_in<bool> osw_enable;
	sc_in<bool> alarm_detected;

	sc_out<bool> osw_state;

	void prepareOutletState() {
		if (osw_enable == 1 && alarm_detected != 1) {
			if ((actual_hours == start_hour) && (actual_minutes == start_minute)) {
				osw_state = 1;
			}
			else if ((actual_hours == end_hour) && (actual_minutes == end_minute)) {
				osw_state = 0;
			}
		}
		else {
			osw_state = 0;
		}
	}

	SC_CTOR(AutomatyczneOswietlenie) {
		SC_METHOD(prepareOutletState);
		sensitive << clock.neg();
	}
};
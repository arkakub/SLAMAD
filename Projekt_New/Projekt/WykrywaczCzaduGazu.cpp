#include "systemc.h"

SC_MODULE(WykrywaczCzaduGazu) {

	sc_in_clk clock;
	sc_in<bool> czad_sensor;
	sc_in<bool> gaz_sensor;

	sc_out<bool> alarm_state;

	void prepareAlarmState() {
		if (czad_sensor == 1 || gaz_sensor == 1) {
			alarm_state = 1;
		}
		else {
			alarm_state = 0;
		}
	}

	SC_CTOR(WykrywaczCzaduGazu) {
		SC_METHOD(prepareAlarmState);
		sensitive << clock.neg();
	}
};
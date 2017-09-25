#include "systemc.h"

SC_MODULE(ModulAlarmowy) {

	sc_in_clk clock;
	sc_in<bool> air_sensor_state;
	sc_in<bool> module_enable;

	sc_out<bool> callEmergency;

	void callEmergencyHandler() {
		if (module_enable == 1) {
			callEmergency = air_sensor_state;
		}
		else {
			callEmergency = 0;
		}
	}

	SC_CTOR(ModulAlarmowy) {
		SC_METHOD(callEmergencyHandler);
		sensitive << clock.pos();
	}
};
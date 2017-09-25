#include "systemc.h"

SC_MODULE(OswietlenieChodnika) {

	sc_in_clk clock;
	sc_in<bool> light_sensor;
	sc_in<bool> sensor_enable;

	sc_out<bool> lighting_state; 

	void prepareLightingState() {
		
		if (sensor_enable == 1) {
			lighting_state = light_sensor;
		}
		else {
			lighting_state = 0;
		}
	}

	SC_CTOR(OswietlenieChodnika) {
		SC_METHOD(prepareLightingState);
		sensitive << clock.neg();
	}
};
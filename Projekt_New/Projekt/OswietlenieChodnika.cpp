#include "systemc.h"

SC_MODULE(OswietlenieChodnika) {

	sc_in_clk clock;
	sc_in_clk light_sensor;

	sc_out<bool> lighting_state; 
	//lighting is activated when the sensor is in low state
	void prepareLightingState() {
		lighting_state = !light_sensor;
	}

	SC_CTOR(OswietlenieChodnika) {
		SC_METHOD(prepareLightingState);
		sensitive << clock.neg();
	}
};
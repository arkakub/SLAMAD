#include "systemc.h"

SC_MODULE(AutomatyczneOgrzewanieWentylacja) {

	sc_in_clk clock;
	sc_in< sc_int<7> > actual_temperature;

	sc_in<sc_int<7> > heat_threshold_temp;
	sc_in<sc_int<7> > cold_threshold_temp;

	sc_out<bool> heater;
	sc_out<bool> cold_ventilation;

	void prepareOutputsState() {
		if (actual_temperature >= heat_threshold_temp) {
			cold_ventilation = 1;
			heater = 0;
		} else if (actual_temperature <= cold_threshold_temp) {
			cold_ventilation = 0;
			heater = 1;
		}
	}

	SC_CTOR(AutomatyczneOgrzewanieWentylacja) {
		SC_METHOD(prepareOutputsState);
		sensitive << clock.pos();
	}
};
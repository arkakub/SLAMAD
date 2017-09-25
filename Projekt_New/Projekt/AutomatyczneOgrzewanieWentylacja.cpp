#include "systemc.h"

SC_MODULE(AutomatyczneOgrzewanieWentylacja) {
	// wejscia modulu
	sc_in_clk clock; //clk
	sc_in< sc_int<7> > actual_temperature;

	sc_in<sc_int<7> > heat_threshold_temp;
	sc_in<sc_int<7> > cold_threshold_temp;
	sc_in<bool> temp_enable;
	sc_in<bool> alarm_detected;

	//wyjscia modulu
	sc_out<bool> heater;
	sc_out<bool> cold_ventilation;

	//logika modulu
	void prepareOutputsState() {
		if (temp_enable == 1 && alarm_detected != 1) {
			if (actual_temperature >= heat_threshold_temp) {
				cold_ventilation = 1;
				heater = 0;
			}
			else if (actual_temperature <= cold_threshold_temp) {
				cold_ventilation = 0;
				heater = 1;
			}
		}
		else {
			cold_ventilation = 0;
			heater = 0;
		}
	}

	// Konstruktor modulu
	SC_CTOR(AutomatyczneOgrzewanieWentylacja) {
		SC_METHOD(prepareOutputsState); //metoda wykonywana przy kazdym sygnale zegarowym
		sensitive << clock.pos(); //modul wyzwalany narastajacym zboczem zegarowym
	}
};
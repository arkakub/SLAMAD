#include "systemc.h"
#include "Clock_Module.cpp"
#include "AutomatyczneOswietlenie.cpp"
#include "AutomatyczneOgrzewanieWentylacja.cpp"
#include "OswietlenieChodnika.cpp"

#include <windows.h>
#include <conio.h>


sc_signal<bool> clock;

sc_signal<sc_uint<5> > start_hour_sig;
sc_signal<sc_uint<6> > start_minute_sig;
sc_signal<sc_uint<5> > end_hour_sig;
sc_signal<sc_uint<6> > end_minute_sig;

sc_signal<sc_int<7> > actual_temperature_out;
sc_signal<sc_int<7> > heat_threshold_temp;
sc_signal<sc_int<7> > cold_threshold_temp;

void showMainMenu() {
	system("cls");
	cout << "========================================================" << endl;
	cout << "                   Inteligentny dom" << endl;
	cout << "========================================================" << endl << endl;
	cout << "0. Zakoncz.." << endl;
	cout << "1. Ustaw czasy automatycznego oswietlenia" << endl;
	cout << "2. Ustaw temperature automatycznego ogrzewania/wentylacji" << endl << endl;
	cout << "Wybierz cyfre z listy aby skonfigurowac odpowiedni parametr: ";
}

void simulate(sc_time t1) {
	int i = 0;
	sc_start(t1);

	for (i = 0; i<3000; i++) {

		if (i >= 1500) {
			actual_temperature_out = 32;
		}

		clock = 0;
		sc_start(t1);
		clock = 1;
		sc_start(t1);
	}
}

void showWarningDialog() {

	system("cls");
	cout << "UWAGA! Podano nieprawidlowa wartosc" << endl;
	Sleep(3000);
}

void handleUserInterface(sc_time t1) {
	int menuId = 1;

	int start_hour;
	int start_minute;
	int end_hour;
	int end_minute;

	int heat_threshold;
	int cold_threshold;

	showMainMenu();
	cin >> menuId;

	while (menuId) {

		switch (menuId) {
		case 1:
			system("cls");
			cout << endl << "Czas wlaczenia oswietlenia: " << endl;
			cout << "h: "; cin >> start_hour; if (0 > start_hour || start_hour > 23) {
				showWarningDialog(); 
				break; }
			cout << "min: "; cin >> start_minute; if (0 > start_minute || start_minute > 59) {
				showWarningDialog();
				break; }
			cout << endl << "Czas wylczenia oswietlenia: " << endl;
			cout << "h: "; cin >> end_hour; if (0 > end_hour || end_hour > 23) {
				showWarningDialog(); 
				break; }
			cout << "min: "; cin >> end_minute; if (0 > end_minute || end_minute > 59) {
				showWarningDialog(); 
				break; }

			start_hour_sig = start_hour; start_minute_sig = start_minute;
			end_hour_sig = end_hour; end_minute_sig = end_minute;

			simulate(t1);
			showMainMenu();
			cin >> menuId;
		case 2:
			system("cls");
			cout << endl << "Max prog temperaturowy wlaczenia ogrzewania: " << endl;
			cout << "temp: "; cin >> heat_threshold; if (0 > heat_threshold || heat_threshold> 70) { 
				showWarningDialog(); 
				break; }
			cout << endl << "Min prog temperaturowy wlaczenia wentylacji: " << endl;
			cout << "temp: "; cin >> cold_threshold; if (heat_threshold > cold_threshold || cold_threshold> 70) { 
				showWarningDialog(); 
				break; }

			heat_threshold_temp = heat_threshold; cold_threshold_temp = cold_threshold;

			simulate(t1);
			showMainMenu();
			cin >> menuId;
		}
	}
}

int sc_main(int argc, char* argv[]) {

	sc_signal<sc_uint<5> > actual_time_hours_out;
	sc_signal<sc_uint<6> > actual_time_minutes_out;
	sc_signal<bool> electrical_outlet_state;

	sc_signal<bool> heater_state;
	sc_signal<bool> cold_ventilation_state;

	sc_signal<bool> light_detection;
	sc_signal<bool> light_output_state;

	actual_temperature_out = 2; //2 deg Celsius
	
	Clock_Module clock_module("Clock");
	clock_module.clock(clock);
	clock_module.actual_hours_out(actual_time_hours_out);
	clock_module.actual_minutes_out(actual_time_minutes_out);

	AutomatyczneOswietlenie outlet_module("AutomatyczneOswietlenie");
	outlet_module.clock(clock);
	outlet_module.actual_hours(actual_time_hours_out);
	outlet_module.actual_minutes(actual_time_minutes_out);
	outlet_module.start_hour(start_hour_sig);
	outlet_module.start_minute(start_minute_sig);
	outlet_module.end_hour(end_hour_sig);
	outlet_module.end_minute(end_minute_sig);
	outlet_module.electrical_outlet_state(electrical_outlet_state);

	AutomatyczneOgrzewanieWentylacja ventil_heat_module("AutomatyczneOgrzewanieWentylacja");
	ventil_heat_module.clock(clock);
	ventil_heat_module.actual_temperature(actual_temperature_out);
	ventil_heat_module.heat_threshold_temp(heat_threshold_temp);
	ventil_heat_module.cold_threshold_temp(cold_threshold_temp);
	ventil_heat_module.heater(heater_state);
	ventil_heat_module.cold_ventilation(cold_ventilation_state);

	OswietlenieChodnika lighting_Module("OswietlenieChodnika");
	lighting_Module.clock(clock);
	lighting_Module.light_sensor(light_detection);
	lighting_Module.lighting_state(light_output_state);

/*** Test Bench ***/

	sc_set_time_resolution(100, SC_MS);
	sc_time t1(500, SC_MS); //half a CLK period (1s)

	// Open VCD file
	sc_trace_file *wf = sc_create_vcd_trace_file("Simulation");
	
	sc_trace(wf, clock, "clk");
	sc_trace(wf, actual_time_hours_out, "h");
	sc_trace(wf, actual_time_minutes_out, "min");
	sc_trace(wf, electrical_outlet_state, "aut_osw");
	sc_trace(wf, actual_temperature_out, "temp");
	sc_trace(wf, heater_state, "heater");
	sc_trace(wf, cold_ventilation_state, "cooler");

	simulate(t1);
	handleUserInterface(t1);

	sc_close_vcd_trace_file(wf);
	return 0;
}
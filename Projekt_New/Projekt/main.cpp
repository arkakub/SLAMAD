#include "systemc.h"
#include "Clock_Module.cpp"
#include "AutomatyczneOswietlenie.cpp"
#include "AutomatyczneOgrzewanieWentylacja.cpp"
#include "OswietlenieChodnika.cpp"
#include "WykrywaczCzaduGazu.cpp"
#include "ModulAlarmowy.cpp"

#include <windows.h>


sc_signal<bool> clock; //rejestr 1 bit (polocznenie miedzy odulami)

sc_signal<sc_uint<5> > start_hour_sig; //rejestr 5 bit
sc_signal<sc_uint<6> > start_minute_sig; //rejestr 6 bit
sc_signal<sc_uint<5> > end_hour_sig; 
sc_signal<sc_uint<6> > end_minute_sig;
sc_signal<bool> osw_enable;

sc_signal<sc_int<7> > actual_temperature_out;
sc_signal<sc_int<7> > heat_threshold_temp;
sc_signal<sc_int<7> > cold_threshold_temp;
sc_signal<bool> temp_enable;

sc_signal<bool> light_detection;
sc_signal<bool> sensor_enable;

sc_signal<bool> alarm_module_enable;

int menuId; // wybrana pozycja 

void showMainMenu() {
// GUI programu, glowne menu
	system("cls");
	cout << "============================================================" << endl;
	cout << "                    Inteligentny dom" << endl;
	cout << "============================================================" << endl << endl;
	cout << "0. Zakoncz.." << endl;
	cout << "1. Aktywacja funkcji automatycznego oswietlenia pomieszczen" << endl;
	cout << "2. Ustaw czasy automatycznego oswietlenia pomieszczen" << endl;
	cout << "3. Aktywacja funkcji automatycznego ogrzewania/wentylacji" << endl;
	cout << "4. Ustaw temperature automatycznego ogrzewania/wentylacji" << endl;
	cout << "5. Aktywacja funkcji automatycznego oswietlenia chodnikowego" << endl;
	cout << "6. Aktywacja funkcji automatycznego zawiadamiania sluzb ratunkowych w przypadku alarmow" << endl << endl;
	cout << "Wybierz cyfre z listy i potwierdz wciskajac ENTER, aby skonfigurowac odpowiedni parametr: ";
	cin >> menuId; // uzytkownik podaje liczbe pozycji ktora chce wybrac z listy
}

void simulate(sc_time t1) {
// Generacja symulacji
	int i = 0;
	sc_start(t1);
	// pojedynczy ciag symulacji trwa 3000 okresow zegara, czyli ok 3000 sek.
	for (i = 0; i<3000; i++) {

		if (i >= 1500) {
			actual_temperature_out = 32; //wymuszenie po uplywie 1500 okresow zegara temperatury o wartosci 32 stC
			light_detection = 1; //wymusznenie wykrycia promieni slonecznych
		}
		// zegar clk generowany z okresem 1s, czas ts = 500ms
		clock = 0;
		sc_start(t1);
		clock = 1;
		sc_start(t1);
	}
}

void showWarningDialog() {
// Wyswietlanie ostrzezenia w przypadku wpisania nieprawidlowych wartosci np. godziny z poza zakresu
	system("cls");
	cout << "UWAGA! Podano nieprawidlowa wartosc" << endl;
	Sleep(3000);
}

void handleUserInterface(sc_time t1) {
// Funkcja komunikacji z urzytkownikiem, determinuje odpowiednie akcje podejmowane przy wybraniu odpowiedniej pozycji z menu podstawowego
	int start_hour;
	int start_minute;
	int end_hour;
	int end_minute;
	int osw_enb;

	int heat_threshold;
	int cold_threshold;
	int temp_enb;

	int sensor_enb;
	int alarm_module_enb;

	showMainMenu();

	while (menuId > 0) {

		switch (menuId) {
		case 0: break;
		case 1:
			system("cls");
			cout << endl << "Wlacz lub wylacz funkcje czasowego oswietlenia pomieszczen: " << endl;
			cout << "wl(1)/wyl(0): "; cin >> osw_enb; if (0 > osw_enb || osw_enb > 1) {
				osw_enb = 0; //uzytkownik zatwierdzil bledne dane wejsciowe
				showWarningDialog();
				break; // przerwanie akcji, bedzie ona wyknywana, az uzytkownik poda poprawne dane
			}
			osw_enable = osw_enb; //dane wejsciowe sa poprawne, nastepuje przepisanie otrzymanej wartosci do rejestru

			simulate(t1);
			showMainMenu();
			break;
		case 2:
			system("cls");
			cout << endl << "Czas wlaczenia oswietlenia: " << endl;
			cout << "h: "; cin >> start_hour; if (0 > start_hour || start_hour > 23) {
				start_hour = 0;
				showWarningDialog();
				break; }
			cout << "min: "; cin >> start_minute; if (0 > start_minute || start_minute > 59) {
				start_minute = 0;
				showWarningDialog();
				break; }
			cout << endl << "Czas wylczenia oswietlenia: " << endl;
			cout << "h: "; cin >> end_hour; if (0 > end_hour || end_hour > 23) {
				end_hour = 0;
				showWarningDialog(); 
				break; }
			cout << "min: "; cin >> end_minute; if (0 > end_minute || end_minute > 59) {
				end_minute = 0;
				showWarningDialog(); 
				break; }

			start_hour_sig = start_hour; start_minute_sig = start_minute;
			end_hour_sig = end_hour; end_minute_sig = end_minute;

			simulate(t1);
			showMainMenu();
			break;
		case 3:
			system("cls");
			cout << endl << "Wlacz lub wylacz funkcje automatycznego ogrzewania/wentylacji: " << endl;
			cout << "wl(1)/wyl(0): "; cin >> temp_enb; if (0 > temp_enb || temp_enb > 1) {
				temp_enb = 0;
				showWarningDialog();
				break;
			}
			temp_enable = temp_enb;

			simulate(t1);
			showMainMenu();
			break;
		case 4:
			system("cls");
			cout << endl << "Max prog temperaturowy wlaczenia ogrzewania: " << endl;
			cout << "temp: "; cin >> heat_threshold; if (0 > heat_threshold || heat_threshold> 70) { 
				heat_threshold = 0;
				showWarningDialog(); 
				break; }
			cout << endl << "Min prog temperaturowy wlaczenia wentylacji: " << endl;
			cout << "temp: "; cin >> cold_threshold; if (heat_threshold > cold_threshold || cold_threshold> 70) { 
				cold_threshold = 0;
				showWarningDialog(); 
				break; }

			heat_threshold_temp = heat_threshold; cold_threshold_temp = cold_threshold;

			simulate(t1);
			showMainMenu();
			break;
		case 5:
			system("cls");
			cout << endl << "Wlacz lub wylacz funkcje automatycznego oswietlenia chodnikowego: " << endl;
			cout << "wl(1)/wyl(0): "; cin >> sensor_enb; if (0 > sensor_enb || sensor_enb > 1) {
				sensor_enb = 0;
				showWarningDialog();
				break;
			}
			sensor_enable = sensor_enb;

			simulate(t1);
			showMainMenu();
			break;
		case 6:
			system("cls");
			cout << endl << "Wlacz lub wylacz funkcje automatycznego powiadamiania sluzb alarmowych: " << endl;
			cout << "wl(1)/wyl(0): "; cin >> alarm_module_enb; if (0 > alarm_module_enb || alarm_module_enb > 1) {
				alarm_module_enb = 0;
				showWarningDialog();
				break;
			}
			alarm_module_enable = alarm_module_enb;

			simulate(t1);
			showMainMenu();
			break;
		}
	}
}

int sc_main(int argc, char* argv[]) {

	// Deklaracje poszczegolnych sygnalow i polaczen miedzy modulami
	sc_signal<sc_uint<5> > actual_time_hours_out;
	sc_signal<sc_uint<6> > actual_time_minutes_out;
	sc_signal<bool> osw_state;

	sc_signal<bool> heater_state;
	sc_signal<bool> cold_ventilation_state;
	sc_signal<bool> alarm_detected;

	sc_signal<bool> light_output_state;

	sc_signal<bool> czad_sensor;
	sc_signal<bool> gaz_sensor;
	sc_signal<bool> alarm_state;

	sc_signal<bool> air_sensor_state;
	sc_signal<bool> callEmergency;

	system("cls"); // Wyczyszczenie widoku konsoli
	actual_temperature_out = 2;
	light_detection = 0;
	sensor_enable = 0;

	// Wartosci domyslne
	start_hour_sig = 19; start_minute_sig = 00;
	end_hour_sig = 23; end_minute_sig = 45;
	heat_threshold_temp = 3; cold_threshold_temp = 28;
	
	// Deklaracja poszczegulnych modulow w funkcji glownej. Zintegrowanie modulow miedzy soba, podpiecie poszczegolnych syngalow pod moduly
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
	outlet_module.osw_enable(osw_enable);
	outlet_module.alarm_detected(alarm_detected);
	outlet_module.osw_state(osw_state);

	AutomatyczneOgrzewanieWentylacja ventil_heat_module("AutomatyczneOgrzewanieWentylacja");
	ventil_heat_module.clock(clock);
	ventil_heat_module.actual_temperature(actual_temperature_out);
	ventil_heat_module.heat_threshold_temp(heat_threshold_temp);
	ventil_heat_module.cold_threshold_temp(cold_threshold_temp);
	ventil_heat_module.heater(heater_state);
	ventil_heat_module.temp_enable(temp_enable);
	ventil_heat_module.alarm_detected(alarm_detected);
	ventil_heat_module.cold_ventilation(cold_ventilation_state);

	OswietlenieChodnika lighting_Module("OswietlenieChodnika");
	lighting_Module.clock(clock);
	lighting_Module.light_sensor(light_detection);
	lighting_Module.sensor_enable(sensor_enable);
	lighting_Module.lighting_state(light_output_state);

	WykrywaczCzaduGazu air_scaner("WykrywaczCzaduGazu");
	air_scaner.clock(clock);
	air_scaner.czad_sensor(czad_sensor);
	air_scaner.gaz_sensor(gaz_sensor);
	air_scaner.alarm_state(alarm_state);

	ModulAlarmowy alarm_module("ModulAlarmowy");
	alarm_module.clock(clock);
	alarm_module.air_sensor_state(air_sensor_state);
	alarm_module.module_enable(alarm_module_enable);
	alarm_module.callEmergency(callEmergency);

/*** Test Bench ***/
	
	sc_set_time_resolution(100, SC_MS);
	sc_time t1(500, SC_MS); //okres zegara wynosi 1s

	// Otwarcie pliku do ktorego generowana jest symulacja, nazwa pliku Simulation.vcd
	sc_trace_file *wf = sc_create_vcd_trace_file("Simulation");
	
	// Podpiecie poszczegolnych sygnalow pod symulacje, nadanie im nazw
	sc_trace(wf, clock, "clk");
	sc_trace(wf, actual_time_hours_out, "h");
	sc_trace(wf, actual_time_minutes_out, "min");
	sc_trace(wf, osw_state, "aut_osw");
	sc_trace(wf, actual_temperature_out, "temp");
	sc_trace(wf, heater_state, "heater");
	sc_trace(wf, cold_ventilation_state, "cooler");
	sc_trace(wf, light_output_state, "osw_chodnik");

	simulate(t1);
	handleUserInterface(t1);

	// Zamkniecie pliku wraz z symulacja
	sc_close_vcd_trace_file(wf);
	return 0;
}
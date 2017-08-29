#include "systemc.h"
#include "Clock_Module.cpp"
#include "Electrical_Outlet_Module.cpp"

int sc_main(int argc, char* argv[]) {
	sc_signal<bool> clock; //1 bit

	sc_signal<sc_uint<5> > actual_time_hours_out; //5 bit
	sc_signal<sc_uint<6> > actual_time_minutes_out; //5 bit

	sc_signal<bool> electrical_outlet_state;

	int i = 0;
	
	Clock_Module clock_module("Clock");
	clock_module.clock(clock);
	clock_module.actual_hours_out(actual_time_hours_out);
	clock_module.actual_minutes_out(actual_time_minutes_out);

	Electrical_Outlet_Module outlet_module("ElectricalOutlet");
	outlet_module.clock(clock);
	outlet_module.actual_hours(actual_time_hours_out);
	outlet_module.actual_minutes(actual_time_minutes_out);
	outlet_module.electrical_outlet_state(electrical_outlet_state);

/*** Test Bench ***/

	sc_set_time_resolution(100, SC_MS);
	sc_time t1(500, SC_MS); //half a CLK period (1s)

	// Open VCD file
	sc_trace_file *wf = sc_create_vcd_trace_file("Simulation");
	
	sc_trace(wf, clock, "clk");
	sc_trace(wf, actual_time_hours_out, "hours");
	sc_trace(wf, actual_time_minutes_out, "minutes");
	sc_trace(wf, electrical_outlet_state, "el_outlet");

	sc_start(t1);

	for (i = 0; i<3000; i++) {
		clock = 0;
		sc_start(t1);
		clock = 1;
		sc_start(t1);
	}

	sc_close_vcd_trace_file(wf);
	return 0;
}
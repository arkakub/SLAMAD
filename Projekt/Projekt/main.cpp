#include "systemc.h"
#include "first_counter.cpp"

int sc_main(int argc, char* argv[]) {
	sc_signal<bool> clock; //1 bit
	sc_signal<bool> reset;
	sc_signal<bool> enable;
	sc_signal<sc_uint<5> > hours_out; //5 bit
	sc_signal<sc_uint<6> > minutes_out; //5 bit
	int i = 0;
	
	first_counter counter("COUNTER");
	counter.clock(clock);
	counter.reset(reset);
	counter.enable(enable);
	counter.hours_out(hours_out);
	counter.minutes_out(minutes_out);

/*** Test Bench ***/

	sc_set_time_resolution(1, SC_SEC);
	sc_time t1(1, SC_SEC); //half a CLK period (1s)

	// Open VCD file
	sc_trace_file *wf = sc_create_vcd_trace_file("counter");
	
	sc_trace(wf, clock, "clock");
	sc_trace(wf, reset, "reset");
	sc_trace(wf, enable, "enable");
	sc_trace(wf, hours_out, "hours");
	sc_trace(wf, minutes_out, "minutes");

	reset = 0;
	enable = 0;
	for (i = 0; i<5; i++) {
		clock = 0;
		sc_start(t1);
		clock = 1;
		sc_start(t1);
	}
	reset = 1;
	for (i = 0; i<10; i++) {
		clock = 0;
		sc_start(t1);
		clock = 1;
		sc_start(t1);
	}
	reset = 0;
	for (i = 0; i<5; i++) {
		clock = 0;
		sc_start(t1);
		clock = 1;
		sc_start(t1);
	}
	enable = 1;
	for (i = 0; i<65; i++) {
		clock = 0;
		sc_start(t1);
		clock = 1;
		sc_start(t1);
	}

	enable = 0;

	sc_close_vcd_trace_file(wf);
	return 0;
}
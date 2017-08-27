#include "systemc.h"

SC_MODULE(sc_andm)
{
	sc_in<bool> a, b;
	sc_out<bool> c;

	void do_and()
	{
		c.write(a.read() & b.read());
	}

	SC_CTOR(sc_andm)
	{
		SC_METHOD(do_and);
		sensitive << a << b;
	}
};

int sc_main(int argc, char* argv[])
{
	sc_set_time_resolution(1, SC_PS);
	sc_time t1(1, SC_PS);
	sc_andm sc_and1("andm1");

	sc_signal<bool> a, b, c;

	sc_and1.a(a);
	sc_and1.b(b);
	sc_and1.c(c);

	sc_trace_file *wf = sc_create_vcd_trace_file("andm");
	sc_trace(wf, sc_and1.a, "a");
	sc_trace(wf, sc_and1.b, "b");
	sc_trace(wf, sc_and1.c, "c");

	a.write(0);
	b.write(0);

	sc_start(t1);
	a.write(0);
	b.write(1);

	sc_start(t1);
	a.write(1);
	b.write(0);

	sc_start(t1);
	a.write(1);
	b.write(1);

	sc_start(t1);
	a.write(0);
	b.write(0);

	sc_start(t1);
	sc_stop();

	sc_close_vcd_trace_file(wf);
	return(0);
}
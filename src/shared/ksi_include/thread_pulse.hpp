#ifndef DMITRY_DERGACHVEV_02_04_2008_THREAD_PULSE_H
#define DMITRY_DERGACHEV_02_04_2008_THEAD_PULSE_H

#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>

class thread_pulse_t
{
public:
	explicit thread_pulse_t(int sec_): sec_(sec_) {}
	void operator() ()
	{
		boost::xtime xt;
		boost::xtime_get(&xt, boost::TIME_UTC);
		xt.sec += sec_;
		boost::thread::sleep(xt);
	}
private:
	int sec_;
};

#endif


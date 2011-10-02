
#include "drawer.hpp"


#ifndef _NOT_DRAWER_BUILD_

#include <windows.h>
#include <TVichw32.h>
#include <ksi_include/ksi_exceptions.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#endif

void* drawer_t::handle_ = 0;

drawer_t::drawer_t() {
#ifndef _NOT_DRAWER_BUILD_
	if (!handle_) handle_ = OpenTVicHW32(handle_, "TVICHW32", "TvicDevice0");
	if (!GetActiveHW(handle_)) {
		handle_ = 0;
		THROW_MESSAGE(0, "Can't open TVicHW32 driver", "drawer_t::drawer_t()");
	}
#endif
}

drawer_t::~drawer_t() {

}

void drawer_t::open() {
#ifndef _NOT_DRAWER_BUILD_
	SetPortByte(handle_, 0x240, 0xf1);
	boost::this_thread::sleep(boost::posix_time::milliseconds(200));
	SetPortByte(handle_, 0x240, 0xf0);
#endif
}

#ifndef DMITRY_DERGACHEV_14_04_2009_STRERROR_H
#define DMITRY_DERGACHEV_14_04_2009_STRERROR_H

#include <string>
#include <map>

namespace strerr {
	
	class strerr_t {
	public:
		strerr_t();
		std::string get(int);
	private:
		std::map<int, std::string> err_map_;
	};
	
	extern strerr_t err;
};

#endif


#ifndef DMITRY_DERGACHEV_10_04_2008_COM_H
#define DMITRY_DERGACHEV_10_04_2008_COM_H

#if !defined(_UNIX_BUILD_) && !defined(_WIN32_BUILD_)
#error define _UNIX_BUILD_ or _WIN32_BUILD_
#endif

#ifdef _UNIX_BUILD_

#include <termios.h>
#include <string>

#include <fcntl.h>
#include <cstring>
#include <ksi_include/ksi_exceptions.hpp>
#include <boost/lexical_cast.hpp>
#include <poll.h>

class com_t
{
public:
	com_t(const std::string& port, int rate);
	com_t(int comnum, int speed, bool parity);
	~com_t();
	template <typename charT> size_t write(const charT* buff, size_t len) {
		for (size_t i=0; i<len; ++i) {
			do {
				struct pollfd pfd;
				pfd.fd = fd;
				pfd.events = POLLOUT | POLLWRBAND;
				int ret = poll(&pfd, 1, 500);
				if (ret <=0) THROW_MESSAGE(1, "poll failed.", "size_t com_t::write(const void*, size_t)");
				if (pfd.revents & POLLHUP) std::cout << "POLLHUP" << std::endl;
				else if (pfd.revents & POLLERR) {}//std::cout << "POLLERR" << std::endl;
				else if (pfd.revents & POLLNVAL) {}// std::cout << "POLLNVAL" << std::endl;
				else break;
			} while (1);
			int res = ::write(fd, buff + i, 1);
			if (res == -1) THROW_MESSAGE(0, "write failed", "size_t com_t::write(const void*, size_t)");
		}
	}
	void set_timeouts(size_t, size_t, size_t) {}
	//void write(const char *ptr, int size);
	//void read(char *ptr, int size);
	void purge();
private:
	int fd;
	termios oldios;
};

#endif //_UNIX_BUILD_

#ifdef _WIN32_BUILD_

#include <WinDef.h>
#include <WinBase.h>
#include <string>

class com_t
{
public:
	explicit com_t(const std::string& port, int rate_, int mode = 1);
  	~com_t();
  	template <typename charT> size_t write(const charT* buff, size_t len) {
		DWORD res_len;
		if (!WriteFile (hCom, buff, len, &res_len, 0)) return 0;
		return res_len;
	}
	template <typename charT> size_t writeByte(charT ch) { 
		charT buff[1] = {ch}; 
		return write<charT>(buff, 1); 
	}
  	template <typename charT> size_t read(charT* buff, size_t len) {
		DWORD res_len;
		if (!ReadFile (hCom, buff, len, &res_len, 0)) return 0;
		buff[res_len] = 0;
		return res_len;
	}
	void set_timeouts(size_t, size_t, size_t);
  	void purge();
private:
  	void* hCom;
	int rate_;
};

#endif // _WIN32_BUILD_

#endif

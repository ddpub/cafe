#ifndef DMITRY_DERGACHEV_20_10_2008_CUSTOMER_DISPLAY_H
#define DMITRY_DERGACHEV_20_10_2008_CUSTOMER_DISPLAY_H

#include <serial/com.hpp>

class customer_display_t {
public:
	explicit customer_display_t(const std::string& port): com_(port, 9600) { clear_all(); }
	void clear_all();
	void clear_line(int num);
	void print(int rownum, const std::string& str, const std::string& price = "", int align = 0);
private:
	void pos_cursor(int x, int y);
	void writebyte(char);
private:
	com_t com_;
};

void vfd_print(customer_display_t* const display, int rownum, const std::string& str, const std::string& price, int align);

void vfd_clear_all(customer_display_t* const display);

void vfd_clear_line(customer_display_t* const display, int num);

#endif

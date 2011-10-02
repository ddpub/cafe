
#include "customer_display.hpp"


void customer_display_t::writebyte(char ch) {
	char buff[1] = {ch};
	com_.write<char>(buff, 1);
}

void customer_display_t::pos_cursor(int x, int y) {
	writebyte(0x1B);
	writebyte(0x6C);
	writebyte(x);
	writebyte(y);
}

void customer_display_t::clear_all() {
	writebyte(0x0C);
	writebyte(0x1B);
	writebyte(0x40);
}

void customer_display_t::clear_line(int num) {
  pos_cursor(1, num);
  writebyte(0x18);
}

void customer_display_t::print(int rownum, const std::string& str, const std::string& price, int align) {
	int max = str.length() > 20 ? 20 : str.length();
	std::string data = str.substr(0, max);
	pos_cursor(1, rownum);
	writebyte(0x18);
	int st = 1;
	if (align == 1) {
		st = ((20 - data.length())/2) + 1;
	}
	else if (align == 2) {
		st = 20 - data.length() + 1;
	}
	pos_cursor(st, rownum);
	for (int i = 0; i <max; ++i) writebyte(data[i]);
	if (align == 1 || align == 2 || !price.length()) return;
	//pos_cursor(20, rownum);
	for (int i = 0; i < price.length(); i++) {
		pos_cursor(20 - i, rownum);
		writebyte(price[price.length() - i - 1]);
	}
}

void vfd_print(customer_display_t* const display, int rownum, const std::string& str, const std::string& price, int align) {
	if (display == 0) return;
	display->print(rownum, str, price, align);
}

void vfd_clear_all(customer_display_t* const display) {
	if (display == 0) return;
	display->clear_all();
}

void vfd_clear_line(customer_display_t* const display, int num) {
	if (display == 0) return;
	display->clear_line(num);
}

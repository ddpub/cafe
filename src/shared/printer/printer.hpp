#ifndef DMITRY_DERGACHEV_27_03_2008_PRINTER_H
#define DMITRY_DERGACHEV_27_03_2008_PRINTER_H

#include <string>
#include <serial/com.hpp>
#include <vector>


class base_printer_t
{
public:	
	typedef char charT;
	base_printer_t(int wide, int ccount): wide(wide), ccount(ccount) {}
	virtual ~base_printer_t() {}
	int wide_size() const { return wide; }
	void set_wide(int new_wide) { wide = new_wide; }
	int ccount_size() const { return ccount; }
	virtual void init() = 0;
	virtual void write_ch(charT ch)=0;
	virtual void lf()=0;
	virtual void cut()=0;
	virtual void absolute_move(int)=0;
	virtual void set_12dot()=0;
	virtual void set_cp1251()=0;
	virtual void print(const std::string& str)=0;
	virtual void set_align_left()=0;
	virtual void set_align_center()=0;
	virtual void print_logo()=0;
	virtual void test()=0;
	virtual void set_font_size(int size=0)=0;
	virtual std::string name() const =0;
	virtual void set_logo(const std::vector<charT>&)=0;
	virtual void clear_logo()=0;
	virtual void print_code(const std::string&)=0;
	virtual void open_drawer()=0;
	virtual void bell()=0;
	virtual size_t height() const { return wide*ccount; }
	virtual bool done(const std::string& xml)=0;
private:
	int wide;
	int ccount;
};

class null_printer_t: public base_printer_t
{
public:
	explicit null_printer_t(): base_printer_t(12, 48) {}
	~null_printer_t() {}
	void init() {}
	void write_ch(charT ch) {}
	void lf() {}
	void cut() {}
	void absolute_move(int) {}
	void set_12dot() {}
	void set_cp1251() {}
	void print(const std::string& str) {}
	void set_align_left() {}
	void set_align_center() {}
	void print_logo() {}
	void test() {}
	void set_font_size(int size=0) {}
	std::string name() const { return "Null"; }
	void set_logo(const std::vector<charT>&) {}
	void clear_logo() {}
	void print_code(const std::string&) {}
	void open_drawer() {}
	void bell() {}
	bool done(const std::string& xml) { return true; }
};

class epson_printer_t: public base_printer_t
{
public:
	explicit epson_printer_t(com_t& os_): base_printer_t(12, 42), os_(os_) { init(); set_cp1251(); }
	~epson_printer_t() {}
	void init();
	void write_ch(charT ch);
	void lf();
	void cut();
	void absolute_move(int);
	void set_12dot();
	void set_cp1251();
	void print(const std::string&);
	void set_align_left();
	void set_align_center();
	void print_logo();
	void test();
	void set_font_size(int size=0);
	std::string name() const { return "Epson"; }
	void set_logo(const std::vector<charT>&);
	void clear_logo() {}
	void print_code(const std::string&);
	void open_drawer();
	void bell();
	bool done(const std::string& xml) { return true; }
private:
	com_t& os_;
};

class star_printer_t: public base_printer_t
{
public:
	explicit star_printer_t(com_t& os_): base_printer_t(12, 48), os_(os_) { init(); set_cp1251(); }
	~star_printer_t() {}
	void init();
	void write_ch(charT ch);
	void lf();
	void cut();
	void absolute_move(int);
	void set_12dot();
	void set_cp1251();
	void print(const std::string&);
	void set_align_left();
	void set_align_center();
	void print_logo();
	void test();
	void set_font_size(int size=0);
	std::string name() const { return "Star"; }
	void set_logo(const std::vector<charT>&);
	void clear_logo() {}
	void print_code(const std::string&);
	void open_drawer();
	void bell();
	bool done(const std::string& xml) { return  true; }
private:
	com_t& os_;
};

class net_printer_t: public base_printer_t {
public:
	net_printer_t(const std::string& host_, int port_): base_printer_t(12, 48), host_(host_), port_(port_) {}
	~net_printer_t() {}
	void init() {}
	void write_ch(charT ch) {}
	void lf() {}
	void cut() {}
	void absolute_move(int) {}
	void set_12dot() {}
	void set_cp1251() {}
	void print(const std::string& str) {}
	void set_align_left() {}
	void set_align_center() {}
	void print_logo() {}
	void test() {}
	void set_font_size(int size=0) {}
	std::string name() const { return "Net"; }
	void set_logo(const std::vector<charT>&) {}
	void clear_logo() {}
	void print_code(const std::string&) {}
	void open_drawer() { done("<?xml version=\"1.0\" encoding=\"windows-1251\"?><receipt><open_drawer/></receipt>"); }
	void bell() {}
	bool done(const std::string& xml);
private:
	std::string host_;
	int port_;
};

#endif

#ifndef DMITRY_DERGACHEV_27_03_2008_XML_CHECK_PRINTER_H
#define DMITRY_DERGACHEV_27_03_2008_XML_CHECK_PRINTER_H


#include <printer/printer.hpp>
#include <string>
#include <vector>
#include <map>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace xml_attr
{
  enum align_t { left, center, right };
  enum font_t { normal, hx2, hx2wx2 };
};

size_t width(const xml_attr::font_t& font);
std::string align_str(const xml_attr::align_t& align);
std::string font_str(const xml_attr::font_t& font);
std::vector<std::string> split(std::string, size_t max_length);

class column_t
{
public:
	column_t(const std::string& text_, bool multiline_, bool stretch_, const xml_attr::align_t& align_, size_t lspace_, size_t rspace_, const xml_attr::font_t& font_, size_t lf_=0)
		: text_(text_), multiline_(multiline_), stretch_(stretch_), align_(align_), lspace_(lspace_), rspace_(rspace_), font_(font_), lf_(lf_) {}
	const std::string& text() const { return text_; }
	bool multiline() const { return multiline_; }
	bool stretch() const { return stretch_; }
	const xml_attr::align_t& align() const { return align_; }
	size_t lspace() const { return lspace_; }
	size_t rspace() const { return rspace_; }
	size_t lf() const { return lf_; }
	const xml_attr::font_t& font() const { return font_; }
	std::string xml_str() const;
private:
	std::string text_;
	bool multiline_;
	bool stretch_;
	xml_attr::align_t align_;
	size_t lspace_; // per dots
	size_t rspace_; // per dots
	xml_attr::font_t font_;
	size_t lf_;
};

typedef std::vector<column_t> row_t;


class xml_table_t
{
public:
	xml_table_t(const std::vector<row_t> rows_): rows_(rows_) {}
	xml_table_t(const row_t& row) { rows_.push_back(row); }
	size_t size() const { return rows_.size(); }
	const row_t& row(size_t i) const { return rows_[i]; }
	std::string xml_str() const;
private:
	std::vector<row_t> rows_;
};

class check_printer_t
{
public:
	explicit check_printer_t(base_printer_t* printer_): printer_(printer_) {}
	void bell() const { printer_->bell(); }
	void open_drawer() const { printer_->open_drawer(); }
	void barcode(const std::string& code) const { printer_->print_code(code); }
	void test() const { printer_->test(); }
	void lf() const { printer_->lf(); }
	void cut() const { printer_->cut(); }
	void logo() const { printer_->print_logo(); }
	void fl() const;
	void table(const xml_table_t&) const;
	void set_font(const xml_attr::font_t& font) const;
private:
	base_printer_t* printer_;
};

void print_xml(base_printer_t*, const std::string& message);


#endif

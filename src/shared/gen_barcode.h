//---------------------------------------------------------------------------

#ifndef gen_barcodeH
#define gen_barcodeH
#include <vector>
//#include <System.hpp>
//
//#include <Controls.hpp>
#include <string>
//---------------------------------------------------------------------------

class barcode_t
{
private:
  int Line;
  int Column;
  int Count_data;
  int err_count;
  std::string In_data;
  std::vector<std::string> out_code;
//  std::vector<String> code;
protected:
	std::string start() { return "81111113"; }
	std::string stop() { return "7113111211"; }
	std::string start_descr(int line);
	std::string stop_descr(int line);
	int get_line() const {return Line;}
	int get_column() const {return Column;}
	void set_line(int line) { Line = line; }
	void set_column(int column) { Column = column; }
public:
  barcode_t() {};
  barcode_t(std::string);
  std::vector<std::string> print_barcode();

  std::string gen_code_str(int code_num, int line);
  std::vector<int> gen_code(std::string&);
  int mod900(std::string& in);
  std::string reverse(std::string& in);
};


#endif

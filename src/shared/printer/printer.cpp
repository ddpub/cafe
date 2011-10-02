

#include "printer.hpp"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <ksi_include/ksi_exceptions.hpp>

//#include "gen_barcode.h"

//---------------------------------------------------------------------------
void star_printer_t::init()
{
	charT buff[2] = {27, 64};
	os_.write(buff, 2);
}
//---------------------------------------------------------------------------
void star_printer_t::write_ch(char ch)
{
	charT buff[1]= {ch};
	os_.write(buff, 1);
}
//---------------------------------------------------------------------------
void star_printer_t::set_logo(const std::vector<charT>& data)
{
	charT buff[4] = { 27, 28, 113, 1};
	os_.write(buff,4);
	os_.write(&data[0], data.size());
}
//---------------------------------------------------------------------------
void epson_printer_t::bell()
{
//  char buff[6] = { 27, 112, 1, 1, 10, 2 };
//  com.write(buff, 5);
}
//---------------------------------------------------------------------------
void star_printer_t::bell()
{
	charT buff[6] = { 27, 29, 7, 49, 100, 100 };
	os_.write(buff, 6);
}
//---------------------------------------------------------------------------
void star_printer_t::lf()
{
	charT buff[1] = { 10 };
	os_.write(buff, 1);
}
//---------------------------------------------------------------------------
void star_printer_t::print(const std::string& str)
{
	os_.write(str.c_str(), str.length());
}
//---------------------------------------------------------------------------
void star_printer_t::cut()
{
	charT buff[3] = {27, 100, 51};
	os_.write(buff, 3);
}	
//---------------------------------------------------------------------------
void star_printer_t::set_align_left()
{
	charT buff[4] = {27, 29, 97, 48};
	os_.write(buff, 4);
}
//---------------------------------------------------------------------------
void star_printer_t::set_align_center()
{
	charT buff[4] = {27, 29, 97, 49};
	os_.write(buff, 4);
}
//---------------------------------------------------------------------------
void star_printer_t::absolute_move(int size)
{
	charT buff[5] = {27, 29, 65, size-int(size/256)*256, size/256};
	os_.write(buff, 5);
}
//---------------------------------------------------------------------------
void star_printer_t::set_font_size(int size)
{
	if (size == 0) {
		charT buff[4] = {27, 105, 48, 48};
		os_.write(buff, 4);
		set_wide(12);
	}
	else if (size == 1) {
		charT buff[4] = {27, 105, 49, 48};
		os_.write(buff, 4);
		set_wide(12);
	}
	else if (size == 2) {
		charT buff[4] = {27, 105, 49, 49};
		os_.write(buff, 4);
		set_wide(24);
	}
}
//---------------------------------------------------------------------------
void star_printer_t::print_logo()
{
	charT buff[5] = {27, 28, 112, 1, 0};
	os_.write(buff, 5);
}
//---------------------------------------------------------------------------
void star_printer_t::test()
{
	charT buff[4] = {27, 63, 10, 0 };
	os_.write(buff, 4);
}
//---------------------------------------------------------------------------
void star_printer_t::set_cp1251()
{
	charT buff[4] = { 27, 29, 116, 34 };
	os_.write(buff, 4);
}
//---------------------------------------------------------------------------
void star_printer_t::set_12dot()
{
	charT buff[2] = { 27, 77 };
	os_.write(buff, 2);
}
//---------------------------------------------------------------------------
void star_printer_t::open_drawer()
{
	charT buff[5] = { 27, 7, 10, 50, 7 };
	os_.write(buff, 5);
//  char buff[4] = { 27, 7, 10, 50 };
//  com.write(buff, 4);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void epson_printer_t::set_logo(const std::vector<charT>& data)
{
	charT buff[3] = { 28, 113, 1 };
	os_.write(buff, 3);
	os_.write(&data[0], data.size());
}
//---------------------------------------------------------------------------
void epson_printer_t::init()
{
	charT buff[2] = {27, 64};
	os_.write(buff, 2);
}
//---------------------------------------------------------------------------
void epson_printer_t::write_ch(char ch)
{
	charT buff[1]= {ch};
	os_.write(buff, 1);
}
//---------------------------------------------------------------------------
void epson_printer_t::open_drawer()
{
	charT buff[5] = { 27, 112, 48, 55, 121 };
	os_.write(buff, 5);
}
//---------------------------------------------------------------------------
void epson_printer_t::absolute_move(int size)
{
	charT buff[4] = { 27, 36, size-int(size/256)*256, size/256 };
	os_.write(buff, 4);
}
//---------------------------------------------------------------------------
void epson_printer_t::set_12dot()
{
	charT buff[3] = { 27, 76, 48 };
	os_.write(buff, 3);
}
//---------------------------------------------------------------------------
void epson_printer_t::lf()
{
	charT buff[3] = { 27, 100, 1 };
	os_.write(buff, 3);
}
//---------------------------------------------------------------------------
void epson_printer_t::set_cp1251()
{
	charT buff[3] = { 27, 116, 28 };
	os_.write(buff, 3);
}
//---------------------------------------------------------------------------
void epson_printer_t::cut()
{
	charT buff[4] = { 29, 86, 65, 0 };
	os_.write(buff, 4);
}
//---------------------------------------------------------------------------
void epson_printer_t::print(const std::string& str)
{
	os_.write(str.c_str(), str.length());
}
//---------------------------------------------------------------------------
void epson_printer_t::set_align_left()
{
	charT buff[3] = { 27, 97, 48 };
	os_.write(buff, 3);
}
//---------------------------------------------------------------------------
void epson_printer_t::set_align_center()
{
	charT buff[3] = { 27, 97, 49 };
	os_.write(buff, 3);
}
//---------------------------------------------------------------------------
void epson_printer_t::set_font_size(int size)
{
	if (size == 0) {
		charT buff[3] = {27, 33, 0};
		os_.write(buff, 3);
		set_wide(12);
	}
	else if (size == 1) {
		charT buff[3] = {27, 33, 16};
		os_.write(buff, 3);
		set_wide(12);
	}
	else if (size == 2) {
		charT buff[3] = {27, 33, 48};
		os_.write(buff, 3);
		set_wide(24);
	}
}
//---------------------------------------------------------------------------
void epson_printer_t::print_logo()
{
	charT buff[4] = { 28, 112, 1, 48 };
	os_.write(buff, 4);
}
//---------------------------------------------------------------------------
void epson_printer_t::test()
{
	charT buff[7] = { 29, 40, 65, 2, 0, 48, 50 };
	os_.write(buff, 7);
}
//---------------------------------------------------------------------------
void star_printer_t::print_code(const std::string& code)
{
/*
  barcode_t barcode(code);
  std::vector<String> print;
  print = barcode.print_barcode();
  for(size_t i=0; i<print.size(); i++)
  {
    print[i] = "0000000000" + print[i];
    int k = print[i].Length()/8;
    while (k*8 != print[i].Length())
    {
      print[i] += "0";
      k = print[i].Length()/8;
    }
  }
  int sh = 3;
  int x = (print[0].Length())/8*sh;
  int y = 12;
  char buff[10000];
  buff[0] = 27;
  buff[1] = 107;
  buff[2] = x;
  buff[3] = 0;
  int bb = 3; //номер текущего байта     //barcode.get_line()
  for (size_t ii = 0; ii<print.size(); ii++)
  {
    for (int m=0; m<y; m++)
    {
      int b_len=7;   //длина текущего байта
      int len(0);    // значение текущего байта
      for(int i=1; i<print[ii].Length()+1; i++)
      {
        int ss_i = StrToInt(print[ii][i]);
        for(int j=0; j<sh; j++)
        {
          int st=1;  for (int k=0; k<b_len; k++) st*=2;
          len += ss_i*st;
          if(b_len-- == 0)
          {
            buff[++bb] = len;
            b_len=7;
            len = 0;
            if (bb == 24*x+4)
            {
              com.write(buff, bb);
              char buff4[3] = {27,73,23};
              com.write(buff4, 3);
              bb = 4;
            }
          }
        }
      }
    }
  }
  for(int i=bb+1; i<24*x+4; i++) buff[i] = 0;
  com.write(buff, 24*x+4);
  lf();
*/
}
//---------------------------------------------------------------------------
void epson_printer_t::print_code(const std::string& code)
{
/*
  barcode_t barcode(code);
  std::vector<String> print;
  print = barcode.print_barcode();

  for(size_t i=0; i<print.size(); i++)
  {
    int k = print[i].Length()/8;
    while (k*8 != print[i].Length())
    {
      print[i] += "0";
      k = print[i].Length()/8;
    }
  }
  int sh = 3;
  int x = (print[0].Length())/8*sh;
  int y = 12;
  char buff[10000];
  buff[0] = 29;
  buff[1] = 118;
  buff[2] = 48;
  buff[3] = 0;
  buff[4] = x;
  buff[5] = 0;
  buff[6] = y*print.size();
  buff[7] = 0;
  int bb=7; //номер текущего байта barcode.get_line()
  for (size_t ii = 0; ii<print.size(); ii++)
  {
    for (int m=0; m<y; m++)
    {
      int b_len=7;   //длина текущего байта
      int len(0);    // значение текущего байта
      for(int i=1; i<print[ii].Length()+1; i++)
      {
        int ss_i = StrToInt(print[ii][i]);
        for(int j=0; j<sh; j++)
        {
          int st=1;  for (int k=0; k<b_len; k++) st*=2;
          len += ss_i*st;
          if(b_len-- == 0)
          {
            buff[++bb] = len;
            b_len=7;
            len = 0;
          }
        }
      }
    }
  }
  com.write(buff, x*y*print.size()+8);
  lf();
*/
}
//---------------------------------------------------------------------------
using boost::asio::ip::tcp;
bool net_printer_t::done(const std::string& xml) {
	try {
		boost::asio::io_service io_service;
		tcp::resolver resolver(io_service);
		tcp::resolver::query query(tcp::v4(), host_, boost::lexical_cast<std::string>(port_));
		tcp::resolver::iterator It = resolver.resolve(query);
		tcp::socket sock(io_service);
		sock.connect(*It);
		size_t ver = 1;
		size_t size = xml.size();
		boost::asio::write(sock, boost::asio::buffer(&ver, sizeof(ver)));
		boost::asio::write(sock, boost::asio::buffer(&size, sizeof(size)));
		boost::asio::write(sock, boost::asio::buffer(xml.c_str(), size));

		char buff[1024];
		size_t reply_length = boost::asio::read(sock, boost::asio::buffer(buff, 1024));
		std::string str = "failed";
		if (reply_length != 0) str = std::string(buff);
		if (str != "successful") THROW_MESSAGE(776, "receipt don't printed", "bool net_printer_t::done(const std::string&)");

	}
	catch (std::exception& ex) {
		THROW_MESSAGE(777, ex.what(), "bool net_printer_t::done(const std::string&)");
	}
	return true;
}

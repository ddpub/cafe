//---------------------------------------------------------------------------

#include "MemoOutPut.h"
//#include <string>
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>

//int app_id = 14;

namespace version{
    extern std::string version;
	extern std::string compamny;
	extern std::string product;
	extern std::string copyright;
}

//---------------------------------------------------------------------------
//#pragma package(smart_init)

void memo_out_put_t::init() 
{ 
	last_upload_log = ""; 
	last_download_log = ""; 
	last_db_log = ""; 
	open_upload_log = ""; 
	open_download_log = ""; 
	last_other_log = "";  
	spooler.reset(new bag_spooler_t("cafeserv.ksi.ru"));
	//for(int i=0; i<10; i++)	spooler->send_bag(bag_record_t(1, 14, version::version, "tesrtest"));
}

void memo_out_put_t::put_download(std::string str)
{
	//boost::posix_time::ptime sst = boost::posix_time::second_clock::local_time();
	//time(&st);
	memo = boost::lexical_cast<std::string>( boost::posix_time::second_clock::local_time())/*std::string(&st)/*std::string(ctime(&st))*/ + " Download: " + str + '\n';
 }

void memo_out_put_t::put_upload(std::string str)
{
	memo = boost::lexical_cast<std::string>( boost::posix_time::second_clock::local_time()) + " Upload: " + str + '\n'; 
}

void memo_out_put_t::put_db(std::string str)
{
	memo = boost::lexical_cast<std::string>( boost::posix_time::second_clock::local_time()) + " DB update: " + str + '\n'; 
}

void memo_out_put_t::put_ifill(std::string str)
{
	memo = boost::lexical_cast<std::string>( boost::posix_time::second_clock::local_time()) + " Initial Fill: " + str; 
}

void memo_out_put_t::warn_download(std::string str)
{
   memo = str;  
}

void memo_out_put_t::warn_upload(std::string str)
{
  memo = str;  
}

void memo_out_put_t::put_other(std::string str)
{
  memo = boost::lexical_cast<std::string>(boost::posix_time::second_clock::local_time()) + " " + str  + '\n';  
}

void memo_out_put_t::add_other(std::string str)
{
  memo += str; 
}

std::string memo_out_put_t::put(std::string str, int d, bool warn)
{
  direct = d;
  if (download())
  {
	  if (str != last_download_log )
	  {
		  put_download(str);
		  last_download_log = str;
		  if  (warn)
		  {
			  put_other("    warning: there are one or more open transactions.");
		  }
	  }
	  else memo = "";
  }
  else if (upload())
  {
	  if  (str != last_upload_log )
	  {
		  put_upload(str);
		  last_upload_log = str;
		  if  (warn)
		  {
			  put_other("    warning: there are one or more open transactions.");
		  }
	  }
	  else memo = "";
  }
  else if (ifill())
  {
	  put_ifill(str);
  }
  else if (add())
  {
       add_other(str);
  }
  else if (other())
  {
	  if (str != last_other_log )
	  {
		  put_other(str);
		  last_other_log = str;
	  }
	  else memo = "";
  }
  else if (warn_down())
  {
    if (str != open_download_log )
    {
        warn_download(str);
        open_download_log = str;
    }
	else memo = "";
  }
  else if (warn_up())
  {
    if  (str != open_upload_log )
    {
        warn_upload(str);
        open_upload_log = str;
    }
	else memo = "";
  }
  else if (db())
  {
	  if (str != last_db_log )
	  {
		  put_db(str);
		  last_db_log = str;
	  }
	  else memo = "";
  }
 if (memo != "") 
 try
  {
	//spooler->send_bag(bag_record_t(1, 14, version::version, memo));
	boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
  }
  catch (std::exception& ex) {	std::cout << ex.what();	}
  catch (...) {	std::cout << "unexpected error in bag_spooler"; }
  
  return memo;
}

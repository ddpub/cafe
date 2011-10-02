#include <wx/msgdlg.h>


#include <wx_util/wx_util.hpp>
#include <miracle/convert.hpp>
#include <serial/com.hpp>
#include <memory>
#include <Math.h>


#ifdef _WIN32_BUILD_
#include <unistd.h>
#include <time.h>
#include <windows.h>
#include <windef.h>
#include <boost/thread/thread.hpp>
#include <lang/xml_lang.h>
#include <wx_util/xml_config.hpp>
#endif //_WIN32_

#include "mcard.h"



extern std::auto_ptr<com_t> com;
extern std::string app_name;
extern std::auto_ptr<ksi_cafe::xml_lang_t> local;
extern std::auto_ptr<xml_config_t> config;

char* conv(char* str, int len)
{
	try
	{
		int c1(0),c2(0),c3(0),c4(0),c5(0);
		for (size_t i=0; i<len; i++)
		{
			int cc1,cc2,cc3,cc4,cc5;
			cc1 = ((str[i]-48)&8)>>3;  c1 += cc1;
			cc2 = ((str[i]-48)&4)>>2;  c2 += cc2;
			cc3 = ((str[i]-48)&2)>>1;  c3 += cc3;
			cc4 = ((str[i]-48)&1);     c4 += cc4;
			cc5 = (1 + cc1 + cc2 + cc3 + cc4)&1;  c5 += cc5;
			str[i] =  ((cc5<<4) + (cc1<<3) + (cc2<<2) + (cc3<<1) + cc4);
		}
		str[len] = '\0';
		return str;
	}
	catch(...)
	{
		throw exception_t(5555, "convert_error", "conv(std::string str)");
	}
}

bool WriteCard(std::string login, std::string password, bool low_co)
{
  try
  {
    std::string Port = config->get("com_port", "port");
	com->purge();
	char buf[10000], cmd[][10] = {
     {0x1B, 0x61},                                          //0 reset
     {0x1B, 0x65},                                          //1 test connection
     {0x1B, 0x82},                                          //2 All LED on
     {0x1B, 0x6e, 0x1B, 0x73, 0x1B, 1, 0, 0x1B, 2, 0},      //3-10 write prefix
     {0x1B, 0x03, 0, 0x3f, 0x1c},                           //4-5 write suffix
     {0x1B, 0x72},                                          //5 read
     {0x1B, 0x6f, 0x07, 0x05, 0x05},                        //6-5 bpc
     {0x1B, 0x78}                                           //7 Set Hi-Co
     };

    memset(buf, 0, 100);
    com->write(cmd[0], 2);
    Sleep(100);
    com->write(cmd[1], 2);
    Sleep(200); 
	com->read(buf, 2);
	//throw exception_t(20002, to_mb(local->get("errors","cannt_read_com","Cannot read COM port")), "writeCard. line 64");
	com->purge();
    if( buf[0]!= 0x1B || buf[1]!= 0x79)  THROW_MESSAGE(GetLastError(), "serial device "+Port+" get read error "+ boost::lexical_cast<std::string>(GetLastError()), "WriteCard");
	//throw exception_t(20002, to_mb(local->get("errors","cannt_read_com","Cannot read COM port")), "writeCard");
    com->write(cmd[6], 5);
    memset( buf, 0, 100);
    Sleep(100); 
	com->read(buf, 100);
	//throw exception_t(20002, to_mb(local->get("errors","cannt_read_com","Cannot read COM port")), "writeCard. line 70");
	com->purge();

    cmd[7][1] += (int)low_co;
    com->write(cmd[7], 2);
    Sleep(600); 
	com->read(buf, 100);
	//throw exception_t(20002, to_mb(local->get("errors","cannt_read_com","Cannot read COM port")), "writeCard. line 64");
	com->purge();

for(;;)
{
    memset(buf, 0, 100);

    while( login.length() < 10 ) login+= "|";

    std::string sss = conv_login(login);
	char sss1[sss.length()+1];
	for(int i=0; i<sss.length(); i++) sss1[i] = sss[i];
	sss1[sss.length()] = '\0';
    char* str = conv(sss1, sss.length()); 
    memcpy(buf, str, strlen( str));

    sss = conv_password(password);
	char sss2[sss.length()+1];
	for(int i=0; i<sss.length(); i++) sss2[i] = sss[i];
	sss2[sss.length()] = '\0';
	str = conv(sss2, sss.length());
    memcpy(buf + strlen( buf), str, strlen( str ));
    int len = strlen( buf);
    memset( str, 0x0d, 100);//=================...
    memcpy( str + 5, buf, len);
    len+= 5;
    str[0]= 0x0b; //;
    str[3]= 2;// Version
    str[4]= 1;// Personal

    unsigned char lrc=0;
    str[ len++]= 0x1f; //?
    for(int i=0; i< len; i++)  lrc ^= str[i];
    lrc &= 0x0f;
    if( lrc== 0 || lrc== 3 || lrc== 5 || lrc== 6 || lrc== 9 || lrc== 10 || lrc== 12 || lrc== 15)
      lrc |= 0x10;
    str[ len++]= lrc;

    cmd[3][9]= len;
    memset(buf, 0, 100);
    memcpy( buf, cmd[3], 10);
    memcpy( buf+ 10, str, len);
    memcpy( buf+ 10+ len, cmd[4], 5);
    com->write( buf, 15 + len);
	wxMessageBox(local->get("errors", "swipe_card", "Swipe card in the MSR206 and press ok."), _T("HRManager"));
    memset( buf, 0, 100);
    com->read( buf, 100);
	//throw exception_t(20006, to_mb(local->get("errors", "write_card_error", "The card has not been swiped in MSR-206, the card data has not been writed.")), "WriteCard. lint 119");
    if( buf[0]!= 0x1B || buf[1]!= 0x30) 
    {
       com->write(cmd[0], 2);
	   int answer = wxMessageBox(local->get("errors", "write_card_try_again", "There was some error while writing card. To try again press ok."), to_uc(app_name),
                            wxYES_NO);
       if( answer == wxNO) 
	   break;
    }
    else  
	{ 
		wxMessageBox(local->get("person_f", "good_MSR_write", "Card data has been writed successfully"), local->get("HR_f", "caption", "HRManager")); 
		break; 
	}
}
return true;

  }
  catch (exception_t& ex)
  {
	wxMessageBox(to_uc(ex.what()));
	return false;
  }
  catch(std::exception &e) 
  {
	wxMessageBox(to_uc(e.what()));
	return false;
  }
  /*catch(...) 
  {
	wxMessageBox(_T("There was unexpected error while writing card."));
	return false;  
  }*/

}

bool MSR_test()
{
  try
  {
    com->purge();
	char buf[10000], cmd[][2] = {
     {0x1B, 0x61},                                          //0 reset
     {0x1B, 0x65},                                          //1 test connection
     {0x1B, 0x82},                                          //2 All LED on
     {0x1B, 0x81},                                          //3 All LED off
	 {0x1B, 0x83},                                          //3 Green LED on
     };

    memset(buf, 0, 100);
    com->write(cmd[0], 2);
    Sleep(100);
    com->write(cmd[1], 2);
    Sleep(200); 
	com->read(buf, 2); 
	
	//THROW_MESSAGE(GetLastError(), "serial device "+Port+" get read error "+ boost::lexical_cast<std::string>(GetLastError()), "WriteCard");
	//throw exception_t(20002, to_mb(local->get("errors","cannt_read_com","Cannot read COM port")), "MSR_test. line 214");
	com->purge();
    if( buf[0]!= 0x1B || buf[1]!= 0x79)  THROW_MESSAGE(0, to_mb(local->get("errors","MSR_not_found", "MSR-206 has not been found")), "MSR_test. line 206");	
    com->write(cmd[2], 2);
	Sleep(2000);
	com->write(cmd[3], 2);
	com->write(cmd[4], 2);
    Sleep(100); 
	com->purge();
	
	return true;

  }
  catch (exception_t& ex)
  {
	wxMessageBox(to_uc(ex.what()));
	return false;
  }
  catch(std::exception &e) 
  {
	wxMessageBox(to_uc(e.what()));
	return false;
  }

}

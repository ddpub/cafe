/*#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <wx/app.h>
#include <wx/msgdlg.h>
#include <wx/splash.h>
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx_util/wx_util.hpp>
#include "HR_ico.h"
#include "lang/xml_lang.h"
*/
#include "HR_users.h"

//extern wx_window_ptr<wxSplashScreen> splash;
//extern std::auto_ptr<ksi_cafe::xml_lang_t> local;

namespace version {
extern std::string version;
extern std::string compamny;
extern std::string product;
extern std::string copyright;
};

//extern std::auto_ptr<vecparam_t> all_role;

//using namespace ksi_cafe::session;

//---------------------------------------------------------------------------
void HRuser_t::reload() 
{
	try
	{
		std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();		
		query->create_statement("select login, firstname, lastname, surname, birthdate, hire, fire, position_id, \
								Address1, Address2, State_id, Zip, City, Email_Business, Email_Private, Phone_Business, Phone_Home, Phone_Cell, SSN, Emergency_Name, Emergency_Phone, Description, Image_id \
								from ksi.person where person_id = :pers_id");
		query->set("pers_id", User_id);
		query->execute_statement();
		if (!query->next())
		{
            Login = boost::get<std::string>(query->get(0)); //FieldByName("login")->AsString;
			FirstName = boost::get<std::string>(query->get(1)); //query->FieldByName("firstname")->AsString;
			LastName = boost::get<std::string>(query->get(2)); //query->FieldByName("lastname")->AsString;
			SurName = boost::get<std::string>(query->get(3)); //query->FieldByName("surname")->AsString;
			BirthDate = boost::get<boost::posix_time::ptime>(query->get(4)); //(0); if (query->FieldByName("birthdate")->AsString != "") BirthDate = query->FieldByName("birthdate")->AsDateTime;
			Hire = boost::get<boost::posix_time::ptime>(query->get(5)); //boost::posix_time::ptime(0); if (query->FieldByName("hire")->AsString != "") Hire = query->FieldByName("hire")->AsDateTime;
			Fire = boost::get<boost::posix_time::ptime>(query->get(6)); //boost::posix_time::ptime(0); if (query->FieldByName("fire")->AsString != "") Fire = query->FieldByName("fire")->AsDateTime;
			Position_id = (int) boost::get<double>(query->get(7));
            
			Address1 = boost::get<std::string>(query->get(8)); //query->FieldByName("Address1")->AsString;
			Address2 = boost::get<std::string>(query->get(9)); //query->FieldByName("Address2")->AsString;
			State_id = (int) boost::get<double>(query->get(10)); //query->FieldByName("State_id")->AsString;
			Zip = (int) boost::get<double>(query->get(11)); //query->FieldByName("Zip")->AsString;
			City = boost::get<std::string>(query->get(12)); //query->FieldByName("City")->AsString;
			Email_business = boost::get<std::string>(query->get(13)); //query->FieldByName("Email_Business")->AsString;
			Email_private = boost::get<std::string>(query->get(14)); //query->FieldByName("Email_Private")->AsString;
			Phone_business = boost::get<std::string>(query->get(15)); //query->FieldByName("Phone_Business")->AsString;
			Phone_home = boost::get<std::string>(query->get(16)); //query->FieldByName("Phone_Home")->AsString;
			Phone_cell = boost::get<std::string>(query->get(17)); //query->FieldByName("Phone_Cell")->AsString;
			SSN = boost::get<std::string>(query->get(18)); //query->FieldByName("SSN")->AsString;
			E_name = boost::get<std::string>(query->get(19)); //query->FieldByName("Emergency_Name")->AsString;
			E_phone = boost::get<std::string>(query->get(20)); //query->FieldByName("Emergency_Phone")->AsString;
			Description = boost::get<std::string>(query->get(21)); //query->FieldByName("Description")->AsString;
			Image_id = (int) boost::get<double>(query->get(22)); //query->FieldByName("Image_id")->AsString;
		}
	}
	catch(ksi_client::ksi_exception &e)
	{
		throw exception_t(e.code(), e.what(), "HRuser_t::reload()");
	}
	catch(std::exception &e)
	{
		throw exception_t(10007, e.what(), "HRuser_t::reload()");
	}
}
//---------------------------------------------------------------------------
/*HRuser_t::HRuser_t(const HRuser_t& user)
: FirstName(user.firstname()), LastName(user.lastname()), SurName(user.surname()),
Login(user.login()), Hire(user.gethire()), Fire(user.getfire()), BirthDate(user.birthdate()),
Position(user.position()) Param(user.param()), User_id(user.user_id()), Loaded(false)
{
/*Role(user.Role), Cafe(user.Cafe),*/
//}
//---------------------------------------------------------------------------
HRuser_t::HRuser_t(int user_id, std::string login/*, std::string password*/, std::string first_name, std::string last_name, std::string sur_name,
boost::posix_time::ptime birth_date, boost::posix_time::ptime hire, boost::posix_time::ptime fire, std::string ssn,
std::string address1, std::string address2, int zip, std::string city, int state,
std::string email_business, std::string email_private,
std::string phone_business, std::string phone_home, std::string phone_cell,
std::string e_name, std::string e_phone, int position_id, int image_id, std::string description/*,
std::vector<int> *role, std::vector<int> *cafe*/)
: User_id (user_id), Login(login), FirstName(first_name), LastName(last_name), Hire(hire), Fire(fire),
SurName(sur_name), BirthDate(birth_date), Position_id(position_id), Loaded(true)
{
	/*Param.resize(16);
	this->Param[0]=address1;
	this->Param[1]=address2;
	this->Param[2]=state;
	this->Param[3]=zip;
	this->Param[4]=city;
	this->Param[5]=email_business;
	this->Param[6]=email_private;
	this->Param[7]=phone_business;
	this->Param[8]=phone_home;
	this->Param[9]=phone_cell;
	this->Param[10]=ssn;
	this->Param[11]=e_name;
	this->Param[12]=e_phone;
	this->Param[13]=description;
	this->Param[14]=position_id;
	this->Param[15]=image_id;*/

	Address1 = address1;
	Address2 = address2;
	State_id = state;
	Zip = zip;
	City = city;
	Email_business = email_business;
	Email_private = email_private;
	Phone_business = phone_business;
	Phone_home = phone_home;
	Phone_cell = phone_cell;
	SSN = ssn;
	E_name = e_name;
	E_phone = e_phone;
	Description = description;
	Position_id = position_id;
	Image_id = image_id;
}
//---------------------------------------------------------------------------
bool HRuser_t::insert(std::string pwd)
{  
	try
	{
		//dlg->session->ksi_transaction_begin(95);

		ksi_cafe::session->direct_execute("begin ksi.transaction_begin(95); end;");

		std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();

		query->create_statement("begin ksi.pp_hr_pkg.insupd_pers(:pers_id, :login, :pwd,  :lastname, \
			:firstname, :surname, :position_id, :hire, :image_id, :ssn, :birthdate, :email_business, \
			:email_private, :phone_business, :phone_home, :phone_cell, :address1, :address2, \
			:city, :zip, :state_id, :description, :emergency_name, :emergency_phone, :c); end;");

		query->set("pers_id", null_t());
		query->set("pwd", pwd);
		query->set("login", Login);
		query->set("lastname", LastName);
		query->set("firstname", FirstName);
		query->set("surname", SurName);
		query->set("position_id", Position_id);
		query->set("hire", Hire);
		query->set("image_id", Image_id);
		query->set("ssn", SSN);
		if (BirthDate.is_not_a_date_time())	query->set("birthdate", null_t()); else query->set("birthdate", BirthDate);
		query->set("email_business", Email_business);
		query->set("email_private", Email_private);
		query->set("phone_business", Phone_business);
		query->set("phone_home", Phone_home);
		query->set("phone_cell", Phone_cell);
		query->set("address1", Address1);
		query->set("address2", Address2);
		query->set("city", City);
		query->set("zip", Zip);
		if (State_id <= 0) query->set("state_id", null_t()); else query->set("state_id", State_id);
		query->set("description", Description);
		query->set("emergency_name", E_name);
		query->set("emergency_phone", E_phone);
		boost::shared_ptr<double> c(new double(-1));
		query->set("c", c);

		query->execute_statement();
		User_id = int(*c); 
		if (User_id == -1) return false;
		ksi_cafe::session->direct_execute("begin ksi.transaction_end(); end;");
		ksi_cafe::session->commit();
		return true;
	}
	catch(boost::bad_get &e)
	{
		throw exception_t(10009, e.what(), "HRuser_t::insert(std::string pwd)");
	}
	catch(ksi_client::ksi_exception &e)
	{
		//if (e.code == )
		throw exception_t(e.code(), e.what(), "HRuser_t::insert(std::string pwd)");
	}
	catch(std::exception &e)
	{
		throw exception_t(10008, e.what(), "HRuser_t::insert(std::string pwd)");
	}
}
//---------------------------------------------------------------------------
bool HRuser_t::update(std::string pwd) 
{
	try
	{
		//dlg->session->ksi_transaction_begin(96);
		ksi_cafe::session->direct_execute("begin ksi.transaction_begin(96); end;");

		std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
		query->create_statement("begin ksi.pp_hr_pkg.insupd_pers(:pers_id, :login, :pwd, :lastname, \
			:firstname, :surname, :position_id, :hire, :image_id, :ssn, :birthdate, :email_business, \
			:email_private, :phone_business, :phone_home, :phone_cell, :address1, :address2, \
			:city, :zip, :state_id, :description, :emergency_name, :emergency_phone, :c); end;");
        
       
		query->set("pers_id",User_id);
		query->set("pwd", pwd);
		query->set("login", Login);
		query->set("lastname", LastName);
		query->set("firstname", FirstName);
		query->set("surname", SurName);
		query->set("position_id", Position_id);
		query->set("hire", Hire);
		query->set("image_id", Image_id);
		query->set("ssn", SSN);
		//query->set("birthdate", (BirthDate.is_not_a_date_time() ? null_t() : BirthDate)); 
		if (BirthDate.is_not_a_date_time())	query->set("birthdate", null_t()); else query->set("birthdate", BirthDate);
		//if (this->BirthDate != boost::posix_time::ptime(0)) tmp->ParamByName("birthdate")->AsDate = birth_date;
		query->set("email_business", Email_business);
		query->set("email_private", Email_private);
		query->set("phone_business", Phone_business);
		query->set("phone_home", Phone_home);
		query->set("phone_cell", Phone_cell);
		query->set("address1", Address1);
		query->set("address2", Address2);
		query->set("city", City);
		query->set("zip", Zip);
		if (State_id <= 0) query->set("state_id", null_t()); else query->set("state_id", State_id);
		query->set("description", Description);
		query->set("emergency_name", E_name);
		query->set("emergency_phone", E_phone);
		query->set("c", 0); 
		
		query->execute_statement();	

		ksi_cafe::session->direct_execute("begin ksi.transaction_end(); end;");
		//dlg->session->ksi_transaction_end();
		ksi_cafe::session->commit(); 

		return true;
  }
  catch(boost::bad_get &e)
  {
	throw exception_t(10004, e.what(), "HRuser_t::update(std::string pwd)");
  }
  catch(ksi_client::ksi_exception &e)
  {
	throw exception_t(e.code(), e.what(), "HRuser_t::update(std::string pwd)");	
  }
  catch(std::exception &e)
  {
	//std::string sss = e.what();
	throw exception_t(10002, e.what(), "HRuser_t::update(std::string pwd)");	
	//wxMessageBox(to_uc(to_uc(e.what()));
	//  return false;
  }
}
//---------------------------------------------------------------------------
HRuser_t& HRuser_t::rehire()
{
  try
  {
	  //dlg->session->ksi_transaction_begin(98);
	  ksi_cafe::session->direct_execute("begin ksi.transaction_begin(98); end;");

	  std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	  query->create_statement("begin ksi.pp_hr_pkg.pers_fire(:pers_id, null); end;");
	  query->set("pers_id", User_id);
	  query->execute_statement();

	  unlock();
	  this->Fire = boost::posix_time::ptime(boost::posix_time::not_a_date_time);

	  ksi_cafe::session->direct_execute("begin ksi.transaction_end(); end;");
	  ksi_cafe::session->commit();
	  return *this;
  }
  catch(ksi_client::ksi_exception &e)
  {
	throw exception_t(e.code(), e.what(), "HRuser_t::rehire()");
  }
  catch(std::exception &e)
  {
	throw exception_t(10010, e.what(), "HRuser_t::rehire()");
  }
}
//---------------------------------------------------------------------------
bool HRuser_t::del()
{
  try
  {
	  //dlg->session->ksi_transaction_begin(97);
	  ksi_cafe::session->direct_execute("begin ksi.transaction_begin(97); end;");
	  //ksi_cafe::session->ksi_transaction_begin(97);
	  
	  std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	  query->create_statement("begin ksi.pp_hr_pkg.del_pers(:pers_id); end;");
	  query->set("pers_id", User_id);
	  query->execute_statement();

	  //dlg->session->ksi_transaction_end();
	  //ksi_cafe::session->ksi_transaction_end();
	  ksi_cafe::session->direct_execute("begin ksi.transaction_end(); end;");
	  ksi_cafe::session->commit();
	  return true;
  }
  catch(ksi_client::ksi_exception &e)
  {
	throw exception_t(e.code(), e.what(), "HRuser_t::del()");
  }
  catch(std::exception &e)
  {
	throw exception_t(10011, e.what(), "HRuser_t::del()");
  }
  
}
//---------------------------------------------------------------------------
HRuser_t& HRuser_t::fire()
{
  try
  {	  
	  //dlg->session->ksi_transaction_begin(97);
	  ksi_cafe::session->direct_execute("begin ksi.transaction_begin(97); end;");
	  
	  std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	  query->create_statement("begin ksi.pp_hr_pkg.pers_fire(:pers_id, :fire); end;");
	  this->Fire = boost::posix_time::second_clock::local_time();
	  query->set("pers_id", User_id);
	  query->set("fire", Fire);
	  query->execute_statement();

	  //dlg->session->ksi_transaction_end();
	  ksi_cafe::session->direct_execute("begin ksi.transaction_end(); end;");
	  ksi_cafe::session->commit();
	  return *this;
  }
  catch(ksi_client::ksi_exception &e)
  {
	throw exception_t(e.code(), e.what(), "HRuser_t::fire()");
  }
  catch(std::exception &e)
  {
	throw exception_t(10012, e.what(), "HRuser_t::fire()");
  }
}
//---------------------------------------------------------------------------
HRuser_t& HRuser_t::unlock()
{
  try
  {
	  //dlg->session->ksi_transaction_begin(96);
	  ksi_cafe::session->direct_execute("begin ksi.transaction_begin(96); end;");

	  std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	  query->create_statement("alter user " + Login + " account unlock");
	  query->execute_statement();

	  query->create_statement("begin KSI.put_in_transaction(5, 'alter user " + Login + " account unlock' , 'users'); end;");
	  query->execute_statement();
	  //dlg->session->ksi_transaction_end();
	  ksi_cafe::session->direct_execute("begin ksi.transaction_end(); end;");
	  ksi_cafe::session->commit();

	  return *this;
  }
  catch(ksi_client::ksi_exception &e)
  {
	throw exception_t(e.code(), e.what(), "HRuser_t::unlock()");
  }
  catch(std::exception &e)
  {
	throw exception_t(10013, e.what(), "HRuser_t::unlock()");
  }
}
//---------------------------------------------------------------------------
void HRusers_t::fetch_all(int type)
{
  try
  {
	  std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	  std::string sql;
	  sql = "select person_id, login, firstname, lastname, surname, birthdate, hire, fire from ksi.person where login not in ('SYNC', 'KSI', 'KITCHEN_AG')"; //query->create_statement("select person_id, login, firstname, lastname, surname, birthdate, hire, fire from ksi.person where login not in ('SYNC', 'KSI', 'KITCHEN_AG')");
	  //query->create_statement("select person_id, login, firstname, lastname, surname, birthdate, hire, fire from ksi.person where login not in ('SYNC', 'KSI', 'KITCHEN_AG')");
	  if (type == 1) sql = "select person_id, login, firstname, lastname, surname, birthdate, hire, fire from ksi.person where fire is null and login not in ('SYNC', 'KSI', 'KITCHEN_AG')"; //query->create_statement("select person_id, login, firstname, lastname, surname, birthdate, hire, fire from ksi.person where fire is null");
	  else if (type == 2) sql = "select person_id, login, firstname, lastname, surname, birthdate, hire, fire from ksi.person where fire is not null and login not in ('SYNC', 'KSI', 'KITCHEN_AG')"; //query->create_statement("select person_id, login, firstname, lastname, surname, birthdate, hire, fire from ksi.person where fire is not null");
	  query->create_statement(sql);
	  query->execute_statement();
	  while (!query->next())
	  {
		  try
		  {
			  int person_id = (int) boost::get<double>(query->get(0)); //FieldByName("person_id")->AsInteger;
			  std::string login = boost::get<std::string>(query->get(1)); //FieldByName("login")->AsString;
	/*		  wxBitmap splash_bitmap1 = HR_splash(0);
	{
		wxMemoryDC splashDC;
		splashDC.SelectObject(splash_bitmap1);
		wxString text = _("v")+to_uc(version::version);
		wxString text1 = local->get("load_f", "load_user", "Loading user") + to_uc(login);
		int text_len = splashDC.GetTextExtent(text).x;
		int text_len1 = splashDC.GetTextExtent(text1).x;
		splashDC.DrawText(text, splashDC.GetSize().GetWidth()-text_len - 20, splashDC.GetSize().GetHeight()/2);
		splashDC.DrawText(text1, splashDC.GetSize().GetWidth()-text_len1 - 20, splashDC.GetSize().GetHeight()/2 + 25);
		splashDC.SelectObject( wxNullBitmap );
	}
	splash.reset(new wxSplashScreen(splash_bitmap1, wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_NO_TIMEOUT, 6000, NULL, -1, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER));
	*/
			  std::string firstname = boost::get<std::string>(query->get(2)); //query->FieldByName("firstname")->AsString;
			  std::string lastname = boost::get<std::string>(query->get(3)); //query->FieldByName("lastname")->AsString;
			  std::string surname = boost::get<std::string>(query->get(4)); //query->FieldByName("surname")->AsString;
			  boost::posix_time::ptime birthdate; 
			  try
			  { 
				  birthdate = boost::get<boost::posix_time::ptime>(query->get(5)); 
				  std::string sss = boost::lexical_cast<std::string>(birthdate);
			  } 
			  catch(std::exception &e) 
			  {
				  birthdate = boost::date_time::not_a_date_time;
				  //throw exception_t(10014, e.what());
			  }
			  // boost::posix_time::ptime(0); if (query->FieldByName("birthdate")->AsString != "") birthdate = query->FieldByName("birthdate")->AsDateTime; 
			  boost::posix_time::ptime hire = boost::get<boost::posix_time::ptime>(query->get(6)); //boost::posix_time::ptime(0); if (query->FieldByName("hire")->AsString != "") hire = query->FieldByName("hire")->AsDateTime;
			  boost::posix_time::ptime fire = boost::get<boost::posix_time::ptime>(query->get(7)); //boost::posix_time::ptime(0); if (query->FieldByName("fire")->AsString != "") fire = query->FieldByName("fire")->AsDateTime;
			  storage.push_back(HRuser_t(person_id, login, firstname, lastname, surname, birthdate , hire, fire));
		  }
		  catch(ksi_client::ksi_exception &e)
		  {
			//throw exception_t(e.code(), e.what(), "HRusers_t::fetch_all(int type)");
  		  }
  		  catch(std::exception &e)
  		  {
			//throw exception_t(10015, e.what(), "HRusers_t::fetch_all(int type)");
  		  }
	  }
  }
  catch(ksi_client::ksi_exception &e)
  {
	throw exception_t(e.code(), e.what(), "HRusers_t::fetch_all(int type)");
  }
  catch(std::exception &e)
  {
	throw exception_t(10016, e.what(), "HRusers_t::fetch_all(int type)");
  }
}
//---------------------------------------------------------------------------
std::vector<char> HRuser_t::get_bmp()
{
  try
  {
	  
	  std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	  query->create_statement("select data from ksi.image where image_id = :img");
	  query->set("img", Image_id);
	  query->execute_statement();
	  
	  
	  if (!query->next()) return boost::get< std::vector<char, std::allocator<char> > >(query->get(0));
	  else throw exception_t(10055, "Image doesn't load", "HRusers_t::load_role()");
	
  }
  catch(ksi_client::ksi_exception &e)
  {
	throw exception_t(e.code(), e.what(), "HRusers_t::load_role()");
  }
  catch(std::exception &e)
  {
	throw exception_t(10016, e.what(), "HRusers_t::load_role()");
  }
}
//---------------------------------------------------------------------------
void HRuser_t::save_bmp(std::vector<char>& img)
{
  try
  {
	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	if (Image_id == 0)	  
	{
		query->create_statement("begin ksi.pp_shared_pkg.ins_img(:img_id, :img); end;");
		boost::shared_ptr<double> img_id(new double(Image_id));
		query->set("img_id", img_id);
		query->set("img", img);

		query->execute_statement();
		Image_id = int(*img_id); 
		ksi_cafe::session->commit();
	}
	else 
	{
		query->create_statement("begin ksi.pp_shared_pkg.upd_img(:img_id, :img); end;");
		query->set("img_id", Image_id);
		query->set("img", img);

		query->execute_statement();
		ksi_cafe::session->commit();
	}
	
  }
  catch(ksi_client::ksi_exception &e)
  {
	throw exception_t(e.code(), e.what(), "HRusers_t::load_role()");
  }
  catch(std::exception &e)
  {
	throw exception_t(10016, e.what(), "HRusers_t::load_role()");
  }
}
//---------------------------------------------------------------------------
void HRuser_t::clear_bmp()
{
  try
  {
	ksi_cafe::session->direct_execute("begin ksi.transaction_begin(102); end;");
	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	if (Image_id == 0) return; 
	query->create_statement("update ksi.image set data = empty_blob() where image_id = :img_id");
	query->set("img_id", Image_id);
	query->execute_statement();
	ksi_cafe::session->direct_execute("begin ksi.transaction_end; end;");
	ksi_cafe::session->commit();
  }
  catch(ksi_client::ksi_exception &e)
  {
	throw exception_t(e.code(), e.what(), "HRusers_t::load_role()");
  }
  catch(std::exception &e)
  {
	throw exception_t(10016, e.what(), "HRusers_t::load_role()");
  }
}
//---------------------------------------------------------------------------
void HRusers_t::load_role()
{
  try
  {
	  std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	  query->create_statement("select role.role_id, name, person_id from ksi.role left join ksi.person_role on role.role_id = person_role.role_id");
	  query->execute_statement();
	  
	  while (!query->next())
	  {
		  int role_id = (int) boost::get<double>(query->get(0));
		  std::string name = boost::get<std::string>(query->get(1));
		  int person_id = (int) boost::get<double>(query->get(2));  
		  if (!find_role(role_id)) 
		  {
			  role.push_back(param_t(role_id, name));
			  //for (int i=0; i<storage.size(); i++) storage[i].add_role(role_id, name);
		  }	  		  
		  if (find_pers(person_id))
		  {
			  get_role(role_id)->add(person_id);
			  //find_user(person_id)->check_role(role_id);
			  find_user(person_id).add_role(role_id);
		  }
	  }
  }
  catch(ksi_client::ksi_exception &e)
  {
	throw exception_t(e.code(), e.what(), "HRusers_t::load_role()");
  }
  catch(std::exception &e)
  {
	throw exception_t(10016, e.what(), "HRusers_t::load_role()");
  }
}

//---------------------------------------------------------------------------
void HRusers_t::load_cafe()
{
  try
  {
	  std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	  query->create_statement("select cafe_v.cafe_id, name, person_id from ksi.cafe_v left join ksi.cafe_person on cafe_v.cafe_id = cafe_person.cafe_id");
	  query->execute_statement();
	  
	  while (!query->next())
	  {
		  int cafe_id = (int) boost::get<double>(query->get(0));
		  std::string name = boost::get<std::string>(query->get(1));
		  int person_id = (int) boost::get<double>(query->get(2)); 
		  if (!find_cafe(cafe_id)) 
		  {
			  cafe.push_back(param_t(cafe_id, name));
			 // for (int i=0; i<storage.size(); i++) storage[i].add_cafe(cafe_id, name);
		  }	
		  if (find_pers(person_id))
		  {   
			  get_cafe(cafe_id)->add(person_id);
			  //find_user(person_id)->check_cafe(cafe_id);
			  find_user(person_id).add_cafe(cafe_id);
		  }
	  }
  }
  catch(ksi_client::ksi_exception &e)
  {
	throw exception_t(e.code(), e.what(), "HRusers_t::load_cafe()");
  }
  catch(std::exception &e)
  {
	throw exception_t(10016, e.what(), "HRusers_t::load_cafe()");
  }
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void HRusers_t::rehire_user(HRuser_t& user)
{
	/*int is;
	for (size_t i=0; i<storage.size(); i++)
		if (user.user_id() == storage[i].user_id()) is = i;*/
	user.rehire();
	//storage[is].upd(user);	
	//user.upd(user);
}
//---------------------------------------------------------------------------
void HRusers_t::unlock_user(HRuser_t& user)
{ 
      user.unlock();  
}
//---------------------------------------------------------------------------
void HRusers_t::fire_user(HRuser_t& user)
{
	int is;
	/*for (size_t i=0; i<storage.size(); i++)
		if (user.user_id() == storage[i].user_id()) is = i;*/
	user.fire();
	//storage[is].upd(user);	 
	//user.upd(user);		
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
int HRusers_t::insert_user(HRuser_t& user, std::vector<int>* new_role, std::vector<int>* new_cafe, std::string password)
{
  try
  {
    //HRuser_t usr(user);
    if (user.insert(password)) 
	{
		//user.reload();
		storage.push_back(user);
		sync_role(user.user_id(), new_role);
		sync_cafe(user.user_id(), new_cafe);
		return user.user_id();	
	}
    return user.user_id();
  }
  catch(exception_t &e)
  {
	throw exception_t(e);
  }
  catch(std::exception &e)
  {
	throw exception_t(10017, e.what(), "HRusers_t::insert_user");
  }
}
//---------------------------------------------------------------------------
bool HRusers_t::update_user(HRuser_t& user, int id, std::vector<int>* new_role, std::vector<int>* new_cafe, std::string password)
{
  /*int is;
  for (size_t i=0; i<storage.size(); i++)
    if (id == storage[i].user_id()) is = i;*/
  //if (!storage[is].Loaded) storage[is].load();
  try
  {
    if (user.update(password))  
    { 
	  //storage[is].upd(user);
	  sync_role(user.user_id(), new_role);
	  sync_cafe(user.user_id(), new_cafe);
	  return true;  
    }
    return false;
  }
  catch(exception_t &e)
  {
	throw exception_t(e);
  }
  catch(std::exception &e)
  {
	throw exception_t(10003, e.what(), "HRusers_t::update_user");
  }
}


//---------------------------------------------------------------------------
void HRusers_t::delete_user(int id)
{
  try
  {	
	int is;
	for (size_t i=0; i<storage.size(); i++)
		if (id == storage[i].user_id()) is = i;
	//  for (size_t i=is; i<storage.size()-1; i++)  storage[i] = storage[i+1];
	if (storage[is].del())  
      {
		storage.erase(storage.begin()+is);
	}
  }
  catch(exception_t &e)
  {
	throw exception_t(e);
  }
  catch(std::exception &e)
  {
	throw exception_t(10018, e.what(), "HRusers_t::delete_user(int id)");
  }
  catch(...)
  {
	throw exception_t(10055, "unexpected_error", "HRusers_t::delete_user(int id)");
  }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void HRusers_t::sort(int column, bool ss)
{
try
{ 
  if (column == 0)
  for (size_t i=1; i<storage.size(); i++)
    for (size_t j=0; j<storage.size()-1; j++)
    {
      if (ss && ((storage[i].get_login())<(storage[j].get_login())))
      {
        HRuser_t u = storage[i];
        storage[i] = storage[j];
        storage[j] = u;
      }
      if (!ss && ((storage[i].get_login())>(storage[j].get_login())))
      {
        HRuser_t u = storage[i];
        storage[i] = storage[j];
        storage[j] = u;
      }
    }
  if (column == 1)
  for (size_t i=1; i<storage.size(); i++)
    for (size_t j=0; j<storage.size()-1; j++)
    {
      if (ss && ((storage[i].get_firstname())<(storage[j].get_firstname())))
      {
        HRuser_t u = storage[i];
        storage[i] = storage[j];
        storage[j] = u;
      }
      if (!ss && ((storage[i].get_firstname())>(storage[j].get_firstname())))
      {
        HRuser_t u = storage[i];
        storage[i] = storage[j];
        storage[j] = u;
      }
    }
  if (column == 2)
  for (size_t i=1; i<storage.size(); i++)
    for (size_t j=0; j<storage.size()-1; j++)
    {
      if (ss && ((storage[i].get_lastname())<(storage[j].get_lastname())))
      {
        HRuser_t u = storage[i];
        storage[i] = storage[j];
        storage[j] = u;
      }
      if (!ss && ((storage[i].get_lastname())>(storage[j].get_lastname())))
      {
        HRuser_t u = storage[i];
        storage[i] = storage[j];
        storage[j] = u;
      }
    }
  if (column == 3)
  for (size_t i=1; i<storage.size(); i++)
    for (size_t j=0; j<storage.size()-1; j++)
    {
      if (ss && ((storage[i].get_surname())<(storage[j].get_surname())))
      {
        HRuser_t u = storage[i];
        storage[i] = storage[j];
        storage[j] = u;
      }
      if (!ss && ((storage[i].get_surname())>(storage[j].get_surname())))
      {
        HRuser_t u = storage[i];
        storage[i] = storage[j];
        storage[j] = u;
      }
    }
  if (column == 4)
  for (size_t i=1; i<storage.size(); i++)
    for (size_t j=0; j<storage.size()-1; j++)
    {
      if (ss && ((storage[i].get_birthdate())<(storage[j].get_birthdate())))
      {
        HRuser_t u = storage[i];
        storage[i] = storage[j];
        storage[j] = u;
      }
      if (!ss && ((storage[i].get_birthdate())>(storage[j].get_birthdate())))
      {
        HRuser_t u = storage[i];
        storage[i] = storage[j];
        storage[j] = u;
      }
    }
  }
catch(std::exception &e)
{
	throw exception_t(10019, e.what(), "HRusers_t::sort(int column, bool ss)");
}

}

//users_t upd_role();
//users_t upd_cafe();
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void HRusers_t::sync_role(int pers_id, std::vector<int>* new_role)
{
  try
  {
	//dlg->session->ksi_transaction_begin(102);
	//ksi_cafe::session->ksi_transaction_begin(102);
	ksi_cafe::session->direct_execute("begin ksi.transaction_begin(102); end;");
	bool is_old_role(false); bool is_new_role(false);
	for (size_t i=0; i<role.size(); i++) 
	{
		is_old_role = role[i].find(pers_id);
		is_new_role = false;
		for (size_t j=0; j<new_role->size(); j++) if ((*new_role)[j] == role[i].get_id()) { is_new_role = true; break; }
		if (!is_old_role && is_new_role)
		{ // create role to person			
			//find_user(pers_id)->check_role(role[i].get_id());
			try
			{
				std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
				query->create_statement("begin ksi.pp_hr_pkg.addpers_role(:pers, :role); end;");
				query->set("pers", pers_id);
				query->set("role", role[i].get_id());
				query->execute_statement();
                
				role[i].add(pers_id);
				find_user(pers_id).add_role(role[i].get_id());
			}
			catch(ksi_client::ksi_exception &e)
			{
				throw exception_t(e.code(), e.what(), "HRusers_t::sync_role(int pers_id, std::vector<int>* new_role)");
			}
			catch(exception_t &e)
			{
				throw exception_t(e);
			}
			catch(std::exception &e)
			{
				throw exception_t(10020, e.what(), "HRusers_t::sync_role(int pers_id, std::vector<int>* new_role)");
			}
		}
		if (is_old_role && !is_new_role)
		{ // revoke role from person			
			try
			{
				std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
				query->create_statement("begin ksi.pp_hr_pkg.delpers_role(:pers, :role); end;");
				query->set("pers", pers_id);
				query->set("role", role[i].get_id());
				query->execute_statement();
                
				role[i].del(pers_id);
				find_user(pers_id).del_role(role[i].get_id());                
			}
			catch(ksi_client::ksi_exception &e)
			{
				throw exception_t(e.code(), e.what(), "HRusers_t::sync_role(int pers_id, std::vector<int>* new_role)");
			}
			catch(exception_t &e)
			{
				throw exception_t(e);
			}
			catch(std::exception &e)
			{
				throw exception_t(10021, e.what(), "HRusers_t::sync_role(int pers_id, std::vector<int>* new_role)");
			}
		}
	}
	//dlg->session->ksi_transaction_end();
	//ksi_cafe::session->ksi_transaction_end();
	ksi_cafe::session->direct_execute("begin ksi.transaction_end(); end;");
	ksi_cafe::session->commit();
  } 
  catch(ksi_client::ksi_exception &e)
  {
	throw exception_t(e.code(), e.what(), "HRusers_t::sync_role(int pers_id, std::vector<int>* new_role)");
  }
  catch(exception_t &e)
  {
	throw exception_t(e);
  }
  catch(std::exception &e)
  {
	throw exception_t(10031, e.what(), "HRusers_t::sync_role(int pers_id, std::vector<int>* new_role)");
  }
}
//---------------------------------------------------------------------------
void HRusers_t::sync_cafe(int pers_id, std::vector<int>* new_cafe)
{
  try
  {
	ksi_cafe::session->direct_execute("begin ksi.transaction_begin(96); end;");
	//dlg->session->ksi_transaction_begin(96);
	bool is_old_cafe(false); bool is_new_cafe(false);
	for (size_t i=0; i<cafe.size(); i++) 
	{
		is_old_cafe = cafe[i].find(pers_id);
		is_new_cafe = false;
		for (size_t j=0; j<new_cafe->size(); j++) if ((*new_cafe)[j]==cafe[i].get_id()) { is_new_cafe = true; break; }
		if (!is_old_cafe && is_new_cafe)
		{ // create role to person
			try
			{
				std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
				query->create_statement("begin ksi.pp_hr_pkg.addpers_cafe(:pers, :cafe); end;");
				query->set("pers", pers_id);
				query->set("cafe", cafe[i].get_id());
				query->execute_statement();

				cafe[i].add(pers_id);
				find_user(pers_id).add_cafe(cafe[i].get_id());
			} 
			catch(ksi_client::ksi_exception &e)
			{
				throw exception_t(e.code(), e.what(), "HRusers_t::sync_cafe(int pers_id, std::vector<int>* new_cafe)");
			}
			catch(exception_t &e)
			{
				throw exception_t(e);
			}
			catch(std::exception &e)
			{
				throw exception_t(10033, e.what(), "HRusers_t::sync_cafe(int pers_id, std::vector<int>* new_cafe)");
			}
		}
		if (is_old_cafe && !is_new_cafe)
		{ // revoke role from person
			try
			{
				std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
				query->create_statement("begin ksi.pp_hr_pkg.delpers_cafe(:pers, :cafe); end;");
				query->set("pers", pers_id);
				query->set("cafe", cafe[i].get_id());
				query->execute_statement();
                
				cafe[i].del(pers_id);
				find_user(pers_id).del_cafe(cafe[i].get_id());

			} 
			catch(ksi_client::ksi_exception &e)
			{
				throw exception_t(e.code(), e.what(), "HRusers_t::sync_cafe(int pers_id, std::vector<int>* new_cafe)");
			}
			catch(exception_t &e)
			{
				throw exception_t(e);
			}
			catch(std::exception &e)
			{
				throw exception_t(10032, e.what(), "HRusers_t::sync_cafe(int pers_id, std::vector<int>* new_cafe)");
			}
		}
	}
	ksi_cafe::session->direct_execute("begin ksi.transaction_end(); end;");
	//dlg->session->ksi_transaction_end();
	ksi_cafe::session->commit();
  } 
  catch(ksi_client::ksi_exception &e)
  {
	throw exception_t(e.code(), e.what(), "HRusers_t::sync_cafe(int pers_id, std::vector<int>* new_cafe)");
  }
  catch(exception_t &e)
  {
	throw exception_t(e);
  }
  catch(std::exception &e)
  {
	throw exception_t(10031, e.what(), "HRusers_t::sync_cafe(int pers_id, std::vector<int>* new_cafe)");
  }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void vecparam_t::load_state()
{
  try
  {
	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	query->create_statement("select state_id, name from ksi.state order by name");
	query->execute_statement();
        
	storage.clear();
	while (!query->next())
		storage.push_back(param_t((int) boost::get<double>(query->get(0)), boost::get<std::string>(query->get(1)))); 
	Loaded = true;
  } 
  catch(ksi_client::ksi_exception &e)
  {
	throw exception_t(e.code(), e.what(), "vecparam_t::load_state()");
  }
  catch(std::exception &e)
  {
	throw exception_t(10030, e.what(), "vecparam_t::load_state()");
  }
}
//---------------------------------------------------------------------------
void vecparam_t::load_position()
{
  try
  {
	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	query->create_statement("select position_id, name from ksi.position order by name");
	query->execute_statement();
        
	storage.clear();
	while (!query->next())
		storage.push_back(param_t((int)boost::get<double>(query->get(0)), boost::get<std::string>(query->get(1)))); 
	Loaded = true;
  } 
  catch(ksi_client::ksi_exception &e)
  {
	throw exception_t(e.code(), e.what(), "vecparam_t::load_position()");
  }
  catch(std::exception &e)
  {
	throw exception_t(10029, e.what(), "vecparam_t::load_position()");
  }
}
//---------------------------------------------------------------------------
void vecparam_t::load_role()
{
  try
  {
	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	query->create_statement("select role_id, name from ksi.role order by name");
	query->execute_statement();
        
	storage.clear();
	while (!query->next())
		storage.push_back(param_t((int)boost::get<double>(query->get(0)), boost::get<std::string>(query->get(1)))); 
	Loaded = true;
  } 
  catch(ksi_client::ksi_exception &e)
  {
	throw exception_t(e.code(), e.what(), "vecparam_t::load_role()");
  }
  catch(std::exception &e)
  {
	throw exception_t(10027, e.what(), "vecparam_t::load_role()");
  }

}

//---------------------------------------------------------------------------
void vecparam_t::load_cafe()
{
  try
  {
	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	query->create_statement("select cafe_id, name from ksi.cafe_v order by name");
	query->execute_statement();
        
	storage.clear();
	while (!query->next())
		storage.push_back(param_t((int)boost::get<double>(query->get(0)), boost::get<std::string>(query->get(1)))); 
	Loaded = true;
  } 
  catch(ksi_client::ksi_exception &e)
  {
	throw exception_t(e.code(), e.what(), "vecparam_t::load_cafe()");
  }
  catch(std::exception &e)
  {
	throw exception_t(10027, e.what(), "vecparam_t::load_cafe()");
  }

}
//---------------------------------------------------------------------------
void vecparam_t::load_positionrole()
{
  try
  {
	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	query->create_statement("select position.position_id, name, role_id from ksi.position left join ksi.position_role on position_role.position_id = position.position_id order by name");
	query->execute_statement();
	storage.clear();
	while (!query->next())
	{
		int position_id = (int) boost::get<double>(query->get(0));
		std::string name = boost::get<std::string>(query->get(1));
		int role_id = (int) boost::get<double>(query->get(2));  
		if (!exist(position_id)) 
			storage.push_back(param_t(position_id, name)); 		  
		find(position_id)->add(role_id);
	}
	Loaded = true;
  } 
  catch(ksi_client::ksi_exception &e)
  {
	throw exception_t(e.code(), e.what(), "vecparam_t::load_positionrole()");
  }
  catch(std::exception &e)
  {
	throw exception_t(10026, e.what(), "vecparam_t::load_positionrole()");
  }

}
//---------------------------------------------------------------------------
void vecparam_t::add_position(param_t *new_pos)
{	
	try
	{
		//dlg->session->ksi_transaction_begin(102);
		ksi_cafe::session->direct_execute("begin ksi.transaction_begin(102); end;");
		std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
		query->create_statement("begin ksi.pp_hr_pkg.add_position(:name, :res); end;");
		query->set("name", new_pos->get_name());
		boost::shared_ptr<double> out_param(new double(-1));
		query->set("res", out_param);
		query->execute_statement();
		//if (!query->next()) 
		new_pos->set_id(int(*out_param));
		if (new_pos->get_id() != -1) 
		{
			storage.push_back(param_t(new_pos->get_id(), new_pos->get_name()));
			for (int i=0; i<new_pos->size(); i++)  storage[storage.size()-1].add_role(new_pos->get(i));
		}
		ksi_cafe::session->direct_execute("begin ksi.transaction_end(); end;");
		ksi_cafe::session->commit();
	}
	catch(ksi_client::ksi_exception &e)
	{
		throw exception_t(e.code(), e.what(), "vecparam_t::add_position(param_t *new_pos)");
	}
	catch(std::exception &e)
	{
		throw exception_t(10025, e.what(), "vecparam_t::add_position(param_t *new_pos)");
	}
}
//---------------------------------------------------------------------------
void vecparam_t::edit_position(param_t *ed_pos) // after load position_role
{	
	try
	{
		//dlg->session->ksi_transaction_begin(102);
		ksi_cafe::session->direct_execute("begin ksi.transaction_begin(102); end;");
		std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
		query->create_statement("begin ksi.pp_hr_pkg.edit_position(:id, :name, :res); end;");
		query->set("id", ed_pos->get_id());
		query->set("name", ed_pos->get_name());
		boost::shared_ptr<double> out_param(new double(-1));
		query->set("res", out_param);
		query->execute_statement();
		find(ed_pos->get_id())->set_name(ed_pos->get_name());
		param_t* old_pos = find(ed_pos->get_id());
		
		std::auto_ptr<ksi_client::oracle_query_i> query_roles = ksi_cafe::session->create_query();
		query_roles->create_statement("select role_id, name from ksi.role order by name");
		query_roles->execute_statement();
        
		while (!query_roles->next())
		{
			bool exist(false);
			bool add_new(false);
			int rl_id = int(boost::get<double>(query_roles->get(0)));
			if (old_pos->find(rl_id)) exist = true; //find(ed_pos->get_id())->del_role(find(ed_pos->get_id())->get(i));
			if (ed_pos->find(rl_id)) add_new = true; //for (int i=0; i<ed_pos->size(); i++)  find(ed_pos->get_id())->add_role(ed_pos->get(i));
			if (!exist && add_new) find(ed_pos->get_id())->add_role(rl_id);
			if (exist && !add_new) find(ed_pos->get_id())->del_role(rl_id);
		}
		/*if (!query->next()) 
			ed_pos->set_id((int)boost::get<double>(query->get(0)));
		if (new_pos->get_id() != -1) 
			storage.push_back(param_t(new_pos->get_id(), new_pos->get_name()));*/
		ksi_cafe::session->direct_execute("begin ksi.transaction_end(); end;");
		ksi_cafe::session->commit();
	}
	catch(ksi_client::ksi_exception &e)
	{
		throw exception_t(e.code(), e.what(), "vecparam_t::add_position(param_t *new_pos)");
	}
	catch(std::exception &e)
	{
		throw exception_t(10025, e.what(), "vecparam_t::add_position(param_t *new_pos)");
	}
}
//---------------------------------------------------------------------------
void vecparam_t::del_position(int pos)  //after load position_role
{	
	try
	{
		//dlg->session->ksi_transaction_begin(103);
		ksi_cafe::session->direct_execute("begin ksi.transaction_begin(103); end;");
		std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
		query->create_statement("begin ksi.pp_hr_pkg.del_position(:id, :res); end;");
		query->set("id", get(pos)->get_id());
		boost::shared_ptr<double> out_param(new double(-1));
		query->set("res", out_param);
		query->execute_statement();
		//if (!query->next()) old_pos->set_id((int)boost::get<double>(query->get(0)));
		//int i = 
		storage.erase(storage.begin()+pos);
		//dlg->session->ksi_transaction_end();
		ksi_cafe::session->direct_execute("begin ksi.transaction_end(); end;");
		ksi_cafe::session->commit();
	}
	catch(ksi_client::ksi_exception &e)
	{
		throw exception_t(e.code(), e.what(), "vecparam_t::del_position(param_t *old_pos)");
	}
	catch(std::exception &e)
	{
		throw exception_t(10024, e.what(), "vecparam_t::del_position(param_t *old_pos)");
	}

}
//---------------------------------------------------------------------------
void param_t::add_role(int rl_id)
{	
	try
	{
		//dlg->session->ksi_transaction_begin(103);
		ksi_cafe::session->direct_execute("begin ksi.transaction_begin(103); end;");
		std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
		query->create_statement("begin ksi.pp_hr_pkg.add_posrole(:pos_id, :rl_id); end;");
		query->set("pos_id", get_id());
		query->set("rl_id", rl_id);
		query->execute_statement();
		if (!find(rl_id)) storage.push_back(rl_id);
		//dlg->session->ksi_transaction_end();
		ksi_cafe::session->direct_execute("begin ksi.transaction_end(); end;");
		ksi_cafe::session->commit();
	}
	catch(ksi_client::ksi_exception &e)
	{
		//throw exception_t(e.code(), e.what(), "param_t::add_role(int rl_id)");
	}
	catch(std::exception &e)
	{
		//throw exception_t(10023, e.what(), "param_t::add_role(int rl_id)");
	}
	catch(...) {  }
}
//---------------------------------------------------------------------------
void param_t::del_role(int rl_id)
{	
	try
	{
		//dlg->session->ksi_transaction_begin(102);
		ksi_cafe::session->direct_execute("begin ksi.transaction_begin(102); end;");
		std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
		query->create_statement("begin ksi.pp_hr_pkg.del_posrole(:pos_id, :rl_id); end;");
		query->set("pos_id", get_id());
		query->set("rl_id", rl_id);
		query->execute_statement();
		if (find(rl_id)) storage.erase(storage.begin() + find_id(rl_id));
		//dlg->session->ksi_transaction_end();
		ksi_cafe::session->direct_execute("begin ksi.transaction_end(); end;");
		ksi_cafe::session->commit();
	}
	catch(ksi_client::ksi_exception &e)
	{
		//throw exception_t(e.code(), e.what(), "param_t::add_role(int rl_id)");
	}
	catch(std::exception &e)
	{
		//throw exception_t(10022, e.what(), "param_t::add_role(int rl_id)");
	}
	catch(...) {  }

}


 

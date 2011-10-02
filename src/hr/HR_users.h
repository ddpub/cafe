//---------------------------------------------------------------------------

#ifndef HR_usersH
#define HR_usersH

#include <vector>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "boost/date_time/special_defs.hpp"
#include "boost/date_time/gregorian/gregorian.hpp" 

#include <memory>

#include <ksi_include/ksi_exceptions.hpp>
#include <oracle_client/oracle_client.hpp>
#include <oracle_client/ksi_common.hpp>
#include <boost/shared_ptr.hpp>
#include <ksi_util/users.hpp>
#include <ksi_util/util.hpp>

#include <connect.hpp>
//---------------------------------------------------------------------------

class param_t
{
private:

	int param_id;
	std::string name;
	std::vector<int> storage;
public:
	param_t() : param_id(0), name("") {  }
	param_t(int Param_id, std::string Name) : param_id(Param_id), name(Name) {  }
	void add(int id) { if (!find(id)) storage.push_back(id); }
	void del(int id) { if (find(id)) storage.erase(storage.begin() + find_id(id)); }
	bool find(int id) { for (size_t i=0; i<storage.size(); i++) if (storage[i] == id) return true; return false; }
	int find_id(int id) { for (size_t i=0; i<storage.size(); i++) if (storage[i] == id) return i; return -1; }
	int get_id() const { return param_id; }
	std::string get_name() const { return name; }
	int size() const { return storage.size(); }
	void set_id(int id) { param_id = id; }
	void set_name(std::string Name) { name = Name; }
	int get(int id) { return storage[id]; }

	void add_role(int rl_id);
	void del_role(int rl_id);

};

class vecparam_t
{
private:
	std::vector<param_t> storage;
	bool Loaded;
public:
	vecparam_t() : Loaded(false)  { }
	void load_state();
	void load_role();
	void load_position();
	void load_positionrole();
	void load_cafe();
	bool exist(int id) { for (size_t i=0; i < storage.size(); i++) if (storage[i].get_id() == id) return true; return false; }
	param_t* find(int id) { for (size_t i=0; i< storage.size(); i++) if (storage[i].get_id() == id) return &storage[i]; return 0; }
	void add(param_t item) { storage.push_back(item); }
	void del(int id) { for (size_t i=0; i< storage.size(); i++) if (storage[i].get_id() == id) { storage.erase(storage.begin()+i); return; } return; }
	int size() const { return storage.size(); }
	param_t* get(int id) { return &storage[id]; }
	std::string get_name(int id) const { return storage[id].get_name(); }
	int find_id(std::string sss) { for (int i=0; i<size(); i++) if (storage[i].get_name() == sss) return storage[i].get_id(); return -1;}
	void add_position(param_t *new_pos);
	void edit_position(param_t *ed_pos);
	//void del_position(param_t *old_pos);
	void del_position(int pos);
	bool loaded() { return Loaded; }
};


class HRuser_t
{
private:
	//ConnectDlg *dlg;
	std::string Login;
	std::string FirstName;
	std::string LastName;
	std::string SurName;
	boost::posix_time::ptime BirthDate;
	boost::posix_time::ptime Hire;
	boost::posix_time::ptime Fire;

	int Position_id;
	bool Loaded;
	int User_id;

	std::vector<int> Role;
	std::vector<int> Cafe;

	int Image_id;
	std::string Address1;
	std::string Address2;
	std::string City;
	int Zip;
	int State_id;
	std::string Email_business;
	std::string Email_private;
	std::string Phone_business;
	std::string Phone_home;
	std::string Phone_cell;
	std::string E_name;
	std::string E_phone;
	std::string SSN;
	std::string Description;

	
public:
	HRuser_t() : User_id(0), Image_id(0), Login(""), Position_id(0), Hire(), Fire(), BirthDate() { } 
	HRuser_t(int id) : User_id(id), Image_id(0), Login(""), Position_id(0), Hire(), Fire(), BirthDate()   { }

	HRuser_t(int user_id, std::string login, std::string firstname, std::string lastname, std::string surname,
        boost::posix_time::ptime birthdate, boost::posix_time::ptime hire, boost::posix_time::ptime fire): User_id(user_id), Login(login), FirstName(firstname),
		LastName(lastname), SurName(surname), BirthDate(birthdate), Fire(fire), Hire(hire) {  }
	
	HRuser_t(int user_id, std::string login/*, std::string password*/, std::string first_name, std::string last_name, std::string sur_name,
        boost::posix_time::ptime birth_date, boost::posix_time::ptime hire, boost::posix_time::ptime fire,
        std::string ssn, std::string address1, std::string address2, int zip,
        std::string city, int state, std::string email_busyness, std::string email_private,
        std::string phone_busyness, std::string phone_home, std::string phone_cell, std::string e_name,
        std::string e_phone, int position_id, int image_id, std::string description/*,
        std::vector<int> *role, std::vector<int> *cafe*/);
	//user_t(const user_t& user);
	//HRuser_t(const HRuser_t&);
	void upd(const HRuser_t& usr)
	{
		//User_id = usr.user_id();
		Login = usr.get_login();
		FirstName =usr.get_firstname();
		LastName =usr.get_lastname();
		SurName =usr.get_surname();
		Hire =usr.get_hire();
		Fire =usr.get_fire();
		BirthDate =usr.get_birthdate();
		Address1 = usr.get_address1();
		Address2 = usr.get_address2();
		State_id = usr.get_state();
		Zip = usr.get_zip();
		City = usr.get_city();
		Email_business = usr.get_emailbusiness();
		Email_private = usr.get_emailprivate();
		Phone_business = usr.get_phonebusiness();
		Phone_home = usr.get_phonehome();
		Phone_cell = usr.get_phonecell();
		SSN = usr.get_ssn();
		E_name = usr.get_ename();
		E_phone = usr.get_ephone();
		Description = usr.get_description();
		Position_id = usr.get_position();
		Image_id = usr.get_image_id();

		//Role.clear();
		//for (int i=0; i<usr.size_role(); i++) Role.push_back(usr.get_role(i)); 
		//Cafe.clear();
		//for (int i=0; i<usr.size_cafe(); i++) Cafe.push_back(usr.get_cafe(i)); 
	}
	HRuser_t& operator=(const HRuser_t& usr)
	{
		User_id = usr.user_id();
		Login = usr.get_login();
		FirstName =usr.get_firstname();
		LastName =usr.get_lastname();
		SurName =usr.get_surname();
		Hire =usr.get_hire();
		Fire =usr.get_fire();
		BirthDate =usr.get_birthdate();
		Address1 = usr.get_address1();
		Address2 = usr.get_address2();
		State_id = usr.get_state();
		Zip = usr.get_zip();
		City = usr.get_city();
		Email_business = usr.get_emailbusiness();
		Email_private = usr.get_emailprivate();
		Phone_business = usr.get_phonebusiness();
		Phone_home = usr.get_phonehome();
		Phone_cell = usr.get_phonecell();
		SSN = usr.get_ssn();
		E_name = usr.get_ename();
		E_phone = usr.get_ephone();
		Description = usr.get_description();
		Position_id = usr.get_position();
		Image_id = usr.get_image_id();

		//Role.clear();
		//for (int i=0; i<usr.size_role(); i++) Role.push_back(usr.get_role(i)); 
		//Cafe.clear();
		//for (int i=0; i<usr.size_cafe(); i++) Cafe.push_back(usr.get_cafe(i)); 

		return *this;
	}   

	std::string get_login() const { return Login; }
	int get_position() const { return Position_id; }
	std::string get_firstname() const { return FirstName; }
	std::string get_lastname() const { return LastName; }
	std::string get_surname() const { return SurName; }
	//std::vector<std::string> *param() const { return &Param; }
	boost::posix_time::ptime get_hire() const { return Hire; }
	boost::posix_time::ptime get_fire() const { return Fire; }
	boost::posix_time::ptime get_birthdate() const { return BirthDate; }
	int user_id() const { return User_id; }
	std::string fio() const { return FirstName + ' ' + LastName; }

	bool fired() { if (Fire == boost::posix_time::ptime()) return false; return true; }
	
	bool insert(std::string pwd);
	bool update(std::string pwd);
	bool del();
	//user_t& insert();
	HRuser_t& rehire();
	HRuser_t& fire();
	HRuser_t& unlock();
    
	int find_cafe(int cafe_id) { for (size_t i=0; i<Cafe.size(); i++) if (Cafe[i] == cafe_id) return i; return -1; }
	int find_role(int role_id) { for (size_t i=0; i<Role.size(); i++) if (Role[i] == role_id) return i; return -1; }

	void add_role(int role_id) { if (find_role(role_id) == -1 ) Role.push_back(role_id); } 
	void add_cafe(int cafe_id) { if (find_cafe(cafe_id) == -1) Cafe.push_back(cafe_id); } 

	void del_role(int role_id) { int res = find_role(role_id); if (res != -1) Role.erase(Role.begin() + res); } 
	void del_cafe(int cafe_id) { int res = find_cafe(cafe_id); if (res != -1) Cafe.erase(Cafe.begin() + res); } 

	int size_cafe() const { return Cafe.size(); }
	int size_role() const { return Role.size(); }

	std::string get_description() const { return Description; }
	std::string get_address1() const { return Address1; }
	std::string get_address2() const { return Address2; }
	std::string get_city() const { return City; }
	std::string get_ssn() const { return SSN; }
	std::string get_ename() const { return E_name; }
	std::string get_ephone() const { return E_phone; }
	std::string get_phonebusiness() const { return Phone_business; }
	std::string get_phonehome() const { return Phone_home; }
	std::string get_phonecell() const { return Phone_cell; }
	std::string get_emailbusiness() const { return Email_business; }
	std::string get_emailprivate() const { return Email_private; }
	int get_image_id() const { return Image_id; }
	int get_zip() const { return Zip; }
	int get_state() const { return State_id; }

	int get_role(int ii) const { return Role[ii]; }
	int get_cafe(int ii) const { return Cafe[ii]; }
    
	void reload(); 
	std::vector<char> get_bmp();
	void save_bmp(std::vector<char>& img);
	void clear_bmp();
};

class HRusers_t
{
private:
	std::vector<HRuser_t> storage;
	std::vector<param_t> role;
	std::vector<param_t> cafe;
	bool Loaded;
public:
	HRusers_t(int type = 0) 
	{ 
		fetch_all(type); 
		load_role(); 
		load_cafe(); 
		Loaded = true;
	}
	bool loaded() { return Loaded; }
//  const user_t& user();
  const HRuser_t& user(int user_id);
//  user_t& find_user(int user_id);
  HRuser_t* get_user(int i)  {return &storage[i]; }
  HRuser_t& get_user_(int i)  {return storage[i]; }
  HRuser_t& find_user(int id) { for (size_t i=0; i<storage.size(); i++) if (storage[i].user_id() == id) return storage[i]; throw exception_t(5222, "user doesn't exist", "HRusers_t::find_user"); }

  HRuser_t* operator[] (int i) { return &storage[i]; }
  void fetch_all(int type);
  void load_role();
  void load_cafe();
  int size() { return int(storage.size()); }
  std::vector<HRuser_t> get_all() const { return storage; }
  
  int insert_user(HRuser_t& user, std::vector<int>* new_role, std::vector<int>* new_cafe, std::string password);
  bool update_user(HRuser_t& user, int id, std::vector<int>* new_role, std::vector<int>* new_cafe, std::string password);

  //int insert(HRuser_t* user, std::string pwd);
  void sort(int column, bool ss);
  void fire_user(HRuser_t& user);
  void rehire_user(HRuser_t& user);
  void unlock_user(HRuser_t& user);
  //void update(HRuser_t* user, int id, std::string pwd);
  void delete_user(int id);
  
  void sync_role(int role_id, std::vector<int>* new_role);
  void sync_cafe(int cafe_id, std::vector<int>* new_cafe);

  int size_role() const { return role.size(); }
  int size_cafe() const { return cafe.size(); }

  //wxString[] get_role_name() const { wxString[] res; for (int i=0; i<role.size(); i++) res += role[i].get_name() }
  param_t* get_role(int role_id) { for (size_t i=0; i<role.size(); i++) if (role[i].get_id() == role_id) return &role[i]; return 0; }
  param_t* get_cafe(int cafe_id) { for (size_t i=0; i<cafe.size(); i++) if (cafe[i].get_id() == cafe_id) return &cafe[i]; return 0; }

  bool find_role(int role_id) { for (size_t i=0; i<role.size(); i++) if (role[i].get_id() == role_id) return true; return false; }
  bool find_cafe(int cafe_id) { for (size_t i=0; i<cafe.size(); i++) if (cafe[i].get_id() == cafe_id) return true; return false; }
  bool find_pers(int pers_id) { for (size_t i=0; i<storage.size(); i++) if (storage[i].user_id() == pers_id) return true; return false; }

  //std::vector<std::string> find_cafe_str(int person_id) { std::vector<std::string> res; for (int i=0; i<cafe.size(); i++)}

};
#endif

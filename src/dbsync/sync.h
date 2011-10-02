#include <string>
#include <oracle_client/oracle_client.hpp>
#include <oracle_client/ksi_exception.hpp>
#include "MemoOutPut.h"

using namespace ksi_client;

class connect_t
{
private:
	std::string host;
	int type;
	int id;
	std::auto_ptr<oracle_session_i> sess;
	bool valid;
public:
	connect_t(std::string& Host, int Type=0);
	connect_t(connect_t& Con): host(Con.get_host()), type(Con.get_type()), id(int(Con.get_id())), sess(Con.session()), valid(Con.is_valid()) {  }
	//connect_t(std::string Host, int Id, int Type = 0) : host(Host), id(Id), type(Type) { }
	bool refresh();
	double get_id() {  return id;  }
	int get_type() {  return type;  }
	std::string get_host() 
	{  
		int pos = host.rfind(":");
		
		if (pos != std::string::npos) 
		{
		std::string sss = host.substr(0, pos);
		return host.substr(0, pos);
		}
		return host;  
	}
	std::string get_sid() 
	{  
		int pos = host.rfind(":");
		if (pos != std::string::npos) 
		{
			std::string sss = host.substr(pos+1, host.length()-pos);
			return host.substr(pos+1, host.length()-pos);
		}
		return "cafe";  
	}
	//std::string get_tns() {  return host;  } //std::string("(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(HOST=") + host + ")(PORT=1521)))(CONNECT_DATA=(SERVICE_NAME=cafe)))";  }
	std::string get_host(std::string Host) {  host=Host; return get_host(); } //std::string("(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(HOST=") + Host + ")(PORT=1521)))(CONNECT_DATA=(SERVICE_NAME=cafe)))";  }
	std::string get_sid(std::string Host) {  host=Host; return get_sid(); } //std::string("(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(HOST=") + Host + ")(PORT=1521)))(CONNECT_DATA=(SERVICE_NAME=cafe)))";  }
	std::auto_ptr<oracle_session_i> session() { return sess; }
	void validate() { valid = true; }
	void error() { valid = false; }
	bool is_valid() { return valid; }
	
	std::auto_ptr<oracle_query_i> create_query() { return sess->create_query(); }
	void direct_execute(std::string sss) { sess->direct_execute(sss); }
	void commit() { sess->commit(); }
	void rollback() { sess->rollback(); }
};

class sync_t
{
private:
	//memo_out_put_t m;
	connect_t local;
	connect_t main;
	int action_t;  //0 - wait;  1 - download;  2 - upload;  3 - initial_fill;  4 - db_update
public:
	sync_t(connect_t Local, connect_t Main) : local(Local), main(Main), action_t(0) {  }
	sync_t(std::string Local, std::string Main, int LType = 0, int MType = 0) : local(Local, LType), main(Main, MType), action_t(0)  {  }
	void DBAction();
	void Action(int Action_t);
	void Download() { copy_orders(); Action(1); }
	void Upload() { Action(2); }
	void InitFill(std::string ksi_pwd, std::string from, std::string to, std::string format);
	void copy_orders();
	void Sync(bool db_upd);
	void DBClear(std::string& from, std::string& to, std::string& format, std::string& pwd);
};

class config_t
{
public:
	std::string main;
	std::string local;
	int refresh;
	int LType;
	int MType;
	bool db;
	int db_upd;
	bool save_config(std::string file_name);
	bool load_config(std::string file_name);
};



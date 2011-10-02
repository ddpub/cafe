#ifndef DMITRY_DERGACHEV_28_03_2008_USERS_H
#define DMITRY_DERGACHEV_28_03_2008_USERS_H


#include <vector>
#include <string>
#include <boost/thread/thread.hpp>
#include <oracle_client/oracle_client.hpp>
#include <oracle_client/ksi_exception.hpp>

namespace ksi_cafe {

class user_t
{
public:
	explicit user_t(int user_id, ksi_client::oracle_session_i* session_);
	explicit user_t(ksi_client::oracle_session_i* session_);
	const std::string& login() const { return Login; }
	const std::string& firstname() const { return FirstName; }
	const std::string& lastname() const { return LastName; }
	int user_id() const { return User_id; }
	std::string fio() const { return FirstName + ' ' + LastName; }
	bool ccashier() const;
	bool waiter() const;
	bool bartender() const;
	bool manager() const;
	bool administrator() const;
	bool superuser() const;
	bool superwaiter() const;
	std::string role_name() const;
	std::string ru_role_name() const;
private:
	std::string Login;
	std::string FirstName;
	std::string LastName;
	int User_id;
	std::vector<int> Role;
};

class users_t
{
public:
	explicit users_t(ksi_client::oracle_session_i* session_=0): session_(session_) { fetch_all(); }
	const user_t& user(const std::string& login) const;
	const user_t& user(int user_id) const;
	const user_t& user() const;
	void fetch_all();
	size_t size() const { return storage.size(); }
	const std::vector<user_t>& get_all() const { return storage; }
	void setNewSession(ksi_client::oracle_session_i* new_session) { session_ = new_session; }
private:
	mutable std::vector<user_t> storage;
	mutable boost::mutex  mutex;
	mutable boost::mutex  mutex1;
	mutable ksi_client::oracle_session_i* session_;
};

};

#endif

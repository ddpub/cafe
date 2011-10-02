
#include "users.hpp"

#include <ksi_include/ksi_exceptions.hpp>
#include <shared_modules/connect.hpp>
#include <memory>
#include <locale>

namespace ksi_cafe {

user_t::user_t(ksi_client::oracle_session_i* session_) {
	std::auto_ptr<ksi_client::oracle_query_i> query = session_ == 0 ? session->create_query() : session_->create_query();

	query->create_statement
	(
	"select login, firstname, lastname, person_id  \
	from ksi.person \
	where login = user"
	);

	query->execute_statement();

	Login = boost::get<std::string>(query->get(0));
	FirstName = boost::get<std::string>(query->get(1));
	LastName = boost::get<std::string>(query->get(2));
	User_id = static_cast<int>(boost::get<double>(query->get(3)));

	query->create_statement
	(
	"select role_id from ksi.person_role \
	where person_id = :person_id"
	);
	query->set("person_id", User_id);

	query->execute_statement();

	while (!query->next()) {
		int role_id = static_cast<int>(boost::get<double>(query->get(0)));
		Role.push_back(role_id);
	}
}

user_t::user_t(int user_id, ksi_client::oracle_session_i* session_): User_id(user_id) {
	if (user_id == 0) {
		Login = "Empty";
		FirstName = "";
		LastName = "<Unregistered user>";
		return;
	}

	std::auto_ptr<ksi_client::oracle_query_i> query = session_ == 0 ? session->create_query() : session_->create_query();

	query->create_statement
	(
	"select login, firstname, lastname \
	from ksi.person \
	where person_id=:person_id"
	);
	query->set("person_id", user_id);

	query->execute_statement();

	Login = boost::get<std::string>(query->get(0));
	FirstName = boost::get<std::string>(query->get(1));
	LastName = boost::get<std::string>(query->get(2));

	query->create_statement
	(
	"select role_id from ksi.person_role \
	where person_id = :person_id"
	);
	query->set("person_id", user_id);

	query->execute_statement();

	while (!query->next()) {
		int role_id = static_cast<int>(boost::get<double>(query->get(0)));
		Role.push_back(role_id);
	}
}

bool user_t::ccashier() const {
	bool super_role = false;
	bool administrator_role = false;
	bool cashier_role = false;
	for (size_t i=0; i<Role.size(); i++)
		if (Role[i] == 1) super_role = true;
		else if (Role[i] == 2) administrator_role = true;
		else if (Role[i] == 11) cashier_role = true;
	if (super_role || administrator_role || cashier_role) return true;
	return false;
}

bool user_t::waiter() const {
	bool super_role = false;
	bool administrator_role = false;
	bool waiter_role = false;
	for (size_t i=0; i<Role.size(); i++)
		if (Role[i] == 1) super_role = true;
		else if (Role[i] == 2) administrator_role = true;
		else if (Role[i] == 12) waiter_role = true;
	if (super_role || administrator_role || waiter_role) return true;
		return false;
}

bool user_t::bartender() const {
	bool super_role = false;
	bool administrator_role = false;
	bool bartender_role = false;
	for (size_t i=0; i<Role.size(); i++)
		if (Role[i] == 1) super_role = true;
		else if (Role[i] == 2) administrator_role = true;
		else if (Role[i] == 13) bartender_role = true;
	if (super_role || administrator_role || bartender_role) return true;
		return false;
}

bool user_t::superwaiter() const {
	bool super_role = false;
	bool administrator_role = false;
	bool manager_role = false;
	bool superwaiter_role = false;
	for (size_t i=0; i<Role.size(); ++i)
		if (Role[i] == 1) super_role = true;
		else if (Role[i] == 2) administrator_role = true;
		else if (Role[i] == 7) manager_role = true;
		else if (Role[i] == 9) superwaiter_role = true;
	if (super_role || administrator_role || manager_role || superwaiter_role) return true;
	return false;
}

bool user_t::manager() const {
	bool super_role = false;
	bool administrator_role = false;
	bool manager_role = false;
	for (size_t i=0; i<Role.size(); i++)
		if (Role[i] == 1) super_role = true;
		else if (Role[i] == 2) administrator_role = true;
		else if (Role[i] == 7) manager_role = true;
	if (super_role || administrator_role || manager_role) return true;
		return false;
}

bool user_t::administrator() const {
	bool super_role = false;
	bool administrator_role = false;
	for (size_t i=0; i<Role.size(); i++)
		if (Role[i] == 1) super_role = true;
		else if (Role[i] == 2) administrator_role = true;
	if (super_role || administrator_role) return true;
		return false;
}

bool user_t::superuser() const {
	for (size_t i=0; i<Role.size(); i++)
		if (Role[i] == 1) return true;
	return false;
}

const user_t& users_t::user(const std::string& login) const {
	boost::lock_guard<boost::mutex> g(mutex);
	std::locale loc;
	std::string upper_login = login;
	std::use_facet<std::ctype<char> > (loc).toupper(&upper_login[0], &upper_login[upper_login.length()]);

	int find = -1;
	for (size_t i=0; i<storage.size(); i++) {
		std::string upper_storage_login = storage[i].login();
		std::use_facet<std::ctype<char> > (loc).toupper(&upper_storage_login[0], &upper_storage_login[upper_storage_login.length()]);
		if (upper_storage_login == upper_login)
		find = i;
	}
	if (find == -1) {
		storage.push_back(user_t(session_));
		return storage[storage.size()-1];
	}
	else
		return storage[find];
}

const user_t& users_t::user(int user_id) const {
	boost::lock_guard<boost::mutex> g(mutex);
	int find = -1;
	for (size_t i=0; i<storage.size(); i++)
		if (storage[i].user_id() == user_id)
	find = i;
	if (find == -1) {
		storage.push_back(user_t(user_id, session_));
		return storage[storage.size()-1];
	}
	else
		return storage[find];
}

const user_t& users_t::user() const {
	boost::lock_guard<boost::mutex> g(mutex1);
	return user(session_ == 0 ? session->username() : session_->username());
}

void users_t::fetch_all() {
	std::auto_ptr<ksi_client::oracle_query_i> query = session_ == 0 ? session->create_query() : session_->create_query();

	query->create_statement
	(
	"select person.person_id \
	from ksi.person \
	left join ksi.cafe_person on cafe_person.person_id = person.person_id \
	where position_id is not null and fire is null and cafe_id = ksi.util.get_cur_cafe_id"
	);

	query->execute_statement();

	while (!query->next()) {
		int person_id = static_cast<int>(boost::get<double>(query->get(0)));
		user(person_id);
	}
}



std::string user_t::role_name() const {
	if (manager()) return "cafe manager";
	else if (waiter()) return "waiter";
	else if (bartender()) return "bartender";
	else if (ccashier()) return "cashier";

	THROW_LOCATION(1, "std::string user_t::role_name() const");
}

std::string user_t::ru_role_name() const {
	if (manager()) return "менеджер";
	else if (waiter()) return "официант";
	else if (bartender()) return "бармен";

	THROW_LOCATION(1, "std::string user_t::ru_role_name() const");
}

};

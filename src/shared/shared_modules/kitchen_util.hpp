#ifndef DMITRY_DERGACHEV_27_03_2008_KITCHEN_UTIL_H
#define DMITRY_DERGACHEV_27_03_2008_KITCHEN_UTIL_H

#include <string>
#include <vector>
#include <map>
#include <printer/printer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <oracle_client/oracle_client.hpp>
#include <oracle_client/ksi_exception.hpp>


namespace ksi_kitchen {

class order_dish_t;

class comp_t {
public:
	comp_t(const std::map<int, int>& val_): val_(val_) {}
	bool operator() (int id1, int id2) { return val_[id1] < val_[id2]; }
private:
	std::map<int, int> val_;
};

class kitchen_t	{
public:
	kitchen_t(ksi_client::oracle_session_i* session_, int kitchen_id_, const std::string& name_, bool enable_);
public:
	int kitchen_id() const { return kitchen_id_; }
	const std::string& name() const { return name_; }
	bool enable() const { return enable_; }
	const std::vector<int>& get_types() const { return kitchen_type_id_vec_; }
	std::vector<int> get_types(int table_group_id) const;
	bool kitchen_execute(int cur_table_group_id, int cur_type_id) const;
	bool kitchen_execute_for_modifier(int cur_table_group_id, const order_dish_t& dish) const;
	bool kitchen_execute_for_modifier(int cur_table_group_id, const order_dish_t& dish, int cur_kitchen_type_id) const;
	const std::string& type_name(int kt_type_id) const;
private:
	ksi_client::oracle_session_i* session_;
	int kitchen_id_;
	std::string name_;
	bool enable_;
	std::vector<int> table_group_id_vec_;
	std::vector<int> kitchen_type_id_vec_;
	std::vector<std::string> kitchen_type_name_vec_;
};

class container_kitchen_t {
public:
	container_kitchen_t(ksi_client::oracle_session_i* session_, const std::string& str);
	void setNewSession(ksi_client::oracle_session_i* new_session) { session_ = new_session; }
	int count() const { return int(kitchen_vec_.size()); }
	const kitchen_t& kitchen(int i) const { return kitchen_vec_[i]; }
	int find(const std::string& kitchen_name);
	std::string find(int kitchen_id);
	std::string type_name(int kt_type_id) const;
	std::vector<int> kitchen_types() const;
	bool container_execute(int cur_table_group_id, int cur_type_id) const {
		for (int i=0; i<count(); i++)
			if (kitchen(i).enable() && kitchen(i).kitchen_execute(cur_table_group_id, cur_type_id)) return true;
		return false;
	}
	bool printMiscCharges() const { return printMiscCharges_; }
private:
	ksi_client::oracle_session_i* session_;
	std::vector<kitchen_t> kitchen_vec_;
	std::map<int, int> kitchen_type_weight_;
	bool printMiscCharges_;
};


class order_dish_modifier_t {
public:
	order_dish_modifier_t(ksi_client::oracle_session_i* session_, int order_dish_id_, const std::string name_, int modifier_id_, int mod_dish_id_
    , int kitchen_type_id_, int count_, int count_wasted_, int count_registered_)
	: session_(session_), order_dish_id_(order_dish_id_), name_(name_), modifier_id_(modifier_id_), mod_dish_id_(mod_dish_id_)
	, kitchen_type_id_(kitchen_type_id_), count_(count_), count_wasted_(count_wasted_), count_registered_(count_registered_) {}
public:
	const std::string& name() const { return name_; }
	int mod_dish_id() const { return mod_dish_id_; }
	int kitchen_type_id() const { return kitchen_type_id_; }
	int count() const { return count_; }
	int count_wasted() const { return count_wasted_; }
	int count_registered() const { return count_registered_; }
	bool registered(int dish_count) const { if (count_registered_ == count_*dish_count+count_wasted_) return true; return false; }
public:
	void mod_register(int reg_count, const std::string& ver, int kitchen_id, bool auto_ready) const;
	void mod_self_register(int dish_count_registered, const std::string& ver, int kitchen_id, bool auto_ready) const;
private:
	int order_dish_id_;
	std::string name_;
	int modifier_id_;
	int mod_dish_id_;
	int kitchen_type_id_;
	int count_;
	int count_wasted_;
	int count_registered_;
	ksi_client::oracle_session_i* session_;
};

class order_dish_t {
public:
	order_dish_t(ksi_client::oracle_session_i* session_, int order_dish_id_, int dish_id_, const std::string& name_, int kitchen_type_id_, int count_, int count_wasted_, int count_registered_, const std::string& description_, boost::posix_time::ptime time_, const std::vector<order_dish_modifier_t>& mods_): session_(session_), order_dish_id_(order_dish_id_), dish_id_(dish_id_), name_(name_), kitchen_type_id_(kitchen_type_id_), count_(count_), count_wasted_(count_wasted_), count_registered_(count_registered_), description_(description_), time_(time_), mods_(mods_) {}
public:
	int dish_id() const { return dish_id_; }
	const std::string& name() const { return name_; }
	int kitchen_type_id() const { return kitchen_type_id_; }
	int count() const { return count_; }
	int count_wasted() const { return count_wasted_; }
	int count_registered() const { return count_registered_; }
	int total_count() const { return count_; }
	const std::string& description() const { return description_; }
	const boost::posix_time::ptime& time() const { return time_; }
	int mod_count() const { return int(mods_.size()); }
	const order_dish_modifier_t& modifier(int i) const { return mods_[i]; }
	bool registered() const { if (count_registered_ == count_+count_wasted_) return true; return false; }
	bool modifier_registered() const {
		for (int i=0; i<mod_count(); i++)
			if (!modifier(i).registered(total_count())) return false;
		return true;
	}
public:
	void dish_register(int count, const std::string& ver, int kitchen_id, bool auto_ready) const;
	void mods_register(int table_group_id, const container_kitchen_t& container, int count, const std::string& ver, bool auto_ready) const;
	void mods_self_register(int table_group_id, const container_kitchen_t& container, const std::string& ver, bool auto_ready) const;
private:
	int order_dish_id_;
	int dish_id_;
	std::string name_;
	int kitchen_type_id_;
	int count_;
	int count_wasted_;
	int count_registered_;
	std::string description_;
	boost::posix_time::ptime time_;
	std::vector<order_dish_modifier_t> mods_;
	ksi_client::oracle_session_i* session_;
};

class table_t {
public:
	table_t() {}
	table_t(int table_group_id_, int table_id_, const std::string& name_, const std::string& group_name_)
	: table_group_id_(table_group_id_), table_id_(table_id_), name_(name_), group_name_(group_name_) {}
public:
	int table_group_id() const { return table_group_id_; }
	int table_id() const { return table_id_; }
	const std::string& group_name() const { return group_name_; }
	const std::string& name() const { return name_; }
	bool exist() const { return table_id_ != 0; }
private:
	int table_group_id_;
	int table_id_;
	std::string name_;
	std::string group_name_;
};

class token_t {
public:
	token_t() {}
	explicit token_t(const std::string& name_): name_(name_) {}
public:
	bool exist() const { return name_ != ""; }
	const std::string& name() const { return name_; }
private:
	std::string name_;
};


class order_t {
public:
	explicit order_t(ksi_client::oracle_session_i* session_, int order_id);
public:
	int dish_count() const { return int(order_dish_vec_.size()); }
	const order_dish_t& dish(int i) const { return order_dish_vec_[i]; }
	const table_t& table() const { return table_; }
	const token_t& token() const { return token_; }
	bool type_for_here() const { return type_id_ == 1; }
	bool type_to_go() const { return type_id_ == 2; }
	bool type_delivery() const { return type_id_ == 3; }
	bool type_pickup() const { return type_id_ == 4; }
	bool type_employee_meal() const { return type_id_ == 5; }
	int employee_id() const { return employee_id_; }
	const std::string& memo() const { return memo_; }
	boost::posix_time::ptime get_first_not_register_time() const;
	boost::posix_time::ptime delivery_time() const { return delivery_time_; }
	int creator_id() const { return creator_id_; }
	int order_id() const { return order_id_; }
	std::string type_name() const;
private:
	int order_id_;
	int creator_id_;
	int type_id_;
	table_t table_;
	token_t token_;
	int employee_id_;
	std::string memo_;
	std::vector<order_dish_t> order_dish_vec_;
	boost::posix_time::ptime delivery_time_;
	ksi_client::oracle_session_i* session_;
};

bool try_order_register(const order_t&, const container_kitchen_t&, const std::string&, bool);
bool try_mod_register(const order_t&, const container_kitchen_t&, const std::string&, bool);
};

#endif

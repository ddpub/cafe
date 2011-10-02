#ifndef MENU_CONTAINER_H_DERGACHEV_30_11_2007
#define MENU_CONTAINER_H_DERGACHEV_30_11_2007

#include "menu_interface.hpp"
#include <string>
#include <vector>
#include <map>
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace ksi_cafe {

class dish_modifier_container_t
{
public:
	dish_modifier_container_t(): loaded_(false) {}
	void load();
	const dish_t& dish(int dish_id);
	const modifier_group_t& modifier_group(int group_id);
	const modifier_t& modifier(int group_id, int modifier_id);
private:
	typedef std::vector<dish_t> dish_vector_t;
	typedef std::vector<modifier_group_t> modifier_group_vector_t;
	typedef std::vector<modifier_t> modifier_vector_t;
private:
	dish_vector_t dish_;
	modifier_group_vector_t modifier_group_;
	modifier_vector_t modifier_;
	bool loaded_;
};

class menu_t
{
public:
	menu_t(int menu_id_, int cafe_id_, const std::string& name_, int version_);
	bool loaded() const { return loaded_; }
	int menu_id() const { return menu_id_; }
	int cafe_id() const { return cafe_id_; }
	const std::string& name() const { return name_; }
	int version() const { return version_; }
	int find_menu_item_id(int parent_id, const std::string& menu_item_name) const;
	bool find_menu_item(int parent_id, const std::string& menu_item_name) const;
	bool find_menu_item(int parent_id, int menu_item_id) const;
	bool find_menu_dish(int menu_item_id, int dish_id) const;
	std::vector<const menu_item_t*> menu_item(int parent_id) const;
	std::vector<const menu_dish_t*> menu_dish(int parent_id) const;
	std::vector<const menu_item_t*> menu_item() const;
	std::vector<const menu_dish_t*> hot_dish() const;
	std::map<const menu_dish_modifier_t*, const modifier_group_t*> menu_dish_modifier(const menu_dish_t* menu_dish_ptr) const;
	void refresh();
	void set_table_group_id(const std::vector<int>& kt_id_vec);
	const menu_dish_t* find_by_code(const std::string& code) const; 
public:
	static void container_load() { container_.load(); }
private:
	bool find_menu_item(int parent_id, int menu_item_id, const std::vector<menu_item_t>& new_menu_item_) const;
	bool find_menu_dish(int menu_item_id, int dish_id, const std::vector<menu_dish_t>& new_menu_dish_) const;
	std::vector<const menu_dish_t*> menu_dish(int parent_id, const std::vector<menu_dish_t>& new_menu_dish_) const;
	std::vector<const menu_item_t*> menu_item(int parent_id, const std::vector<menu_item_t>& new_menu_item_) const;
private:
	static dish_modifier_container_t container_;
	int menu_id_;
	int cafe_id_;
	std::string name_;
	int version_;
	std::vector<menu_item_t> menu_item_;
	std::vector<menu_dish_t> menu_dish_;
	std::vector<menu_dish_modifier_t> menu_dish_modifier_;
	bool loaded_;
};

class menu_container_t
{
public:
	menu_container_t();
	std::vector<const menu_t*> menu_for_cafe(int cafe_id) const;
	std::vector<menu_t*> menu_for_cafe(int cafe_id);
	menu_t& menu(int menu_id);
private:
	std::list<menu_t> menus_;
};

class table_group_kitchen_t
{
public:
	table_group_kitchen_t(int table_group_id_, int kitchen_type_id_, int kitchen_id_)
		: table_group_id_(table_group_id_), kitchen_type_id_(kitchen_type_id_), kitchen_id_(kitchen_id_) {}
	int table_group_id() const { return table_group_id_; }
	int kitchen_type_id() const { return kitchen_type_id_; }
	int kitchen_id() const { return kitchen_id_; }
private:
	int table_group_id_;
	int kitchen_type_id_;
	int kitchen_id_;
};

class menu_manager_t
{
public:
	menu_manager_t(const std::vector<menu_t*>& source_menu_vec);
	const std::vector<menu_t*>& menu_vec() const { return menu_vec_; }
	void select(int menu_id, int table_group_id, const std::vector<int>& kt_only = std::vector<int>());
	menu_t* selected() const { return selected_; }
private:
	int current_table_group_id_;
	std::vector<menu_t*> menu_vec_;
	std::vector<table_group_kitchen_t> tgk_;
	mutable std::map<menu_t*, boost::posix_time::ptime> form_;
	mutable std::map<menu_t*, boost::posix_time::ptime> to_;
	menu_t* selected_;
};

class stop_list_entry_t
{
public:
	stop_list_entry_t(int stop_list_id_, int dish_id_, const boost::posix_time::ptime& since_dt_, const boost::posix_time::ptime& till_dt_)
	: stop_list_id_(stop_list_id_), dish_id_(dish_id_), since_dt_(since_dt_), till_dt_(till_dt_) {}
//  stop_list_entry_t(const stop_list_entry_t& o)
//  : stop_list_id_(o.stop_list_id_), dish_id_(o.dish_id_), since_dt_(o.since_dt_), till_dt_(o.till_dt_) {}
	int stop_list_id() const { return stop_list_id_; }
	int dish_id() const { return dish_id_; }
	const boost::posix_time::ptime& since_dt() const { return since_dt_; }
	const boost::posix_time::ptime& till_dt() const { return till_dt_; }
	void change_st(const boost::posix_time::ptime& since, const boost::posix_time::ptime& till) { since_dt_ = since; till_dt_ = till; }
private:
	int stop_list_id_;
	int dish_id_;
	boost::posix_time::ptime since_dt_;
	boost::posix_time::ptime till_dt_;
};

class simple_stop_list_t
{
public:
	simple_stop_list_t();
	std::vector<const stop_list_entry_t*> all_entry() const;
	bool find_entry(int dish_id) const;
	const stop_list_entry_t& get_fined_entry(int dish_id) const;
private:
	std::vector<stop_list_entry_t> entry_;
	simple_stop_list_t(const simple_stop_list_t&);
	simple_stop_list_t& operator=(simple_stop_list_t&);
};

class  kitchen_type_group_t {
public:
	kitchen_type_group_t(int kitchen_type_group_id_, const std::string& name_): kitchen_type_group_id_(kitchen_type_group_id_), name_(name_) {}
	int kitchen_type_group_id() const { return kitchen_type_group_id_; }
	const std::string& name() const { return name_; }
private:
	int kitchen_type_group_id_;
	std::string name_;
};

class container_kitchen_type_group_t {
public:
	container_kitchen_type_group_t(): loaded_(false) {}
	void load();
	bool loaded() const { return loaded_; }
	std::string kitchen_type_group_name(int group_id);
	std::map<int, int> kt_ktg_map; // kitchen_type_id, group_id
	std::map<int, int> ktg_kt_map; // group_id, kitchen_type_id
	std::vector<kitchen_type_group_t> ktg_vec;
private:
	bool loaded_;
};

class table_container_t {
public:
	table_container_t() { load(); }
	void load();
	std::vector<table_t> get_tables(int table_group_id) const;
private:
	std::vector<int> table_id_vec_;
	std::vector<std::string> table_name_vec_;
	std::vector<int> group_id_vec_;
	std::vector<std::string> group_name_vec_;
	mutable std::map<int, int> group_table_map_;
	mutable std::map<int, std::string> group_id_name_map_;
	mutable std::map<int, std::string> table_id_name_map_;
	mutable std::map<int, int> table_group_map_;
};

}


#endif

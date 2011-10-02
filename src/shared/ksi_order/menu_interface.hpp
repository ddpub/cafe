#ifndef MENU_INTERFACE_H_DERGACHEV_29_11_2007
#define MENU_INTERFACE_H_DERGACHEV_29_11_2007

#include <string>

namespace ksi_cafe {

class dish_t
{
public:
	dish_t(int dish_id_, const std::string& name_, const std::string& code_, int hotkey_color_, int kitchen_type_id_, int employee_meal_);
public:
	int dish_id() const { return dish_id_; }
	const std::string& name() const { return name_; }
	const std::string& code() const { return code_; }
	int hotkey_color() const { return hotkey_color_; }
	int kitchen_type_id() const { return kitchen_type_id_; }
	int employee_meal() const { return employee_meal_; }
private:
	int dish_id_;
	std::string name_;
	std::string code_;
	int hotkey_color_;
	int kitchen_type_id_;
	int employee_meal_;
private:
	dish_t();
};

class modifier_group_t
{
public:
	modifier_group_t(int pos_, int group_id_, const std::string& name_, int max_weight_);
	int pos() const { return pos_; }
	int group_id() const { return group_id_; }
	const std::string& name() const { return name_; }
	int max_weight() const { return max_weight_; }
private:
	int pos_;
	int group_id_;
	std::string name_;
	int max_weight_;
private:
	modifier_group_t();
};

class modifier_t
{
public:
	modifier_t(int group_id_, int modifier_id_, int dish_id_, int kitchen_type_id_, const std::string& name_, int color_, int weight_);
	int group_id() const { return group_id_; }
	int modifier_id() const { return modifier_id_; }
	int dish_id() const { return dish_id_; }
	int kitchen_type_id() const { return kitchen_type_id_; }
	const std::string& name() const { return name_; }
	int weight() const { return weight_; }
	int color() const { return color_; }
private:
	int group_id_;
	int modifier_id_;
	int dish_id_;
	int kitchen_type_id_;
	std::string name_;
	int color_;
	int weight_;
};


class menu_item_t
{
public:
	menu_item_t(int parent_id_, int menu_item_id_, const std::string& name_, int color_);
	const menu_item_t& operator=(const menu_item_t* menu_item_ptr);
	int parent_id() const { return parent_id_; }
	int menu_item_id() const { return menu_item_id_; }
	const std::string& name() const { return name_; }
	int color() const { return color_; }
	void set_color(int new_color) { color_ = new_color; }
private:
	int parent_id_;
	int menu_item_id_;
	std::string name_;
	int color_;
private:
	menu_item_t();
};


class menu_dish_t : public dish_t
{
public:
	menu_dish_t(const dish_t& dish, int menu_item_id_, double price_, int hotkey_id_, bool disable_);
	int menu_item_id() const { return menu_item_id_; }
	int parent_id() const { return menu_item_id(); }
	double price() const { return price_; }
	int hotkey_id() const { return hotkey_id_; }
	bool disabled() const { return disable_; }
	bool enabled() const { return !disable_; }
	void disable() { disable_ = true; }
	void enable() { disable_ = false; }
private:
	int menu_item_id_;
	double price_;
	int hotkey_id_;
	bool disable_;
};

class menu_dish_modifier_t: public modifier_t
{
public:
	menu_dish_modifier_t(const modifier_t& modifier, int menu_item_id_, int dish_id_, double price_);
	int menu_item_id() const { return menu_item_id_; }
	int modifier_dish_id() const { return dish_id_; }
	double price() const { return price_; }
private:
	int menu_item_id_;
	int dish_id_;
	double price_;
};

class token_t
{
public:
	token_t();
	token_t(int token_id_, const std::string& name_);
	token_t(const token_t&);
	int token_id() const { return token_id_; }
	const std::string& name() const { return name_; }
	bool exist() const;
private:
	int token_id_;
	std::string name_;
};

class table_t
{
public:
	table_t();
	table_t(int table_group_id_, const std::string& table_group_name_);
	table_t(int table_group_id_, const std::string& table_group_name_, int table_id_, const std::string& name_);
	table_t(const table_t&);
	int table_group_id() const { return table_group_id_; }
	const std::string& table_group_name() const { return table_group_name_; }
	int table_id() const { return table_id_; }
	const std::string& name() const { return name_; }
	bool exist() const;
private:
	int table_group_id_;
	std::string table_group_name_;
	int table_id_;
	std::string name_;
};

};

#endif


#include "menu_interface.hpp"

namespace ksi_cafe {

dish_t::dish_t(int dish_id_, const std::string& name_, const std::string& code_,
												 int hotkey_color_, int kitchen_type_id_, int employee_meal_)
: dish_id_(dish_id_), name_(name_), code_(code_), hotkey_color_(hotkey_color_),
kitchen_type_id_(kitchen_type_id_), employee_meal_(employee_meal_)
{
}

modifier_group_t::modifier_group_t(int pos_, int group_id_, const std::string& name_, int max_weight_)
: pos_(pos_), group_id_(group_id_), name_(name_), max_weight_(max_weight_)
{
}

modifier_t::modifier_t(int group_id_, int modifier_id_, int dish_id_, int kitchen_type_id_, const std::string& name_, int color_, int weight_)
: group_id_(group_id_), modifier_id_(modifier_id_), dish_id_(dish_id_), kitchen_type_id_(kitchen_type_id_), name_(name_), color_(color_), weight_(weight_)
{
}

menu_item_t::menu_item_t(int parent_id_, int menu_item_id_, const std::string& name_, int color_)
: parent_id_(parent_id_), menu_item_id_(menu_item_id_), name_(name_), color_(color_)
{
}


const menu_item_t& ksi_cafe::menu_item_t::operator=(const menu_item_t* menu_item_ptr)
{
	parent_id_ = menu_item_ptr->parent_id_;
  menu_item_id_ = menu_item_ptr->menu_item_id_;
  name_ = menu_item_ptr->name_;
  color_ = menu_item_ptr->color_;
  return *this;
}

menu_dish_t::menu_dish_t(const dish_t& dish, int menu_item_id_, double price_, int hotkey_id_, bool disable_)
: dish_t(dish), menu_item_id_(menu_item_id_), price_(price_), hotkey_id_(hotkey_id_), disable_(disable_)
{
}

menu_dish_modifier_t::menu_dish_modifier_t(const modifier_t& modifier, int menu_item_id_, int dish_id_, double price_)
: modifier_t(modifier), menu_item_id_(menu_item_id_), dish_id_(dish_id_), price_(price_)
{
}

token_t::token_t()
: token_id_(0), name_("")
{
}

token_t::token_t(int token_id_, const std::string& name_)
: token_id_(token_id_), name_(name_)
{
}

token_t::token_t(const ksi_cafe::token_t& token)
: token_id_(token.token_id_), name_(token.name_)
{
}

bool token_t::exist() const
{
	return token_id_ && (name_ != "");
}

table_t::table_t()
: table_group_id_(0), table_group_name_(""), table_id_(0), name_("")
{
}

table_t::table_t(int table_group_id_, const std::string& table_group_name_)
: table_group_id_(table_group_id_), table_group_name_(table_group_name_), table_id_(0), name_("")
{
}

table_t::table_t(int table_group_id_, const std::string& table_group_name_, int table_id_, const std::string& name_)
: table_group_id_(table_group_id_), table_group_name_(table_group_name_), table_id_(table_id_), name_(name_)
{
}

table_t::table_t(const table_t& table)
: table_group_id_(table.table_group_id_), table_group_name_(table.table_group_name_),
	table_id_(table.table_id_), name_(table.name_)
{
}

bool table_t::exist() const
{
	return table_id_ && (name_ != "");
}


};

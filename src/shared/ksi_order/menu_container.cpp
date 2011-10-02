#include "menu_container.hpp"
#include <ksi_include/ksi_exceptions.hpp>
#include <functional>
#include <algorithm>
#include <boost/bind.hpp>
#include <queue>
#include <deque>
#include <ksi_include/algo_functor.hpp>
#include <shared_modules/connect.hpp>
#include <ksi_util/util.hpp>
#include <boost/lexical_cast.hpp>

namespace ksi_cafe {

dish_modifier_container_t menu_t::container_;

void dish_modifier_container_t::load() {
	//dish_.clear(); modifier_group_.clear(); modifier_.clear();
	dish_vector_t new_dish_;
	modifier_group_vector_t new_modifier_group_;
	modifier_vector_t new_modifier_;

	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();

	query->create_statement
	(
	"select dish_id, name, is_beverage, hotkey_color, employee_meal, code \
	from ksi.dish \
	where deleted is null"
	);
	query->execute_statement();

	while (!query->next())
	{
		int dish_id = static_cast<int>(boost::get<double>(query->get(0)));
		std::string name = boost::get<std::string>(query->get(1));
		int kitchen_type_id = static_cast<int>(boost::get<double>(query->get(2)));
		std::string hotkey_color_str = boost::get<std::string>(query->get(3));
		int employee_meal = static_cast<int>(boost::get<double>(query->get(4)));
		std::string code = boost::get<std::string>(query->get(5));
		new_dish_.push_back(dish_t(dish_id, full_trim(name), code, color_from_str(hotkey_color_str), kitchen_type_id, employee_meal));
	}

	query->create_statement
	(
	"select group_id, name, max_weight, pos \
	from ksi.mod_group \
	order by nvl(pos, 999999)"
	);
	query->execute_statement();

	while (!query->next())
	{

		int group_id = static_cast<int>(boost::get<double>(query->get(0)));
		std::string name = boost::get<std::string>(query->get(1));
		int max_weight = static_cast<int>(boost::get<double>(query->get(2)));
		int pos = static_cast<int>(boost::get<double>(query->get(3)));

		new_modifier_group_.push_back(modifier_group_t(pos, group_id, name, max_weight));
	}

	query->create_statement
	(
	"select modifier_id, modifier.name, modifier.group_id, modifier.dish_id, \
	modifier.weight, modifier.hotkey_color, dish.is_beverage as kitchen_type_id \
  	from ksi.modifier \
  	left join ksi.dish on dish.dish_id = modifier.dish_id \
  	where modifier.deleted is null \
  	order by nvl(pos, 999999)"
	);
	query->execute_statement();

	while (!query->next())
	{
		int modifier_id = static_cast<int>(boost::get<double>(query->get(0)));
		std::string name = boost::get<std::string>(query->get(1));
		int group_id = static_cast<int>(boost::get<double>(query->get(2)));
		int dish_id = static_cast<int>(boost::get<double>(query->get(3)));
		int weight = static_cast<int>(boost::get<double>(query->get(4)));
		std::string hotkey_color_str = boost::get<std::string>(query->get(5));
		int kitchen_type_id = static_cast<int>(boost::get<double>(query->get(6)));
		new_modifier_.push_back(modifier_t(group_id, modifier_id, dish_id, kitchen_type_id, full_trim(name), color_from_str(hotkey_color_str), weight));
	}

	std::swap(dish_, new_dish_);
	std::swap(modifier_group_, new_modifier_group_);
	std::swap(modifier_, new_modifier_);
	loaded_ = true;
}

const dish_t& dish_modifier_container_t::dish(int dish_id) {
	if (!loaded_) load();
	dish_vector_t::const_iterator It =
		std::find_if(dish_.begin(), dish_.end(),
		boost::bind(std::equal_to<int>(), boost::bind(&dish_t::dish_id, _1), dish_id));
	if (It == dish_.end()) THROW_MESSAGE(10128, "dish not found: dish_id = " + boost::lexical_cast<std::string>(dish_id), "dish_modifier_container_t::dish(int)");
	return *It;
}

const modifier_group_t& dish_modifier_container_t::modifier_group(int group_id) {
	if (!loaded_) load();
	modifier_group_vector_t::const_iterator It =
		std::find_if(modifier_group_.begin(), modifier_group_.end(),
		boost::bind(std::equal_to<int>(), boost::bind(&modifier_group_t::group_id, _1), group_id));
	if (It == modifier_group_.end())
		THROW_MESSAGE(10129, "modifier_group not found: group_id = " + boost::lexical_cast<std::string>(group_id), "dish_modifier_container_t::modifier_group(int)");
	return *It;
}

const modifier_t& dish_modifier_container_t::modifier(int group_id, int modifier_id) {
	if (!loaded_) load();
	modifier_vector_t::const_iterator It =
		std::find_if(modifier_.begin(), modifier_.end(),
			boost::bind(std::logical_and<bool>(),
				boost::bind(std::equal_to<int>(), boost::bind(&modifier_t::group_id, _1), group_id),
				boost::bind(std::equal_to<int>(), boost::bind(&modifier_t::modifier_id, _1), modifier_id)));
	if (It == modifier_.end()) THROW_MESSAGE(10130, "modifier not found: group_id = " + boost::lexical_cast<std::string>(group_id) + ", modifier_id = " + boost::lexical_cast<std::string>(modifier_id), "dish_modifier_container_t::modifier(int, int)");
	return *It;
}

menu_t::menu_t(int menu_id_, int cafe_id_, const std::string& name_, int version_)
: menu_id_(menu_id_), cafe_id_(cafe_id_), name_(name_), version_(version_), loaded_(false)
{
}

bool menu_t::find_menu_item(int parent_id, const std::string& menu_item_name) const {
	std::vector<menu_item_t>::const_iterator It =
		std::find_if(menu_item_.begin(), menu_item_.end(),
			boost::bind(std::logical_and<bool>(),
				boost::bind(std::equal_to<int>(), boost::bind(&menu_item_t::parent_id, _1), parent_id),
				boost::bind(std::equal_to<std::string>(), boost::bind(&menu_item_t::name, _1), menu_item_name)));
	if (It == menu_item_.end()) return false;
	return true;
}

int menu_t::find_menu_item_id(int parent_id, const std::string& menu_item_name) const {
	std::vector<menu_item_t>::const_iterator It =
		std::find_if(menu_item_.begin(), menu_item_.end(),
			boost::bind(std::logical_and<bool>(),
				boost::bind(std::equal_to<int>(), boost::bind(&menu_item_t::parent_id, _1), parent_id),
				boost::bind(std::equal_to<std::string>(), boost::bind(&menu_item_t::name, _1), menu_item_name)));
	return It->menu_item_id();
}

bool menu_t::find_menu_item(int parent_id, int menu_item_id, const std::vector<menu_item_t>& new_menu_item_) const {
	std::vector<menu_item_t>::const_iterator It =
		std::find_if(new_menu_item_.begin(), new_menu_item_.end(),
			boost::bind(std::logical_and<bool>(),
				boost::bind(std::equal_to<int>(), boost::bind(&menu_item_t::parent_id, _1), parent_id),
				boost::bind(std::equal_to<int>(), boost::bind(&menu_item_t::menu_item_id, _1), menu_item_id)));
	if (It == new_menu_item_.end()) return false;
	return true;
}

bool menu_t::find_menu_item(int parent_id, int menu_item_id) const {
	std::vector<menu_item_t>::const_iterator It =
		std::find_if(menu_item_.begin(), menu_item_.end(),
			boost::bind(std::logical_and<bool>(),
				boost::bind(std::equal_to<int>(), boost::bind(&menu_item_t::parent_id, _1), parent_id),
				boost::bind(std::equal_to<int>(), boost::bind(&menu_item_t::menu_item_id, _1), menu_item_id)));
	if (It == menu_item_.end()) return false;
	return true;
}

bool menu_t::find_menu_dish(int menu_item_id, int dish_id, const std::vector<menu_dish_t>& new_menu_dish_) const {
	std::vector<menu_dish_t>::const_iterator It =
		std::find_if(new_menu_dish_.begin(), new_menu_dish_.end(),
			boost::bind(std::logical_and<bool>(),
			boost::bind(std::equal_to<int>(), boost::bind(&menu_dish_t::menu_item_id, _1), menu_item_id),
			boost::bind(std::equal_to<int>(), boost::bind(&menu_dish_t::dish_id, _1), dish_id)));
	if (It == new_menu_dish_.end()) return false;
	return true;
}

bool menu_t::find_menu_dish(int menu_item_id, int dish_id) const {
	std::vector<menu_dish_t>::const_iterator It =
		std::find_if(menu_dish_.begin(), menu_dish_.end(),
			boost::bind(std::logical_and<bool>(),
			boost::bind(std::equal_to<int>(), boost::bind(&menu_dish_t::menu_item_id, _1), menu_item_id),
			boost::bind(std::equal_to<int>(), boost::bind(&menu_dish_t::dish_id, _1), dish_id)));
	if (It == menu_dish_.end()) return false;
	return true;
}

const menu_dish_t* menu_t::find_by_code(const std::string& code) const {
	std::vector<menu_dish_t>::const_iterator It = 
		std::find_if(menu_dish_.begin(), menu_dish_.end(),
			boost::bind(std::equal_to<std::string>(), boost::bind(&menu_dish_t::code, _1), code));
	if (It == menu_dish_.end()) return 0;
	return &(*It);
}

void menu_t::refresh() {
	//menu_item_.clear(); menu_dish_.clear(); menu_dish_modifier_.clear();

	std::vector<menu_item_t> new_menu_item_;
	std::vector<menu_dish_t> new_menu_dish_;
	std::vector<menu_dish_modifier_t> new_menu_dish_modifier_;
	
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();

	query->create_statement
	(
	"select menu_item.menu_item_id \
    , menu_item.parent_id \
    , menu_item.name as menu_item_name \
    , nvl(menu_item.color, 'CAE4FF') as menu_item_color \
    , menu_dish.dish_id \
    , menu_dish.hotkey_id \
    , menu_dish.hotkey_color \
    , menu_dish.price as dish_price \
    , modifier.group_id \
    , modifier.modifier_id \
    , menu_dish_modifier.price as modifier_price \
	from ksi.menu_item \
	left join ksi.menu_dish on menu_dish.menu_item_id = menu_item.menu_item_id \
	left join ksi.menu_dish_modifier on menu_dish_modifier.menu_item_id = menu_dish.menu_item_id and menu_dish_modifier.dish_id = menu_dish.dish_id \
	left join ksi.modifier on modifier.modifier_id = menu_dish_modifier.modifier_id and menu_dish_modifier.active = 1\
	where menu_id = :menu_id \
	order by nvl(menu_item.pos, 999999), nvl(menu_dish.pos, 999999), nvl(modifier.pos, 999999)"
	);

	query->set("menu_id", menu_id_);

	query->execute_statement();

	while (!query->next())
	{
		int menu_item_id = static_cast<int>(boost::get<double>(query->get(0)));
		int parent_id = static_cast<int>(boost::get<double>(query->get(1)));
		std::string menu_item_name = boost::get<std::string>(query->get(2));
		std::string menu_item_color_str = boost::get<std::string>(query->get(3));
		int dish_id = static_cast<int>(boost::get<double>(query->get(4)));
		int hotkey_id = static_cast<int>(boost::get<double>(query->get(5)));
		std::string hotkey_color_str = boost::get<std::string>(query->get(6));
		double dish_price = boost::get<double>(query->get(7));
		int group_id = static_cast<int>(boost::get<double>(query->get(8)));
		int modifier_id = static_cast<int>(boost::get<double>(query->get(9)));
		double modifier_price = boost::get<double>(query->get(10));

		if (!find_menu_item(parent_id, menu_item_id, new_menu_item_)) new_menu_item_.push_back(menu_item_t(parent_id, menu_item_id, menu_item_name, color_from_str(menu_item_color_str)));
		if (dish_id != 0 && !find_menu_dish(menu_item_id, dish_id, new_menu_dish_)) new_menu_dish_.push_back(menu_dish_t(container_.dish(dish_id), menu_item_id, dish_price, hotkey_id, false));
		if (modifier_id != 0) new_menu_dish_modifier_.push_back(menu_dish_modifier_t(container_.modifier(group_id, modifier_id), menu_item_id, dish_id, modifier_price));
	}

	std::deque<const menu_item_t*> queue1, queue2;

	int menu_item_id = 0;
	do {
		std::vector<const menu_item_t*> source;
		std::copy(source.begin(), source.end(), std::back_inserter(queue1));
		if (queue1.empty()) break;
		queue2.push_back(queue1.front());
		menu_item_id = queue1.front()->menu_item_id();
		queue1.pop_front();
	} while (!queue1.empty());


	std::copy(queue2.begin(), queue2.end(), new_menu_item_.begin());

	{
		std::vector<menu_item_t> tmp_menu_item_;
		for (size_t i=0; i<new_menu_item_.size(); ++i) {
			std::vector<const menu_dish_t*> dish_vec = menu_dish(new_menu_item_[i].menu_item_id(), new_menu_dish_);
			if (dish_vec.size()) new_menu_item_[i].set_color(dish_vec[0]->hotkey_color());
			std::vector<const menu_item_t*> menu_item_vec = menu_item(new_menu_item_[i].menu_item_id(), new_menu_item_);
			if (dish_vec.size() || menu_item_vec.size()) tmp_menu_item_.push_back(new_menu_item_[i]);
		}
		std::swap(new_menu_item_, tmp_menu_item_);
	}

	std::swap(menu_item_, new_menu_item_);
	std::swap(menu_dish_, new_menu_dish_);
	std::swap(menu_dish_modifier_, new_menu_dish_modifier_);
	
	loaded_ = true;
}

std::vector<const menu_item_t*> menu_t::menu_item(int parent_id, const std::vector<menu_item_t>& new_menu_item_) const {
	std::vector<const menu_item_t*> res_vec;
	
	copy_ptr_if(new_menu_item_.begin(), new_menu_item_.end(), std::back_inserter(res_vec),
		boost::bind(std::equal_to<int>(), boost::bind(&menu_item_t::parent_id, _1), parent_id));
	
	return res_vec;
}

std::vector<const menu_item_t*> menu_t::menu_item(int parent_id) const {
	std::vector<const menu_item_t*> res_vec;

	copy_ptr_if(menu_item_.begin(), menu_item_.end(), std::back_inserter(res_vec),
		boost::bind(std::equal_to<int>(), boost::bind(&menu_item_t::parent_id, _1), parent_id));

	return res_vec;
}

std::vector<const menu_dish_t*> menu_t::menu_dish(int parent_id, const std::vector<menu_dish_t>& new_menu_dish_) const {
	std::vector<const menu_dish_t*> res_vec;

	copy_ptr_if(new_menu_dish_.begin(), new_menu_dish_.end(), std::back_inserter(res_vec),
		boost::bind(std::equal_to<int>(), boost::bind(&menu_dish_t::parent_id, _1), parent_id));

	return res_vec;
}

std::vector<const menu_dish_t*> menu_t::menu_dish(int parent_id) const {
	std::vector<const menu_dish_t*> res_vec;

	copy_ptr_if(menu_dish_.begin(), menu_dish_.end(), std::back_inserter(res_vec),
		boost::bind(std::equal_to<int>(), boost::bind(&menu_dish_t::parent_id, _1), parent_id));

	return res_vec;
}

std::vector<const menu_dish_t*> menu_t::hot_dish() const {
	std::vector<const menu_dish_t*> res_vec;

	copy_ptr_if(menu_dish_.begin(), menu_dish_.end(), std::back_inserter(res_vec),
    boost::bind(std::not_equal_to<int>(), boost::bind(&menu_dish_t::hotkey_id, _1), 0));

	return res_vec;
}

std::vector<const menu_item_t*> menu_t::menu_item() const {
	std::vector<const menu_item_t*> res_vec;

	std::transform(menu_item_.begin(), menu_item_.end(), std::back_inserter(res_vec), ref_to_ptr_functor<menu_item_t>());

	return res_vec;
}

std::map<const menu_dish_modifier_t*, const modifier_group_t*> menu_t::menu_dish_modifier(const menu_dish_t* menu_dish_ptr) const {
	std::map<const menu_dish_modifier_t*, const modifier_group_t*> res_map;

	for (size_t i=0; i<menu_dish_modifier_.size(); i++)
	{
		if (menu_dish_modifier_[i].menu_item_id() == menu_dish_ptr->menu_item_id() && menu_dish_modifier_[i].modifier_dish_id() == menu_dish_ptr->dish_id())
			res_map[&menu_dish_modifier_[i]] = &container_.modifier_group(menu_dish_modifier_[i].group_id());
	}
	return res_map;
}

void menu_t::set_table_group_id(const std::vector<int>& kt_id_vec) {
	std::for_each(menu_dish_.begin(), menu_dish_.end(), boost::bind(&menu_dish_t::enable, _1));

	for (std::vector<menu_dish_t>::iterator i=menu_dish_.begin(); i!=menu_dish_.end(); ++i) {
		std::vector<int>::const_iterator It;
		It = std::find_if(kt_id_vec.begin(), kt_id_vec.end(), boost::bind(std::equal_to<int>(), (*i).kitchen_type_id(), _1));
		if (It == kt_id_vec.end()) (*i).disable();
	}
}

menu_container_t::menu_container_t() {
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();

	query->create_statement
	(
	"select menu_id, name, cafe_id, snapshot_version \
	from ksi.menu \
	where deleted is null and cafe_id is not null \
	order by cafe_id, nvl(pos, 999999)"
	);

	query->execute_statement();

	while (!query->next())
	{
		int menu_id = static_cast<int>(boost::get<double>(query->get(0)));
		std::string name = boost::get<std::string>(query->get(1));
		int cafe_id = static_cast<int>(boost::get<double>(query->get(2)));
		int version = static_cast<int>(boost::get<double>(query->get(3)));

		menus_.push_back(menu_t(menu_id, cafe_id, name, version));
	}
}

std::vector<const menu_t*> menu_container_t::menu_for_cafe(int cafe_id) const {
	std::vector<const menu_t*> res_vec;
	copy_ptr_if(menus_.begin(), menus_.end(), std::back_inserter(res_vec), boost::bind(std::equal_to<int>(), boost::bind(&menu_t::cafe_id, _1), cafe_id));
	return res_vec;
}

std::vector<menu_t*> menu_container_t::menu_for_cafe(int cafe_id) {
	std::vector<menu_t*> res_vec;
	copy_ptr_if(menus_.begin(), menus_.end(), std::back_inserter(res_vec), boost::bind(std::equal_to<int>(), boost::bind(&menu_t::cafe_id, _1), cafe_id));
	return res_vec;
}

menu_manager_t::menu_manager_t(const std::vector<menu_t*>& source_menu_vec) {
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();

	query->create_statement
	(
	"select menu_shift.menu_id, menu_shift.time_from, menu_shift.time_to \
	from ksi.menu_shift \
	inner join ksi.menu on menu_shift.menu_id = menu.menu_id \
	where shift_id = ksi.util.get_cur_shift_id \
	order by menu.pos"
	);

	query->execute_statement();

	while (!query->next())
	{
		int menu_id = static_cast<int>(boost::get<double>(query->get(0)));
		boost::posix_time::ptime time_from = boost::get<boost::posix_time::ptime>(query->get(1));
		boost::posix_time::ptime time_to = boost::get<boost::posix_time::ptime>(query->get(2));

		std::vector<menu_t*>::const_iterator It;

		It = std::find_if
		(
		source_menu_vec.begin()
		, source_menu_vec.end()
		, boost::bind(std::equal_to<int>(), boost::bind(&menu_t::menu_id, _1), menu_id)
		);

		if (It == menu_vec_.end()) continue;
		menu_vec_.push_back(*It);
		form_[*It] = time_from;
		to_[*It] = time_to;
	}

	query->create_statement
	(
	"select table_group_id, kitchen_type_id, table_group_kitchen.kitchen_id \
	from ksi.table_group_kitchen \
	inner join ksi.kitchen on kitchen.kitchen_id = table_group_kitchen.kitchen_id and kitchen.cafe_id = ksi.util.get_cur_cafe_id \
	inner join ksi.table_group on table_group_kitchen.table_group_id = table_group.table_group_id and table_group.cafe_id = ksi.util.get_cur_cafe_id and table_group.deleted is null"
	);

	query->execute_statement();

	while (!query->next())
	{
		int table_group_id = static_cast<int>(boost::get<double>(query->get(0)));
		int kitchen_type_id = static_cast<int>(boost::get<double>(query->get(1)));
		int kitchen_id = static_cast<int>(boost::get<double>(query->get(2)));
		tgk_.push_back(table_group_kitchen_t(table_group_id, kitchen_type_id, kitchen_id));
	}
}

void menu_manager_t::select(int menu_id, int table_group_id, const std::vector<int>& kt_only) {
	current_table_group_id_ = table_group_id;
	std::vector<int> vec;
	if (std::distance(kt_only.begin(), kt_only.end()) == 0)
		std::transform(tgk_.begin(), tgk_.end(), std::back_inserter(vec), boost::bind(&table_group_kitchen_t::kitchen_type_id, _1));

	for (std::vector<table_group_kitchen_t>::iterator i=tgk_.begin(); i!=tgk_.end(); ++i) {
		std::vector<int>::const_iterator It;
		It = std::find_if(kt_only.begin(), kt_only.end(), boost::bind(std::equal_to<int>(), (*i).kitchen_id(), _1));
		if (It != kt_only.end()) vec.push_back((*i).kitchen_type_id());
	}

	std::vector<int> tg;

	transform_if(tgk_.begin(), tgk_.end(), std::back_inserter(tg), boost::bind(std::equal_to<int>(), current_table_group_id_, boost::bind(&table_group_kitchen_t::table_group_id, _1)), boost::bind(&table_group_kitchen_t::kitchen_type_id, _1));

	std::vector<int> dest;

	for (std::vector<int>::iterator i=vec.begin(); i!=vec.end(); ++i) {
		std::vector<int>::iterator It;
		It = std::find(tg.begin(), tg.end(), *i);
		if (It != tg.end()) dest.push_back(*i);
	}

	std::vector<menu_t*>::iterator It;
	It = std::find_if(menu_vec_.begin(), menu_vec_.end(), boost::bind(std::equal_to<int>(), menu_id, boost::bind(&menu_t::menu_id, _1)));
	if (It == menu_vec_.end()) THROW_LOCATION(0, "void menu_manager_t::select(int menu_id, int table_group_id, const std::vector<int>& kt_only)");
	selected_ = *It;
	(*It)->set_table_group_id(dest);
}

simple_stop_list_t::simple_stop_list_t() {
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();

	query->create_statement
	(
	"select stop_list_id \
        , dish_id \
        , since_dt \
        , till_dt \
	from ksi.menu_stoplist \
	where cafe_id = ksi.util.get_cur_cafe_id and till_dt > sysdate"
	);

	query->execute_statement();

	while (!query->next())
	{
	  int stop_list_id = static_cast<int>(boost::get<double>(query->get(0)));
	  int dish_id = static_cast<int>(boost::get<double>(query->get(1)));
	  boost::posix_time::ptime since_dt = boost::get<boost::posix_time::ptime>(query->get(2));
	  boost::posix_time::ptime till_dt = boost::get<boost::posix_time::ptime>(query->get(3));
	  entry_.push_back(stop_list_entry_t(stop_list_id, dish_id, since_dt, till_dt));
	}
}

bool simple_stop_list_t::find_entry(int dish_id) const
{
  std::vector<stop_list_entry_t>::const_iterator It;
  It = std::find_if(entry_.begin(), entry_.end(), boost::bind(std::equal_to<int>(), dish_id, boost::bind(&stop_list_entry_t::dish_id, _1)));
  if (It == entry_.end()) return false;
  return true;
}

std::vector<const stop_list_entry_t*> simple_stop_list_t::all_entry() const
{
  std::vector<const stop_list_entry_t*> res_vec;
  copy_ptr_if(entry_.begin(), entry_.end(), std::back_inserter(res_vec), approve_t<stop_list_entry_t>());
  return res_vec;
}

const stop_list_entry_t& simple_stop_list_t::get_fined_entry(int dish_id) const
{
  return *std::find_if(entry_.begin(), entry_.end(), boost::bind(std::equal_to<int>(), dish_id, boost::bind(&stop_list_entry_t::dish_id, _1)));
}

void container_kitchen_type_group_t::load() {
	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	query->create_statement
	(
	"select ktg_id, name \
	from ksi.kitchen_type_group \
	order by pos"
	);
	query->execute_statement();
	while (!query->next()) {
		int ktg_id = static_cast<int>(boost::get<double>(query->get(0)));
		std::string name = boost::get<std::string>(query->get(1));
		ktg_vec.push_back(kitchen_type_group_t(ktg_id, name));
	}
	query->create_statement
	(
	"select kt_id, group_id \
	from ksi.kitchen_type"
	);
	query->execute_statement();
	kt_ktg_map.clear();
	ktg_kt_map.clear();
	while (!query->next()) {
		int kt_id = static_cast<int>(boost::get<double>(query->get(0)));
		int group_id = static_cast<int>(boost::get<double>(query->get(1)));
		kt_ktg_map[kt_id] = group_id;
		ktg_kt_map[group_id] = kt_id;		
	}
	loaded_ = true;
}

std::string container_kitchen_type_group_t::kitchen_type_group_name(int group_id) {
	if (!loaded()) load();
	std::vector<kitchen_type_group_t>::iterator It;
	It = std::find_if(ktg_vec.begin(), ktg_vec.end(), boost::bind(std::equal_to<int>(), boost::bind(&kitchen_type_group_t::kitchen_type_group_id, _1), group_id));
	
	if (It == ktg_vec.end()) THROW_LOCATION(0, "std::string container_kitchen_type_group_t::kitchen_type_group_name(int)");
	
	return It->name();
}

void table_container_t::load() {
	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	query->create_statement
	(
	"select tables.name as table_name, tables.table_id, tables.table_group_id, table_group.name as table_group_name \
	from ksi.tables \
	left join ksi.table_group on table_group.table_group_id = tables.table_group_id and cafe_id = ksi.util.get_cur_cafe_id \
	where tables.table_group_id in \
	( \
		select shift_person.table_group_id \
		from ksi.shift_person \
		left join ksi.table_group on table_group.table_group_id = shift_person.table_group_id and table_group.cafe_id = ksi.util.get_cur_cafe_id \
		where person_id = ksi.util.get_cur_user_id and shift_id = ksi.util.get_cur_shift_id and shift_person.cafe_id = ksi.util.get_cur_cafe_id \
	) order by lower(table_name)"
	);
	query->execute_statement();	
	while (!query->next()) {
		std::string table_name = boost::get<std::string>(query->get(0));
		int table_id = static_cast<int>(boost::get<double>(query->get(1)));
		int table_group_id = static_cast<int>(boost::get<double>(query->get(2)));
		std::string table_group_name = boost::get<std::string>(query->get(3));
		table_name_vec_.push_back(table_name);
		table_id_name_map_[table_id] = table_name;
		table_id_vec_.push_back(table_id);
		{
			std::vector<int>::const_iterator It;
			It = std::find(group_id_vec_.begin(), group_id_vec_.end(), table_group_id);
			if (It == group_id_vec_.end()) {
				group_id_vec_.push_back(table_group_id);
				group_name_vec_.push_back(table_group_name);
				group_id_name_map_[table_group_id] = table_group_name;
			}
		}
		group_table_map_[table_group_id] = table_id;
		table_group_map_[table_id] = table_group_id;
	}
}

std::vector<table_t> table_container_t::get_tables(int table_group_id) const {
	std::vector<table_t> res;
	if (table_group_id == 0) {
		for (size_t i=0; i<group_id_vec_.size(); ++i) 
			res.push_back(table_t(group_id_vec_[i], group_name_vec_[i]));
	}
	else {
		for (size_t i=0; i<table_id_vec_.size(); ++i)
			if (table_group_map_[table_id_vec_[i]] == table_group_id)
				res.push_back(table_t(table_group_id, group_id_name_map_[table_group_id], table_id_vec_[i], table_id_name_map_[table_id_vec_[i]]));
	}
	return res;
}

};

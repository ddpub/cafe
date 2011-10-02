
#include "kitchen_util.hpp"

#include <memory>
#include <ksi_include/ksi_exceptions.hpp>
#include <oracle_client/ksi_exception.hpp>
#include <boost/bind.hpp>
#include <ksi_include/algo_functor.hpp>
#include <algorithm>
#include <sstream>
#include <ksi_util/util.hpp>
#include <shared_modules/connect.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

extern int lang;

namespace ksi_kitchen {

kitchen_t::kitchen_t(ksi_client::oracle_session_i* session_, int kitchen_id_, const std::string& name_, bool enable_)
: session_(session_), kitchen_id_(kitchen_id_), name_(name_), enable_(enable_)
{
	std::auto_ptr<ksi_client::oracle_query_i> query = session_->create_query();

	query->create_statement
	(
	"select table_group_id, kitchen_type_id, kitchen_type.name \
	from ksi.table_group_kitchen \
	left join ksi.kitchen_type on kitchen_type.kt_id = table_group_kitchen.kitchen_type_id \
	left join ksi.table_group on table_group.table_group_id = table_group_kitchen.table_group_id \
	where table_group.cafe_id = ksi.util.get_cur_cafe_id and table_group.deleted is null and kitchen_id = :kitchen_id \
	order by kitchen_type.pos"
	);

	query->set("kitchen_id", kitchen_id_);

	query->execute_statement();

	while (!query->next()) {
		int base_table_group_id = static_cast<int>(boost::get<double>(query->get(0)));
		int base_kitchen_type_id = static_cast<int>(boost::get<double>(query->get(1)));
		std::string base_kitchen_name = boost::get<std::string>(query->get(2));

		table_group_id_vec_.push_back(base_table_group_id);
		kitchen_type_id_vec_.push_back(base_kitchen_type_id);
		kitchen_type_name_vec_.push_back(base_kitchen_name);
	}
}
//---------------------------------------------------------------------------
const std::string& kitchen_t::type_name(int kt_type_id) const {
	for (size_t i=0; i<kitchen_type_id_vec_.size(); i++) {
		if (kitchen_type_id_vec_[i] == kt_type_id)
			return kitchen_type_name_vec_[i];
	}

	THROW_LOCATION(100, "const std::string& kitchen_t::type_name(int kt_type_id) const");
}
//---------------------------------------------------------------------------
bool kitchen_t::kitchen_execute(int cur_table_group_id, int cur_type_id) const {
	for (size_t i=0; i<table_group_id_vec_.size(); i++) {
		if(table_group_id_vec_[i] == cur_table_group_id && kitchen_type_id_vec_[i] == cur_type_id)
			return true;
	}
	return false;
}
//---------------------------------------------------------------------------
bool kitchen_t::kitchen_execute_for_modifier(int cur_table_group_id, const order_dish_t& dish) const {
	for (size_t i=0; i<table_group_id_vec_.size(); i++) {
		if (table_group_id_vec_[i] == cur_table_group_id) {
			for (int j=0; j<dish.mod_count(); j++) {
				if (dish.modifier(j).mod_dish_id() != 0 && dish.modifier(j).kitchen_type_id() == kitchen_type_id_vec_[i]
				&& ((dish.modifier(j).count()*dish.count() + dish.modifier(j).count_wasted()) != dish.modifier(j).count_registered()))
					return true;
			}
		}
	}
	return false;
}
//---------------------------------------------------------------------------
bool kitchen_t::kitchen_execute_for_modifier(int cur_table_group_id, const order_dish_t& dish, int cur_kitchen_type_id) const {
	for (size_t i=0; i<table_group_id_vec_.size(); i++) {
		if (table_group_id_vec_[i] == cur_table_group_id) {
			for (int j=0; j<dish.mod_count(); j++) {
				if (dish.modifier(j).mod_dish_id() != 0
				&& dish.modifier(j).kitchen_type_id() == kitchen_type_id_vec_[i]
				&& kitchen_type_id_vec_[i] == cur_kitchen_type_id
				&& ((dish.modifier(j).count()*dish.count() + dish.modifier(j).count_wasted()) != dish.modifier(j).count_registered()))
					return true;
			}
		}
	}
	return false;
}
//---------------------------------------------------------------------------
std::string container_kitchen_t::type_name(int kt_type_id) const {
	for (int i=0; i<count(); i++) {
		try {
			std::string str = kitchen(i).type_name(kt_type_id);
			if (str != "") return str;
		}
		catch(exception_t& ex) {
			if (ex.code() != 100) throw;
		}
	}
	return "";
}
//---------------------------------------------------------------------------
std::vector<int> container_kitchen_t::kitchen_types() const {
	std::vector<int> res;
	for (size_t i=0; i<kitchen_vec_.size(); i++) {
		std::vector<int> cur_res = kitchen_vec_[i].get_types();
		for (size_t j=0; j<cur_res.size(); j++) {
			bool find = false;
			for (size_t k=0; k<res.size(); k++)
				if (res[k] == cur_res[j]) find = true;
			if (!find) res.push_back(cur_res[j]);
		}
	}

	std::sort(res.begin(), res.end(), comp_t(kitchen_type_weight_));
	return res;
}
//---------------------------------------------------------------------------
container_kitchen_t::container_kitchen_t(ksi_client::oracle_session_i* session_, const std::string& str): session_(session_), printMiscCharges_(false) {
	std::string par = str;
	std::stringstream tmp(ksi_cafe::full_trim(par));
	std::vector<int> kitchen_id_vec;
	std::string id_val_str;
	if (str.length() != 0)
	for (;tmp && !tmp.eof();) {
		tmp >> id_val_str;
		if (boost::to_lower_copy(id_val_str) == "mc") printMiscCharges_ = true;
		else kitchen_id_vec.push_back(boost::lexical_cast<int>(id_val_str));
	}

	std::auto_ptr<ksi_client::oracle_query_i> query = session_->create_query();

	query->create_statement
	(
	"select kitchen_id, name \
	from ksi.kitchen \
	where cafe_id = ksi.util.get_cur_cafe_id"
	);
	query->execute_statement();
	while (!query->next()) {
		int kitchen_id = static_cast<int>(boost::get<double>(query->get(0)));
		std::string name = boost::get<std::string>(query->get(1));
		bool enabled = false;
		std::vector<int>::const_iterator It;
		It = std::find_if(kitchen_id_vec.begin(), kitchen_id_vec.end(), boost::bind(std::equal_to<int>(), kitchen_id, _1));
		if (It != kitchen_id_vec.end()) enabled = true;
    //bool enabled = false;
    //for (size_t i=0; i<kitchen_id_vec.size(); i++)
    //  if (kitchen_id_vec[i] == kitchen_id) enabled = true;
		kitchen_vec_.push_back(kitchen_t(session_, kitchen_id, name, enabled));
	}

	query->create_statement
	(
	"select kt_id, pos from ksi.kitchen_type"
	);
	query->execute_statement();
	while (!query->next()) {
		int kitchen_type_id = static_cast<int>(boost::get<double>(query->get(0)));
		int pos = static_cast<int>(boost::get<double>(query->get(1)));
		kitchen_type_weight_[kitchen_type_id] = pos;
	}
}
//---------------------------------------------------------------------------
std::string container_kitchen_t::find(int kitchen_id) {
	for (int i=0; i<count(); i++)
		if (kitchen(i).kitchen_id() == kitchen_id)
			return kitchen(i).name();
	THROW_LOCATION(0, "std::string container_kitchen_t::find(int kitchen_id)");
}
//---------------------------------------------------------------------------
int container_kitchen_t::find(const std::string& kitchen_name) {
	for (int i=0; i<count(); i++)
		if (kitchen(i).name() == kitchen_name)
			return kitchen(i).kitchen_id();
	THROW_LOCATION(0, "int container_kitchen_t::find(const String& kitchen_name)");
}
//---------------------------------------------------------------------------
std::vector<int> kitchen_t::get_types(int table_group_id) const {
	std::vector<int> res;
	for (size_t i=0; i<kitchen_type_id_vec_.size(); i++) {
		if (kitchen_execute(table_group_id, kitchen_type_id_vec_[i]))
		res.push_back(kitchen_type_id_vec_[i]);
	}

	return res;
}
//---------------------------------------------------------------------------
void order_dish_modifier_t::mod_register(int reg_count, const std::string& ver, int kitchen_id, bool auto_ready) const {
	std::auto_ptr<ksi_client::oracle_query_i> query = session_->create_query();

	query->create_statement
	(
	"update ksi.order_dish_modifier set count_registered = nvl(count_registered,0)+:count_registered, \
	ki_version = :ki_version, kitchen_id = decode(:kitchen_id, 0, null, :kitchen_id), ready = nvl(ready,0)+:ready \
	where order_dish_id = :order_dish_id and cafe_id = ksi.util.get_cur_cafe_id and modifier_id = :modifier_id"
	);

	query->set("order_dish_id", order_dish_id_);
	query->set("count_registered", reg_count*count_);
	query->set("ready", auto_ready ? reg_count*count_ : 0);
	query->set("modifier_id", modifier_id_);
	query->set("ki_version", ver);
	query->set("kitchen_id", kitchen_id);

	ksi_cafe::logical_transaction_t logical_trans(session_, 104);

	query->execute_statement();
}
//---------------------------------------------------------------------------
void order_dish_modifier_t::mod_self_register(int dish_count_registered, const std::string& ver, int kitchen_id, bool auto_ready) const {
	std::auto_ptr<ksi_client::oracle_query_i> query = session_->create_query();

	query->create_statement
	(
	"update ksi.order_dish_modifier set count_registered = :count_registered, ki_version = :ki_version, \
	kitchen_id = decode(:kitchen_id, 0, null, :kitchen_id), ready = :ready \
	where order_dish_id = :order_dish_id and cafe_id = ksi.util.get_cur_cafe_id and modifier_id = :modifier_id"
	);
	query->set("order_dish_id", order_dish_id_);
	query->set("count_registered", dish_count_registered*count_);
	query->set("modifier_id", modifier_id_);
	query->set("ready", auto_ready ? dish_count_registered*count_ : 0);
	query->set("ki_version", ver);
	query->set("kitchen_id", kitchen_id);

	ksi_cafe::logical_transaction_t logical_trans(session_, 104);

	query->execute_statement();
}
//---------------------------------------------------------------------------
void order_dish_t::dish_register(int count, const std::string& ver, int kitchen_id, bool auto_ready) const {
	std::auto_ptr<ksi_client::oracle_query_i> query = session_->create_query();

	query->create_statement
	(
	"update ksi.order_dish set count_registered = nvl(count_registered,0)+:count_registered, \
	ki_version = :ki_version, kitchen_id = decode(:kitchen_id, 0, null, :kitchen_id), \
	ready = nvl(ready,0) + :ready \
	where order_dish_id = :order_dish_id and cafe_id = ksi.util.get_cur_cafe_id"
	);

	query->set("order_dish_id", order_dish_id_);
	query->set("count_registered", count);
	query->set("ki_version", ver);
	query->set("kitchen_id", kitchen_id);
	query->set("ready", auto_ready ? count : 0);

	ksi_cafe::logical_transaction_t logical_trans(session_, 104);

	query->execute_statement();
}
//---------------------------------------------------------------------------
void order_dish_t::mods_register(int table_group_id, const container_kitchen_t& container, int count, const std::string& ver, bool auto_ready) const {
	for (int index = 0; index < container.count(); index++) {
		if (container.kitchen(index).enable())
			for (int i=0; i<mod_count(); i++) {
				if (mods_[i].mod_dish_id() != 0)
					if (container.kitchen(index).kitchen_execute(table_group_id, mods_[i].kitchen_type_id())) {
						mods_[i].mod_register(count_, ver, container.kitchen(index).kitchen_id(), auto_ready);
					}
			}
	}
}
//---------------------------------------------------------------------------
void order_dish_t::mods_self_register(int table_group_id, const container_kitchen_t& container, const std::string& ver, bool auto_ready) const {
	for (int index = 0; index < container.count(); index++) {
		if (container.kitchen(index).enable())
			for (int i=0; i<mod_count(); i++) {
				if (mods_[i].mod_dish_id() != 0)
					if (container.kitchen(index).kitchen_execute(table_group_id, mods_[i].kitchen_type_id())) {
						mods_[i].mod_self_register(count_registered(), ver, container.kitchen(index).kitchen_id(), auto_ready);
					}
			}
	}
}
//---------------------------------------------------------------------------
bool try_order_register(const order_t& order, const container_kitchen_t& container, const std::string& ver, bool auto_ready) {
	bool find = false;
	for (int index = 0; index < container.count(); ++index) {
		if (container.kitchen(index).enable())
		for (int i=0; i<order.dish_count(); ++i) {
			if (!order.dish(i).registered() 
			&& (container.printMiscCharges() ? true : order.dish(i).dish_id()) 
			&& (order.dish(i).dish_id() ? container.kitchen(index).kitchen_execute(order.table().table_group_id(), order.dish(i).kitchen_type_id()) : true)
			) {
				int reg_count = order.dish(i).count() + order.dish(i).count_wasted() - order.dish(i).count_registered();
				order.dish(i).dish_register(reg_count, ver, container.kitchen(index).kitchen_id(), auto_ready);
				order.dish(i).mods_register(order.table().table_group_id(), container, reg_count, ver, auto_ready);
				find = true;
			}
		}
	}
	return find;
}
//---------------------------------------------------------------------------
bool try_mod_register(const order_t& order, const container_kitchen_t& container, const std::string& ver, bool auto_ready) {
	bool find = false;
	for (int index = 0; index < container.count(); index++) {
		if (container.kitchen(index).enable()) {
			for (int i=0; i<order.dish_count(); i++) {
				if (order.dish(i).registered() && container.kitchen(index).kitchen_execute_for_modifier(order.table().table_group_id(), order.dish(i))) {
					order.dish(i).mods_self_register(order.table().table_group_id(), container, ver, auto_ready);
					find = true;
				}
			}
		}
	}
	return find;
}
//---------------------------------------------------------------------------
std::string order_t::type_name() const {
	std::string str = "language is not set";
	if (type_for_here()) {
		if (lang == 1) str = "FOR HERE";
		else if (lang == 2) str = "ÇÄÅÑÜ";
	}
	else if (type_to_go()) {
		if (lang == 1) str = "TO GO";
		else if (lang == 2) str = "Ñ ÑÎÁÎÉ";
	}
	else if (type_delivery()) {
		if (lang == 1) str = "DELIVERY";
		else if (lang == 2) str = "ÄÎÑÒÀÂÊÀ";
	}
	else if (type_employee_meal()) {
		if (lang == 1) str = "EMPLOYEE MEAL";
		else if (lang == 2) str = "ÏÈÒÀÍÈÅ ÑÎÒÐÓÄÍÈÊÀ";
	}
	else if (type_pickup()) {
		if (lang == 1) str = "PICKUP";
		else if (lang == 2) str = "ÏÈÊÀÏ";
	}
	else {
		str = "type is not set";
	}
	return str;
}
//---------------------------------------------------------------------------
boost::posix_time::ptime order_t::get_first_not_register_time() const {
	boost::posix_time::ptime res = boost::posix_time::second_clock::local_time();

	for (int i=0; i<dish_count(); i++) {
		if (!dish(i).time().is_not_a_date_time() && dish(i).total_count() != 0) {
			if (!dish(i).registered() || !dish(i).modifier_registered()) {
				if (res.is_not_a_date_time()) res = dish(i).time();
				else if (dish(i).time() < res) res = dish(i).time();
			}
		}
	}
	return res;
}
//---------------------------------------------------------------------------
order_t::order_t(ksi_client::oracle_session_i* session_, int order_id): session_(session_), order_id_(order_id) {
	std::auto_ptr<ksi_client::oracle_query_i> query = session_->create_query();

	query->create_statement
	(
	"select meal_person_id, type_id, creator_id \
	, orders.table_group_id, tables, token, table_group.name \
	, memo, order_delivery.delivery_date, tables.table_id \
	from ksi.orders \
	left join ksi.table_group on table_group.table_group_id = orders.table_group_id and table_group.cafe_id = ksi.util.get_cur_cafe_id \
	left join ksi.order_delivery on order_delivery.delivery_id = orders.delivery_id \
	left join ksi.tables on tables.name = orders.tables \
	where order_id = :order_id and orders.cafe_id = ksi.util.get_cur_cafe_id"
	);
	query->set("order_id", order_id_);

	query->execute_statement();

	if (!query->next()) {
		employee_id_ = static_cast<int>(boost::get<double>(query->get(0)));
		type_id_ = static_cast<int>(boost::get<double>(query->get(1)));
		creator_id_ = static_cast<int>(boost::get<double>(query->get(2)));
		int table_group_id = static_cast<int>(boost::get<double>(query->get(3)));
		std::string table_name = boost::get<std::string>(query->get(4));
		std::string token_name = boost::get<std::string>(query->get(5));
		std::string table_group_name = boost::get<std::string>(query->get(6));
		memo_ = boost::get<std::string>(query->get(7));
		delivery_time_ = boost::get<boost::posix_time::ptime>(query->get(8));
		int table_id = static_cast<int>(boost::get<double>(query->get(9)));
		table_ = table_t(table_group_id, table_id, table_name, table_group_name);
		token_ = token_t(token_name);
	}
	else THROW_LOCATION(1, "order_t::order_t(int order_id): order_id_(order_id)");

	query->create_statement
	(
	"select order_dish.dish_id, order_dish.count, order_dish.order_dish_id, \
	dish.name, order_dish.count_wasted, time, order_dish.count_registered \
	, dish.is_beverage, order_dish.description, order_id \
	from ksi.order_dish \
	left join ksi.dish on dish.dish_id = order_dish.dish_id \
	where order_id=:order_id and cafe_id = ksi.util.get_cur_cafe_id \
	for update nowait"
	);
	query->set("order_id", order_id_);

	std::auto_ptr<ksi_client::oracle_query_i> mquery = session_->create_query();
	mquery->create_statement
	(
	"select order_dish_modifier.modifier_id, \
	order_dish_modifier.count, modifier.name, modifier.dish_id, dish.is_beverage, \
	order_dish_modifier.count_wasted, order_dish_modifier.count_registered \
	from ksi.order_dish_modifier \
	left  join ksi.modifier on modifier.modifier_id = order_dish_modifier.modifier_id \
	left  join ksi.dish on dish.dish_id = modifier.dish_id \
	where order_dish_id = :order_dish_id and order_dish_modifier.cafe_id = ksi.util.get_cur_cafe_id"
	);

	int i = 0;

	for (; i<10; ++i) {
		try {
			query->execute_statement();
			i = 22;
		}
		catch (ksi_client::ksi_exception&) {
			continue;
		}
	}
	if (i != 23) {
		THROW_LOCATION(666, "order_t::order_t(int order_id): order_id_(order_id)");
	}

	while (!query->next()) {
		int dish_id = static_cast<int>(boost::get<double>(query->get(0)));
		int count = static_cast<int>(boost::get<double>(query->get(1)));
		int order_dish_id = static_cast<int>(boost::get<double>(query->get(2)));
		std::string dish_name = boost::get<std::string>(query->get(3));
		int count_wasted = static_cast<int>(boost::get<double>(query->get(4)));
		boost::posix_time::ptime time = boost::get<boost::posix_time::ptime>(query->get(5));
		int count_registered = static_cast<int>(boost::get<double>(query->get(6)));
		int kitchen_type_id = static_cast<int>(boost::get<double>(query->get(7)));
		std::string description = boost::get<std::string>(query->get(8));

		mquery->set("order_dish_id", order_dish_id);
		mquery->execute_statement();
		std::vector<order_dish_modifier_t> mod_vec;
		while (!mquery->next()) {
			int modifier_id = static_cast<int>(boost::get<double>(mquery->get(0)));
			int count = static_cast<int>(boost::get<double>(mquery->get(1)));
			std::string modifier_name = boost::get<std::string>(mquery->get(2));
			int modifier_dish_id = static_cast<int>(boost::get<double>(mquery->get(3)));
			int kitchen_type_id = static_cast<int>(boost::get<double>(mquery->get(4)));
			int count_wasted = static_cast<int>(boost::get<double>(mquery->get(5)));
			int count_registered = static_cast<int>(boost::get<double>(mquery->get(6)));
			mod_vec.push_back(order_dish_modifier_t(session_, order_dish_id, modifier_name, modifier_id, modifier_dish_id, kitchen_type_id, count, count_wasted, count_registered));
		}
		order_dish_vec_.push_back(order_dish_t(session_, order_dish_id, dish_id, dish_name, kitchen_type_id, count, count_wasted, count_registered, description, time, mod_vec));

	}
}
//---------------------------------------------------------------------------

};


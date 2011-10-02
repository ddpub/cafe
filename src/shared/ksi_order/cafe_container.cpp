#include "cafe_container.hpp"
#include <shared_modules/connect.hpp>
#include <algorithm>
#include <functional>
#include <memory>
#include <boost/bind.hpp>

namespace ksi_cafe {

cafe_container_t::cafe_container_t(bool load_for_all_cafe)
{
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();
	if (load_for_all_cafe) {
		query->create_statement
		(
		"select \
		    cafe.cafe_id \
		  , cafe.name \
		  , cafe.state_id \
		  , state.shortname \
		  , state.name \
		  , cafe.zip \
		  , cafe.city \
		  , cafe.streetaddress \
		  , cafe.phone \
		  , cafe.delivery_fee \
		  , cafe.delivery_order_setup_time \
		  , cafe.prepare_order_time \
		  , cafe.sales_tax \
		  , cafe.delivery_free_subtotal \
		  , cafe.delivery_min_subtotal \
		  , cafe.meal_free_limit \
		  , cafe.chain_name \
		  , cafe.delivery_notes \
		  , cafe.receipt_footer \
		  , cafe.expense_auth_limit \
		  , table_group_cafe.table_group_id \
		  , table_group.name \
		  , gratuity \
		  , receipt_header \
		  from ksi.cafe \
		  left join ksi.state on state.state_id = cafe.state_id \
		  left join \
		  ( \
		  select table_group_id, cafe_id from ksi.table_group where deleted is null and is_default = 1 \
		  UNION \
		  select max(table_group_id), cafe_id \
		  from ksi.table_group \
		  where deleted is null and cafe_id in \
		  ( \
		  select cafe_id \
		  from \
		  (select cafe_id, max(is_default) as max_is_default from ksi.table_group where deleted is null group by cafe_id) \
		  where max_is_default = 0 \
		  ) \
		  group by cafe_id \
		  ) table_group_cafe on table_group_cafe.cafe_id = cafe.cafe_id \
		  left join ksi.table_group on table_group.table_group_id = table_group_cafe.table_group_id \
		  where cafe.type_id = 2 and cafe.deleted is null and table_group_cafe.table_group_id = table_group.table_group_id \
		  order by cafe.cafe_id"
		);
		query->execute_statement();

		while (!query->next())
		{
			int cafe_id = static_cast<int>(boost::get<double>(query->get(0)));
			std::string name = boost::get<std::string>(query->get(1));
			int state_id = static_cast<int>(boost::get<double>(query->get(2)));
			std::string fullstate = boost::get<std::string>(query->get(3));
			std::string shortstate = boost::get<std::string>(query->get(4));
			int zip = static_cast<int>(boost::get<double>(query->get(5)));
			std::string city = boost::get<std::string>(query->get(6));
			std::string streetaddress = boost::get<std::string>(query->get(7));
			std::string phone = boost::get<std::string>(query->get(8));
			int delivery_time = static_cast<int>(boost::get<double>(query->get(9)));
			int prepare_time = static_cast<int>(boost::get<double>(query->get(10)));
			double sales_tax = boost::get<double>(query->get(11));
			double delivery_fee = boost::get<double>(query->get(12));
			double delivery_free_subtotal = boost::get<double>(query->get(13));
			double delivery_min_subtotal = boost::get<double>(query->get(14));
			double meal_free_limit = boost::get<double>(query->get(15));
			std::string chain = boost::get<std::string>(query->get(16));
			std::string delivery_notes = boost::get<std::string>(query->get(17));
			std::string receipt_footer = boost::get<std::string>(query->get(18));
			int expense_auth_limit = static_cast<int>(boost::get<double>(query->get(19)));
			int table_group_id = static_cast<int>(boost::get<double>(query->get(20)));
			std::string table_group_name = boost::get<std::string>(query->get(21));
			double gratuity = boost::get<double>(query->get(22));
			std::string receipt_header = boost::get<std::string>(query->get(23));

			state_t state = state_t(state_id, fullstate, shortstate);
			address_t address = address_t(streetaddress, city, state, zip);
			cafe_t cafe = cafe_t(cafe_id, chain, name, address, phone, delivery_time, prepare_time,
	                         sales_tax, delivery_fee, delivery_free_subtotal,
	                         delivery_min_subtotal, meal_free_limit, delivery_notes, 
							 receipt_header, receipt_footer, expense_auth_limit, 
							 table_group_id, table_group_name, gratuity);

			container_.push_back(cafe);
		}
	}
	query->create_statement
	(
	"select cafe.cafe_id, cafe.name as cafe_name, cafe.state_id, \
	state.shortname as state_shortname, state.name as state_fullname, \
	zip, city, streetaddress, phone, delivery_fee, \
	delivery_order_setup_time, prepare_order_time, sales_tax, \
	delivery_free_subtotal, delivery_min_subtotal, meal_free_limit, \
	chain_name, delivery_notes, receipt_footer, expense_auth_limit, \
	table_group.table_group_id, table_group.name, gratuity, receipt_header \
	from ksi.cafe \
	left join ksi.state on state.state_id = cafe.state_id \
	left join ksi.table_group on table_group.cafe_id = ksi.util.get_cur_cafe_id \
	where deleted is null and cafe.cafe_id = ksi.util.get_cur_cafe_id \
	and table_group.table_group_id in \
	( \
	select max(table_group_id) \
	from ksi.table_group \
	where deleted is null \
	and cafe_id = ksi.util.get_cur_cafe_id \
	and is_default = (select max(is_default) from ksi.table_group where deleted is null and cafe_id = ksi.util.get_cur_cafe_id) \
	)"
	);
	query->execute_statement();

	int cafe_id = static_cast<int>(boost::get<double>(query->get(0)));
	std::string name = boost::get<std::string>(query->get(1));
	int state_id = static_cast<int>(boost::get<double>(query->get(2)));
	std::string shortstate = boost::get<std::string>(query->get(3));
	std::string fullstate = boost::get<std::string>(query->get(4));
	int zip = static_cast<int>(boost::get<double>(query->get(5)));
	std::string city = boost::get<std::string>(query->get(6));
	std::string streetaddress = boost::get<std::string>(query->get(7));
	std::string phone = boost::get<std::string>(query->get(8));
	double delivery_fee = boost::get<double>(query->get(9));
	int delivery_time = static_cast<int>(boost::get<double>(query->get(10)));
	int prepare_time = static_cast<int>(boost::get<double>(query->get(11)));
	double sales_tax = boost::get<double>(query->get(12));	
	double delivery_free_subtotal = boost::get<double>(query->get(13));
	double delivery_min_subtotal = boost::get<double>(query->get(14));
	double meal_free_limit = boost::get<double>(query->get(15));
	std::string chain = boost::get<std::string>(query->get(16));
	std::string delivery_notes = boost::get<std::string>(query->get(17));
	std::string receipt_footer = boost::get<std::string>(query->get(18));
	double expense_auth_limit = boost::get<double>(query->get(19));
	int table_group_id = static_cast<int>(boost::get<double>(query->get(20)));
	std::string table_group_name = boost::get<std::string>(query->get(21));	
	double gratuity = boost::get<double>(query->get(22));
	std::string receipt_header = boost::get<std::string>(query->get(23));

	state_t state = state_t(state_id, fullstate, shortstate);
	address_t address = address_t(streetaddress, city, state, zip);

	current_cafe_ptr_.reset(new cafe_t(cafe_id, chain, name, address, phone, delivery_time, prepare_time,
							sales_tax, delivery_fee, delivery_free_subtotal, delivery_min_subtotal, 
							meal_free_limit, delivery_notes, receipt_header, receipt_footer
							, expense_auth_limit, table_group_id, table_group_name, gratuity));
}

int cafe_container_t::position_cafe(int cafe_id) const
{
	std::vector<cafe_t>::const_iterator It;

	It = std::find_if
		(
		    container_.begin()
			, container_.end()
			, boost::bind
			(
			    std::equal_to<int>()
				, boost::bind(&cafe_t::cafe_id, _1)
				, cafe_id
			)
		);


	if (It == container_.end()) return -1;
	return int(std::distance(container_.begin(), It));
}

std::vector<int> cafe_container_t::all_cafe_id() const
{
	std::vector<int> res;
	std::transform
		(
		   container_.begin()
		 , container_.end()
		 , std::back_inserter(res)
		 , boost::bind(&cafe_t::cafe_id, _1));
	return res;
}

std::vector<state_t> fetch_state()
{
	std::vector<state_t> storage_;

	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();

	query->create_statement
	(
	"select state_id, name, shortname from ksi.state order by state_id"
	);

	query->execute_statement();

	while (!query->next())
	{
		int state_id = static_cast<int>(boost::get<double>(query->get(0)));
		std::string name = boost::get<std::string>(query->get(1));
		std::string shortname = boost::get<std::string>(query->get(2));
		storage_.push_back(state_t(state_id, name, shortname));
	}

	//storage_.push_back(state_t(6, "California", "CA"));
	return storage_;
}

};

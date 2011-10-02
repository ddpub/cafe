
#include "delivery_info.hpp"
#include <boost/lexical_cast.hpp>
#include <oracle_client/ksi_exception.hpp>
#include <ksi_include/ksi_exceptions.hpp>
#include <shared_modules/connect.hpp>

namespace ksi_cafe {

std::auto_ptr<delivery_info_t> make_delivery_info(int delivery_id, int cafe_id) {
	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();
	query->create_statement
	(	
	"SELECT "
	" FIRSTNAME "
	", LASTNAME "
	", ORDER_DELIVERY.STATE_ID "
	", ZIP "
	", STREETADDRESS "
	", PHONE_HOME "
	", DELIVERY_PRICE "
	", DELIVERY_DATE "
	", STATE.NAME "
	", STATE.SHORTNAME "
	", CITY "
	" FROM KSI.ORDER_DELIVERY "
	" LEFT JOIN KSI.STATE ON STATE.STATE_ID = ORDER_DELIVERY.STATE_ID "
	" WHERE CAFE_ID = CASE WHEN :cafe_id = 0 THEN KSI.UTIL.GET_CUR_CAFE_ID ELSE :cafe_id END AND DELIVERY_ID = :delivery_id"
	);
	query->set("delivery_id", delivery_id);
	query->set("cafe_id", cafe_id);
	query->execute_statement();
	if (query->next()) THROW_LOCATION(0, "std::auto_ptr<delivery_info_t> make_delivery_info(int delivery_id="+boost::lexical_cast<std::string>(delivery_id)+", int cafe_id="+boost::lexical_cast<std::string>(cafe_id)+")");
	std::string firstname = boost::get<std::string>(query->get(0));
	std::string lastname = boost::get<std::string>(query->get(1));
	int state_id = static_cast<int>(boost::get<double>(query->get(2)));
	int zip = static_cast<int>(boost::get<double>(query->get(3)));
	std::string streetaddress = boost::get<std::string>(query->get(4));
	std::string phone = boost::get<std::string>(query->get(5));
	boost::posix_time::ptime delivery_date = boost::get<boost::posix_time::ptime>(query->get(7));
	std::string state_name = boost::get<std::string>(query->get(8));
	std::string state_shortname = boost::get<std::string>(query->get(9));
	std::string city = boost::get<std::string>(query->get(10));

	delivery_info_t* info = new delivery_info_t(firstname, lastname, state_id, state_name, zip, streetaddress, phone, delivery_date, city);
	return std::auto_ptr<delivery_info_t>(info);
}

}

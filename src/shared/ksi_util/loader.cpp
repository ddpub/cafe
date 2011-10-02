
#include "loader.hpp"
#include <ksi_include/ksi_exceptions.hpp>
#include <oracle_client/oracle_client.hpp>
#include <ksi_util/log.hpp>
#include <memory>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

extern std::auto_ptr<file_log_t> thflog;


void load_order1(ksi_cafe::order_t** ord, const ksi_cafe::cafe_t& cafe_, int order_id_) {
	*thflog << "load_order, order_id = " + boost::lexical_cast<std::string>(order_id_);
	while (*ord == 0) {
		boost::this_thread::interruption_point();
		try {
			*ord = new ksi_cafe::order_t(cafe_, order_id_);
			*thflog << "successful";
		}
		catch (ksi_client::db_exception& ex) {
			*thflog << ex.what();
		}
		catch (exception_t& ex) {
			*thflog << ex.what();
		}
		catch (std::exception& ex) {
			*thflog << ex.what();
		}
		catch (...) {
			*thflog << ERROR_IN_LOCATION;
		}
	}
}

void load_order2(ksi_cafe::order_t** ord, const ksi_cafe::cafe_t& cafe_, int order_id_, bool for_update, bool nowait) {
	*thflog << "load_order, order_id = " + boost::lexical_cast<std::string>(order_id_);
	while (*ord == 0) {
		boost::this_thread::interruption_point();
		try {
			*ord = new ksi_cafe::order_t(cafe_, order_id_, for_update,  nowait);
			*thflog << "successful";
		}
		catch (ksi_client::db_exception& ex) {
			*thflog << ex.what();
		}
		catch (exception_t& ex) {
			*thflog << ex.what();
		}
		catch (std::exception& ex) {
			*thflog << ex.what();
		}
		catch (...) {
			*thflog << ERROR_IN_LOCATION;
		}	
	}
}

void load_order3(ksi_cafe::order_t** ord, const ksi_cafe::cafe_t& cafe_, const ksi_cafe::token_t& token_, int order_id_, bool for_update, bool nowait) {
	*thflog << "load_order, order_id = " + boost::lexical_cast<std::string>(order_id_);
	while (*ord == 0) {
		boost::this_thread::interruption_point();
		try {
			*ord = new ksi_cafe::order_t(cafe_, token_, order_id_, for_update, nowait);
			*thflog << "successful";
		}
		catch (ksi_client::db_exception& ex) {
			*thflog << ex.what();
		}
		catch (exception_t& ex) {
			*thflog << ex.what();
		}
		catch (std::exception& ex) {
			*thflog << ex.what();
		}
		catch (...) {
			*thflog << ERROR_IN_LOCATION;
		}
	}
}

void load_order4(ksi_cafe::order_t** ord, const ksi_cafe::cafe_t& cafe_, const ksi_cafe::table_t& table_, int order_id_, bool for_update, bool nowait) {
	*thflog << "load_order, order_id = " + boost::lexical_cast<std::string>(order_id_);
	while (*ord == 0) {
		boost::this_thread::interruption_point();
		try {
			*ord = new ksi_cafe::order_t(cafe_, table_, order_id_, for_update, nowait);
			*thflog << "successful";
		}
		catch (ksi_client::db_exception& ex) {
			*thflog << ex.what();
		}
		catch (exception_t& ex) {
			*thflog << ex.what();
		}
		catch (std::exception& ex) {
			*thflog << ex.what();
		}	
		catch (...) {
			*thflog << ERROR_IN_LOCATION;
		}
	}
}

void load_order5(ksi_cafe::order_t** ord, const ksi_cafe::cafe_t& cafe_, const ksi_cafe::table_t& table_, const ksi_cafe::token_t& token_, int order_id_, bool for_update, bool nowait) {
	*thflog << "load_order, order_id = " + boost::lexical_cast<std::string>(order_id_);
	while (*ord == 0) {
		boost::this_thread::interruption_point();
		try {
			*ord = new ksi_cafe::order_t(cafe_, table_, token_, order_id_, for_update, nowait);
			*thflog << "successful";
		}
		catch (ksi_client::db_exception& ex) {
			*thflog << ex.what();
		}
		catch (exception_t& ex) {
			*thflog << ex.what();
		}
		catch (std::exception& ex) {
			*thflog << ex.what();
		}
		catch (...) {
			*thflog << ERROR_IN_LOCATION;
		}
	}
}

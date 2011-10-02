
#include "connect.hpp"

namespace ksi_cafe {

bool ora_transaction_t::in_trans = false;

std::auto_ptr<ksi_client::oracle_session_i> session;

bool session_exist() {
	if (session.get()) {
		try {
			std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();
			query->create_statement("select ksi.session_exist as res from dual");
			query->execute_statement();
			if (query->next()) return false;
			int res = static_cast<int>(boost::get<double>(query->get(0)));
			return res;
		}
		catch (...) {
			return false;
		}
	}
	return false;
}

};

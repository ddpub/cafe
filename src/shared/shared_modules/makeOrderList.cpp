/*
 * makeOrderList.cpp
 *
 *  Created on: 06.11.2009
 *      Author: Dmitry Dergachev, dmitry.dergachev@gmail.com
 */


#include "makeOrderList.hpp"
#include <oracle_client/oracle_client.hpp>
#include <oracle_client/ksi_exception.hpp>
#include <wx_util/wx_util.hpp>
#include <string>
#include <vector>


bnt_list_f* makeDelayedOrderListForm() {
	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	std::vector<wxString> cap_vec;
	std::vector<wxColour> color_vec;
	std::vector<bool> enabled_vec;
}


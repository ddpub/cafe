
#include "cctrans_manager.hpp"
#include <shared_modules/connect.hpp>
#include <memory>
#include <boost/bind.hpp>
#include <functional>
#include <ksi_include/ksi_exceptions.hpp>


namespace ksi_cafe {

cctrans_vec_t fetch_cctrans_for_order(int order_id)
{
	cctrans_vec_t vec;

	std::auto_ptr<ksi_client::oracle_query_i> query = session->create_query();

	query->create_statement
	(
	"select type_id \
				, amount \
				, tips \
				, transaction_id \
				, last_4_digits \
				, ref_transaction_id \
				, batch_id \
				, card_type \
				, trans_id\
	from ksi.order_cctrans \
	where order_id = :order_id and cafe_id = ksi.util.get_cur_cafe_id \
	order by trans_id"
	);

	query->set("order_id", order_id);
	query->execute_statement();

	while (!query->next())
	{
		int type_id = static_cast<int>(boost::get<double>(query->get(0)));
		double amount = boost::get<double>(query->get(1));
		double tips = boost::get<double>(query->get(2));
		int txn_id = static_cast<int>(boost::get<double>(query->get(3)));
		std::string l4d = boost::get<std::string>(query->get(4));
		int ref_txn_id = static_cast<int>(boost::get<double>(query->get(5)));
		int batch_id = static_cast<int>(boost::get<double>(query->get(6)));
		std::string card_type = boost::get<std::string>(query->get(7));
		int trans_id = static_cast<int>(boost::get<double>(query->get(8)));
		vec.push_back(cctrans_record_t(type_id, amount, tips, txn_id, l4d, batch_id, ref_txn_id, card_type, trans_id));
	}

	return vec;
}

cctrans_vec_t valid_cctrans(const cctrans_vec_t& vec)
{
	cctrans_vec_t tmp;
	std::copy(vec.begin(), vec.end(), std::back_inserter(tmp));
/*
	cctrans_vec_t::const_iterator end = vec.end();
	for (cctrans_vec_t::const_iterator i=vec.begin(); i!=end; ++i)
	{
		if (i->type_id() == 5)
		{
			cctrans_vec_t::const_iterator pos1 = vec.end();
			cctrans_vec_t::const_iterator pos2 = vec.end();
			for (cctrans_vec_t::const_iterator j = i+1; j!=end; ++j)
			{
				if (j->ref_txn_id() == i->txn_id())
				{
					if (j->type_id() == 2) pos2 = j;
					else if (j->type_id() == 1) pos1 = j;

					if (pos2 == end)
					{
						tmp.push_back(*i);
						if (pos1 != end) tmp.push_back(*pos1);
					}
				}
			}
		}
	}
*/
	return tmp;
}

cctrans_record_t prepayment_auth(const cctrans_vec_t& vec, int payment_type_id, int prepayment_type_id)
{
	cctrans_vec_t tmp = valid_cctrans(vec);
	size_t cnt = std::count_if(tmp.begin(), tmp.end(), boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));

	if (prepayment_type_id == 2 && payment_type_id == 2)
	{
		if (cnt != 2) THROW_LOCATION(10201, "prepayment_auth(const cctrans_vec_t&, int, int)");
		cctrans_vec_t::iterator i = std::find_if(tmp.begin(), tmp.end(), boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));
		if (i == tmp.end()) THROW_LOCATION(10202, "prepayment_auth(const cctrans_vec_t&, int, int)");
		cctrans_vec_t::iterator i2 = std::find_if(i+1, tmp.end(), boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));
		if (i2 == tmp.end()) THROW_LOCATION(10203, "prepayment_auth(const cctrans_vec_t&, int, int)");
		return *i;
	}
	if (prepayment_type_id == 2 && payment_type_id != 2)
	{
		if (cnt != 1) THROW_LOCATION(10206, "prepayment_auth(const cctrans_vec_t&, int, int)");
		cctrans_vec_t::iterator i = std::find_if(tmp.begin(), tmp.end(), boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));
		if (i == tmp.end()) THROW_LOCATION(10204, "prepayment_auth(const cctrans_vec_t&, int, int)");
		return *i;
	}
	THROW_LOCATION(10205, "prepayment_auth(const cctrans_vec_t&, int, int)");
}

cctrans_record_t payment_auth(const cctrans_vec_t& vec, int payment_type_id, int prepayment_type_id) {
	cctrans_vec_t tmp = valid_cctrans(vec);

	size_t cnt = std::count_if(tmp.begin(), tmp.end(), boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));

	if (prepayment_type_id == 2 && payment_type_id == 2) {
		if (cnt != 2) THROW_LOCATION(10201, "payment_auth(const cctrans_vec_t&, int, int)");
		cctrans_vec_t::iterator i = std::find_if(tmp.begin(), tmp.end(), boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));
		if (i == tmp.end()) THROW_LOCATION(10202, "payment_auth(const cctrans_vec_t&, int, int)");
		cctrans_vec_t::iterator i2 = std::find_if(i+1, tmp.end(), boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));
		if (i2 == tmp.end()) THROW_LOCATION(10203, "payment_auth(const cctrans_vec_t&, int, int)");
		return *i2;
	}
	if (prepayment_type_id != 2 && payment_type_id == 2) {
		if (cnt != 1) THROW_LOCATION(10206, "payment_auth(const cctrans_vec_t&, int, int). valid.size()="+boost::lexical_cast<std::string>(tmp.size()));
		cctrans_vec_t::iterator i = std::find_if(tmp.begin(), tmp.end(), boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));
		if (i == tmp.end()) THROW_LOCATION(10204, "payment_auth(const cctrans_vec_t&, int, int)");
		return *i;
	}
	THROW_LOCATION(10205, "payment_auth(const cctrans_vec_t&, int, int)");
}

cctrans_record_t prepayment_add_to_batch(const cctrans_vec_t& vec, int payment_type_id, int prepayment_type_id)
{
	cctrans_vec_t tmp = valid_cctrans(vec);

	size_t cnt = std::count_if(tmp.begin(), tmp.end(), boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));

	if (prepayment_type_id == 2 && payment_type_id == 2)
	{
		if (cnt != 2) THROW_LOCATION(10201, "prepayment_add_to_batch(const cctrans_vec_t&, int, int)");
		cctrans_vec_t::iterator i =
			std::find_if(tmp.begin(), tmp.end(), boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));
		if (i == tmp.end()) THROW_LOCATION(10202, "prepayment_add_to_batch(const cctrans_vec_t&, int, int)");
		cctrans_vec_t::iterator i2 =
			std::find_if(i+1, tmp.end(), boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));
		if (i2 == tmp.end()) THROW_LOCATION(10203, "prepayment_add_to_batch(const cctrans_vec_t&, int, int)");
		cctrans_vec_t::iterator i3 =
			std::find_if(i2+1, tmp.end()
			, boost::bind(std::logical_and<bool>()
			, boost::bind(std::equal_to<int>(), 1, boost::bind(&cctrans_record_t::type_id, _1))
			, boost::bind(std::equal_to<int>(), i2->txn_id(), boost::bind(&cctrans_record_t::ref_txn_id, _1)))
			);
		if (i3 == tmp.end()) THROW_LOCATION(10207, "prepayment_add_to_batch(const cctrans_vec_t&, int, int)");
		return *i3;
	}
	if (prepayment_type_id == 2 && payment_type_id != 2)
	{
		if (cnt != 1) THROW_LOCATION(10206, "prepayment_add_to_batch(const cctrans_vec_t&, int, int)");
		cctrans_vec_t::iterator i =
			std::find_if(tmp.begin(), tmp.end()
			, boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));
		if (i == tmp.end()) THROW_LOCATION(10204, "prepayment_add_to_batch(const cctrans_vec_t&, int, int)");
		cctrans_vec_t::iterator i2 =
			std::find_if(i+1, tmp.end()
			, boost::bind(std::logical_and<bool>()
			, boost::bind(std::equal_to<int>(), 1, boost::bind(&cctrans_record_t::type_id, _1))
			, boost::bind(std::equal_to<int>(), i->txn_id(), boost::bind(&cctrans_record_t::ref_txn_id, _1)))
			);
		if (i2 == tmp.end()) THROW_LOCATION(10208, "prepayment_add_to_batch(const cctrans_vec_t&, int, int)");
		return *i2;
	}
	THROW_LOCATION(10205, "prepayment_add_to_batch(const cctrans_vec_t&, int, int)");
}

cctrans_record_t payment_add_to_batch(const cctrans_vec_t& vec, int payment_type_id, int prepayment_type_id)
{
	cctrans_vec_t tmp = valid_cctrans(vec);

	size_t cnt =
		std::count_if(tmp.begin(), tmp.end()
								, boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));

	if (prepayment_type_id == 2 && payment_type_id == 2)
	{
		if (cnt != 2) THROW_LOCATION(10201, "payment_add_to_batch(const cctrans_vec_t&, int, int)");
		cctrans_vec_t::iterator i =
			std::find_if(tmp.begin(), tmp.end(), boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));
		if (i == tmp.end()) THROW_LOCATION(10202, "payment_add_to_batch(const cctrans_vec_t&, int, int)");
		cctrans_vec_t::iterator i2 =
			std::find_if(i+1, tmp.end(), boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));
		if (i2 == tmp.end()) THROW_LOCATION(10203, "payment_add_to_batch(const cctrans_vec_t&, int, int)");
		cctrans_vec_t::iterator i3 =
			std::find_if(i2+1, tmp.end()
			, boost::bind(std::logical_and<bool>()
			, boost::bind(std::equal_to<int>(), 1, boost::bind(&cctrans_record_t::type_id, _1))
			, boost::bind(std::equal_to<int>(), i2->txn_id(), boost::bind(&cctrans_record_t::ref_txn_id, _1)))
			);
		if (i3 == tmp.end()) THROW_LOCATION(10207, "payment_add_to_batch(const cctrans_vec_t&, int, int)");
		return *i3;
	}
	if (prepayment_type_id != 2 && payment_type_id == 2)
	{
		if (cnt != 1) THROW_LOCATION(10206, "payment_add_to_batch(const cctrans_vec_t&, int, int)");
		cctrans_vec_t::iterator i =
			std::find_if(tmp.begin(), tmp.end()
			, boost::bind(std::equal_to<int>(), 5, boost::bind(&cctrans_record_t::type_id, _1)));
		if (i == tmp.end()) THROW_LOCATION(10204, "payment_add_to_batch(const cctrans_vec_t&, int, int)");
		cctrans_vec_t::iterator i2 =
			std::find_if(i+1, tmp.end()
			, boost::bind(std::logical_and<bool>()
			, boost::bind(std::equal_to<int>(), 1, boost::bind(&cctrans_record_t::type_id, _1))
			, boost::bind(std::equal_to<int>(), i->txn_id(), boost::bind(&cctrans_record_t::ref_txn_id, _1)))
			);
		if (i2 == tmp.end()) THROW_LOCATION(10208, "payment_add_to_batch(const cctrans_vec_t&, int, int)");
		return *i2;
	}
	THROW_LOCATION(10205, "payment_add_to_batch(const cctrans_vec_t&, int, int)");
}

};


#include "../oracle_client/ksi_common.hpp"
#include "../oracle_client/ksi_exception.hpp"
#include "../oracle_client/oracle_client.hpp"
#include <string>



using namespace std;
using namespace ksi_client;



int
main(int argc, char **argv)
{
    auto_ptr<oracle_session_i> os = create_session();
    os->connect("sync", "ksi", "172.16.67.3", "cafe");
		string query = 
			"select \
	count(order_cctrans.trans_id)																					\
	from ksi.orders																												\
	inner join ksi.seance on orders.seance_id = seance.seance_id and seance.commited is null \
	inner join ksi.shift on shift.shift_id = seance.shift_id and shift.stop_time is null \
	inner join ksi.terminal on terminal.terminal_id = seance.terminal_id	\
	left join ksi.payment_type on payment_type.payment_type_id = orders.payment_type_id \
	left join ksi.payment_type prepayment_type on prepayment_type.payment_type_id = orders.prepayment_type_id \
	left join ksi.table_group on table_group.table_group_id = orders.table_group_id \
	left join ksi.order_cctrans on order_cctrans.order_id = orders.order_id and order_cctrans.cafe_id = ksi.util.get_cur_cafe_id \
	where orders.status_id = 3 and orders.type_id in (1,2,3,4,5)					\
	and shift.shift_id = 370 \
	group by orders.order_id, orders.created, orders.subtotal, orders.payment_type_id, \
	payment_type.name, table_group.name, terminal.name, orders.status_id, orders.creator_id, \
	orders.closed, orders.closer_id, orders.prepayment_type_id, prepayment_type.name, orders.paid \
	order by orders.paid desc";
		auto_ptr<oracle_query_i> q = os->create_query();
		q->create_statement(query);
		q->execute_statement();
		while (!q->next());
}

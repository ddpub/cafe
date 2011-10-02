#include "receipt.hpp"


#include <algorithm>
#include <numeric>
#include <shared_modules/connect.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <ksi_util/users.hpp>
#include <memory>
#include <ksi_util/util.hpp>
#include <ksi_order/menu_container.hpp>
#include <ksi_order/coupon.hpp>
#include <ksi_util/currency.hpp>
#include <deque>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>


extern int lang;
bool print_cafename = true;
//extern bool by_group;


ksi_cafe::container_kitchen_type_group_t cktg;
extern std::auto_ptr<ksi_cafe::users_t> users;

extern std::auto_ptr<ksi_cafe::coupon_in_cafe_t> coupon_in_cafe;

extern std::auto_ptr<ksi_cafe::xml_lang_t> locale;
extern std::auto_ptr<ksi_cafe::xml_lang_t> shared_locale;

std::string row_tip_total(double amount, double tip)
{
	std::vector<std::string> xml;
	xml.push_back("<lf/>");
	{
		row_t row;
		row.push_back(column_t("Amount: ", false, true, xml_attr::left, 200-30, 0, xml_attr::hx2wx2));
		row.push_back(column_t(to_mb(locale->get_currency(amount)), false, false, xml_attr::right, 0, 0, xml_attr::hx2wx2));
		xml.push_back(xml_table_t(row).xml_str());
	}
	{
		row_t row;
		row.push_back(column_t("TIP: ", false, true, xml_attr::left, 200-30, 0, xml_attr::hx2wx2));
		if (tip == -1)
			row.push_back(column_t("_____.___", false, false, xml_attr::right, 0, 0, xml_attr::hx2wx2));
		else
			row.push_back(column_t(to_mb(locale->get_currency(tip)), false, false, xml_attr::right, 0, 0, xml_attr::hx2wx2));
		xml.push_back(xml_table_t(row).xml_str());
	}
	{
		row_t row;
		row.push_back(column_t("TOTAL: ", false, true, xml_attr::left, 200-30, 0, xml_attr::hx2wx2));
		if (tip == -1)
			row.push_back(column_t("_____.___", false, false, xml_attr::right, 0, 0, xml_attr::hx2wx2));
		else
			row.push_back(column_t(to_mb(locale->get_currency(amount+tip)), false, false, xml_attr::right, 0, 0, xml_attr::hx2wx2));
		xml.push_back(xml_table_t(row).xml_str());
	}
	xml.push_back("<lf/>");
	return std::accumulate(xml.begin(), xml.end(), std::string(""));
}

xml_table_t row_signature(const std::string& title)
{
	std::vector<row_t> rows;
	{
		row_t row;
		row.push_back(column_t(" ", false, false, xml_attr::left, 0, 0, xml_attr::normal));
		rows.push_back(row);
	}
	{
		row_t row;
		row.push_back(column_t("__________________________________________", false, true, xml_attr::left, 0, 0, xml_attr::normal));
		rows.push_back(row);
	}
	{
		row_t row;
		row.push_back(column_t(title, false, true, xml_attr::left, 0, 0, xml_attr::normal));
		rows.push_back(row);
	}
	return xml_table_t(rows);
}

row_t row_mod_discount(const std::string& discount_name, double sum, int mod_count, int dish_count)
{
	row_t row;
	std::string prefix = "";
	//if (mod_count > 1) prefix = std::string(IntToStr(mod_count).c_str()) + "x";
	row.push_back(column_t(prefix+discount_name, true, true, xml_attr::left, 36, 0, xml_attr::normal));
	row.push_back(column_t(boost::lexical_cast<std::string>(mod_count*dish_count), false, false, xml_attr::right, 12, 0, xml_attr::normal));
	row.push_back(column_t("-"+prefix+to_mb(locale->get_currency(fabs(sum))), false, false, xml_attr::right, 12, 0, xml_attr::normal));
	row.push_back(column_t("-"+to_mb(locale->get_currency(fabs(sum*mod_count*dish_count))), false, false, xml_attr::right, 12, 0, xml_attr::normal));
	return row;
}

row_t row_discount(const std::string& discount_name, double sum, bool indent)
{
	row_t row;
	row.push_back(column_t(discount_name, true, true, xml_attr::left, (indent ? 24 : 0), 0, xml_attr::normal));
	row.push_back(column_t("", false, false, xml_attr::left, 0, 0, xml_attr::normal));
	row.push_back(column_t("", false, false, xml_attr::left, 0, 0, xml_attr::normal));
	row.push_back(column_t(to_mb(locale->get_currency(-1*sum)), false, false, xml_attr::right, 12, 0, xml_attr::normal));
	return row;
}

row_t row_order_dish(const std::string& name, double price, int qty, double amount)
{
	row_t row;
	std::string name_str = ksi_cafe::full_trim(name.c_str()).c_str();
	std::string price_str = "-";
	if (price != 0) price_str = to_mb(locale->get_currency(price));
	std::string amount_str = "-";
	if (amount != 0) amount_str = to_mb(locale->get_currency(amount));
	std::string qty_str = boost::lexical_cast<std::string>(qty);
	row.push_back(column_t(name_str, true, true, xml_attr::left, 0, 0, xml_attr::normal));
	row.push_back(column_t(qty_str, false, false, xml_attr::right, 12, 0, xml_attr::normal));
	row.push_back(column_t(price_str, false, false, xml_attr::right, 12, 0, xml_attr::normal));
	row.push_back(column_t(amount_str, false, false, xml_attr::right, 12, 0, xml_attr::normal));
	return row;
}

row_t row_order_mod(const std::string& name, int qty, double price)
{
	row_t row;
	std::string name_str = ksi_cafe::full_trim(name.c_str()).c_str();
	if (qty > 1) name_str = boost::lexical_cast<std::string>(qty) + "x" + name_str;
	std::string price_str = "-";
	if (price != 0) price_str = to_mb(locale->get_currency(price));
	row.push_back(column_t(name_str, true, true, xml_attr::left, 24, 0, xml_attr::normal));
	row.push_back(column_t("", false, false, xml_attr::left, 0, 0, xml_attr::normal));
	row.push_back(column_t(price_str, false, false, xml_attr::right, 12, 0, xml_attr::normal));
	row.push_back(column_t("", false, false, xml_attr::left, 0, 0, xml_attr::normal));
	return row;
}

row_t row_description(const std::string& name)
{
	row_t row;
	row.push_back(column_t(name, true, true, xml_attr::left, 2, 0, xml_attr::normal));
	row.push_back(column_t("", false, false, xml_attr::left, 0, 0, xml_attr::normal));
	row.push_back(column_t("", false, false, xml_attr::left, 0, 0, xml_attr::normal));
	row.push_back(column_t("", false, false, xml_attr::left, 0, 0, xml_attr::normal));
	return row;
}

std::vector<row_t> rows_total(double sub, double disc, double tax, double gratuity, double total
, double de_fee, double tip, double prepayment, double curr_tax, double curr_gratuity, int status) 
// status 1 - prepayment, 2 - payment, 3 - prepayment2, 4 - payment2
{
	std::vector<row_t> res;
	int margin = 43;
	if (lang == 2 && int (disc*100) != 0) {
		row_t row;
		std::string str = "Сумма без скидок:";
		row.push_back(column_t(str, false, true, xml_attr::left, margin, 0, xml_attr::normal));
		row.push_back(column_t(to_mb(locale->get_currency(disc+total)), false, false, xml_attr::right, 0, 0, xml_attr::normal));
		res.push_back(row);
	}
	if (int (disc*100) != 0) {
		row_t row;
		std::string str;
		if (lang == 1)  str = "Total Discount:";
		else if (lang == 2) str = "Сумма скидок:";
		row.push_back(column_t(str, false, true, xml_attr::left, margin, 0, xml_attr::normal));
		row.push_back(column_t(to_mb(locale->get_currency(-1*disc)), false, false, xml_attr::right, 0, 0, xml_attr::normal));
		res.push_back(row);
	}
	if (std::fabs(sub) != std::fabs(total)) {
		row_t row;
		row.push_back(column_t("Subtotal", false, true, xml_attr::left, margin, 0, xml_attr::normal));
		row.push_back(column_t(to_mb(locale->get_currency(sub)), false, false, xml_attr::right, 0, 0, xml_attr::normal));
		res.push_back(row);
	}
	if (int(tax*100) != 0) {
		row_t row;
		if (curr_tax == 0) {
			row.push_back(column_t("Sales Tax: ", false, true, xml_attr::left, margin, 0, xml_attr::normal));
		}
		else {
			row.push_back(column_t("Sales Tax(" + std::string(ksi_cafe::percent_t().format(100*curr_tax))+ "): ", false, true, xml_attr::left, margin, 0, xml_attr::normal));
		}
		row.push_back(column_t(to_mb(locale->get_currency(tax)), false, false, xml_attr::right, 0, 0, xml_attr::normal));
		res.push_back(row);
	}
	if (int(gratuity*100) != 0) {
		row_t row;
		if (curr_gratuity == 0) {
			row.push_back(column_t("Gratuity: ", false, true, xml_attr::left, margin, 0, xml_attr::normal));
		}
		else {
			row.push_back(column_t("Gratuity(" + std::string(ksi_cafe::percent_t().format(100*curr_gratuity))+"): ", false, true, xml_attr::left, margin, 0, xml_attr::normal));			
		}
		row.push_back(column_t(to_mb(locale->get_currency(gratuity)), false, false, xml_attr::right, 0, 0, xml_attr::normal));
		res.push_back(row);
	}
	if (de_fee) {
		row_t row;
		row.push_back(column_t("Delivery fee: ", false, true, xml_attr::left, margin, 0, xml_attr::normal));
		row.push_back(column_t(to_mb(locale->get_currency(de_fee)), false, false, xml_attr::right, 0, 0, xml_attr::normal));
		res.push_back(row);
	}
	if (tip) {
		std::string str;
		if (lang == 1) str = "Tip: ";
		else if (lang == 2) str = "Чаевые: ";
		row_t row;
		row.push_back(column_t(str, false, true, xml_attr::left, margin, 0, xml_attr::normal));
		row.push_back(column_t(to_mb(locale->get_currency(tip)), false, false, xml_attr::right, 0, 0, xml_attr::normal));
		res.push_back(row);
	}
	{
		std::string str;
		if (lang == 1 && (status == 1 || status == 2)) str = "TOTAL: ";
		else if (lang == 1 && (status == 3 || status == 4)) str = "TOTAL PAID: ";
		else if (lang == 2) str = "ИТОГО: ";
		row_t row;
		xml_attr::font_t font = xml_attr::normal;
		if (status == 1) //prepayment 
		{
			if (int(prepayment*100) == 0) font = xml_attr::hx2wx2;
		}
		else if (status == 2 || status == 6) //payment 
		{
			if (int(prepayment*100) == 0) font = xml_attr::hx2wx2;
		}
		row.push_back(column_t(str, false, true, xml_attr::left, margin, 0, font));
		if (total >= 0)
			row.push_back(column_t(to_mb(locale->get_currency(total+tip)), false, false, xml_attr::right, 0, 0, font));
		else
			row.push_back(column_t(to_mb(locale->get_currency(total-tip)), false, false, xml_attr::right, 0, 0, font));

		res.push_back(row);
	}

	if (int(prepayment*100) != 0) {
		{
		xml_attr::font_t font = xml_attr::normal;
		if (status == 1) font = xml_attr::hx2wx2;
		
		std::string str;
		if (lang == 2) str = "ПРЕДОПЛАТА: ";
		else if (status == 1 || status == 2) str = "PREPAYMENT: ";
		else if (status == 3 || status == 4) str = "DEPOSIT: ";

		row_t row;
		row.push_back(column_t(str, false, true, xml_attr::left, margin, 0, font));
		row.push_back(column_t(to_mb(locale->get_currency(prepayment)), false, false, xml_attr::right, 0, 0, font));
		res.push_back(row);       
		}

		
		if (lang != 2) {
			xml_attr::font_t font = xml_attr::normal;
			if (status == 2) font = xml_attr::hx2wx2;
			std::string str;
			if (status == 1 || status == 2) str = "AMOUNT TO PAY: ";
			else if (status == 3 || status == 4) str = "PAID TODAY: ";
			
			row_t row;
			row.push_back(column_t(str, false, true, xml_attr::left, margin, 0, font));
			row.push_back(column_t(to_mb(locale->get_currency(tip+total-prepayment)), false, false, xml_attr::right, 0, 0, font));
			res.push_back(row);
		}
	}
	return res;
}

std::vector<row_t> rows_change(const std::string& type, double cash, double change)
{
	std::vector<row_t> res;
	if (lang == 2) return res;
	const int margin = 33;
	if (int (cash*100) != 0) {
		std::string str;
		if (lang == 1) str = "Cash: ";
		else if (lang == 2) str = "Наличные: ";
		row_t row;
		row.push_back(column_t(str, false, true, xml_attr::left, margin, 0, xml_attr::normal));
		row.push_back(column_t(to_mb(locale->get_currency(cash)), false, false, xml_attr::right, 0, 0, xml_attr::normal));
		res.push_back(row);
	}
	if (int (change*100) != 0) {
		std::string str;
		if (lang == 1) str = "Change: ";
		else if (lang == 2) str = "Сдача: ";
		row_t row;
		row.push_back(column_t(str, false, true, xml_attr::left, margin, 0, xml_attr::normal));
		row.push_back(column_t(to_mb(locale->get_currency(change)), false, false, xml_attr::right, 0, 0, xml_attr::normal));
		res.push_back(row);
	}
	return res;
}

std::string print_header(const std::string& receiptType, const std::string& header) {
	std::vector<std::string> xml;
	xml.push_back("<logo/>");
	if (receiptType != "")
		xml.push_back("<table><row><col font=\"hx2wx2\" align=\"center\" stretch=\"1\">"+receiptType+"</col></row></table>");
	if (header != "") {
		std::string::const_iterator It = header.begin();
		while (It != header.end()) {
			std::string::const_iterator eIt = std::find(It, header.end(), '\n');
			std::string line(It, eIt);
			//for (std::string::const_iterator i=It; i!=eIt; ++i) line += *i;
			//if (line != "") {
			xml.push_back("<table><row><col font=\"normal\" align=\"center\" stretch=\"1\">"+line+"</col></row></table>");	
			//}
			if (eIt == header.end()) It = header.end();
			else It = eIt + 1;
		}		
	}
	return std::accumulate(xml.begin(), xml.end(), std::string(""));
}
/*
std::string print_refund_header(const std::string& cafe, const std::string& ref, const std::string& title)
{
	std::vector<std::string> xml;
	xml.push_back("<logo/>");	
	if (cafe != "") xml.push_back("<table><row><col font=\"normal\" align=\"center\" stretch=\"1\">"+cafe+"</col></row></table>");	
	if (ref != "") xml.push_back("<table><row><col font=\"hx2wx2\" align=\"center\" stretch=\"1\">"+ref+"</col></row></table>");
	if (title != "") xml.push_back("<table><row><col font=\"normal\" align=\"center\" stretch=\"1\">"+title+"</col></row></table>");
	return std::accumulate(xml.begin(), xml.end(), std::string(""));
}

std::string xml_refund_header(const std::string& cafe, const std::string& ref, const std::string& title)
{
	std::vector<std::string> xml;
	xml.push_back("<logo/>");
	if (cafe != "") xml.push_back("<table><row><col font=\"normal\" align=\"center\" stretch=\"1\">"+cafe+"</col></row></table>");
	if (ref != "") xml.push_back("<table><row><col font=\"hx2wx2\" align=\"center\" stretch=\"1\">"+ref+"</col></row></table>");
	if (title != "") xml.push_back("<table><row><col font=\"normal\" align=\"center\" stretch=\"1\">"+title+"</col></row></table>");
	return std::accumulate(xml.begin(), xml.end(), std::string(""));
}
*/

	struct print_raw_data {
		int type; // 1 - discount, 2 - dish, 3 - modifier, 4 - description, 5 - mod_discount 
		bool indent;
		std::string name;
		double price;
		int count;
		double amount;
		int dish_count;
		print_raw_data(const std::string& name, double price, bool indent): name(name), price(price), type(1), count(0), amount(0), indent(indent) {}
		print_raw_data(const std::string& name, double price, int count, double amount): name(name), price(price), count(count), amount(amount), type(2) {}
		print_raw_data(const std::string& name, double price, int count): name(name), price(price), count(count), type(3), amount(0)  {}
		print_raw_data(const std::string& name): name(name), type(4) {}
		print_raw_data(const std::string& name, double price, int count, int dish_count): name(name), price(price), count(count), dish_count(dish_count), type(5) {}
    };

std::string print_receipt_duplicate(const ksi_cafe::cafe_t& cafe_, int dub, int order_id, const std::string& receipt_type, bool by_group, bool heartland)
{
	std::vector<std::string> xml;
 
	std::string header_title = cafe_.address().full_address();
	if (lang == 2) {
		header_title = cafe_.address().city() + ", " + cafe_.address().streetaddress() + ", " + cafe_.phone() + ".";
		if (cafe_.phone() == "") header_title = cafe_.address().city() + ", " + cafe_.address().streetaddress() + ".";
	}
  
	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	query->create_statement
	(
	"select \
		orders.created \
		, orders.closed \
		, orders.creator_id \
		, orders.closer_id \
		, orders.type_id \
		, order_type.name as type_name \
		, orders.sales_tax \
		, orders.subtotal \
		, orders.status_id \
		, order_status.name as status_name \
		, orders.token \
		, orders.tables \
		, seance.terminal_id \
		, terminal.name as terminal_name \
		, orders.payment_type_id \
		, payment_type.name as payment_type_name \
		, orders.guest_count \
		, orders.prepayment \
		, orders.prepayment_type_id \
		, prepayment_type.name as prepayment_type_name \
		, orders.refund_note \
		, orders.flags \
		, orders.meal_person_id \
		, orders.tendered_amount \
		, orders.memo \
		, order_delivery.delivery_price \
		, (order_delivery.firstname || ' ' || order_delivery.lastname) as delivery_fio \
		, order_delivery.streetaddress as delivery_streetaddress \
		, order_delivery.phone_home as delivery_phone \
		, order_delivery.delivery_date \
		, orders.gratuity \
	from ksi.orders \
	inner join ksi.order_type on order_type.type_id = orders.type_id \
	inner join ksi.order_status on order_status.status_id = orders.status_id \
	inner join ksi.seance on seance.seance_id = orders.seance_id \
	inner join ksi.terminal on terminal.terminal_id = seance.terminal_id \
	left join ksi.payment_type on payment_type.payment_type_id = orders.payment_type_id \
	left join ksi.payment_type  prepayment_type on prepayment_type.payment_type_id = orders.prepayment_type_id \
	left join ksi.order_delivery on order_delivery.delivery_id = orders.delivery_id \
	where orders.order_id = :order_id and orders.cafe_id = ksi.util.get_cur_cafe_id"
	);
	query->set("order_id", order_id);
	query->execute_statement();
	
	if (query->next()) THROW_LOCATION(0, "std::string print_receipt_duplicate(const ksi_cafe::cafe_t&, int, int, const std::strings&, bool)");
		
	boost::posix_time::ptime created = boost::get<boost::posix_time::ptime>(query->get(0));
	boost::posix_time::ptime closed = boost::get<boost::posix_time::ptime>(query->get(1));
	int creator_id = static_cast<int>(boost::get<double>(query->get(2)));
	int closer_id = static_cast<int>(boost::get<double>(query->get(3)));
	int order_type_id = static_cast<int>(boost::get<double>(query->get(4)));
	std::string type_name = boost::get<std::string>(query->get(5));
	double sales_tax = boost::get<double>(query->get(6));
	double subtotal = boost::get<double>(query->get(7));
	int status_id = static_cast<int>(boost::get<double>(query->get(8)));
	std::string status_name = boost::get<std::string>(query->get(9));
	std::string token = boost::get<std::string>(query->get(10));
	std::string table = boost::get<std::string>(query->get(11));
	int terminal_id = static_cast<int>(boost::get<double>(query->get(12)));
	std::string terminal_name = boost::get<std::string>(query->get(13));
	int payment_type_id = static_cast<int>(boost::get<double>(query->get(14)));
	std::string payment_type_name = boost::get<std::string>(query->get(15));
	int guest_count = static_cast<int>(boost::get<double>(query->get(16)));
	double prepayment = boost::get<double>(query->get(17));
	int prepayment_type_id = static_cast<int>(boost::get<double>(query->get(18)));
	std::string prepayment_type_name = boost::get<std::string>(query->get(19));
	std::string refund_note = boost::get<std::string>(query->get(20));
	int is_hide = static_cast<int>(boost::get<double>(query->get(21))) & 0x8;
	int meal_person_id = static_cast<int>(boost::get<double>(query->get(22)));
	double tendered_amount = boost::get<double>(query->get(23));
	std::string order_memo = boost::get<std::string>(query->get(24));
	double delivery_fee = boost::get<double>(query->get(25));
	std::string delivery_fio = boost::get<std::string>(query->get(26));
	std::string delivery_streetaddress = boost::get<std::string>(query->get(27));
	std::string delivery_phone = boost::get<std::string>(query->get(28));
	boost::posix_time::ptime delivery_date = boost::get<boost::posix_time::ptime>(query->get(29));
	double gratuity = boost::get<double>(query->get(30));
	
	{
		//std::string xml_line = xml_refund_header(cafe_.chain() + " " + (print_cafename ? cafe_.name() : ""), "** " + receipt_type + " **", header_title);
		std::string xml_line = print_header(receipt_type, cafe_.receipt_header());
		xml.push_back(xml_line);
	}
	std::string str = "Order #";
	if (lang == 2) str = "Заказ #";
	
	std::string order_id_str = boost::lexical_cast<std::string>(order_id);
	{
		row_t row;
		row.push_back(column_t(str, false, false, xml_attr::left, 0, 0, xml_attr::normal));
		if (lang == 1) {
			if (order_id_str.length() > 3) {
				std::string pref_order_id = order_id_str.substr(0, order_id_str.length()-3);
				std::string end_order_id = order_id_str.substr(order_id_str.length()-3, 3);
				row.push_back(column_t(pref_order_id+"-", false, false, xml_attr::left, 0, 0, xml_attr::normal));
				row.push_back(column_t(end_order_id+(is_hide ? "." : ""), false, false, xml_attr::left, 0, 0, xml_attr::hx2wx2));
			}
			else {
				row.push_back(column_t(order_id_str+(is_hide ? "." : ""), false, false, xml_attr::left, 0, 0, xml_attr::hx2wx2));
				row.push_back(column_t("", false, false, xml_attr::left, 0, 0, xml_attr::normal));
			}
		}
		else if (lang == 2) {
			row.push_back(column_t(order_id_str, false, false, xml_attr::left, 0, 0, xml_attr::normal));
			row.push_back(column_t("", false, false, xml_attr::left, 0, 0, xml_attr::normal));
		}
		
		if (token != "") {
			std::string str = "  Token: ";
			if (lang == 2) str = "  Токен: ";
			row.push_back(column_t(str+token, false, false, xml_attr::center, 12, 0, xml_attr::normal));
		}
		
		if (lang == 1) row.push_back(column_t(type_name, false, true, xml_attr::right, 0, 0, xml_attr::normal));
		else row.push_back(column_t("", false, true, xml_attr::right, 0, 0, xml_attr::normal));
		
		xml.push_back(xml_table_t(row).xml_str());
	}
	
	{
		row_t row;
		std::string str = "Opened: ";
		if (lang == 2) str = "Открыт: ";
		
		row.push_back(column_t(str + to_mb(locale->get_date_time(created)), false, false, xml_attr::left, 0, 0, xml_attr::normal));
		if (table != "") {
			std::string str = "  Table: ";
			if (lang == 2) str = "  Стол: ";
			row.push_back(column_t(str + table, false, true, xml_attr::right, 0, 0, xml_attr::hx2));
		}
		xml.push_back(xml_table_t(row).xml_str());
	}
	
	{
		row_t row;
		if (status_id == 3 || status_id == 4 || status_id == 5) {
			std::string str = "Closed: ";
			if (lang == 2) str = "Закрыт: ";
			row.push_back(column_t(str + to_mb(locale->get_date_time(closed)), false, false, xml_attr::left, 0, 0, xml_attr::normal));
		}
		else row.push_back(column_t("", false, false, xml_attr::left, 0, 0, xml_attr::normal));
		if (guest_count != 0) {
			std::string str = "Guests: ";
			if (lang == 2) str = "Гостей: ";
			row.push_back(column_t(str + boost::lexical_cast<std::string>(guest_count), false, true, xml_attr::right, 0, 0, xml_attr::normal));
		}
		xml.push_back(xml_table_t(row).xml_str());
	}
	
	if (dub == 1) {
		std::string str = "Refunded: ";
		if (lang == 2) str = "Возвращен: ";
		row_t row;
		row.push_back(column_t(str + to_mb(locale->get_date_time(boost::posix_time::second_clock::local_time())), false, true, xml_attr::left, 0, 0, xml_attr::normal));
		xml.push_back(xml_table_t(row).xml_str());
	}
	else {
		if (status_id == 4) {
			std::string str = "Order has been refunded";
			if (lang == 2) str = "Заказ был возвращен";
			row_t row;
			row.push_back(column_t(str, false, false, xml_attr::left, 0, 0, xml_attr::normal));
			xml.push_back(xml_table_t(row).xml_str());
		}
		else if (status_id == 5) {
			std::string str = "Oder has been voided";
			if (lang == 2) str = "Заказ был отменен";
			row_t row;
			row.push_back(column_t(str, false, false, xml_attr::left, 0, 0, xml_attr::normal));
			xml.push_back(xml_table_t(row).xml_str());
		}
	}
	
	{
		std::string str = "Operator: ";
		if (lang == 2) str = "Официант: ";
		row_t row;
		row.push_back(column_t(str+users->user(creator_id).fio() + " @ " + terminal_name, false, false, xml_attr::left, 0, 0, xml_attr::normal));
		xml.push_back(xml_table_t(row).xml_str());
	}
	
	double tips = 0;
	if (prepayment_type_id == 2 && !heartland) {
	
	}
	else if (payment_type_id == 2 && heartland) {
		query->create_statement
		(
		"select \
		  amount \
		, tips \
		, ref_transaction_id \
		, last_4_digits \
		, card_type \
		, type_id \
		, swiped_table.swiped \
		from ksi.order_cctrans, (select swiped from ksi.order_cctrans where type_id = 5 and cafe_id = ksi.util.get_cur_cafe_id and order_id = :order_id) swiped_table \
		where cafe_id = ksi.util.get_cur_cafe_id and order_id = :order_id and type_id in (1,4) \
		order by tran_date desc"
		);
		query->set("order_id", order_id);
		query->execute_statement();
		if (query->next()) THROW_LOCATION(1, "std::string print_receipt_duplicate(const ksi_cafe::cafe_t&, int, int, const std::strings&, bool)");
		
		tips = boost::get<double>(query->get(1));
		int auth_txn_id = static_cast<int>(boost::get<double>(query->get(2)));
		std::string cc_num = boost::get<std::string>(query->get(3));
		std::string cc_type = boost::get<std::string>(query->get(4));
		int swipted = static_cast<int>(boost::get<double>(query->get(6)));
		
		{
			row_t row;
			row.push_back(column_t("Transaction_id: " + boost::lexical_cast<std::string>(auth_txn_id), false, false, xml_attr::left, 0, 0, xml_attr::normal));
			xml.push_back(xml_table_t(row).xml_str());
		}
		{
			row_t row;
			row.push_back(column_t("CARD TYPE: " + cc_type, false, false, xml_attr::left, 0, 0, xml_attr::normal));
			xml.push_back(xml_table_t(row).xml_str());
		}
		{
			std::string processing_type = "SWIPED";
			if (!swipted) processing_type = "ENTERED";
			row_t row;
			row.push_back(column_t("CARD NUMBER: " + cc_num + " " + processing_type, false, false, xml_attr::left, 0, 0, xml_attr::normal));
			xml.push_back(xml_table_t(row).xml_str());
		}
		xml.push_back("<lf/>");
	}
	
	query->create_statement
	(
	"select \
	    dish.name as dish_name \
	  , order_dish.dish_id \
	  , order_dish.count \
	  , order_dish.order_dish_id \
	  , order_dish.description \
	  , coupon.title as coupon_name \
	  , order_dish.coupon_id \
	  , order_dish.price \
	  , order_dish.base_price \
	  , kitchen_type.name as kitchen_type_name \
	  , dish.is_beverage as kitchen_type_id \
	  , kitchen_type_group.name as kitchen_type_group_name \
	  , kitchen_type_group.ktg_id as kitchen_type_group_id \
	from ksi.order_dish \
	left join ksi.coupon on coupon.coupon_id = order_dish.coupon_id \
	left join ksi.dish on dish.dish_id = order_dish.dish_id \
	left join ksi.kitchen_type on kitchen_type.kt_id = dish.is_beverage \
	left join ksi.kitchen_type_group on kitchen_type_group.ktg_id = kitchen_type.group_id \
	where order_dish.order_id = :order_id and order_dish.cafe_id = ksi.util.get_cur_cafe_id \
	order by coupon_id desc, order_dish.dish_id, order_dish_id"
	);
	query->set("order_id", order_id);
	query->execute_statement();
	
	std::auto_ptr<ksi_client::oracle_query_i> query2 = ksi_cafe::session->create_query();
	
	query2->create_statement
	(
	"select \
	    modifier.name modifier_name \
	  , order_dish_modifier.price \
	  , order_dish_modifier.base_price \
	  , order_dish_modifier.count \
	  , modifier.dish_id \
	  , coupon.title as coupon_name \
	  , order_dish_modifier.coupon_id \
	  , kitchen_type.name as kitchen_type_name \
	  , dish.is_beverage as kitchen_type_id \
	  , kitchen_type_group.name as kitchen_type_group_name \
	  , kitchen_type_group.ktg_id as kitchen_type_group_id \
	from ksi.order_dish_modifier \
	inner join ksi.modifier on modifier.modifier_id = order_dish_modifier.modifier_id  \
	left join ksi.coupon on coupon.coupon_id = order_dish_modifier.coupon_id \
	left join ksi.dish on dish.dish_id = modifier.dish_id \
	left join ksi.kitchen_type on kitchen_type.kt_id = dish.is_beverage \
	left join ksi.kitchen_type_group on kitchen_type_group.ktg_id = kitchen_type.group_id \
	where cafe_id = ksi.util.get_cur_cafe_id and order_dish_id = :order_dish_id \
	order by modifier.name"
	);
	
	std::map<int, double> ktg_discount; // group_id, discount
	std::map<int, double> ktg_total; // group_id, total
	double total_save = 0;
	std::vector<print_raw_data> vec;
	while (!query->next()) {
		std::string dish_name = boost::get<std::string>(query->get(0));
		int dish_id = static_cast<int>(boost::get<double>(query->get(1)));
		int dish_count = static_cast<int>(boost::get<double>(query->get(2)));
		int order_dish_id = static_cast<int>(boost::get<double>(query->get(3)));
		std::string misc_name = boost::get<std::string>(query->get(4));
		std::string dish_coupon_title = boost::get<std::string>(query->get(5));
		int coupon_id = static_cast<int>(boost::get<double>(query->get(6)));
		double dish_price = boost::get<double>(query->get(7));
		double dish_base_price = boost::get<double>(query->get(8));
		int dish_kitchen_type_group_id =static_cast<int>(boost::get<double>(query->get(12)));

		if (dish_count != 0) {

			if (dish_id == 0 && coupon_id == 0) {
				dish_name = "misc charges: " + misc_name;
				ktg_total[dish_kitchen_type_group_id] += dish_count*dish_price;
			}
			else if (dish_id == 0 && coupon_id > 0) {
				ktg_total[dish_kitchen_type_group_id] += dish_count*dish_price;
				ktg_discount[dish_kitchen_type_group_id] += dish_base_price-dish_price;
				dish_name = dish_coupon_title;
				dish_base_price = dish_price;
				total_save += std::fabs(dish_base_price);
			}
			else if (coupon_id == -1) {
				if (lang == 1)
					dish_name = "adjustable discount";
				else if (lang == 2)
					dish_name = "скидка менеджера к заказу";
				dish_base_price = dish_price;
				total_save += std::fabs(dish_base_price);
			}
			
			double save = (dish_base_price-dish_price);
			
			if (dish_id != 0) {
				ktg_total[dish_kitchen_type_group_id] += dish_price*dish_count;
				ktg_discount[dish_kitchen_type_group_id] += dish_count*(dish_base_price-dish_price);
			}

			query2->set("order_dish_id", order_dish_id);
			query2->execute_statement();
	    
			std::vector<print_raw_data> mod_vec;
			double sum_mod_base_price = 0;
			double sum_mod_price = 0;
			while (!query2->next()) {
				std::string modifier = boost::get<std::string>(query2->get(0));
				double mod_price = boost::get<double>(query2->get(1));
				double mod_base_price = boost::get<double>(query2->get(2));
				int mod_count = static_cast<int>(boost::get<double>(query2->get(3)));
				int mod_dish_id = static_cast<int>(boost::get<double>(query2->get(4)));
				std::string mod_coupon_title = boost::get<std::string>(query2->get(5));
				int mod_coupon_id = static_cast<int>(boost::get<double>(query2->get(6)));
				int mod_kitchen_type_group_id = static_cast<int>(boost::get<double>(query2->get(10)));
				
				if (mod_dish_id == 0) {
					ktg_total[dish_kitchen_type_group_id] += mod_price*mod_count*dish_count;
					ktg_discount[dish_kitchen_type_group_id] += dish_count*(mod_base_price-mod_price)*mod_count;
				}
				else {
					ktg_total[mod_kitchen_type_group_id] += mod_price*mod_count*dish_count;
					ktg_discount[mod_kitchen_type_group_id] += dish_count*(mod_base_price-mod_price)*mod_count;
				}
				
				mod_vec.push_back(print_raw_data(modifier, mod_base_price, mod_count));
				
				sum_mod_base_price += mod_base_price*mod_count;
				sum_mod_price += mod_price*mod_count;
				
				if (mod_coupon_id != 0 && (int(100*(mod_price-mod_base_price)) != 0)) {
					mod_vec.push_back(print_raw_data(mod_coupon_title, -1*(mod_price-mod_base_price), mod_count, dish_count));
				}

				save += (mod_base_price-mod_price)*mod_count;
			}

			if (coupon_id != 0 && dish_id == 0) { // fixed discount
				vec.push_back(print_raw_data(dish_name, -1*dish_base_price, false));
			}
			else {
				vec.push_back(print_raw_data(dish_name, dish_base_price, dish_count, (dish_base_price+sum_mod_base_price)*dish_count));
			}
			
			for (size_t i=0; i<mod_vec.size(); ++i) vec.push_back(mod_vec[i]);

			if (dish_id != 0 && misc_name != "") vec.push_back(print_raw_data(misc_name));
			
			if (int((dish_base_price-dish_price)*100) != 0) {
				vec.push_back(print_raw_data(dish_coupon_title, dish_count*(dish_base_price-dish_price), true));
			}
			
			total_save += dish_count*save;
		}
	}
	
	{
		std::vector<row_t> rows;
		{
			row_t row;
			std::string str = "Entree";
			if (lang == 2) str = "Блюдо";
			row.push_back(column_t(str, true, true, xml_attr::left, 0, 0, xml_attr::normal));
			
			str = "Qty";
			if (lang == 2) str = "Кол";
			row.push_back(column_t(str, false, false, xml_attr::center, 12, 12, xml_attr::normal));
			
			str = "Price";
			if (lang == 2) str = "Цена";
			row.push_back(column_t(str, false, false, xml_attr::center, 12, 0, xml_attr::normal));
			
			str = "Amount";
			if (lang == 2) str = "Сумма";
			row.push_back(column_t(str, false, false, xml_attr::center, 12, 0, xml_attr::normal));
			
			rows.push_back(row);
		}
		
		{
			row_t row;
			row.push_back(column_t("", true, true, xml_attr::left, 0, 0, xml_attr::normal, 100));
			row.push_back(column_t("", false, false, xml_attr::left, 0, 0, xml_attr::normal, 100));
			row.push_back(column_t("", false, false, xml_attr::left, 0, 0, xml_attr::normal, 100));
			row.push_back(column_t("", false, false, xml_attr::left, 0, 0, xml_attr::normal, 100));
			rows.push_back(row);
		}
		
		for (size_t i=0; i<vec.size(); ++i) {
			if (vec[i].type == 1) rows.push_back(row_discount(vec[i].name, vec[i].price, vec[i].indent));
			else if (vec[i].type == 2) rows.push_back(row_order_dish(vec[i].name, vec[i].price, vec[i].count, vec[i].amount));
			else if (vec[i].type == 3) rows.push_back(row_order_mod(vec[i].name, vec[i].count, vec[i].price));
			else if (vec[i].type == 4) rows.push_back(row_description(vec[i].name));
			else if (vec[i].type == 5) rows.push_back(row_mod_discount(vec[i].name, vec[i].price, vec[i].count, vec[i].dish_count));
		}
		xml.push_back(xml_table_t(rows).xml_str());
	}
	
	if (by_group && lang == 2) {
		if (!cktg.loaded()) cktg.load();
		
		for (size_t i=0; i<cktg.ktg_vec.size(); ++i) {
			std::string name = ksi_cafe::full_trim(cktg.ktg_vec[i].name());
			double discount = ktg_discount[cktg.ktg_vec[i].kitchen_type_group_id()];
			double total = ktg_total[cktg.ktg_vec[i].kitchen_type_group_id()];
			if (int((fabs(discount)+fabs(total))*100) == 0) continue;
			xml.push_back("<lf/><lf/>");
			{
				row_t row;
				row.push_back(column_t(name, false, true, xml_attr::left, 0, 0, xml_attr::normal));
				xml.push_back(xml_table_t(row).xml_str());
			}
			if (int(discount*100) != 0) {
				row_t row;
				row.push_back(column_t("Сумма без скидок:", false, false, xml_attr::left, 43, 0, xml_attr::normal));
				row.push_back(column_t(to_mb(locale->get_currency(discount+total)), false, true, xml_attr::right, 0, 0, xml_attr::normal));
				xml.push_back(xml_table_t(row).xml_str());
			}
			if (int(discount*100) != 0) {
				row_t row;
				row.push_back(column_t("Сумма скидок:", false, false, xml_attr::left, 43, 0, xml_attr::normal));
				row.push_back(column_t(to_mb(locale->get_currency(-1*discount)), false, true, xml_attr::right, 0, 0, xml_attr::normal));
		        xml.push_back(xml_table_t(row).xml_str());
			}
			if (int(discount*100) != 0) {
			    row_t row;
				row.push_back(column_t("Итого:", false, false, xml_attr::left, 43, 0, xml_attr::normal));
				row.push_back(column_t(to_mb(locale->get_currency(total)), false, true, xml_attr::right, 0, 0, xml_attr::normal));
				xml.push_back(xml_table_t(row).xml_str());
			}
		}
	}
	
	if (dub == 1) { // refund
		xml.push_back("<fl/><lf/>");
		xml.push_back(xml_table_t(rows_total(subtotal, total_save, sales_tax, gratuity, -1*(sales_tax+subtotal+delivery_fee+gratuity), delivery_fee, tips, 0, 0, 0, 4)).xml_str());
		if (payment_type_id == 1) {
			xml.push_back(xml_table_t(rows_change("Cash tendered", tendered_amount, tendered_amount - (gratuity+sales_tax+subtotal+delivery_fee))).xml_str());
		}
		if (refund_note != "") {
			row_t row;
			std::string refund_note_str = "Refund note: ";
			if (lang == 2) refund_note_str = "Причина возврата: ";
			row.push_back(column_t(refund_note_str + refund_note, true, true, xml_attr::left, 0, 0, xml_attr::normal));
			xml.push_back(xml_table_t(row).xml_str());
		}
	}
	else if (order_type_id == 5) {
		xml.push_back("<fl/><lf/>");
		int curr_status_id = 2;
		if (status_id != 1 || status_id != 6) curr_status_id = 4;
		xml.push_back(xml_table_t(rows_total(subtotal, subtotal, 0, 0, 0, 0, 0, 0, 0, 0, curr_status_id)).xml_str());
		if (meal_person_id != 0) {
			xml.push_back("<lf/>");
			{
				row_t row;
				row.push_back(column_t("Employee: " + users->user(meal_person_id).fio(), false, true, xml_attr::left, 0, 0, xml_attr::normal));
				xml.push_back(xml_table_t(row).xml_str());
			}
			xml.push_back(row_signature(shared_locale->get_std("receipt", "employee_signature_str", "Employee signature")).xml_str());
		}
		else {
			xml.push_back("<lf/>");
			{
				row_t row;
				row.push_back(column_t("Employee: " + order_memo, false, true, xml_attr::left, 0, 0, xml_attr::normal));
				xml.push_back(xml_table_t(row).xml_str());
			}
			xml.push_back(row_signature(shared_locale->get_std("receipt", "employee_signature_str", "Employee signature")).xml_str());		
		}
	}
	else if (status_id != 1 || status_id != 6) { // paid
		xml.push_back("<fl/><lf/>");
		xml.push_back(xml_table_t(rows_total(subtotal, total_save, sales_tax, gratuity, (sales_tax+gratuity+subtotal+delivery_fee), delivery_fee, tips, prepayment, 0, 0, 4)).xml_str());
		if (payment_type_id == 1) {
			xml.push_back(xml_table_t(rows_change("Cash tendered", tendered_amount, prepayment+tendered_amount - (sales_tax+gratuity+subtotal+delivery_fee))).xml_str());
			
			if (refund_note != "") {
				row_t row;
				row.push_back(column_t("refund_note: " +refund_note, true, true, xml_attr::left, 0, 0, xml_attr::normal));
				xml.push_back(xml_table_t(row).xml_str());
			}
		}
		if (order_type_id == 3 || order_type_id == 4) {
			xml.push_back("<lf/>");
			std::string result;
			if (order_type_id == 3) {
				result = "Delivery @ ";
				if (lang == 2) result = "Доставка @ ";
			}
			else if (order_type_id == 4) {
				result = "Pickup @ ";
				result = "Пикап @ ";
			}
			
			row_t row;
			row.push_back(column_t(result + to_mb(locale->get_date_time_short(delivery_date)) + " to " + delivery_fio, true, true, xml_attr::left, 0, 0, xml_attr::normal));
			xml.push_back(xml_table_t(row).xml_str());
			if (order_type_id == 3) {
				row_t row;
				std::string address = "Address: ";
				if (lang == 2) address = "Адрес: ";
				row.push_back(column_t(address + delivery_streetaddress, true, true, xml_attr::left, 0, 0, xml_attr::normal));
				xml.push_back(xml_table_t(row).xml_str());
			}
			{
				row_t row;
				std::string phone = "Phone: ";
				if (lang == 2) phone = "Телефон: ";
				row.push_back(column_t(phone + delivery_phone, true, true, xml_attr::left, 0, 0, xml_attr::normal, 0));
				xml.push_back(xml_table_t(row).xml_str());
			}
			xml.push_back("<lf/>");
			if (order_memo != "") {
				row_t row;
				row.push_back(column_t("order notes: " + order_memo, true, true, xml_attr::left, 0, 0, xml_attr::normal));
				xml.push_back(xml_table_t(row).xml_str());
			}
			{
				row_t row;
				row.push_back(column_t(cafe_.delivery_notes(), true, true, xml_attr::left, 0, 0, xml_attr::normal));
				xml.push_back(xml_table_t(row).xml_str());
			}
			xml.push_back(row_signature(shared_locale->get_std("receipt", "customer_signature_str", "CUSTOMER SIGNATURE")).xml_str());
			
			if (refund_note != "") {
				row_t row;
				std::string refund_note_str = "Refund note: ";
				if (lang == 2) refund_note_str = "Причина возврата: ";
				row.push_back(column_t(refund_note_str + refund_note, true, true, xml_attr::left, 0, 0, xml_attr::normal));
			}
		}
	}
	else { // not paid
		xml.push_back("<fl/><lf/>");
		xml.push_back(xml_table_t(rows_total(subtotal, total_save, sales_tax, gratuity, (sales_tax+gratuity+subtotal+delivery_fee), delivery_fee, 0, prepayment, 0, 0, 1)).xml_str());
		if (order_type_id == 3 || order_type_id == 4) {
			xml.push_back("<lf/>");
			std::string result;
			if (order_type_id == 3) {
				result = "Delivery @ ";
				if (lang == 2) result = "Доставка @ ";
			}
			else if (order_type_id == 4) {
				result = "Pickup @ ";
				result = "Пикап @ ";
			}
			
			row_t row;
			row.push_back(column_t(result + to_mb(locale->get_date_time_short(delivery_date)) + " to " + delivery_fio, true, true, xml_attr::left, 0, 0, xml_attr::normal));
			xml.push_back(xml_table_t(row).xml_str());
			if (order_type_id == 3) {
				row_t row;
				std::string address = "Address: ";
				if (lang == 2) address = "Адрес: ";
				row.push_back(column_t(address + delivery_streetaddress, true, true, xml_attr::left, 0, 0, xml_attr::normal));
				xml.push_back(xml_table_t(row).xml_str());
			}
			{
				row_t row;
				std::string phone = "Phone: ";
				if (lang == 2) phone = "Телефон: ";
				row.push_back(column_t(phone + delivery_phone, true, true, xml_attr::left, 0, 0, xml_attr::normal, 0));
				xml.push_back(xml_table_t(row).xml_str());
			}
			xml.push_back("<lf/>");
			if (order_memo != "") {
				row_t row;
				row.push_back(column_t("order notes: " + order_memo, true, true, xml_attr::left, 0, 0, xml_attr::normal));
				xml.push_back(xml_table_t(row).xml_str());
			}
			{
				row_t row;
				row.push_back(column_t(cafe_.delivery_notes(), true, true, xml_attr::left, 0, 0, xml_attr::normal));
				xml.push_back(xml_table_t(row).xml_str());
			}
			xml.push_back(row_signature(shared_locale->get_std("receipt", "customer_signature_str", "CUSTOMER SIGNATURE")).xml_str());
			
			if (refund_note != "") {
				row_t row;
				std::string refund_note_str = "Refund note: ";
				if (lang == 2) refund_note_str = "Причина возврата: ";
				row.push_back(column_t(refund_note_str + refund_note, true, true, xml_attr::left, 0, 0, xml_attr::normal));
			}
		}
		if (lang == 1 && !(order_type_id == 3 || order_type_id == 4)) {
			row_t row;
			row.push_back(column_t("order is not paid", false, true, xml_attr::right, 0, 0, xml_attr::normal));
			xml.push_back(xml_table_t(row).xml_str());
		}
	}
	
	if (lang == 2) {
		if (payment_type_id == 1) {
			row_t row;
			row.push_back(column_t("Оплата наличными", true, true, xml_attr::left, 0, 0, xml_attr::normal));
			xml.push_back(xml_table_t(row).xml_str());
		}
		else if (payment_type_id == 2) {
			row_t row;
			row.push_back(column_t("Оплата кредитной картой", true, true, xml_attr::left, 0, 0, xml_attr::normal));
			xml.push_back(xml_table_t(row).xml_str());
		}
	}
	
	{
		row_t row;
		row.push_back(column_t(cafe_.receipt_footer(), true, true, xml_attr::center, 0, 0, xml_attr::hx2));
		xml.push_back(xml_table_t(row).xml_str());
	}
	xml.push_back("<lf/><cut/>");
	
	return std::accumulate(xml.begin(), xml.end(), std::string(""));
}

std::string print_order(const ksi_cafe::order_t& order, const std::string& receipt_type, bool by_group, bool heartland)
{
	std::vector<std::string> xml;
	
	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();

	query->create_statement
	(
	"select terminal.name, order_type.name \
	from ksi.orders \
	left join ksi.seance on seance.seance_id = orders.seance_id \
	left join ksi.terminal on terminal.terminal_id = seance.terminal_id \
	left join ksi.order_type on order_type.type_id = orders.type_id \
	where order_id = :order_id"
	);
	query->set("order_id", order.order_id());
	query->execute_statement();
	
	if (query->next()) THROW_LOCATION(0, "std::string print_order(const ksi_cafe::order_t&, const std::string&, bool)");
	
	std::string POS = boost::get<std::string>(query->get(0));
	std::string type_name = boost::get<std::string>(query->get(1));
	
	/*
	std::string header_title = order.cafe().address().full_address();
	if (lang == 2) {
		header_title = order.cafe().address().city() + ", " + order.cafe().address().streetaddress() + ", " + order.cafe().phone() + ".";
		if (order.cafe().phone() == "") header_title = order.cafe().address().city() + ", " + order.cafe().address().streetaddress() + ".";
	}
	*/

	//std::string firstname = users->user(order.creator_id()).firstname();
	//std::string lastname = users->user(order.creator_id()).lastname();
	
	
	std::string user;
	if (order.creator_id() != 0) user = users->user(order.creator_id()).fio();
	else user = users->user().fio();

	if (order.type_employee_meal() && order.payment_dummy()) {
		xml.push_back(print_header(shared_locale->get_std("receipt", "cafe_copy_str", "** CAFE COPY **"), order.cafe().receipt_header()));
		//xml.push_back(print_refund_header(std::string(order.cafe().chain().c_str()) + " " + (print_cafename ? order.cafe().name() : ""), "** "+std::string("CAFE COPY")+" **", header_title.c_str()));
	}
	else if (receipt_type != "") {
		xml.push_back(print_header(receipt_type, order.cafe().receipt_header()));
		//xml.push_back(print_refund_header(std::string(order.cafe().chain().c_str()) + " " + (print_cafename ? order.cafe().name() : ""), "** " + std::string(receipt_type.c_str()) + " **", header_title.c_str()));
	}
	else {
		xml.push_back(print_header("", order.cafe().receipt_header()));
		//std::string str = std::string(order.cafe().chain().c_str()) + " " + (print_cafename ? order.cafe().name() : "");
		//xml.push_back(print_refund_header(str, "" ,header_title.c_str()));
	}
  

	std::string str = "Order #";
	if (lang == 2) str = "Заказ #";
	
	std::string order_id_str = boost::lexical_cast<std::string>(order.order_id());
	{
		row_t row;
		row.push_back(column_t(str, false, false, xml_attr::left, 0, 0, xml_attr::normal));
		if (lang == 1) {
			if (order_id_str.length() > 3) {
				std::string pref_order_id = order_id_str.substr(0, order_id_str.length()-3);
				std::string end_order_id = order_id_str.substr(order_id_str.length()-3, 3);
				row.push_back(column_t(pref_order_id+"-", false, false, xml_attr::left, 0, 0, xml_attr::normal));
				row.push_back(column_t(end_order_id+(order.is_hide() ? "." : ""), false, false, xml_attr::left, 0, 0, xml_attr::hx2wx2));
			}
			else {
				row.push_back(column_t(order_id_str+(order.is_hide() ? "." : ""), false, false, xml_attr::left, 0, 0, xml_attr::hx2wx2));
				row.push_back(column_t("", false, false, xml_attr::left, 0, 0, xml_attr::normal));
			}
		}
		else if (lang == 2) {
			row.push_back(column_t(order_id_str, false, false, xml_attr::left, 0, 0, xml_attr::normal));
			row.push_back(column_t("", false, false, xml_attr::left, 0, 0, xml_attr::normal));
		}
		
		if (order.token().exist()) {
			std::string str = "  Token: ";
			if (lang == 2) str = "  Токен: ";
			row.push_back(column_t(str+order.token().name(), false, false, xml_attr::center, 12, 0, xml_attr::normal));
		}
		
		if (lang == 1) row.push_back(column_t(type_name, false, true, xml_attr::right, 0, 0, xml_attr::normal));
		else row.push_back(column_t("", false, true, xml_attr::right, 0, 0, xml_attr::normal));
		
		xml.push_back(xml_table_t(row).xml_str());
	}
	
	{
		row_t row;
		std::string str = "Opened: ";
		if (lang == 2) str = "Открыт: ";
		
		row.push_back(column_t(str + to_mb(locale->get_date_time(order.order_created())), false, false, xml_attr::left, 0, 0, xml_attr::normal));
		if (order.table().exist()) {
			std::string str = "  Table: ";
			if (lang == 2) str = "  Стол: ";
			row.push_back(column_t(str + order.table().name(), false, true, xml_attr::right, 0, 0, xml_attr::hx2));
		}
		xml.push_back(xml_table_t(row).xml_str());
	}
	
	{
		row_t row;
		if (order.status_closed()) {
			std::string str = "Closed: ";
			if (lang == 2) str = "Закрыт: ";
			row.push_back(column_t(str + to_mb(locale->get_date_time(order.order_closed())), false, false, xml_attr::left, 0, 0, xml_attr::normal));
		}
		else row.push_back(column_t("", false, false, xml_attr::left, 0, 0, xml_attr::normal));
		if (order.guest_count() != 0) {
			std::string str = "Guests: ";
			if (lang == 2) str = "Гостей: ";
			row.push_back(column_t(str + boost::lexical_cast<std::string>(order.guest_count()), false, true, xml_attr::right, 0, 0, xml_attr::normal));
		}
		xml.push_back(xml_table_t(row).xml_str());
	}

	{
		std::string str = "Operator: ";
		if (lang == 2) str = "Официант: ";
		row_t row;
		row.push_back(column_t(str+user + " @ " + POS, false, false, xml_attr::left, 0, 0, xml_attr::normal));
		xml.push_back(xml_table_t(row).xml_str());
	}
	
	if (order.payment_credit_card() && !heartland) {
	}
	else if (order.payment_credit_card() && heartland) {
		const ksi_cafe::credit_payment_t* pay = dynamic_cast<const ksi_cafe::credit_payment_t*>(order.cur_payment());
		if (pay) {
			{
				row_t row;
				row.push_back(column_t("Transaction_id: " + boost::lexical_cast<std::string>(pay->auth_txn_id()), false, false, xml_attr::left, 0, 0, xml_attr::normal));
				xml.push_back(xml_table_t(row).xml_str());
			}
			{
				row_t row;
				row.push_back(column_t("CARD TYPE: " + pay->cc_type(), false, false, xml_attr::left, 0, 0, xml_attr::normal));
				xml.push_back(xml_table_t(row).xml_str());
			}
			{
				std::string processing_type = "SWIPED";
				if (!pay->swiped()) processing_type = "ENTERED";
				row_t row;
				row.push_back(column_t("CARD NUMBER: " + pay->num() + " " + processing_type, false, false, xml_attr::left, 0, 0, xml_attr::normal));
				xml.push_back(xml_table_t(row).xml_str());
			}
			xml.push_back("<lf/>");
		}
	}


	if (order.type_table_booking()) {
		std::map<std::string, std::string> mapper;
		mapper["endl"] = "\n";
		mapper["null"] = "";
		mapper["prepayment_amount"] = to_mb(locale->get_currency(order.prepayment_amount()));
		mapper["date_time"] = to_mb(locale->get_date_time_short(order.get_delivery_info()->delivery_time()));
		mapper["customer_name"] = order.get_delivery_info()->fio();
		mapper["customer_phone"] = order.get_delivery_info()->phone();
		
		std::string text;
		if (lang == 1)
			text = to_mb(locale->get_string("table_booking_message_eng", mapper));
		else 
			text = to_mb(locale->get_string("table_booking_message", mapper));

		std::vector<std::string> resLines;
		boost::split(resLines, text, boost::is_any_of("\n"));
		
		BOOST_FOREACH(const std::string& line, resLines) {
			row_t row;
			row.push_back(column_t(line, false, false, xml_attr::left, 0, 0, xml_attr::normal));
			xml.push_back(xml_table_t(row).xml_str());
		}
	}
	
	std::vector<int> indexes;

	std::map<int, double> ktg_discount; // group_id, discount
	std::map<int, double> ktg_total; // group_id, total
	if(!cktg.loaded()) cktg.load();

	if (order.dish_count() + indexes.size()) {
		std::vector<row_t> rows;
		{
			row_t row;
			std::string str = "Entree";
			if (lang == 2) str = "Блюдо";
			row.push_back(column_t(str, true, true, xml_attr::left, 0, 0, xml_attr::normal));
			
			str = "Qty";
			if (lang == 2) str = "Кол";
			row.push_back(column_t(str, false, false, xml_attr::center, 12, 12, xml_attr::normal));
			
			str = "Price";
			if (lang == 2) str = "Цена";
			row.push_back(column_t(str, false, false, xml_attr::center, 12, 0, xml_attr::normal));
			
			str = "Amount";
			if (lang == 2) str = "Сумма";
			row.push_back(column_t(str, false, false, xml_attr::center, 12, 0, xml_attr::normal));
			
			rows.push_back(row);
		}
		
		{
			row_t row;
			row.push_back(column_t("", true, true, xml_attr::left, 0, 0, xml_attr::normal, 100));
			row.push_back(column_t("", false, false, xml_attr::left, 0, 0, xml_attr::normal, 100));
			row.push_back(column_t("", false, false, xml_attr::left, 0, 0, xml_attr::normal, 100));
			row.push_back(column_t("", false, false, xml_attr::left, 0, 0, xml_attr::normal, 100));
			rows.push_back(row);
		}
		
		for (int i=0; i<order.dish_count(); ++i) {
			if (order.dish(i).total_count() == 0) continue;
			if (order.dish(i).dish_id() == 0) {
				indexes.push_back(i);
			}
			else {
				rows.push_back(row_order_dish(order.dish(i).name(), order.dish(i).def_price(), order.dish(i).total_count(), order.dish(i).def_amount()));
				
				int dish_kitchen_type_group_id = cktg.kt_ktg_map[order.dish(i).kitchen_type_id()];
				ktg_total[dish_kitchen_type_group_id] += order.dish(i).price()*order.dish(i).total_count();
				ktg_discount[dish_kitchen_type_group_id] += (order.dish(i).def_price() - order.dish(i).price())*order.dish(i).total_count();
				for (size_t j=0; j<order.dish(i).modifier_vec().size(); ++j) {
					if (order.dish(i).modifier_vec()[j].mod_dish_id() == 0) {
						ktg_total[dish_kitchen_type_group_id] += order.dish(i).modifier_vec()[j].price()*order.dish(i).modifier_vec()[j].count()*order.dish(i).total_count();
						ktg_discount[dish_kitchen_type_group_id] += (order.dish(i).modifier_vec()[j].def_price()-order.dish(i).modifier_vec()[j].price())*order.dish(i).modifier_vec()[j].count()*order.dish(i).total_count();
					}
					else {
						int modifier_kitchen_type_group_id = cktg.kt_ktg_map[order.dish(i).modifier_vec()[j].kitchen_type_id()];
						ktg_total[modifier_kitchen_type_group_id] += order.dish(i).modifier_vec()[j].price()*order.dish(i).modifier_vec()[j].count()*order.dish(i).total_count();
						ktg_discount[modifier_kitchen_type_group_id] += (order.dish(i).modifier_vec()[j].def_price()-order.dish(i).modifier_vec()[j].price())*order.dish(i).modifier_vec()[j].count()*order.dish(i).total_count();
					}
				}
				for (size_t j=0; j<order.dish(i).modifier_vec().size(); ++j) {
					rows.push_back(row_order_mod(order.dish(i).modifier_vec()[j].name(), order.dish(i).modifier_vec()[j].count(), order.dish(i).modifier_vec()[j].def_price()));
					double disc = -1*(order.dish(i).modifier_vec()[j].price()-order.dish(i).modifier_vec()[j].def_price());
					if (order.dish(i).modifier_vec()[j].coupon_id() != 0 && int(disc*100) != 0) {
						ksi_cafe::coupon_t& cur_coupon = ksi_cafe::find_coupon(order.dish(i).modifier_vec()[j].coupon_id(), coupon_in_cafe->coupon());
						std::string coupon_title = cur_coupon.title();
						int mod_count = order.dish(i).modifier_vec()[j].count();
						int dish_count = order.dish(i).total_count();
						rows.push_back(row_mod_discount(coupon_title, disc, mod_count, dish_count));
					}
				}
				if (order.dish(i).description() != "") {
					rows.push_back(row_description(order.dish(i).description()));
				}
				double discount = order.dish(i).def_price() - order.dish(i).price();
				if ( int(discount*100) != 0 && order.dish(i).coupon_id() != 0) {
					ksi_cafe::coupon_t& cur_coupon = ksi_cafe::find_coupon(order.dish(i).coupon_id(), coupon_in_cafe->coupon());
					std::string coupon_title = cur_coupon.title();
					rows.push_back(row_discount(coupon_title, discount*order.dish(i).total_count(), true));
				}
				else if (discount != 0 && order.dish(i).employee_id() != 0) {
					rows.push_back(row_discount(type_name, discount, true));
				}
			}
		}
		for (size_t i=0; i<indexes.size(); ++i) {
			int& index = indexes[i];
			if (order.dish(index).coupon_id() != 0 && order.dish(index).dish_id() == 0) {
				rows.push_back(row_discount(order.dish(index).name(), -1*order.dish(index).price(), false));
			}
			else {
				rows.push_back(row_order_dish(order.dish(index).name(), order.dish(index).def_price(), order.dish(index).total_count(), order.dish(index).def_amount()));
			}
		}
		xml.push_back(xml_table_t(rows).xml_str());
	}
	
	if (by_group) {
		for (size_t i=0; i<cktg.ktg_vec.size(); ++i) {
			std::string name = ksi_cafe::full_trim(cktg.ktg_vec[i].name());
			double discount = ktg_discount[cktg.ktg_vec[i].kitchen_type_group_id()];
			double total = ktg_total[cktg.ktg_vec[i].kitchen_type_group_id()];
			if ( int(discount+total)*100 == 0) continue;
			xml.push_back("<fl/><lf/>");
			{
				row_t row;
				row.push_back(column_t(name, false, true, xml_attr::left, 0, 0, xml_attr::normal));
				xml.push_back(xml_table_t(row).xml_str());
			}
			if (lang == 2 && int(discount*100) != 0) {
				row_t row;
				row.push_back(column_t("Сумма без скидок:", false, false, xml_attr::left, 43, 0, xml_attr::normal));
				row.push_back(column_t(to_mb(locale->get_currency(discount+total)), false, true, xml_attr::right, 0, 0, xml_attr::normal));
				xml.push_back(xml_table_t(row).xml_str());
			}
			if ( int(discount*100) != 0) {
				row_t row;
				row.push_back(column_t("Сумма скидок:", false, false, xml_attr::left, 43, 43, xml_attr::normal));
				row.push_back(column_t(to_mb(locale->get_currency(-1*discount)), false, true, xml_attr::right, 0, 0, xml_attr::normal));
				xml.push_back(xml_table_t(row).xml_str());
			}
			if ( int(total*100) != 0) {
				row_t row;
				row.push_back(column_t("Итого:", false, false, xml_attr::left, 43, 0, xml_attr::normal));
				row.push_back(column_t(to_mb(locale->get_currency(total)), false, true, xml_attr::right, 0, 0, xml_attr::normal));
				xml.push_back(xml_table_t(row).xml_str());
			}
		}
	}
	
	if (order.type_employee_meal() && order.payment_dummy()) {
		xml.push_back("<fl/><lf/>");
		xml.push_back(xml_table_t(rows_total(order.subtotal(), order.subtotal(), 0, 0, 0, 0, 0, 0, 0, 0, 2)).xml_str());
		if (order.employee_id() != 0) {
			xml.push_back("<lf/>");
			{
				row_t row;
				row.push_back(column_t("Employee: " + users->user(order.employee_id()).fio(), false, true, xml_attr::left, 0, 0, xml_attr::normal));
				xml.push_back(xml_table_t(row).xml_str());
			}
			xml.push_back(row_signature(shared_locale->get_std("receipt", "employee_signature_str", "Employee signature")).xml_str());
		}
		else {
			xml.push_back("<lf/>");
			{
				row_t row;
				row.push_back(column_t("Employee: " + order.memo(), false, true, xml_attr::left, 0, 0, xml_attr::normal));
				xml.push_back(xml_table_t(row).xml_str());
			}
			xml.push_back(row_signature(shared_locale->get_std("receipt", "employee_signature_str", "Employee signature")).xml_str());
		}
	}
	else if (order.payment_cash()) {
		xml.push_back("<fl/><lf/>");
		if (lang == 2 && order.type_table_booking()) {
			xml.push_back(xml_table_t(rows_total(order.subtotal(), order.discount(), order.tax(), order.gratuity(), order.total(), (order.type_delivery() ? order.delivery_fee() : 0), 0, order.prepayment_amount(), order.sales_tax(), order.gratuity_tax(),2)).xml_str());
		}
		else {
			xml.push_back(xml_table_t(rows_total(order.subtotal(), order.discount(), order.tax(), order.gratuity(), order.total(), (order.type_delivery() ? order.delivery_fee() : 0), 0, order.prepayment_amount(), order.sales_tax(), order.gratuity_tax(),2)).xml_str());
		}
		const ksi_cafe::cash_payment_t* pay = dynamic_cast<const ksi_cafe::cash_payment_t*>(order.cur_payment());
		if (pay) {
			xml.push_back(xml_table_t(rows_change("Cash tendered", pay->cash(), pay->change())).xml_str());
		}
		if (order.type_delivery() || order.type_pickup()) {
			xml.push_back("<lf/>");
			std::string result = "";
			if (order.type_delivery()) {
				result = "Delivery @ ";
				if (lang == 2) result = "Доставка @ ";
			}
			else if (order.type_pickup()) {
				result = "Pickup @ ";
				if (lang == 3) result = "Пикап @ ";
			}
			
			{
				row_t row;
				row.push_back(column_t(result+to_mb(locale->get_date_time_short(order.get_delivery_info()->delivery_time())) + " to " + order.get_delivery_info()->fio(), true, true, xml_attr::left, 0, 0, xml_attr::normal, 0));
				xml.push_back(xml_table_t(row).xml_str());
			}
			if (order.type_delivery()) {
				row_t row;
				std::string address = "Address: ";
				if (lang == 2) address = "Адрес: ";
				row.push_back(column_t(address + order.get_delivery_info()->full_delivery_address(), true, true, xml_attr::left, 0, 0, xml_attr::normal, 0));
				xml.push_back(xml_table_t(row).xml_str());
			}
			{
				row_t row;
				std::string phone = "Phone: ";
				if (lang == 2) phone = "Телефон: ";
				row.push_back(column_t(phone + order.get_delivery_info()->phone(), true, true, xml_attr::left, 0, 0, xml_attr::normal, 0));
				xml.push_back(xml_table_t(row).xml_str());
			}
			xml.push_back("<lf/>");
		}
	}
	else if (order.payment_check()) {
	}
	else if (order.payment_credit_card()) {
		xml.push_back("<fl/><lf/>");
		const ksi_cafe::credit_payment_t* pay = dynamic_cast<const ksi_cafe::credit_payment_t*>(order.cur_payment());
		if (pay)
			xml.push_back(xml_table_t(rows_total(order.subtotal(), order.discount(), order.tax(), order.gratuity(), order.total(), (order.type_delivery() ? order.delivery_fee() : 0), pay->tip(), (order.prepayment_empty() ? 0 : order.prepayment_amount()), order.sales_tax(), order.gratuity_tax(), 2)).xml_str());
		else
			xml.push_back(xml_table_t(rows_total(order.subtotal(), order.discount(), order.tax(), order.gratuity(), order.total(), (order.type_delivery() ? order.delivery_fee() : 0), 0, (order.prepayment_empty() ? 0 : order.prepayment_amount()), order.sales_tax(), order.gratuity_tax(), 2)).xml_str());
		if (order.type_delivery() || order.type_pickup()) {
			xml.push_back("<lf/>");
			std::string result = "";
			if (order.type_delivery()) {
				result = "Delivery @ ";
				if (lang == 2) result = "Доставка @ ";
			}
			else if (order.type_pickup()) {
				result = "Pickup @ ";
				if (lang == 3) result = "Пикап @ ";
			}
			{
				row_t row;
				row.push_back(column_t(result+to_mb(locale->get_date_time_short(order.get_delivery_info()->delivery_time())) + " to " + order.get_delivery_info()->fio(), true, true, xml_attr::left, 0, 0, xml_attr::normal, 0));
				xml.push_back(xml_table_t(row).xml_str());
			}
			if (order.type_delivery()) {
				row_t row;
				std::string address = "Address: ";
				if (lang == 2) address = "Адрес: ";
				row.push_back(column_t(address + order.get_delivery_info()->full_delivery_address(), true, true, xml_attr::left, 0, 0, xml_attr::normal, 0));
				xml.push_back(xml_table_t(row).xml_str());
			}
			{
				row_t row;
				std::string phone = "Phone: ";
				if (lang == 2) phone = "Телефон: ";
				row.push_back(column_t(phone + order.get_delivery_info()->phone(), true, true, xml_attr::left, 0, 0, xml_attr::normal, 0));
				xml.push_back(xml_table_t(row).xml_str());
			}
		}
	}
	else {
		xml.push_back("<fl/><lf/>");
		if (order.type_table_booking() && order.total() != 0)
			xml.push_back(xml_table_t(rows_total(order.subtotal(), order.discount(), order.tax(), order.gratuity(), order.total(), (order.type_delivery() ? order.delivery_fee() : 0), 0, 0, order.sales_tax(), order.gratuity_tax(), 1)).xml_str());
		else if (!order.type_table_booking())
			xml.push_back(xml_table_t(rows_total(order.subtotal(), order.discount(), order.tax(), order.gratuity(), order.total(), (order.type_delivery() ? order.delivery_fee() : 0), 0, order.prepayment_amount(), order.sales_tax(), order.gratuity_tax(), 1)).xml_str());
		if (order.type_delivery() || order.type_pickup()) {
			xml.push_back("<lf/>");
			std::string result = "";
			if (order.type_delivery()) {
				result = "Delivery @ ";
				if (lang == 2) result = "Доставка @ ";
			}
			else if (order.type_pickup()) {
				result = "Pickup @ ";
				if (lang == 3) result = "Пикап @ ";
			}
			{
				row_t row;
				row.push_back(column_t(result+to_mb(locale->get_date_time_short(order.get_delivery_info()->delivery_time())) + " to " + order.get_delivery_info()->fio(), true, true, xml_attr::left, 0, 0, xml_attr::normal, 0));
				xml.push_back(xml_table_t(row).xml_str());
			}
			if (order.type_delivery()) {
				row_t row;
				std::string address = "Address: ";
				if (lang == 2) address = "Адрес: ";
				row.push_back(column_t(address + order.get_delivery_info()->full_delivery_address(), true, true, xml_attr::left, 0, 0, xml_attr::normal, 0));
				xml.push_back(xml_table_t(row).xml_str());
			}
			{
				row_t row;
				std::string phone = "Phone: ";
				if (lang == 2) phone = "Телефон: ";
				row.push_back(column_t(phone + order.get_delivery_info()->phone(), true, true, xml_attr::left, 0, 0, xml_attr::normal, 0));
				xml.push_back(xml_table_t(row).xml_str());
			}
			xml.push_back("<lf/>");
			if (order.memo() != "") {
				row_t row;
				row.push_back(column_t("Order notes: " + order.memo(), true, true, xml_attr::left, 0, 0, xml_attr::normal, 0));
				xml.push_back(xml_table_t(row).xml_str());
			}
			{
				row_t row;
				row.push_back(column_t(order.cafe().delivery_notes().c_str(), true, true, xml_attr::left, 0, 0, xml_attr::normal, 0));
				xml.push_back(xml_table_t(row).xml_str());
				if (receipt_type != "CUSTOMER COPY") {
					xml.push_back(row_signature(shared_locale->get_std("receipt", "customer_signature_str", "CUSTOMER SIGNATURE")).xml_str());
				}
			}
		}
		if (lang == 1 && !(order.type_delivery() || order.type_pickup())) {
			row_t row;
			row.push_back(column_t("order is not paid", false, true, xml_attr::right, 0, 0, xml_attr::normal, 0));
			xml.push_back(xml_table_t(row).xml_str());
		}
		else if (lang == 2) {
			row_t row;
			row.push_back(column_t("Предчек", false, true, xml_attr::right, 0, 0, xml_attr::normal, 0));
			xml.push_back(xml_table_t(row).xml_str());
		}
	}
	

	if (lang == 2) {
		if (order.payment_cash()) {
			row_t row;
			row.push_back(column_t("Оплата наличными", true, true, xml_attr::left, 0, 0, xml_attr::normal));
			xml.push_back(xml_table_t(row).xml_str());
		}
		else if (order.payment_credit_card()) {
			row_t row;
			row.push_back(column_t("Оплата кредитной картой", true, true, xml_attr::left, 0, 0, xml_attr::normal));
			xml.push_back(xml_table_t(row).xml_str());
		}
	}
	
	{
		row_t row;
		row.push_back(column_t(order.cafe().receipt_footer(), true, true, xml_attr::center, 0, 0, xml_attr::hx2));
		xml.push_back(xml_table_t(row).xml_str());
	}
	
	xml.push_back("<lf/><cut/>");
    
	return std::accumulate(xml.begin(), xml.end(), std::string(""));
}

std::string print_order(const ksi_cafe::order_t& order, bool by_group, bool heartland)
{
	if (order.type_delivery() || order.type_pickup()) {
		return 
		print_order(order, shared_locale->get_std("receipt", "customer_copy_str", "** CUSTOMER COPY **"), by_group, heartland) 
		+ 
		print_order(order, shared_locale->get_std("receipt", "merchant_copy_str", "** MERCHANT COPY **"), by_group, heartland);
	}
	return print_order(order, "", by_group, heartland);
}


std::string print_csleep_from_base(const ksi_cafe::cafe_t& cafe_, int order_id)
{
	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();

	query->create_statement
	(
	"select amount, tips, ref_transaction_id, last_4_digits, card_type, type_id, swiped_table.swiped \
	from ksi.order_cctrans, (select swiped from ksi.order_cctrans where type_id = 5 and cafe_id = ksi.util.get_cur_cafe_id and order_id = :order_id) swiped_table \
	where cafe_id = ksi.util.get_cur_cafe_id and order_id = :order_id and type_id in (1,4) \
	order by tran_date desc"
	);
	query->set("order_id", order_id);
	query->execute_statement();
	if (query->next()) THROW_LOCATION(0, "std::string print_csleep_from_base(int)");
	
	double amount = boost::get<double>(query->get(0));
	double tips = boost::get<double>(query->get(1));
	int auth_txn_id = static_cast<int>(boost::get<double>(query->get(2)));
	std::string cc_num = boost::get<std::string>(query->get(3));
	std::string cc_type = boost::get<std::string>(query->get(4));
	int swiped = static_cast<int>(boost::get<double>(query->get(6)));
	return print_csleep(cafe_, amount, tips, cc_type, ksi_cafe::last4digits(cc_num), "", auth_txn_id, swiped);
}

std::string print_msleep(const ksi_cafe::cafe_t& cafe_, int order_id, double amount, const std::string& cd_type, const std::string& cd_num, const std::string& name, int auth_txn_id, bool swiped)
{
	std::vector<std::string> xml;
	std::string header_title = cafe_.address().full_address();

	//xml.push_back(print_refund_header(cafe_.name(), "** MERCHANT COPY **", header_title));
	xml.push_back(print_header(shared_locale->get_std("receipt", "merchant_copy_str", "** MERCHANT COPY **"), cafe_.receipt_header()));

	{
		row_t row;
		row.push_back(column_t("order # "+boost::lexical_cast<std::string>(order_id), false, true, xml_attr::left, 0, 0, xml_attr::normal));
		xml.push_back(xml_table_t(row).xml_str());
	}
  
	{
		row_t row;
		row.push_back(column_t("Transaction_id: "+boost::lexical_cast<std::string>(auth_txn_id), false, true, xml_attr::left, 0, 0, xml_attr::normal));
		xml.push_back(xml_table_t(row).xml_str());
	}
	
	{
		row_t row;
		row.push_back(column_t("CARD TYPE: "+cd_type, false, true, xml_attr::left, 0, 0, xml_attr::normal));
		xml.push_back(xml_table_t(row).xml_str());
	}
	
	std::string processing = "";
	if (swiped) processing = "SWIPED";
	else processing = "ENTERED";
	
	{
		row_t row;
		row.push_back(column_t("CARD NUMBER: "+ksi_cafe::last4digits(cd_num) + " " + processing, false, true, xml_attr::left, 0, 0, xml_attr::normal));
		xml.push_back(xml_table_t(row).xml_str());
	}

	xml.push_back("<lf/>");
	{
		row_t row;
		row.push_back(column_t("Date: " + to_mb(locale->get_date_time(boost::posix_time::second_clock::local_time())), false, true, xml_attr::left, 0, 0, xml_attr::normal));
		xml.push_back(xml_table_t(row).xml_str());
	}
	xml.push_back(row_tip_total(amount, -1));

	xml.push_back(row_signature(shared_locale->get_std("receipt", "customer_signature_str", "CUSTOMER SIGNATURE")).xml_str());

	{
		row_t row;
		row.push_back(column_t(cafe_.receipt_footer(), true, true, xml_attr::center, 0, 0, xml_attr::hx2));
		xml.push_back(xml_table_t(row).xml_str());
	}
	xml.push_back("<lf/><cut/>");
	
	return std::accumulate(xml.begin(), xml.end(), std::string(""));
}

std::string print_csleep(const ksi_cafe::cafe_t& cafe_, double amount, double tip, const std::string& cd_type, const std::string& cd_num, const std::string& name, int auth_txn_id, bool swiped)
{
	std::vector<std::string> xml;

	std::string header_title = cafe_.address().full_address();
	//xml.push_back(print_refund_header(cafe_.name(), "** CUSTOMER COPY **", header_title.c_str()));
	xml.push_back(print_header(shared_locale->get_std("receipt", "customer_copy_str", "** CUSTOMER COPY **"), cafe_.receipt_header()));

	{
		row_t row;
		row.push_back(column_t("Transaction_id: "+boost::lexical_cast<std::string>(auth_txn_id), false, true, xml_attr::left, 0, 0, xml_attr::normal));
		xml.push_back(xml_table_t(row).xml_str());
	}
	
	{
		row_t row;
		row.push_back(column_t("CARD TYPE: "+cd_type, false, true, xml_attr::left, 0, 0, xml_attr::normal));
		xml.push_back(xml_table_t(row).xml_str());
	}
	
	std::string processing = "";
	if (swiped) processing = "SWIPED";
	else processing = "ENTERED";
	{
		row_t row;
		row.push_back(column_t("CARD NUMBER: "+ksi_cafe::last4digits(cd_num) + " " + processing, false, true, xml_attr::left, 0, 0, xml_attr::normal));
		xml.push_back(xml_table_t(row).xml_str());
	}

	xml.push_back("<lf/>");
	
	{
		row_t row;
		row.push_back(column_t("Date: " + to_mb(locale->get_date_time(boost::posix_time::second_clock::local_time())), false, true, xml_attr::left, 0, 0, xml_attr::normal));
		xml.push_back(xml_table_t(row).xml_str());
	}

	xml.push_back(row_tip_total(amount, tip));

	xml.push_back(row_signature(shared_locale->get_std("receipt", "customer_signature_str", "CUSTOMER SIGNATURE")).xml_str());
  
	{
		row_t row;
		row.push_back(column_t(cafe_.receipt_footer(), true, true, xml_attr::center, 0, 0, xml_attr::hx2));
		xml.push_back(xml_table_t(row).xml_str());
	}
	xml.push_back("<lf/><cut/>");
	
	return std::accumulate(xml.begin(), xml.end(), std::string(""));
}

std::string print_csleep(const ksi_cafe::cafe_t& cafe_, const std::string& cd_num, const std::string& message, int txn_id, bool swiped)
{
	std::vector<std::string> xml;

	std::string header_title = cafe_.address().full_address();

	//xml.push_back(print_refund_header(cafe_.name(), "** CUSTOMER COPY **", header_title));
	xml.push_back(print_header(shared_locale->get_std("receipt", "customer_copy_str", "** CUSTOMER COPY **"), cafe_.receipt_header()));

	{
		row_t row;
		row.push_back(column_t("Transaction_id: "+boost::lexical_cast<std::string>(txn_id), false, true, xml_attr::left, 0, 0, xml_attr::normal));
		xml.push_back(xml_table_t(row).xml_str());
	}

	std::string processing = "";
	if (swiped) processing = "SWIPED";
	else processing = "ENTERED";
	
	{
		row_t row;
		row.push_back(column_t("CARD NUMBER: "+ksi_cafe::last4digits(cd_num) + " " + processing, false, true, xml_attr::left, 0, 0, xml_attr::normal));
		xml.push_back(xml_table_t(row).xml_str());
	}

	xml.push_back("<lf/>");
	
	{
		row_t row;
		row.push_back(column_t("Date: " + to_mb(locale->get_date_time(boost::posix_time::second_clock::local_time())), false, true, xml_attr::left, 0, 0, xml_attr::normal));
		xml.push_back(xml_table_t(row).xml_str());
	}

	{
		row_t row;
		row.push_back(column_t("Operatation failed: " + message, false, true, xml_attr::left, 0, 0, xml_attr::normal));
		xml.push_back(xml_table_t(row).xml_str());
	}

	{
		row_t row;
		row.push_back(column_t(cafe_.receipt_footer(), true, true, xml_attr::center, 0, 0, xml_attr::hx2));
		xml.push_back(xml_table_t(row).xml_str());
	}
	xml.push_back("<lf/><cut/>");
	
	return std::accumulate(xml.begin(), xml.end(), std::string(""));
}

std::string print_csleep(const ksi_cafe::cafe_t& cafe_, const std::string& cd_num, int txn_id, int auth_txn_id)
{
	std::vector<std::string> xml;

	std::string header_title = cafe_.address().full_address();

	//xml.push_back(print_refund_header(cafe_.name(), "** CUSTOMER COPY **", header_title));
	xml.push_back(print_header(shared_locale->get_std("receipt", "customer_copy_str", "** CUSTOMER COPY **"), cafe_.receipt_header()));

	{
		row_t row;
		row.push_back(column_t("Transaction_id: "+boost::lexical_cast<std::string>(txn_id), false, true, xml_attr::left, 0, 0, xml_attr::normal));
		xml.push_back(xml_table_t(row).xml_str());
	}
	xml.push_back("<lf/>");

	{
		row_t row;
		row.push_back(column_t("Date: " + to_mb(locale->get_date_time(boost::posix_time::second_clock::local_time())), false, true, xml_attr::left, 0, 0, xml_attr::normal));
		xml.push_back(xml_table_t(row).xml_str());
	}
	
	{
		row_t row;
		row.push_back(column_t("Credit card authorization cancelled (auth transaction id: " + boost::lexical_cast<std::string>(auth_txn_id) + ")", false, true, xml_attr::left, 0, 0, xml_attr::normal));
		xml.push_back(xml_table_t(row).xml_str());
	}
  
	{
		row_t row;
		row.push_back(column_t(cafe_.receipt_footer(), true, true, xml_attr::center, 0, 0, xml_attr::hx2));
		xml.push_back(xml_table_t(row).xml_str());
	}
	xml.push_back("<lf/><cut/>");
	
	return std::accumulate(xml.begin(), xml.end(), std::string(""));
}

std::string print_csleep(const ksi_cafe::cafe_t& cafe_, double amount, const std::string& cd_type, const std::string& cd_num, const std::string& name, int auth_txn_id, bool swiped) { 
	return print_csleep(cafe_, amount, double(-1), cd_type, cd_num, name, auth_txn_id, swiped); 
}

void print_refund(const ksi_cafe::cafe_t& cafe_, int order_id, const std::string& receipt_type, fiscal::registrator_t& registrar) {
	std::string footer = cafe_.receipt_footer();
	registrar.setFooter(footer);
	registrar.open_receipt(fiscal::receipt_type::REFUND, false);
	
	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	query->create_statement
	(
	"select \
		orders.created \
		, orders.closed \
		, orders.creator_id \
		, orders.closer_id \
		, orders.type_id \
		, order_type.name as type_name \
		, orders.sales_tax \
		, orders.subtotal \
		, orders.status_id \
		, order_status.name as status_name \
		, orders.token \
		, orders.tables \
		, seance.terminal_id \
		, terminal.name as terminal_name \
		, orders.payment_type_id \
		, payment_type.name as payment_type_name \
		, orders.guest_count \
		, orders.prepayment \
		, orders.prepayment_type_id \
		, prepayment_type.name as prepayment_type_name \
		, orders.refund_note \
		, orders.flags \
		, orders.meal_person_id \
		, orders.tendered_amount \
		, orders.memo \
		, order_delivery.delivery_price \
		, (order_delivery.firstname || ' ' || order_delivery.lastname) as delivery_fio \
		, order_delivery.streetaddress as delivery_streetaddress \
		, order_delivery.phone_home as delivery_phone \
		, order_delivery.delivery_date \
		, orders.gratuity \
	from ksi.orders \
	inner join ksi.order_type on order_type.type_id = orders.type_id \
	inner join ksi.order_status on order_status.status_id = orders.status_id \
	inner join ksi.seance on seance.seance_id = orders.seance_id \
	inner join ksi.terminal on terminal.terminal_id = seance.terminal_id \
	left join ksi.payment_type on payment_type.payment_type_id = orders.payment_type_id \
	left join ksi.payment_type  prepayment_type on prepayment_type.payment_type_id = orders.prepayment_type_id \
	left join ksi.order_delivery on order_delivery.delivery_id = orders.delivery_id \
	where orders.order_id = :order_id and orders.cafe_id = ksi.util.get_cur_cafe_id"
	);
	query->set("order_id", order_id);
	query->execute_statement();
	
	if (query->next()) THROW_LOCATION(0, "std::string print_receipt_duplicate(const ksi_cafe::cafe_t&, int, int, const std::strings&, bool)");
	
	boost::posix_time::ptime created = boost::get<boost::posix_time::ptime>(query->get(0));
	boost::posix_time::ptime closed = boost::get<boost::posix_time::ptime>(query->get(1));
	int creator_id = static_cast<int>(boost::get<double>(query->get(2)));
	int closer_id = static_cast<int>(boost::get<double>(query->get(3)));
	int order_type_id = static_cast<int>(boost::get<double>(query->get(4)));
	std::string type_name = boost::get<std::string>(query->get(5));
	double sales_tax = boost::get<double>(query->get(6));
	double subtotal = boost::get<double>(query->get(7));
	int status_id = static_cast<int>(boost::get<double>(query->get(8)));
	std::string status_name = boost::get<std::string>(query->get(9));
	std::string token = boost::get<std::string>(query->get(10));
	std::string table = boost::get<std::string>(query->get(11));
	int terminal_id = static_cast<int>(boost::get<double>(query->get(12)));
	std::string terminal_name = boost::get<std::string>(query->get(13));
	int payment_type_id = static_cast<int>(boost::get<double>(query->get(14)));
	std::string payment_type_name = boost::get<std::string>(query->get(15));
	int guest_count = static_cast<int>(boost::get<double>(query->get(16)));
	double prepayment = boost::get<double>(query->get(17));
	int prepayment_type_id = static_cast<int>(boost::get<double>(query->get(18)));
	std::string prepayment_type_name = boost::get<std::string>(query->get(19));
	std::string refund_note = boost::get<std::string>(query->get(20));
	int is_hide = static_cast<int>(boost::get<double>(query->get(21))) & 0x8;
	int meal_person_id = static_cast<int>(boost::get<double>(query->get(22)));
	double tendered_amount = boost::get<double>(query->get(23));
	std::string order_memo = boost::get<std::string>(query->get(24));
	double delivery_fee = boost::get<double>(query->get(25));
	std::string delivery_fio = boost::get<std::string>(query->get(26));
	std::string delivery_streetaddress = boost::get<std::string>(query->get(27));
	std::string delivery_phone = boost::get<std::string>(query->get(28));
	boost::posix_time::ptime delivery_date = boost::get<boost::posix_time::ptime>(query->get(29));
	double gratuity = boost::get<double>(query->get(30));
	
	std::string str1 = "Order #";
	if (lang == 2) str1 = "Заказ #";
	std::string order_id_str = boost::lexical_cast<std::string>(order_id);
	str1 += order_id_str;
	registrar.print(fiscal::format::NORMAL, fiscal::format::LEFT, 3, str1, false);
	
	if (status_id == 3 || status_id == 4 || status_id == 5) {
		std::string str = "Closed: ";
		if (lang == 2) str = "Закрыт: ";
		registrar.print(fiscal::format::NORMAL, fiscal::format::LEFT, 3, str + to_mb(locale->get_date_time(closed)), false);
	}
	std::string str2 = "Refunded: ";
	if (lang == 2) str2 = "Возвращен: ";
	registrar.print(fiscal::format::NORMAL, fiscal::format::LEFT, 3, str2 + to_mb(locale->get_date_time(boost::posix_time::second_clock::local_time())), false);
	
	std::string str3 = "Operator: ";
	if (lang == 2) str3 = "Официант: ";
	str3 += users->user(creator_id).fio() + " @ " + terminal_name;
	registrar.print(fiscal::format::NORMAL, fiscal::format::LEFT, 3, str3, false);
	
	query->create_statement
	(
	"select \
	    dish.name as dish_name \
	  , order_dish.dish_id \
	  , order_dish.count \
	  , order_dish.order_dish_id \
	  , order_dish.description \
	  , coupon.title as coupon_name \
	  , order_dish.coupon_id \
	  , order_dish.price \
	  , order_dish.base_price \
	  , kitchen_type.name as kitchen_type_name \
	  , dish.is_beverage as kitchen_type_id \
	  , kitchen_type_group.name as kitchen_type_group_name \
	  , kitchen_type_group.ktg_id as kitchen_type_group_id \
	from ksi.order_dish \
	left join ksi.coupon on coupon.coupon_id = order_dish.coupon_id \
	left join ksi.dish on dish.dish_id = order_dish.dish_id \
	left join ksi.kitchen_type on kitchen_type.kt_id = dish.is_beverage \
	left join ksi.kitchen_type_group on kitchen_type_group.ktg_id = kitchen_type.group_id \
	where order_dish.order_id = :order_id and order_dish.cafe_id = ksi.util.get_cur_cafe_id \
	order by coupon_id desc, order_dish_id"
	);
	query->set("order_id", order_id);
	query->execute_statement();
	
	std::auto_ptr<ksi_client::oracle_query_i> query2 = ksi_cafe::session->create_query();
	
	query2->create_statement
	(
	"select \
	    modifier.name modifier_name \
	  , order_dish_modifier.price \
	  , order_dish_modifier.base_price \
	  , order_dish_modifier.count \
	  , modifier.dish_id \
	  , coupon.title as coupon_name \
	  , order_dish_modifier.coupon_id \
	  , kitchen_type.name as kitchen_type_name \
	  , dish.is_beverage as kitchen_type_id \
	  , kitchen_type_group.name as kitchen_type_group_name \
	  , kitchen_type_group.ktg_id as kitchen_type_group_id \
	from ksi.order_dish_modifier \
	inner join ksi.modifier on modifier.modifier_id = order_dish_modifier.modifier_id  \
	left join ksi.coupon on coupon.coupon_id = order_dish_modifier.coupon_id \
	left join ksi.dish on dish.dish_id = modifier.dish_id \
	left join ksi.kitchen_type on kitchen_type.kt_id = dish.is_beverage \
	left join ksi.kitchen_type_group on kitchen_type_group.ktg_id = kitchen_type.group_id \
	where cafe_id = ksi.util.get_cur_cafe_id and order_dish_id = :order_dish_id"
	);
	
	std::vector<print_raw_data> vec;
	while (!query->next()) {
		std::string dish_name = boost::get<std::string>(query->get(0));
		int dish_id = static_cast<int>(boost::get<double>(query->get(1)));
		int dish_count = static_cast<int>(boost::get<double>(query->get(2)));
		int order_dish_id = static_cast<int>(boost::get<double>(query->get(3)));
		std::string misc_name = boost::get<std::string>(query->get(4));
		std::string dish_coupon_title = boost::get<std::string>(query->get(5));
		int coupon_id = static_cast<int>(boost::get<double>(query->get(6)));
		double dish_price = boost::get<double>(query->get(7));
		double dish_base_price = boost::get<double>(query->get(8));
		int dish_kitchen_type_group_id =static_cast<int>(boost::get<double>(query->get(12)));

		if (dish_count != 0) {

			if (dish_id == 0 && coupon_id == 0) {
				dish_name = "misc charges: " + misc_name;
			}
			else if (dish_id == 0 && coupon_id > 0) {
				continue;
			}
			else if (coupon_id == -1) {
				continue;
			}
			
			query2->set("order_dish_id", order_dish_id);
			query2->execute_statement();
	    
			std::vector<print_raw_data> mod_vec;
			
			while (!query2->next()) {
				std::string modifier = boost::get<std::string>(query2->get(0));
				double mod_price = boost::get<double>(query2->get(1));
				double mod_base_price = boost::get<double>(query2->get(2));
				int mod_count = static_cast<int>(boost::get<double>(query2->get(3)));
				int mod_dish_id = static_cast<int>(boost::get<double>(query2->get(4)));
				std::string mod_coupon_title = boost::get<std::string>(query2->get(5));
				int mod_coupon_id = static_cast<int>(boost::get<double>(query2->get(6)));
				int mod_kitchen_type_group_id = static_cast<int>(boost::get<double>(query2->get(10)));
				
				mod_vec.push_back(print_raw_data(modifier, mod_price, dish_count*mod_count));
			}

			if (coupon_id != 0 && dish_id == 0) { // fixed discount
			}
			else {
				vec.push_back(print_raw_data(dish_name, dish_price, dish_count, dish_price));
			}
			
			for (size_t i=0; i<mod_vec.size(); ++i) vec.push_back(mod_vec[i]);
		}
	}
	registrar.print_fl();
	for (size_t i=0; i<vec.size(); ++i) {
		if (vec[i].type == 2) { // Dish
			registrar.print(fiscal::format::NORMAL, fiscal::format::LEFT, 3, vec[i].name, false);
			registrar.refund(vec[i].price, vec[i].count, false);
		}
		else if (vec[i].type == 3) { // Modifier
			registrar.print(fiscal::format::NORMAL, fiscal::format::LEFT, 3, "  " + vec[i].name, false);
			registrar.refund(vec[i].price, vec[i].count, false);
		}
	}
	registrar.close_receipt(fiscal::payment_type::CASH, false);	
}

void print_order(const ksi_cafe::order_t& order, fiscal::registrator_t& registrar, double cash) {
	std::string footer = order.cafe().receipt_footer();
	registrar.setFooter(footer);
	registrar.open_receipt(fiscal::receipt_type::SALE, false);

	std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();
	query->create_statement
	(
	"select terminal.name, order_type.name \
	from ksi.orders \
	left join ksi.seance on seance.seance_id = orders.seance_id \
	left join ksi.terminal on terminal.terminal_id = seance.terminal_id \
	left join ksi.order_type on order_type.type_id = orders.type_id \
	where order_id = :order_id"
	);
	query->set("order_id", order.order_id());
	query->execute_statement();	
	if (query->next()) THROW_LOCATION(0, "std::string print_order(const ksi_cafe::order_t&, fiscal::registrator_t&)");
	
	std::string POS = boost::get<std::string>(query->get(0));
	std::string type_name = boost::get<std::string>(query->get(1));
	
	std::string user;
	if (order.creator_id() != 0) user = users->user(order.creator_id()).fio();
	else user = users->user().fio();
	
	std::string str1 = "Order # ";
	if (lang == 2) str1 = "Заказ # ";
	std::string order_id_str = boost::lexical_cast<std::string>(order.order_id());
	str1 += order_id_str;
	registrar.print(fiscal::format::NORMAL, fiscal::format::LEFT, 3, str1, false);
	
	std::string str2 = "Operator: ";
	if (lang == 2) str2 = "Официант: ";
	str2 += user + " @ " + POS;
	registrar.print(fiscal::format::NORMAL, fiscal::format::LEFT, 3, str2, false);
	
	std::vector<int> indexes;
	registrar.print_fl();
	for (int i=0; i<order.dish_count(); ++i) {
		if (order.dish(i).total_count() == 0) continue;
		if (order.dish(i).dish_id() == 0) {
			indexes.push_back(i);
		}
		else {
			registrar.print(fiscal::format::NORMAL, fiscal::format::LEFT, 3, order.dish(i).name(), false);
			registrar.sale(order.dish(i).price(), order.dish(i).total_count(), false);
			
			for (size_t j=0; j<order.dish(i).modifier_vec().size(); ++j) {
				registrar.print(fiscal::format::NORMAL, fiscal::format::LEFT, 3, "  " + order.dish(i).modifier_vec()[j].name(), false);
				registrar.sale(order.dish(i).modifier_vec()[j].price(), order.dish(i).modifier_vec()[j].count(), false);
			}
		}
		for (size_t i=0; i<indexes.size(); ++i) {
			int& index = indexes[i];
			if (order.dish(index).coupon_id() != 0 && order.dish(index).dish_id() == 0) {
			}
			else {
				registrar.print(fiscal::format::NORMAL, fiscal::format::LEFT, 3, order.dish(index).name(), false);
				registrar.sale(order.dish(index).price(), order.dish(index).total_count(), false);
			}
		}
	}
	registrar.final_pay(fiscal::payment_type::CASH, cash < order.total() ? order.total() : cash, false);
	registrar.close_receipt(fiscal::payment_type::CASH, false);
}



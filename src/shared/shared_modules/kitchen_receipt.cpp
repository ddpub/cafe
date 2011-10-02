

#include <vector>
#include <string>
#include "kitchen_receipt.hpp"
#include <printer/xml_check_printer.hpp>
#include <ksi_util/util.hpp>
#include <ksi_include/algo_functor.hpp>
#include <numeric>

extern int lang;

std::string print_kitchen_footer(const std::string& table_group, const std::string& table, const std::string& type) {
	row_t row;
	if (lang == 1) {
		if (table != "") {
			row.push_back(column_t("Table: " + table, false, true, xml_attr::left, 0, 0, xml_attr::hx2));
		}
	}
	row.push_back(column_t(type, false, true, xml_attr::right, 0, 0, xml_attr::hx2));
	return "<fl font=\"normal\"/><lf/>" + xml_table_t(row).xml_str() + "<lf/>";
}

std::string print_kitchen_header(int order_id, const std::string& table_group, const std::string& table, const std::string& kitchen_name,
const std::string& creator, const std::string& date_str, const std::string& time_str, const std::string& token, const std::string& employee_fio, const std::string& delivery_pickup_time_str) {
	std::string xml;
	{
		std::string order_str = "language is not set";
		if (lang == 1) order_str = "Order #:";
		else if (lang == 2) order_str = "«‡Í‡Á#: ";

		std::string order_id_str = boost::lexical_cast<std::string>(order_id);
		{
			bool is_hide = false;
			row_t row;
			row.push_back(column_t(order_str, false, false, xml_attr::left, 0, 0, xml_attr::normal));
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
			xml += xml_table_t(row).xml_str();
		}
	}
	
	{
		std::string oper_str = "language is not set";
		if (lang == 1) oper_str = "Operator: ";
		else if (lang == 2) oper_str = "ŒÙËˆË‡ÌÚ: ";
		row_t row;
		row.push_back(column_t(oper_str+creator, false, true, xml_attr::left, 0, 0, xml_attr::normal));
		xml += xml_table_t(row).xml_str();
	}
	
	if (lang == 1) {
	}
	else if (lang == 2) {
		if (table != "") {
			std::string str = "—ÚÓÎ: ";
			if (table_group != "") str += table_group + " / ";
			str += table;
			row_t row;
			row.push_back(column_t(str, false, true, xml_attr::left, 0, 0, xml_attr::hx2));
			xml += xml_table_t(row).xml_str();
		}
	}
	
	{
		row_t row;
		std::string str = "language is not set";
		if (lang == 1) str = "Date: ";
		else if (lang == 2) str = "ƒ‡Ú‡: ";	
		if (lang == 1) {
			row.push_back(column_t(str + date_str + " " + time_str, false, true, xml_attr::left, 0, 0, xml_attr::normal));
		}
		else if (lang == 2) {
			row.push_back(column_t(str + date_str, false, false, xml_attr::left, 0, 0, xml_attr::hx2));
			row.push_back(column_t(" " + time_str, false, false, xml_attr::left, 0, 0, xml_attr::hx2wx2));
		}
		if (token != "") {
			std::string str = "language is not set";
			if (lang == 1) str = " Token: ";
			else if (lang == 2)  str = " “ÓÍÂÌ: ";
			row.push_back(column_t(str, false, false, xml_attr::right, 0, 0, xml_attr::hx2));
		}
		xml += xml_table_t(row).xml_str();
	}
	
	if (employee_fio != "") {
		row_t row;
		row.push_back(column_t("Employee: " + employee_fio, false, true, xml_attr::left, 0, 0, xml_attr::hx2));
		xml += xml_table_t(row).xml_str();
	}
	
	if (delivery_pickup_time_str != "") {
		row_t row;
		row.push_back(column_t(delivery_pickup_time_str.c_str(), false, true, xml_attr::left, 0, 0, xml_attr::hx2));
		xml += xml_table_t(row).xml_str();
	}
	
	if (kitchen_name != "") {
		row_t row;
		row.push_back(column_t(kitchen_name.c_str(), false, true, xml_attr::left, 0, 0, xml_attr::hx2));
		xml += xml_table_t(row).xml_str();
	}
	
  return xml;
}


std::string print_kitchen_order(const ksi_cafe::format_date_time_t& format, const ksi_cafe::users_t& users, const ksi_kitchen::container_kitchen_t& container, const ksi_kitchen::order_t& order, bool print_title, bool print_barcode) {
	std::vector<std::string> xml;
	for (int index=0; index<container.count(); ++index) {
		if (container.kitchen(index).enable()) {
			bool header_printed = false;
			std::vector<int> kitchen_types = container.kitchen_types();
			for (size_t i=0; i<kitchen_types.size(); ++i) {
				bool header_kitchen_type_printed = false;
				std::vector<row_t> rows;
				for (int j=0; j<order.dish_count(); ++j) {
					bool dish_footer_printed = false;
					if (!order.dish(j).registered() 
					&& (container.printMiscCharges() ? true : order.dish(j).dish_id()) 
					&& (order.dish(j).dish_id() ? container.kitchen(index).kitchen_execute(order.table().table_group_id(), order.dish(j).kitchen_type_id()) : true)
					&& (order.dish(j).dish_id() ? (order.dish(j).kitchen_type_id() == kitchen_types[i]) : true)
					) {
						if (!header_printed) {
							/*
							if (print_barcode) {
								xml.push_back("<barcode>");
								std::string tmp = order.bar_code_order(container, container.kitchen(index).kitchen_id());
								xml.push_back(order.bar_code_order(container, container.kitchen(index).kitchen_id()).c_str());
								xml.push_back("</barcode>");
								xml.push_back("<lf/>");								
							}
							*/
							std::string token_name = order.token().exist() ? order.token().name().c_str() : "";
							std::string employee_fio = "";
							if (order.type_employee_meal())
								employee_fio = order.employee_id() ? users.user(order.employee_id()).fio().c_str() : order.memo().c_str();
							std::string delivery_pickup_time = "";
							if (order.type_delivery()) delivery_pickup_time = std::string("Delivery time ") + format.date_time(order.delivery_time());
							else if (order.type_pickup()) delivery_pickup_time = std::string("Pickup time ") + format.date_time(order.delivery_time());
							std::string table_name = order.table().exist() ? order.table().name().c_str() : "";
							if (print_title)
								xml.push_back(print_kitchen_header(order.order_id(), order.table().group_name().c_str(), table_name, container.kitchen(index).name().c_str(), 
								users.user(order.creator_id()).fio().c_str(), format.date(order.get_first_not_register_time()), format.time(order.get_first_not_register_time()), token_name, employee_fio, delivery_pickup_time));
							else 
								xml.push_back(print_kitchen_header(order.order_id(), order.table().group_name().c_str(), table_name, "", 
								users.user(order.creator_id()).fio().c_str(), format.date(order.get_first_not_register_time()), format.time(order.get_first_not_register_time()), token_name, employee_fio, delivery_pickup_time));
							row_t row;
							{
								std::string rus_str = "¡Î˛‰Ó";
								std::string eng_str = "Entree";
								std::string str = "language is not set";
								if (lang == 1) str = eng_str;
								else if (lang == 2) str = rus_str;
								row.push_back(column_t(str, true, true, xml_attr::left, 0, 0, xml_attr::normal));
							}
							{
								std::string rus_str = " ÓÎ-‚Ó";
								std::string eng_str = "Qty";
								std::string str = "language is not set";
								if (lang == 1) str = eng_str;
								else if (lang == 2) str = rus_str;
								row.push_back(column_t(str, false, false, xml_attr::right, 0, 0, xml_attr::normal));
							}
							rows.push_back(row);
							header_printed = true;
						}
						if (!header_kitchen_type_printed) {
							{
								row_t row;
								row.push_back(column_t("", true, true, xml_attr::left, 0, 0, xml_attr::normal, 100));
								row.push_back(column_t("", false, false, xml_attr::left, 0, 0, xml_attr::normal, 100));
								rows.push_back(row);
							}
							row_t row;
							row.push_back(column_t(container.type_name(kitchen_types[i]).c_str(), true, true, xml_attr::left, 0, 0, xml_attr::normal));
							row.push_back(column_t("", false, false, xml_attr::left, 0, 0, xml_attr::normal));
							rows.push_back(row);
							header_kitchen_type_printed = true;
						}
						int reg_count = order.dish(j).count() + order.dish(j).count_wasted() - order.dish(j).count_registered();
						std::string dish_name = order.dish(j).name();
						if (!order.dish(j).dish_id()) dish_name = "misc charges";
						if (reg_count < 0) {
							row_t row;
							if (lang == 1)
								row.push_back(column_t(std::string("[CANCEL] ") + ksi_cafe::full_trim(dish_name).c_str(), true, true, xml_attr::left, 0, 0, xml_attr::hx2));
							else if (lang == 2)
								row.push_back(column_t(std::string("[Œ“Ã≈Õ¿] ") + ksi_cafe::full_trim(dish_name).c_str(), true, true, xml_attr::left, 0, 0, xml_attr::hx2));
							row.push_back(column_t(boost::lexical_cast<std::string>(reg_count), false, false, xml_attr::right, 0, 0, xml_attr::hx2));
							rows.push_back(row);
						}
						else {
							row_t row;
							row.push_back(column_t(ksi_cafe::full_trim(dish_name).c_str(), true, true, xml_attr::left, 0, 0, xml_attr::hx2));
							row.push_back(column_t("+"+boost::lexical_cast<std::string>(reg_count), false, false, xml_attr::right, 0, 0, xml_attr::hx2));
							rows.push_back(row);
						}
						
						for (int k=0; k<order.dish(j).mod_count(); ++k) {
							std::string mod_name = ksi_cafe::full_trim(order.dish(j).modifier(k).name()).c_str();
							if (order.dish(j).modifier(k).count() > 1)
								mod_name = boost::lexical_cast<std::string>(order.dish(j).modifier(k).count())+"x "+mod_name;
							int left_margin = 20;
							xml_attr::font_t cur_font = xml_attr::hx2;
							if (lang == 2) cur_font = xml_attr::normal;
							if (order.dish(j).modifier(k).mod_dish_id() == 0) {
								row_t row;
								row.push_back(column_t(mod_name, true, true, xml_attr::left, left_margin, 0, cur_font));
								row.push_back(column_t("", false, false, xml_attr::left, 0, 0, cur_font));
								rows.push_back(row);
							}
							else if (container.kitchen(index).kitchen_execute(order.table().table_group_id(), order.dish(j).modifier(k).kitchen_type_id()) && order.dish(j).modifier(k).kitchen_type_id() == order.dish(j).kitchen_type_id()) {
								if (reg_count < 0) {
									row_t row;
									std::string str = "[CANCEL] ";
									if (lang == 2) str = "[Œ“Ã≈Õ¿] ";
									row.push_back(column_t(str + mod_name, true, true, xml_attr::left, left_margin, 0, cur_font));
									row.push_back(column_t(boost::lexical_cast<std::string>(reg_count*order.dish(j).modifier(k).count()), false, false, xml_attr::right, 0, 0, cur_font));
									rows.push_back(row);
								}
								else {
									row_t row;
									row.push_back(column_t(mod_name, true, true, xml_attr::left, left_margin, 0, cur_font));
									row.push_back(column_t("+"+boost::lexical_cast<std::string>(reg_count*order.dish(j).modifier(k).count()), false, false, xml_attr::right, 0, 0, cur_font));
									rows.push_back(row);
								}
							}
						}
						if (/*order.dish(j).dish_id() != 0 && */order.dish(j).description() != "") {
							row_t row;
							row.push_back(column_t(order.dish(j).description().c_str(), true, true, xml_attr::left, 2, 0, xml_attr::normal));
							row.push_back(column_t("", false, false, xml_attr::left, 0, 0, xml_attr::normal));
							rows.push_back(row);
						}
					}
					else if (!order.dish(j).registered() && order.dish(j).dish_id() && container.kitchen(index).kitchen_execute_for_modifier(order.table().table_group_id(), order.dish(j), kitchen_types[i])) {
						int reg_count = 0;
						for (int k=0; k<order.dish(j).mod_count(); ++k) {
							if (order.dish(j).modifier(k).mod_dish_id() != 0) {
								if (container.kitchen(index).kitchen_execute(order.table().table_group_id(), order.dish(j).modifier(k).kitchen_type_id())
								&& kitchen_types[i] == order.dish(j).modifier(k).kitchen_type_id()
								&& container.container_execute(order.table().table_group_id(), order.dish(j).kitchen_type_id())) {
									if (!header_printed) {
										/*
										if (print_barcode) {
											xml.push_back("<barcode>");
											xml.push_back(order.bar_code_order_for_modifier(container, container.kitchen(index).kitchen_id()).c_str());
											xml.push_back("</barcode>");
											xml.push_back("<lf/>");								
										}
										*/
										std::string token_name = order.token().exist() ? order.token().name().c_str() : "";
										std::string employee_fio = "";
										if (order.type_employee_meal())
											employee_fio = order.employee_id() ? users.user(order.employee_id()).fio().c_str() : order.memo().c_str();
										std::string delivery_pickup_time = "";
										if (order.type_delivery()) delivery_pickup_time = std::string("Delivery time ") + format.date_time(order.delivery_time());
										else if (order.type_pickup()) delivery_pickup_time = std::string("Pickup time ") + format.date_time(order.delivery_time());
										std::string table_name = order.table().exist() ? order.table().name().c_str() : "";
										if (print_title)
											xml.push_back(print_kitchen_header(order.order_id(), order.table().group_name().c_str(), table_name, container.kitchen(index).name().c_str(), 
											users.user(order.creator_id()).fio().c_str(), format.date(order.get_first_not_register_time()), format.time(order.get_first_not_register_time()), token_name, employee_fio, delivery_pickup_time));
										else 
											xml.push_back(print_kitchen_header(order.order_id(), order.table().group_name().c_str(), table_name, "", 
											users.user(order.creator_id()).fio().c_str(), format.date(order.get_first_not_register_time()), format.time(order.get_first_not_register_time()), token_name, employee_fio, delivery_pickup_time));
										row_t row;
										{
											std::string rus_str = "¡Î˛‰Ó";
											std::string eng_str = "Entree";
											std::string str = "language is not set";
											if (lang == 1) str = eng_str;
											else if (lang == 2) str = rus_str;
											row.push_back(column_t(str, true, true, xml_attr::left, 0, 0, xml_attr::normal));
										}
										{
											std::string rus_str = " ÓÎ-‚Ó";
											std::string eng_str = "Qty";
											std::string str = "language is not set";
											if (lang == 1) str = eng_str;
											else if (lang == 2) str = rus_str;
											row.push_back(column_t(str, false, false, xml_attr::right, 0, 0, xml_attr::normal));
										}
										rows.push_back(row);
										header_printed = true;
									}
									if (!header_kitchen_type_printed) {
										{
											row_t row;
											row.push_back(column_t("", true, true, xml_attr::left, 0, 0, xml_attr::normal, 100));
											row.push_back(column_t("", false, false, xml_attr::left, 0, 0, xml_attr::normal, 100));
											rows.push_back(row);
										}
										row_t row;
										row.push_back(column_t(container.type_name(kitchen_types[i]).c_str(), true, true, xml_attr::left, 0, 0, xml_attr::normal));
										row.push_back(column_t("", false, false, xml_attr::left, 0, 0, xml_attr::normal));
										rows.push_back(row);
										header_kitchen_type_printed = true;
									}
									int new_reg_count = order.dish(j).count()*order.dish(j).modifier(k).count()+order.dish(j).modifier(k).count_wasted()-order.dish(j).modifier(k).count_registered();
									new_reg_count /= order.dish(j).modifier(k).count();
									if (abs(new_reg_count) > abs(reg_count)) reg_count = new_reg_count;
									if (!dish_footer_printed) {
										std::string pref = "";
										if (abs(reg_count) != 1) pref = boost::lexical_cast<std::string>(abs(reg_count)) + "x ";
										std::string str = std::string("modifier(s) for ") + pref + ksi_cafe::full_trim(order.dish(j).name()).c_str() + ": ";
										if (lang == 2) str = std::string("ÏÓ‰ËÙËÍ‡ÚÓ(˚) ‰Îˇ ") + pref + ksi_cafe::full_trim(order.dish(j).name()).c_str() + ": ";
										row_t row;
										row.push_back(column_t(str, true, true, xml_attr::left, 0, 0, xml_attr::normal));
										row.push_back(column_t("", false, false, xml_attr::left, 0, 0, xml_attr::normal));
										rows.push_back(row);
										dish_footer_printed = true;
									}
									xml_attr::font_t cur_font = xml_attr::hx2;
									if (lang == 2) cur_font = xml_attr::normal;
									int left_margin = 20;

									if (reg_count < 0) {
										row_t row;
										std::string str = "[CANCEL] ";
										if (lang == 2) str = "[Œ“Ã≈Õ¿] ";
										row.push_back(column_t(str + order.dish(j).modifier(k).name().c_str(), true, true, xml_attr::left, left_margin, 0, cur_font));
										row.push_back(column_t(boost::lexical_cast<std::string>(-1*reg_count*order.dish(j).modifier(k).count()), false, false, xml_attr::right, 0, 0, cur_font));
										rows.push_back(row);
									}
									else {
										row_t row;
										row.push_back(column_t(order.dish(j).modifier(k).name().c_str(), true, true, xml_attr::left, left_margin, 0, cur_font));
										row.push_back(column_t("+"+boost::lexical_cast<std::string>(reg_count*order.dish(j).modifier(k).count()), false, false, xml_attr::right, 0, 0, cur_font));
										rows.push_back(row);
									}
								}
							}
						}
					}
				}
				
				if (rows.size()) xml.push_back(xml_table_t(rows).xml_str());
			}
			if (header_printed) {
				std::string table_name = order.table().exist() ? order.table().name().c_str() : "";
				xml.push_back(print_kitchen_footer(order.table().group_name().c_str(), table_name, order.type_name().c_str()));
				xml.push_back("<cut/>");
			}
		}
	}
	return std::accumulate(xml.begin(), xml.end(), std::string(""));
}

std::string print_kitchen_order_for_modifier(const ksi_cafe::format_date_time_t& format, const ksi_cafe::users_t& users, const ksi_kitchen::container_kitchen_t& container, const ksi_kitchen::order_t& order, bool print_title, bool print_barcode) {
	std::vector<std::string> xml;
	for (int index=0; index<container.count(); ++index) {
		if (container.kitchen(index).enable()) {
			bool header_printed = false;
			std::vector<int> kitchen_types = container.kitchen_types();
			for (size_t i=0; i<kitchen_types.size(); ++i) {
				bool header_kitchen_type_printed = false;
				std::vector<row_t> rows;
				for (int j=0; j<order.dish_count(); ++j) {
					if (container.kitchen(index).kitchen_execute_for_modifier(order.table().table_group_id(), order.dish(j), kitchen_types[i])) {
						int reg_count = 0;
						bool dish_footer_printed = false;
						for (int k=0; k<order.dish(j).mod_count(); ++k) {
							if (order.dish(j).modifier(k).mod_dish_id() != 0) {
								if (order.dish(j).registered() && container.kitchen(index).kitchen_execute(order.table().table_group_id(), order.dish(j).modifier(k).kitchen_type_id()) && kitchen_types[i] == order.dish(j).modifier(k).kitchen_type_id()) {
									int new_reg_count = order.dish(j).count()*order.dish(j).modifier(k).count()+order.dish(j).modifier(k).count_wasted()-order.dish(j).modifier(k).count_registered();
									new_reg_count /= order.dish(j).modifier(k).count();
									if (abs(new_reg_count) > abs(reg_count)) reg_count = new_reg_count;
									if (!header_printed) {
										/*
										if (print_barcode) {
											xml.push_back("<barcode>");
                      String tmp = order.bar_code_order_for_modifier(container, container.kitchen(index).kitchen_id());
											xml.push_back(order.bar_code_order_for_modifier(container, container.kitchen(index).kitchen_id()).c_str());
											xml.push_back("</barcode>");
											xml.push_back("<lf/>");								
										}
										*/
										std::string token_name = order.token().exist() ? order.token().name().c_str() : "";
										std::string employee_fio = "";
										if (order.type_employee_meal())
											employee_fio = order.employee_id() ? users.user(order.employee_id()).fio().c_str() : order.memo().c_str();
										std::string delivery_pickup_time = "";
										if (order.type_delivery()) delivery_pickup_time = std::string("Delivery time ") + format.date_time(order.delivery_time());
										else if (order.type_pickup()) delivery_pickup_time = std::string("Pickup time ") + format.date_time(order.delivery_time());
										std::string table_name = order.table().exist() ? order.table().name().c_str() : "";
										if (print_title)
											xml.push_back(print_kitchen_header(order.order_id(), order.table().group_name().c_str(), table_name, container.kitchen(index).name().c_str(), 
											users.user(order.creator_id()).fio().c_str(), format.date(order.get_first_not_register_time()), format.time(order.get_first_not_register_time()), token_name, employee_fio, delivery_pickup_time));
										else 
											xml.push_back(print_kitchen_header(order.order_id(), order.table().group_name().c_str(), table_name, "", 
											users.user(order.creator_id()).fio().c_str(), format.date(order.get_first_not_register_time()), format.time(order.get_first_not_register_time()), token_name, employee_fio, delivery_pickup_time));
										row_t row;
										{
											std::string rus_str = "–ë–ª—é–¥–æ";
											std::string eng_str = "Entree";
											std::string str = "language is not set";
											if (lang == 1) str = eng_str;
											else if (lang == 2) str = rus_str;
											row.push_back(column_t(str, true, true, xml_attr::left, 0, 0, xml_attr::normal));
										}
										{
											std::string rus_str = "–ö–æ–ª-–≤–æ";
											std::string eng_str = "Qty";
											std::string str = "language is not set";
											if (lang == 1) str = eng_str;
											else if (lang == 2) str = rus_str;
											row.push_back(column_t(str, false, false, xml_attr::right, 0, 0, xml_attr::normal));
										}
										rows.push_back(row);
										header_printed = true;
									}
									if (!header_kitchen_type_printed) {
										{
											row_t row;
											row.push_back(column_t("", true, true, xml_attr::left, 0, 0, xml_attr::normal, 100));
											row.push_back(column_t("", false, false, xml_attr::left, 0, 0, xml_attr::normal, 100));
											rows.push_back(row);
										}
										row_t row;
										row.push_back(column_t(container.type_name(kitchen_types[i]).c_str(), true, true, xml_attr::left, 0, 0, xml_attr::normal));
										row.push_back(column_t("", false, false, xml_attr::left, 0, 0, xml_attr::normal));
										rows.push_back(row);
										header_kitchen_type_printed = true;
									}
									if (!dish_footer_printed) {
										std::string pref = "";
										if (abs(reg_count) != 1) pref = boost::lexical_cast<std::string>(abs(reg_count)) + "x ";
										std::string str = std::string("modifier(s) for ") + pref + ksi_cafe::full_trim(order.dish(j).name()).c_str() + ": ";
										if (lang == 2) str = std::string("–º–æ–¥–∏—Ñ–∏–∫–∞—Ç–æ—Ä(—ã) –¥–ª—è ") + pref + ksi_cafe::full_trim(order.dish(j).name()).c_str() + ": ";
										row_t row;
										row.push_back(column_t(str, true, true, xml_attr::left, 0, 0, xml_attr::normal));
										row.push_back(column_t("", false, false, xml_attr::left, 0, 0, xml_attr::normal));
										rows.push_back(row);
										dish_footer_printed = true;
									}
									xml_attr::font_t cur_font = xml_attr::hx2;
									if (lang == 2) cur_font = xml_attr::normal;									
									int left_margin = 20;
									
									if (reg_count < 0) {
										row_t row;
										std::string str = "[CANCEL] ";
										if (lang == 2) str = "[–û–¢–ú–ï–ù–ê] ";
										row.push_back(column_t(str + order.dish(j).modifier(k).name().c_str(), true, true, xml_attr::left, left_margin, 0, cur_font));
										row.push_back(column_t(boost::lexical_cast<std::string>(-1*reg_count*order.dish(j).modifier(k).count()), false, false, xml_attr::right, 0, 0, cur_font));
										rows.push_back(row);
									}
									else {
										row_t row;
										row.push_back(column_t(order.dish(j).modifier(k).name().c_str(), true, true, xml_attr::left, left_margin, 0, cur_font));
										row.push_back(column_t("+"+boost::lexical_cast<std::string>(reg_count*order.dish(j).modifier(k).count()), false, false, xml_attr::right, 0, 0, cur_font));
										rows.push_back(row);
									}
								}
							}
						}
					}
				}
				if (rows.size()) xml.push_back(xml_table_t(rows).xml_str());
			}
			if (header_printed) {
				std::string table_name = order.table().exist() ? order.table().name().c_str() : "";
				xml.push_back(print_kitchen_footer(order.table().group_name().c_str(), table_name, order.type_name().c_str()));
				xml.push_back("<cut/>");
			}
		}
	}
	
	return std::accumulate(xml.begin(), xml.end(), std::string(""));

}

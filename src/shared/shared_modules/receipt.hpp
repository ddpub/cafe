#ifndef DMITRY_DERGACHEV_29_08_2008_RECEIPT_H
#define DMITRY_DERGACHEV_29_08_2008_RECEIPT_H

#include <wx_util/wx_util.hpp>
#include <lang/xml_lang.h>
#include <string>
#include <printer/xml_check_printer.hpp>
#include <printer/printer.hpp>
#include <ksi_order/cafe_container.hpp>
#include <ksi_order/order_interface.hpp>
#include <fiscal_register/fiscal.hpp>

//std::string xml_refund_header(const std::string& cafe, const std::string& ref, const std::string& title);
std::string print_receipt_duplicate(const ksi_cafe::cafe_t& cafe_, int dub, int order_id, const std::string& receipt_type, bool by_group, bool heartland);
void print_refund(const ksi_cafe::cafe_t& cafe_, int order_id, const std::string& receipt_type, fiscal::registrator_t& registrar);

std::string print_order(const ksi_cafe::order_t& order, const std::string& receipt_type, bool by_group, bool heartland);
std::string print_order(const ksi_cafe::order_t& order, bool by_group, bool heartland);
void print_order(const ksi_cafe::order_t& order, fiscal::registrator_t& registrar, double cash);

std::string print_csleep_from_base(const ksi_cafe::cafe_t& cafe_, int order_id);
std::string print_msleep(const ksi_cafe::cafe_t& cafe_, int order_id, double amount, const std::string& cd_type, const std::string& cd_num, const std::string& name, int auth_txn_id, bool swiped);
std::string print_csleep(const ksi_cafe::cafe_t& cafe_, const std::string& cd_num, const std::string& message, int txn_id, bool swiped); // failed
std::string print_csleep(const ksi_cafe::cafe_t& cafe_, const std::string& cd_num, int txn_id, int auth_txn_id); // cancel sleep
std::string print_csleep(const ksi_cafe::cafe_t& cafe_, double amount, double tip, const std::string& cd_type, const std::string& cd_num, const std::string& name, int auth_txn_id, bool swiped);
std::string print_csleep(const ksi_cafe::cafe_t& cafe_, double amount, const std::string& cd_type, const std::string& cd_num, const std::string& name, int auth_txn_id, bool swiped);

#endif

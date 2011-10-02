#ifndef DMITRY_DERGACHEV_15_06_2009_RECEIPT_H
#define DMITRY_DERGACHEV_15_06_2009_RECEIPT_H

#include "kitchen_util.hpp"
#include <ksi_util/users.hpp>
#include <ksi_util/date_time.hpp>

std::string print_kitchen_order(const ksi_cafe::format_date_time_t&, const ksi_cafe::users_t&, const ksi_kitchen::container_kitchen_t&, const ksi_kitchen::order_t&, bool print_title, bool print_barcode);

std::string print_kitchen_order_for_modifier(const ksi_cafe::format_date_time_t&, const ksi_cafe::users_t&, const ksi_kitchen::container_kitchen_t&, const ksi_kitchen::order_t&, bool print_title, bool print_barcode);


#endif

#ifndef LOADER_H_DERGACHEV_14_10_2008
#define LOADER_H_DERGACHEV_14_10_2008

#include <ksi_order/order_interface.hpp>


void load_order1(ksi_cafe::order_t** ord, const ksi_cafe::cafe_t& cafe_, int order_id_);
void load_order2(ksi_cafe::order_t** ord, const ksi_cafe::cafe_t& cafe_, int order_id_, bool for_update, bool nowait);
void load_order3(ksi_cafe::order_t** ord, const ksi_cafe::cafe_t& cafe_, const ksi_cafe::token_t& token_, int order_id_, bool for_update, bool nowait);
void load_order4(ksi_cafe::order_t** ord, const ksi_cafe::cafe_t& cafe_, const ksi_cafe::table_t& table_, int order_id_, bool for_update, bool nowait);
void load_order5(ksi_cafe::order_t** ord, const ksi_cafe::cafe_t& cafe_, const ksi_cafe::table_t& table_, const ksi_cafe::token_t& token_, int order_id_, bool for_update, bool nowait);

#endif

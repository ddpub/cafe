#ifndef DMITRY_DERGACHEV_ORDER_SHOW_H_22_05_2008
#define DMITRY_DERGACHEV_ORDER_SHOW_H_22_05_2008

#include <wx_util/treelistctrl/treelistctrl.h>
#include <lang/xml_lang.h>
#include <ksi_order/order_interface.hpp>
#include <string>
#include <ksi_util/log.hpp>

void sg_clear(wxTreeListCtrl* sg);
void sg_order_show(wxTreeListCtrl* sg, ksi_cafe::order_t& order, const std::string& type_check_sum, const std::string& check_sum);
wxString sg_get_text(wxTreeItemData* data, long column);

class wxWindow;
ksi_cafe::order_t* reloadOrder(wxWindow* wnd, int order_id, file_log_t& flog, const ksi_cafe::cafe_container_t& cafe_container);


#endif

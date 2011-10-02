
#include "order_show.hpp"
#include <wx_util/wx_util.hpp>
#include <ksi_include/wx_tree_node.hpp>
#include <vector>
#include <map>
#include <boost/lexical_cast.hpp>
#include <cmath>
#include <ksi_order/coupon.hpp>
//#include <ksi_util/currency.hpp>
#include <boost/bind.hpp>


extern std::auto_ptr<ksi_cafe::coupon_in_cafe_t> coupon_in_cafe;
extern std::auto_ptr<ksi_cafe::xml_lang_t> locale;

void sg_clear(wxTreeListCtrl* sg) {
	sg->DeleteChildren(sg->GetRootItem());
}

void sg_order_show(wxTreeListCtrl* sg, ksi_cafe::order_t& order, const std::string& type_check_sum, const std::string& check_sum)
{
	sg->DeleteChildren(sg->GetRootItem());
	std::map<int, int> coupon_count_map;
	std::vector<int> fix_discount_vec;
	
	for (int i=0; i<order.dish_count(); ++i) {
		if (order.dish(i).total_count() == 0) continue;
		if (order.dish(i).dish_id() == 0 && order.dish(i).coupon_id() != 0) {
			fix_discount_vec.push_back(i);
			continue;
		}
		
		wxTreeItemId curr = sg->AppendItem(sg->GetRootItem(), _(""), -1, -1, new order_node_t(node_type::order_dish, &order.dish(i)));
		for (size_t j=0; j<order.dish(i).modifier_vec().size(); ++j) {
			wxTreeItemId child = sg->AppendItem(curr, _(""), -1, -1, new order_node_t(node_type::order_modifier, &order.dish(i).modifier_vec()[j]));
			sg->SetItemFont(child, wxFont( 8, 74, 90, 90, false, wxT("Arial Narrow")));
			sg->SetItemTextColour(child, wxColour(172, 168, 153));
			if (order.dish(i).modifier_vec()[j].coupon_id() != 0) {
				int coupon_id = order.dish(i).modifier_vec()[j].coupon_id();
				ksi_cafe::coupon_t& cur_coupon = ksi_cafe::find_coupon(coupon_id, coupon_in_cafe->coupon());
				std::string coupon_title = cur_coupon.title();
				double discount_amount = order.dish(i).modifier_vec()[j].price() - order.dish(i).modifier_vec()[j].def_price();
				int modifier_count = order.dish(i).modifier_vec()[j].count();
				int dish_count = order.dish(i).total_count();
				wxTreeItemId discount_child = sg->AppendItem(curr, _(""), -1, -1, new order_node_t(node_type::modifier_discount, &order.dish(i).modifier_vec()[j], discount_amount, coupon_title, modifier_count, dish_count));
				sg->SetItemFont(discount_child, wxFont( 8, 74, 90, 90, false, wxT("Arial Narrow")));
				sg->SetItemTextColour(discount_child, wxColour( 0, 0, 255));
			}
		}
		if (order.dish(i).description() != "") {
			wxTreeItemId child = sg->AppendItem(curr, _(""), -1, -1, new order_node_t(node_type::dish_comment, 0, 0, order.dish(i).description()));
			sg->SetItemFont(child, wxFont( 8, 74, 90, 90, false, wxT("Arial Narrow")));
			sg->SetItemTextColour(child, wxColour(133, 187, 88));
		}
		double discount = order.dish(i).price() - order.dish(i).def_price();
		if (order.dish(i).dish_id() != 0 && order.dish(i).coupon_id() != 0 && discount < 0) {
			int dish_count = order.dish(i).total_count();
			int coupon_id = order.dish(i).coupon_id();
			ksi_cafe::coupon_t& cur_coupon = ksi_cafe::find_coupon(coupon_id, coupon_in_cafe->coupon());
			int coupon_count = cur_coupon.auto_scalable() ? 0 : cur_coupon.apply();
			
			if (!cur_coupon.auto_scalable()) {
				if (coupon_count_map.find(coupon_id) == coupon_count_map.end()) {
					coupon_count_map[coupon_id] = coupon_count;
				}
				else {
					coupon_count = coupon_count_map[coupon_id];
				}
				if (dish_count <= coupon_count) {
					coupon_count_map[coupon_id] -= dish_count;
					coupon_count = dish_count;
				}
				else {
					coupon_count_map[coupon_id] -= coupon_count;
				}
			}
			wxTreeItemId coupon_child = sg->AppendItem(curr, _(""), -1, -1, new order_node_t(node_type::discount, &order.dish(i), coupon_count == 0 ? discount*dish_count : discount, cur_coupon.title(), coupon_count));
			sg->SetItemFont(coupon_child, wxFont( 8, 74, 90, 90, false, wxT("Arial Narrow")));
			sg->SetItemTextColour(coupon_child, wxColour(0, 0, 255));

		}
		sg->ExpandAll(curr);
		if (!sg->GetSelection().IsOk() && type_check_sum == order.dish(i).type_check_sum())
			sg->SelectItem(curr);
		if (check_sum == order.dish(i).check_sum())
			sg->SelectItem(curr);
	}

	for (size_t i=0; i<fix_discount_vec.size(); ++i) {
		int index = fix_discount_vec[i];
		wxTreeItemId child = sg->AppendItem(sg->GetRootItem(), _(""), -1, -1, new order_node_t(node_type::discount, 0, order.dish(index).price(), order.dish(index).name(), 1));
		sg->SetItemFont(child, wxFont( 8, 74, 90, 90, false, wxT("Arial Narrow")));
		sg->SetItemTextColour(child, wxColour(0, 0, 255));
	}

	if (!sg->GetSelection().IsOk()) {
		wxTreeItemIdValue cookie;
		wxTreeItemId id = sg->GetFirstChild(sg->GetRootItem(), cookie);
		if (id.IsOk()) sg->SelectItem(id);
		else sg->SelectItem(sg->GetRootItem());
	}
	sg->Refresh();
}

wxString sg_get_text(wxTreeItemData* data, long column) {
	order_node_t* node = dynamic_cast<order_node_t*>(data);
	if (!node) return _("");

	if (node->type() == node_type::order_dish) {
		const ksi_cafe::order_dish_t* dish = reinterpret_cast<const ksi_cafe::order_dish_t*>(node->col1());
		if (column == 0) {
			return to_uc(dish->name());
		}
		else if (column == 1) {
			if (dish->def_price() == 0) return _("-");
			else return locale->get_currency(dish->def_price());
		}
		else if (column == 2) {
			return to_uc(boost::lexical_cast<std::string>(dish->total_count()));
		}
		else if (column == 3) {
			return locale->get_currency(dish->def_amount());
		}
	}
	else if (node->type() == node_type::order_modifier) {
		const ksi_cafe::order_dish_modifier_t* modifier = reinterpret_cast<const ksi_cafe::order_dish_modifier_t*>(node->col1());
		if (column == 0) {
			if (modifier->count() == 1) return to_uc(modifier->name());
			else return to_uc(boost::lexical_cast<std::string>(modifier->count()))+_("x")+to_uc(modifier->name());
		}
		else if (column == 1) {
			if (modifier->def_price() == 0) return _("-");
			else {
				if (modifier->count() == 1) return locale->get_currency(modifier->def_price());
				else return to_uc(boost::lexical_cast<std::string>(modifier->count()))+_("x")+locale->get_currency(modifier->def_price());
			}
		}
		else if (column == 2) {
			return _("");
		}
		else if (column == 3) {
			return _("");
		}
	}
	else if (node->type() == node_type::discount) {
		if (column == 0) {
			return to_uc(node->col3());
		}
		else if (column == 1) {
			if (node->col4() != 0) return _("-")+locale->get_currency(fabs(node->col2()));
			else return _("");
		}
		else if (column == 2) {
			if (node->col4() != 0) return to_uc(boost::lexical_cast<std::string>(node->col4()));
			else return _("");
		}
		else if (column == 3) {
			return _("-") + locale->get_currency(node->col4() == 0 ? fabs(node->col2()) : fabs(node->col2()*node->col4()));
		}
	}
	else if (node->type() == node_type::modifier_discount) {
		if (column == 0) {
			return _(" ") + to_uc(node->col3());
		}
		else if (column == 1) {
			return _("-") + locale->get_currency(fabs(node->col2()));
		}
		else if (column == 2) {
			return to_uc(boost::lexical_cast<std::string>(node->col4()*node->col5()));
		}
		else if (column == 3) {
			return _("-") + locale->get_currency(fabs(node->col2())*node->col4()*node->col5());
		}
	}
	else if (node->type() == node_type::dish_comment) {
		if (column == 0) {
			return to_uc(node->col3());
		}
		else if(column == 1) {
			return _("");
		}
		else if (column == 2) {
			return _("");
		}
		else if (column == 3) {
			return _("");
		}
	}
	return _("exception");
}


#include <shared_modules/load_f.h>
#include <ksi_util/loader.hpp>

ksi_cafe::order_t* reloadOrder(wxWindow* wnd, int order_id, file_log_t& flog, const ksi_cafe::cafe_container_t& cafe_container) {
	bool catch_exception = false;
	ksi_cafe::order_t* ord_ptr = 0;
	int res = 0;
	try {
		flog << "load order_t, order_id = \"" + boost::lexical_cast<std::string>(order_id) + "\"";
		wx_window_ptr<load_f> form (new load_f(wnd, locale->get("global", "order_load_str", "Order is loading"), locale->get("global", "please_wait_str", "Please wait.\nIf order is not opened for a long time, it might have been already opened by somebody else.")));
		form->start(boost::bind(load_order2, &ord_ptr, *cafe_container.current_cafe(), order_id, true, false));
		res = form->ShowModal();
	}
	catch (std::exception& ex) {
		flog << LLOCATION;
		flog << ex.what();
		wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, wnd);
		if (ord_ptr) {
			flog << "delete ksi_cafe::order_t";
			try {
				delete ord_ptr;
					flog << "sucessful";
			}
			catch (std::exception& ex) {
				flog << ex.what();
				wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, wnd);
			}
			ord_ptr = 0;
			catch_exception = true;
		}
	}
	if (catch_exception) return 0;
	if (res == wxID_OK) {
		if (ord_ptr) {
			flog << "loaded successful, ksi_cafe::order_t::status_id = " + boost::lexical_cast<std::string>(ord_ptr->status_id());
			if (ord_ptr->status_accepted() || ord_ptr->status_ready() || ord_ptr->status_send_to_kitchen()) {
			}
			else {
				flog << "delete ksi_cafe::order_t";
				try {
					delete ord_ptr;
					flog << "successful";
				}
				catch (std::exception& ex) {
					flog << ex.what();
					wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, wnd);
				}
				ord_ptr = 0;
			}
		}
	}
	else {
		flog << "delete ksi_cafe::order_t";
		try {
			delete ord_ptr;
			flog << "successful";
		}
		catch (std::exception& ex) {
			flog << ex.what();
			wxMessageBox(to_uc(ex.what()), _("Message"), wxOK, wnd);
		}
		ord_ptr = 0;
	}
	return ord_ptr;
}

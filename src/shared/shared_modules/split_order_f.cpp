#include "split_order_f.h"

#include <ksi_include/custom_treelistctrl.hpp>
#include <wx_util/order_show.hpp>
#include <ksi_order/order_interface.hpp>
#include <ksi_include/wx_tree_node.hpp>
#include <string>
#include <boost/lexical_cast.hpp>
#include <ksi_order/cafe_container.hpp>
#include <ksi_order/menu_container.hpp>
#include <wx_util/move_up.hpp>
#include <shared_modules/btn_list_f.h>
#include <ksi_order/coupon.hpp>
#include <lang/xml_lang.h>

extern std::auto_ptr<ksi_cafe::cafe_container_t> cafe_container;
extern std::auto_ptr<ksi_cafe::table_container_t> table_container;
extern std::auto_ptr<ksi_cafe::coupon_in_cafe_t> coupon_in_cafe;
extern std::auto_ptr<file_log_t> flog;
extern std::auto_ptr<ksi_cafe::xml_lang_t> shared_locale;
extern std::auto_ptr<ksi_cafe::xml_lang_t> locale;

wxString wxSplitOrderTreeListCtrl::OnGetItemText (wxTreeItemData* item_data, long column) const
{
	return sg_get_text(item_data, column);
}

split_order_f::split_order_f( wxWindow* parent, const wxString& title, ksi_cafe::order_t* order_ )
:
base_split_order_f( parent ), order_(order_)
{
	SetLabel(title);
	source_sg->AddRoot(_("root"));
	destination_sg->AddRoot(_("root"));
	
	source_sg->Connect ( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( split_order_f::source_sgSelChanged ), NULL, this );
	source_sg->Connect ( wxEVT_COMMAND_TREE_SEL_CHANGING, wxTreeEventHandler( split_order_f::source_sgSelChanging ), NULL, this );

	destination_sg->Connect ( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( split_order_f::destination_sgSelChanged ), NULL, this );
	destination_sg->Connect ( wxEVT_COMMAND_TREE_SEL_CHANGING, wxTreeEventHandler( split_order_f::destination_sgSelChanging ), NULL, this );

	source_table_btn->SetLabel(_("Table: ") + to_uc(order_->table().name()));
	source_guests_ed->SetValue(to_uc(boost::lexical_cast<std::string>(order_->guest_count())));
	
	destination_order.reset(new ksi_cafe::order_t(*cafe_container->current_cafe(), order_->table(), 0, false, false));
	destination_table_btn->SetLabel(_("Table: ") + to_uc(destination_order->table().name()));
	destination_guests_ed->SetValue(to_uc(boost::lexical_cast<std::string>(1)));
	
	try {
		coupon_in_cafe->unselect_all();
		for (int i=0; i<order_->dish_count(); ++i) {
			int coupon_id = order_->dish(i).coupon_id();
			int coupon_mng_id = order_->dish(i).coupon_mng_id();
			if (coupon_id && coupon_id != -1) {
				coupon_in_cafe->select(coupon_id, 1, coupon_mng_id);
			}
			const std::vector<ksi_cafe::order_dish_modifier_t>& mods = order_->dish(i).modifier_vec();
			for (size_t j=0; j<mods.size(); ++j) {
				int coupon_id = mods[j].coupon_id();
				int coupon_mng_id = mods[j].coupon_mng_id();
				if (coupon_id && coupon_id != -1) {
					coupon_in_cafe->select(coupon_id, 1, coupon_mng_id);
				}
			}
		}
	}
	catch (std::exception& ex) {
		*flog << ex.what();
	}
	
	wxString postfix = _(" order");
	if (order_->order_id()) postfix = _(" order # ") + to_uc(boost::lexical_cast<std::string>(order_->order_id()));
	source_size->GetStaticBox()->SetLabel(postfix);
	destincation_sizer->GetStaticBox()->SetLabel(_(""));

	
	recalc_coupon("", "");
	sg_order_show(destination_sg, *destination_order.get(), "", "");
	CentreOnParent();
}

split_order_f::~split_order_f() {
	source_sg->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( split_order_f::source_sgSelChanged ), NULL, this );
	source_sg->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGING, wxTreeEventHandler( split_order_f::source_sgSelChanging ), NULL, this );
	
	destination_sg->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( split_order_f::destination_sgSelChanged ), NULL, this );
	destination_sg->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGING, wxTreeEventHandler( split_order_f::destination_sgSelChanging ), NULL, this );
}

void split_order_f::source_sgSelChanged(wxTreeEvent& event) {
	wx_disable_t disable_window(this);
	wxTreeItemId id = event.GetItem();
	if (!event.GetItem().IsOk()) return;
	if (event.GetItem() == source_sg->GetRootItem()) return;

	order_node_t* node = dynamic_cast<order_node_t*>(source_sg->GetItemData(event.GetItem()));
	if (!node) return;
	if (node->type() == node_type::order_dish) {
		move_from_source_btn->Enable(true);
	}
}

void split_order_f::source_sgSelChanging(wxTreeEvent& event) {
	wx_disable_t disable_window(this);
	move_from_source_btn->Enable(false);
}

void split_order_f::destination_sgSelChanged(wxTreeEvent& event) {
	wx_disable_t disable_window(this);
	wxTreeItemId id = event.GetItem();
	if (!event.GetItem().IsOk()) return;
	if (event.GetItem() == destination_sg->GetRootItem()) return;
	
	order_node_t* node = dynamic_cast<order_node_t*>(source_sg->GetItemData(event.GetItem()));
	if (!node) return;
	if (node->type() == node_type::order_dish) {
		move_to_source_btn->Enable(true);
	}
}

void split_order_f::destination_sgSelChanging(wxTreeEvent& event) {
	wx_disable_t disable_window(this);
	move_to_source_btn->Enable(false);
}

void split_order_f::sourceSpinDownClick( wxSpinEvent& event )
{
	// TODO: Implement sourceSpinDownClick
	size_t currentSourceGuestCount = boost::lexical_cast<size_t>(to_mb(source_guests_ed->GetValue()));
	if (currentSourceGuestCount > 1) {
		source_guests_ed->SetValue(to_uc(boost::lexical_cast<std::string>(--currentSourceGuestCount)));
	}
}

void split_order_f::sourceSpinUpClick( wxSpinEvent& event )
{
	// TODO: Implement sourceSpinUpClick
	size_t currentSourceGuestCount = boost::lexical_cast<size_t>(to_mb(source_guests_ed->GetValue()));
	source_guests_ed->SetValue(to_uc(boost::lexical_cast<std::string>(++currentSourceGuestCount)));
}

void split_order_f::recalc_coupon(const std::string& type_check_sum, const std::string& check_sum) {
	order_->save_misc_charges();
	order_->save_adjustable_discount();
	order_->clear_coupon();
	
	std::vector<ksi_cafe::coupon_t*> fix_discount;
	std::vector<ksi_cafe::coupon_t*> selected = coupon_in_cafe->selected();
	std::vector<ksi_cafe::coupon_t*>::iterator end = selected.end();
	
	for (std::vector<ksi_cafe::coupon_t*>::iterator i=selected.begin(); i!=selected.end(); ++i) {
		int cur_size = 1;
		
		if (!(*i)->fixed_discount()) {
			while ( ((*i)->auto_scalable() == 1 || coupon_in_cafe->coupon_count(*i) >= cur_size)  && apply_coupon(**i, order_->all_dish(), coupon_in_cafe->mng_id(*i)) ) {
				++cur_size;
			}
			(*i)->set_apply(cur_size-1);
		}
		else if (apply_coupon(**i, order_->all_dish(), coupon_in_cafe->mng_id(*i)) && (*i)->fixed_discount()) {
			fix_discount.push_back(*i);
		}		
	}
	for (std::vector<ksi_cafe::coupon_t*>::iterator i=fix_discount.begin(); i!=fix_discount.end(); ++i) {
		order_->add_fixed_discount((*i)->fixed_discount(), (*i)->coupon_id(), (*i)->title(), coupon_in_cafe->mng_id(*i));
	}
	
	order_->compress();
	order_->load_misc_charges();
	order_->load_adjustable_discount();	
	
	sg_order_show(source_sg, *order_, type_check_sum, check_sum);
}

void split_order_f::move_from_source_btnClick( wxCommandEvent& event )
{
	// TODO: Implement move_from_source_btnClick
	try {
		wxTreeItemId id = source_sg->GetSelection();
		if (id.IsOk()) {
			order_node_t* node = dynamic_cast<order_node_t*>(source_sg->GetItemData(id));
			if (node && node->type() == node_type::order_dish) {
				const ksi_cafe::order_dish_t* odish = reinterpret_cast<const ksi_cafe::order_dish_t*>(node->col1());
				ksi_cafe::order_dish_t dish(*odish, 1);
				order_->remove_dish(*odish);
				destination_order->add_dish(dish, true);
				destination_order->clear_coupon();
				recalc_coupon(dish.type_check_sum(), dish.check_sum());
				sg_order_show(destination_sg, *destination_order.get(), dish.type_check_sum(), dish.check_sum());
			}
		}
	}
	catch (exception_t& ex) {
		*flog << ex.what();
		wxMessageBox(to_uc(ex.user_message()));
	}
	catch (std::exception& ex) {
		*flog << ex.what();
		wxMessageBox(to_uc(ex.what()));
	}
	catch (...) {
		*flog << ERROR_IN_LOCATION;
		wxMessageBox(to_uc(ERROR_IN_LOCATION));
	}
	ok_btn->Enable(destination_order->real_dish_count());
}

void split_order_f::move_to_source_btnClick( wxCommandEvent& event )
{
	// TODO: Implement move_to_source_btnClick
	try {
		wxTreeItemId id = destination_sg->GetSelection();
		if (id.IsOk()) {
			order_node_t* node = dynamic_cast<order_node_t*>(destination_sg->GetItemData(id));
			if (node && node->type() == node_type::order_dish) {
				const ksi_cafe::order_dish_t* odish = reinterpret_cast<const ksi_cafe::order_dish_t*>(node->col1());
				ksi_cafe::order_dish_t dish(*odish, 1);
				destination_order->remove_dish(*odish);
				order_->add_dish(dish, true);
				destination_order->clear_coupon();
				recalc_coupon(dish.type_check_sum(), dish.check_sum());
				sg_order_show(destination_sg, *destination_order.get(), dish.type_check_sum(), dish.check_sum());
			}
		}
	}
	catch (exception_t& ex) {
		*flog << ex.what();
		wxMessageBox(to_uc(ex.user_message()));
	}
	catch (std::exception& ex) {
		*flog << ex.what();
		wxMessageBox(to_uc(ex.what()));
	}
	catch (...) {
		*flog << ERROR_IN_LOCATION;
		wxMessageBox(to_uc(ERROR_IN_LOCATION));
	}
	ok_btn->Enable(destination_order->real_dish_count());
}

void split_order_f::destinationSpinDownClick( wxSpinEvent& event )
{
	// TODO: Implement destinationSpinDownClick
	size_t currentDestinationGuestCount = boost::lexical_cast<size_t>(to_mb(destination_guests_ed->GetValue()));
	if (currentDestinationGuestCount > 1) {
		destination_guests_ed->SetValue(to_uc(boost::lexical_cast<std::string>(--currentDestinationGuestCount)));
	}
}

void split_order_f::destionationSpinUpClick( wxSpinEvent& event )
{
	// TODO: Implement destionationSpinUpClick
	size_t currentDestinationGuestCount = boost::lexical_cast<size_t>(to_mb(destination_guests_ed->GetValue()));
	destination_guests_ed->SetValue(to_uc(boost::lexical_cast<std::string>(++currentDestinationGuestCount)));
}

void split_order_f::destination_table_btnClick( wxCommandEvent& event )
{
	// TODO: Implement destination_table_btnClick
	std::auto_ptr<table_show_t> table_show (new table_show_t(std::vector<wxButton*>(), table_container.get(), content::REGULAR, true));
	wx_window_ptr<btn_list_f> form (new btn_list_f(this, _("Select a table"), _(""), table_show.get()));
	int result = form->ShowModal() - wxID_HIGHEST;
	if (result >= 0) {
		wxButton* table_btn = form->getTableBtn();
		ksi_cafe::table_t table = table_show->get_table(table_btn);
		destination_order->change_table(table);
		destination_table_btn->SetLabel(_("Table: ") + to_uc(destination_order->table().name()));
	}
}

void split_order_f::ok_btnClick( wxCommandEvent& event )
{
	// TODO: Implement ok_btnClick
	try {
		order_->set_guest_count(boost::lexical_cast<int>(to_mb(source_guests_ed->GetValue())));
		destination_order->set_guest_count(boost::lexical_cast<int>(to_mb(destination_guests_ed->GetValue())));
		*flog << "destination_order::set_accept";
		destination_order->set_accept(0, true);
		*flog << "successful";
		*flog << "souce_order::set_accept";
		order_->set_accept(0, true);
		*flog << "successful";
		order_->splitByOrder(destination_order->order_id());
		*flog << "order commit. order_id = " + boost::lexical_cast<std::string>(order_->order_id());
		if (!order_->commit(true)) *flog << "!!! force apply";
	}
	catch (exception_t& ex) {
		*flog << ex.comp_message();
		wxMessageBox(to_uc(ex.user_message()));
	}
	catch (ksi_client::connection_problem_exception& ex) {
		*flog << ex.what();
		wxMessageBox(to_uc(ex.what()));
	}
	catch (ksi_client::db_exception& ex) {
		*flog << ex.what();
		wxMessageBox(shared_locale->get("db_exception", ex.userFriendly().first, ex.userFriendly().second));
	}	
	catch (std::exception& ex) {
		*flog << LLOCATION;
		*flog << ex.what();
		wxMessageBox(to_uc(ex.what()));
	}
	catch (...) {
		*flog << ERROR_IN_LOCATION;
		wxMessageBox(to_uc(ERROR_IN_LOCATION));
	}
	EndModal(wxID_OK);
}

void split_order_f::cancel_btnClick( wxCommandEvent& event )
{
	// TODO: Implement cancel_btnClick
	EndModal(wxID_CANCEL);
}

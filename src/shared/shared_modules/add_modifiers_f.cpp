#include "add_modifiers_f.h"
#include <wx_util/wx_util.hpp>
#include <ksi_include/algo_functor.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <wx/msgdlg.h>
#include <ksi_util/util.hpp>
#include <algorithm>
#include <ksi_include/wx_tree_node.hpp>
#include <ksi_include/custom_treelistctrl.hpp>
#include <lang/xml_lang.h>


int lable_heigh_ = 20;
int btn_width = 70;
int btn_height = 70;
int btn_space = 0;

extern std::auto_ptr<ksi_cafe::xml_lang_t> locale;

wxString wxModifierTreeListCtrl::OnGetItemText (wxTreeItemData* item_data, long column) const
{
	try {
		mod_node_t* node = dynamic_cast<mod_node_t*>(item_data);
		if (!node) THROW_LOCATION(0, "wxString wxModifierTeeListCtrl::OnGetItemText (wxTreeItemData* item, long column) const");
		if (column == 0) {
			if (node->type() == node_type::menu_modifier)
				return to_uc(reinterpret_cast<const ksi_cafe::menu_dish_modifier_t*>(node->col1())->name());
			else if (node->type() == node_type::modifier_group)
				return to_uc(reinterpret_cast<const ksi_cafe::modifier_group_t*>(node->col1())->name());
			THROW_LOCATION(1, "wxString wxModifierTeeListCtrl::OnGetItemText (wxTreeItemData* item, long column) const");
		}
		else if (column == 1) {
			if (node->type() == node_type::menu_modifier)
				return to_uc(boost::lexical_cast<std::string>(node->col2()));
			else if (node->type() == node_type::modifier_group)
				return _("");
			THROW_LOCATION(2, "wxString wxModifierTeeListCtrl::OnGetItemText (wxTreeItemData* item, long column) const");
		}
	}
	catch (exception_t& ex) {
		wxMessageBox(to_uc(ex.comp_message()));
	}
	return _("");
}


add_modifiers_f::add_modifiers_f( wxWindow* parent, const wxString& cap, ksi_cafe::simple_stop_list_t* stop_list_)
:
base_add_modifiers_f( parent ), stop_list_(stop_list_), curr_page_(0)
{
	SetLabel(cap);
	ok_btn->SetLabel(locale->get("add_modifiers_f", "ok_btn", "Ok"));
	cancel_btn->SetLabel(locale->get("add_modifiers_f", "cancel_btn", "Cancel"));
	sg->GetColumn(0).SetText(locale->get("add_modifiers_f", "sg_group_modifier", "Group/Modifier"));
	sg->GetColumn(1).SetText(locale->get("add_modifiers_f", "sg_qty", "Qty"));
	plus_btn->SetBitmapLabel(CashierBitmapsFunc(2));
	plus_btn->SetBitmapDisabled(CashierBitmapsFunc(3));
	minus_btn->SetBitmapLabel(CashierBitmapsFunc(4));
	minus_btn->SetBitmapDisabled(CashierBitmapsFunc(5));
	delete_btn->SetBitmapLabel(CashierBitmapsFunc(6));
	delete_btn->SetBitmapDisabled(CashierBitmapsFunc(7));
	prev_page_btn->SetLabel(locale->get("global", "prev_str", "<<\nprevious page"));
	next_page_btn->SetLabel(locale->get("global", "next_str", ">>\nnext page"));
  
	wxStaticText* tmp = new wxStaticText(btn_pnl, wxID_ANY, _("()ABCDEFJHIGKLMNOPQRSTUVWXYZ"), wxPoint(0, 0));
	lable_heigh_ = tmp->GetSize().y;
	tmp->Destroy();
	sg->AddRoot(_("root"));
	
	// Connect Events
	sg->Connect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( add_modifiers_f::sel_changed ), NULL, this );
	sg->Connect( wxEVT_COMMAND_TREE_SEL_CHANGING, wxTreeEventHandler( add_modifiers_f::sel_changing ), NULL, this );
	
	CentreOnParent();
}

add_modifiers_f::~add_modifiers_f()
{
	// Disconnect Events
	sg->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( add_modifiers_f::sel_changed ) );
	sg->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGING, wxTreeEventHandler( add_modifiers_f::sel_changing ) );
}

void add_modifiers_f::plus_btnClick( wxCommandEvent& event )
{
	// TODO: Implement plus_btnClick
	try {
		wxTreeItemId curr = sg->GetSelection();
		if (!curr.IsOk()) THROW_LOCATION(0, "void add_modifiers_f::plus_btnClick( wxCommandEvent& event )");
		wxTreeItemData* data = sg->GetItemData(curr);
		mod_node_t* node = dynamic_cast<mod_node_t*>(data);
		if (!node) THROW_LOCATION(1, "void add_modifiers_f::plus_btnClick( wxCommandEvent& event )");
		if (node->type() != node_type::menu_modifier) THROW_LOCATION(2, "void add_modifiers_f::plus_btnClick( wxCommandEvent& event )");
		sg->SetItemData(curr, new mod_node_t(node_type::menu_modifier, node->col1(), node->col2()+1));
		sg->Refresh();
		sg->SelectItem(curr);
		hide_btn_group();
	}
	catch (exception_t& ex) {
		wxMessageBox(to_uc(ex.comp_message()));
	}
}

void add_modifiers_f::minus_btnClick( wxCommandEvent& event )
{
	// TODO: Implement minus_btnClick
	try {
		wxTreeItemId curr = sg->GetSelection();
		if (!curr.IsOk()) THROW_LOCATION(0, "void add_modifiers_f::minus_btnClick( wxCommandEvent& event )");
		wxTreeItemData* data = sg->GetItemData(curr);
		mod_node_t* node = dynamic_cast<mod_node_t*>(data);
		if (!node) THROW_LOCATION(1, "void add_modifiers_f::minus_btnClick( wxCommandEvent& event )");
		if (node->type() != node_type::menu_modifier) THROW_LOCATION(2, "void add_modifiers_f::minus_btnClick( wxCommandEvent& event )");
		if (node->col2() > 1) {
			sg->SetItemData(curr, new mod_node_t(node_type::menu_modifier, node->col1(), node->col2()-1));
			sg->SelectItem(curr);
		}
		else {
			if (sg->GetChildrenCount(sg->GetItemParent(curr)) == 1)
				sg->Delete(sg->GetItemParent(curr));
			else
				sg->Delete(curr);
			wxTreeItemIdValue cookie;
			if (sg->GetFirstChild(sg->GetRootItem(), cookie).IsOk()) {
				sg->SelectItem(sg->GetFirstChild(sg->GetRootItem(), cookie));
			}
			else {
				plus_btn->Disable();
				minus_btn->Disable();
				delete_btn->Disable();
			}
		}
		sg->Refresh();
		hide_btn_group();
	}
	catch (exception_t& ex) {
		wxMessageBox(to_uc(ex.comp_message()));
	}
}

void add_modifiers_f::delete_btnClick( wxCommandEvent& event )
{
	// TODO: Implement delete_btnClick
	try {
		wxTreeItemId curr = sg->GetSelection();
		if (!curr.IsOk()) THROW_LOCATION(0, "void add_modifiers_f::delete_btnClick( wxCommandEvent& event )");
		wxTreeItemData* data = sg->GetItemData(curr);
		mod_node_t* node = dynamic_cast<mod_node_t*>(data);
		if (!node) THROW_LOCATION(1, "void add_modifiers_f::delete_btnClick( wxCommandEvent& event )");
		if (node->type() == node_type::menu_modifier) {
			if (sg->GetChildrenCount(sg->GetItemParent(curr)) == 1)
				sg->Delete(sg->GetItemParent(curr));
			else
				sg->Delete(curr);
		}
		else if (node->type() == node_type::modifier_group) {
			sg->Delete(curr);
		}
		else THROW_LOCATION(2, "void add_modifiers_f::delete_btnClick( wxCommandEvent& event )");
		sg->Refresh();
		wxTreeItemIdValue cookie;
		if (sg->GetFirstChild(sg->GetRootItem(), cookie).IsOk()) {
			sg->SelectItem(sg->GetFirstChild(sg->GetRootItem(), cookie));
		}
		else {
			plus_btn->Disable();
			minus_btn->Disable();
			delete_btn->Disable();
		}
		hide_btn_group();
	}
	catch (exception_t& ex) {
		wxMessageBox(to_uc(ex.comp_message()));
	}
}

void add_modifiers_f::prev_page_btnClick( wxCommandEvent& event )
{
	// TODO: Implement prev_page_btnClick
	curr_page_--;
	show();
}

void add_modifiers_f::next_page_btnClick( wxCommandEvent& event )
{
	// TODO: Implement next_page_btnClick
	curr_page_++;
	show();
}

void add_modifiers_f::modifier_btnClick( wxCommandEvent& event )
{
	wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
	if (btn == 0) THROW_LOCATION(0, "void add_modifiers_f::modifier_btnClick( wxCommandEvent& event )");
	const ksi_cafe::menu_dish_modifier_t* modifier = kv_modifier_[btn];

	add_modifier(modifier);
	hide_btn_group();
}

void add_modifiers_f::sel_changed( wxTreeEvent& event )
{
	plus_btn->Disable();
	minus_btn->Disable();
	delete_btn->Disable();

	wxTreeItemId item = event.GetItem();
	if (!item.IsOk()) return;
	wxTreeItemData* data = sg->GetItemData(item);
	mod_node_t* node = dynamic_cast<mod_node_t*>(data);
	if (!node) THROW_LOCATION(0, "void add_modifiers_f::sel_changed( wxTreeEvent& event )");

	if (node->type() == node_type::menu_modifier) {
		wxTreeItemId parent = sg->GetItemParent(item);
		if (!item.IsOk()) THROW_LOCATION(1, "void add_modifiers_f::sel_changed( wxTreeEvent& event )");
		wxTreeItemData* data = sg->GetItemData(parent);
		mod_node_t* parent_node = dynamic_cast<mod_node_t*>(data);
		if (!parent_node) THROW_LOCATION(2, "void add_modifiers_f::sel_changed( wxTreeEvent& event )");
		const ksi_cafe::modifier_group_t* group = reinterpret_cast<const ksi_cafe::modifier_group_t*>(parent_node->col1());
		if (curr_weight(group) == group->max_weight())
			plus_btn->Disable();
		else
			plus_btn->Enable();
		minus_btn->Enable();
		delete_btn->Enable();
	}
	else if (node->type() == node_type::modifier_group) {
		delete_btn->Enable();
	}
}

void add_modifiers_f::sel_changing( wxTreeEvent& event )
{
}

std::vector<ksi_cafe::order_dish_modifier_t> add_modifiers_f::sel_mods() const
{
	std::vector<ksi_cafe::order_dish_modifier_t> res_vec;
	wxTreeItemIdValue cookie;
	wxTreeItemId curr = sg->GetFirstChild(sg->GetRootItem(), cookie);
	while (curr.IsOk()) {
		{
			wxTreeItemIdValue cookie;
			wxTreeItemId child = sg->GetFirstChild(curr, cookie);
			while (child.IsOk()) {
				wxTreeItemData* data = sg->GetItemData(child);
				mod_node_t* node = dynamic_cast<mod_node_t*>(data);
				if (!node) THROW_LOCATION(0, "std::vector<ksi_cafe::order_dish_modifier_t> add_modifiers_f::sel_mods() const");
				if (node->type() != node_type::menu_modifier) THROW_LOCATION(1, "std::vector<ksi_cafe::order_dish_modifier_t> add_modifiers_f::sel_mods() const");
				const ksi_cafe::menu_dish_modifier_t* modifier = reinterpret_cast<const ksi_cafe::menu_dish_modifier_t*>(node->col1());
				res_vec.push_back(ksi_cafe::order_dish_modifier_t(*modifier, node->col2()));
				child = sg->GetNextChild(curr, cookie);
			}
		}
		curr = sg->GetNextChild(sg->GetRootItem(), cookie);
	}
	return res_vec;
}

void add_modifiers_f::add_modifier(const ksi_cafe::menu_dish_modifier_t* modifier)
{
	const ksi_cafe::modifier_group_t* modifier_group = mods_[modifier];

	try {
		wxTreeItemIdValue cookie;
		wxTreeItemId curr = sg->GetFirstChild(sg->GetRootItem(), cookie);
		while (curr.IsOk()) {
			wxTreeItemData* data = sg->GetItemData(curr);
			mod_node_t* node = dynamic_cast<mod_node_t*>(data);
			if (!node) THROW_LOCATION(0, "void add_modifiers_f::add_modifier(const ksi_cafe::menu_dish_modifier_t* modifier)");
			if (node->type() != node_type::modifier_group) THROW_LOCATION(1, "void add_modifiers_f::add_modifier(const ksi_cafe::menu_dish_modifier_t* modifier)");
			if (reinterpret_cast<const ksi_cafe::modifier_group_t*>(node->col1()) == modifier_group) break;
			curr = sg->GetNextChild(sg->GetRootItem(), cookie);
		}
		if (!curr.IsOk()) {
			curr = sg->AppendItem(sg->GetRootItem(), to_uc(modifier_group->name()), -1, -1, new mod_node_t(node_type::modifier_group, modifier_group, 1));
		}
		wxTreeItemId child = sg->GetFirstChild(curr, cookie);
		while (child.IsOk()) {
			wxTreeItemData* data = sg->GetItemData(child);
			mod_node_t* node = dynamic_cast<mod_node_t*>(data);
			if (!node) THROW_LOCATION(2, "void add_modifiers_f::add_modifier(const ksi_cafe::menu_dish_modifier_t* modifier)");
			if (node->type() != node_type::menu_modifier) THROW_LOCATION(3, "void add_modifiers_f::add_modifier(const ksi_cafe::menu_dish_modifier_t* modifier)");
			if (reinterpret_cast<const ksi_cafe::menu_dish_modifier_t*>(node->col1()) == modifier) break;
			child = sg->GetNextChild(curr, cookie);
		}
		if (!child.IsOk()) {
			child = sg->AppendItem(curr, to_uc(modifier->name()), -1, -1, new mod_node_t(node_type::menu_modifier, modifier, 1));
		}
		else {
			wxTreeItemData* data = sg->GetItemData(child);
			mod_node_t* node = dynamic_cast<mod_node_t*>(data);
			if (!node) THROW_LOCATION(3, "void add_modifiers_f::add_modifier(const ksi_cafe::menu_dish_modifier_t* modifier)");
			if (node->type() != node_type::menu_modifier) THROW_LOCATION(4, "void add_modifiers_f::add_modifier(const ksi_cafe::menu_dish_modifier_t* modifier)");
			sg->SetItemData(child, new mod_node_t(node_type::menu_modifier, node->col1(), node->col2()+1));
		}
		sg->SelectItem(child);
		sg->ExpandAll(curr);
		sg->Refresh();
	}
	catch (exception_t& ex) {
		wxMessageBox(to_uc(ex.comp_message()));
	}
}

void add_modifiers_f::ok_btnClick( wxCommandEvent& event )
{
	EndModal(wxID_OK);
}

void add_modifiers_f::cancel_btnClick( wxCommandEvent& event )
{
	EndModal(wxID_CANCEL);
}

void add_modifiers_f::show()
{
	prev_page_btn->Disable();
	change_bg_colour(prev_page_btn, btn_face_color);
	next_page_btn->Disable();
	change_bg_colour(next_page_btn, btn_face_color);
	for (size_t i=0; i<btn_vec_.size(); ++i) {
		btn_vec_[i]->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( add_modifiers_f::modifier_btnClick ), NULL, this);
		btn_vec_[i]->Destroy();
	}

	for (size_t i=0; i<label_vec_.size(); ++i) {
		label_vec_[i]->Destroy();
	}

	btn_vec_.clear(); label_vec_.clear();
	size_t begin_index = 0;
	size_t curr_page_index = 0;

	wxSize btn_size = btn_pnl->GetClientSize();
	while (curr_page_index != curr_page_) {
		int curr_height = 0;
		for (; begin_index < group_vec_.size(); ++begin_index) {
			curr_height += group_height_[begin_index];
			if (curr_height > btn_size.y) {
				curr_page_index++;
				if (curr_height == 0) {
					wxMessageBox(_("Large modifier group"));
					return;
				}
				break;
			}
			curr_height += btn_space;
		}
	}
	if (begin_index != 0) {
		prev_page_btn->Enable();
		change_bg_colour(prev_page_btn, ok_color);
	}

	int pos = 0;
	for (size_t i=begin_index; i<group_vec_.size(); ++i) {
		int left = 0;
		if (pos + group_height_[i] > btn_size.y) {
			next_page_btn->Enable();
			change_bg_colour(next_page_btn, ok_color);
			break;
		}
		wxString max_weight = to_uc(boost::lexical_cast<std::string>(group_vec_[i]->max_weight()));
		wxStaticText* label = new wxStaticText(btn_pnl, wxID_ANY, to_uc(group_vec_[i]->name()) + _(" (") + max_weight + _(")"), wxPoint(0, pos+lable_heigh_));
		label->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
		label_vec_.push_back(label);
		pos += 2*lable_heigh_ + btn_space;
		for (size_t j=0; j<mod_vec_.size(); ++j) {
			if (mod_vec_[j]->group_id() != group_vec_[i]->group_id()) continue;
			if ( left + btn_width > btn_size.x ) {
				left = 0;
				pos += btn_height + btn_space;
			}

			wxButton* btn = new wxButton(btn_pnl, wxID_ANY, _(""), wxPoint(left, pos), wxSize(btn_width, btn_height), 0);
			btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
			btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( add_modifiers_f::modifier_btnClick ), NULL, this);
			insert_modifier(btn, mod_vec_[j]);
			btn_vec_.push_back(btn);
			left += btn_width + btn_space;
		}
		pos += btn_height + btn_space;
	}
	hide_btn_group();
}

int add_modifiers_f::curr_weight(const ksi_cafe::modifier_group_t* group) const
{
	int res_weight = 0;
	wxTreeItemIdValue cookie;
	wxTreeItemId curr = sg->GetFirstChild(sg->GetRootItem(), cookie);
	while (curr.IsOk()) {
		wxTreeItemData* data = sg->GetItemData(curr);
		mod_node_t* node = dynamic_cast<mod_node_t*>(data);
		if (!node) THROW_LOCATION(0, "int add_modifiers_f::curr_weight(const ksi_cafe::modifier_group_t* group) const");
		if (node->type() == node_type::modifier_group && reinterpret_cast<const ksi_cafe::modifier_group_t*>(node->col1()) == group) {
			wxTreeItemIdValue cookie;
			wxTreeItemId child = sg->GetFirstChild(curr, cookie);
			while (child.IsOk()) {
				wxTreeItemData* data = sg->GetItemData(child);
				mod_node_t* node = dynamic_cast<mod_node_t*>(data);
				if (!node) THROW_LOCATION(1, "int add_modifiers_f::curr_weight(const ksi_cafe::modifier_group_t* group) const");
				if (node->type() != node_type::menu_modifier) THROW_LOCATION(2, "int add_modifiers_f::curr_weight(const ksi_cafe::modifier_group_t* group) const");
				const ksi_cafe::menu_dish_modifier_t* modifier = reinterpret_cast<const ksi_cafe::menu_dish_modifier_t*>(node->col1());
				
				res_weight += modifier->weight()*node->col2();
				child = sg->GetNextChild(curr, cookie);
			}
		}
		curr = sg->GetNextChild(sg->GetRootItem(), cookie);
	}
	return res_weight;
}

void add_modifiers_f::hide_btn_group()
{
	for (size_t i=0; i<btn_vec_.size(); ++i) {
		const ksi_cafe::modifier_group_t* group = mods_[kv_modifier_[btn_vec_[i]]];
		const ksi_cafe::menu_dish_modifier_t* modifier = kv_modifier_[btn_vec_[i]];
		if (group->max_weight() && group->max_weight() < curr_weight(group) + modifier->weight())
			btn_vec_[i]->Disable();
		else btn_vec_[i]->Enable();
		/*
		if (group->max_weight() && group->max_weight() == curr_weight(group))
			btn_vec_[i]->Disable();
		else
		btn_vec_[i]->Enable();
		*/
	}
}

void add_modifiers_f::insert_modifier(wxButton* btn, const ksi_cafe::menu_dish_modifier_t* modifier)
{
	btn->SetLabel(to_uc(modifier->name())+_(" (")+to_uc(boost::lexical_cast<std::string>(modifier->weight()))+_(")"));
	int color = modifier->color();
	change_bg_colour(btn, wxColour(ksi_cafe::red(color), ksi_cafe::green(color), ksi_cafe::blue(color)));
	wrap_label(btn);
	btn->Show();
	kv_modifier_[btn] = modifier;
}

size_t add_modifiers_f::set_modifiers(ksi_cafe::menu_t* menu, const ksi_cafe::menu_dish_t* menu_dish)
{
	wxString caption = locale->get("add_modifiers_f", "select_modifiers_for_the_str1", "Select modifiers for the \"") + to_uc(menu_dish->name()) + locale->get("add_modifiers_f", "select_modifiers_for_the_str2", "\" entree");
	//wxString rus_caption = _("Выберите модификаторы для блюда \"") + to_uc(menu_dish->name()) + _("\"");
	
	SetLabel(caption);
	
	mods_ = menu->menu_dish_modifier(menu_dish);

	for (std::map<const ksi_cafe::menu_dish_modifier_t*, const ksi_cafe::modifier_group_t*>::const_iterator i=mods_.begin(); i!=mods_.end(); ++i) {
		std::vector<const ksi_cafe::menu_dish_modifier_t*>::const_iterator mod_It;
		std::vector<const ksi_cafe::modifier_group_t*>::const_iterator group_It;

		mod_It = std::find(mod_vec_.begin(), mod_vec_.end(), i->first);
		if (mod_It == mod_vec_.end()) mod_vec_.push_back(i->first);

		group_It = std::find(group_vec_.begin(), group_vec_.end(), i->second);
		if (group_It == group_vec_.end()) group_vec_.push_back(i->second);
	}

	std::sort(group_vec_.begin(), group_vec_.end(), boost::bind<bool>(std::greater<int>(), boost::bind(&ksi_cafe::modifier_group_t::group_id, _1), boost::bind(&ksi_cafe::modifier_group_t::group_id, _2)));

	wxSize btn_size = btn_pnl->GetClientSize();
	for (size_t i=0; i<group_vec_.size(); ++i) {
		size_t btn_cnt = std::count_if(mod_vec_.begin(), mod_vec_.end(), boost::bind(std::equal_to<int>(), group_vec_[i]->group_id(), boost::bind(&ksi_cafe::menu_dish_modifier_t::group_id, _1)));
		size_t col_cnt = size_t(btn_size.x+btn_space)/(btn_width+btn_space);
		int row_cnt = int(btn_cnt/col_cnt); if (row_cnt*col_cnt != btn_cnt) row_cnt++;
		group_height_.push_back(2*lable_heigh_+btn_space+row_cnt*(btn_height+btn_space)-btn_space);
	}

	show();

	return mod_vec_.size();
}

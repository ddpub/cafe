
#include "move_up.hpp"
#include <wx_util/wx_util.hpp>
#include <ksi_util/util.hpp>
#include <algorithm>
#include <iterator>
#include <boost/bind.hpp>
#include <ksi_include/ksi_exceptions.hpp>
#include <ksi_include/algo_functor.hpp>
#include <wx/settings.h>
#include <memory>
#include <shared_modules/connect.hpp>
#include <ksi_util/util.hpp>
#include <boost/lexical_cast.hpp>
#include <ksi_include/ksi_exceptions.hpp>


extern std::auto_ptr<ksi_cafe::xml_lang_t> locale;


void tree_buttons_t::assign_empty(wxButton* btn)
{
	std::vector<wxButton*>::iterator It;
	//It = std::find_if(btn_vec_.begin(), btn_vec_.end(), boost::bind(std::equal_to<wxButton*>(), btn, _1));
	It = std::find(btn_vec_.begin(), btn_vec_.end(), btn);
	if (It == btn_vec_.end()) THROW_LOCATION(0, "void tree_buttons_t::assign_empty(wxButton*)");
	kv_status_[btn] = btn_status::empty;
	btn->Hide();
}

void tree_buttons_t::assign_root(wxButton* btn)
{
	std::vector<wxButton*>::iterator It;
	//It = std::find_if(btn_vec_.begin(), btn_vec_.end(), boost::bind(std::equal_to<wxButton*>(), btn, _1));
	It = std::find(btn_vec_.begin(), btn_vec_.end(), btn);
	if (It == btn_vec_.end()) THROW_LOCATION(0, "void tree_buttons_t::assign_root(wxButton*)");
	kv_status_[btn] = btn_status::root;
	kv_menu_item_[btn] = 0;
	btn->SetLabel(locale->get("global", "menu_root_str", "<menu root>"));
	wrap_label(btn);
	set_bold_font(btn);
	btn->Show();
}

void tree_buttons_t::assign_menu_item(wxButton* btn, const ksi_cafe::menu_item_t* menu_item)
{
	std::vector<wxButton*>::iterator It;
	//It = std::find_if(btn_vec_.begin(), btn_vec_.end(), boost::bind(std::equal_to<wxButton*>(), btn, _1));
	It = std::find(btn_vec_.begin(), btn_vec_.end(), btn);
	if (It == btn_vec_.end()) THROW_LOCATION(0, "void tree_buttons_t::assign_menu_item(wxButton*, const ksi_cafe::menu_item_t*)");
	kv_status_[btn] = btn_status::menu_item;
	kv_menu_item_[btn] = menu_item;
	std::string name = menu_item->name();
	btn->SetLabel(to_uc(name));
	int color = menu_item->color();
	change_bg_colour(btn, wxColour(ksi_cafe::red(color), ksi_cafe::green(color), ksi_cafe::blue(color)));
	wrap_label(btn);
	set_bold_font(btn);
	btn->Show();
}

void tree_buttons_t::reset()
{
	assign_root(btn_vec_[0]);
	for (size_t i=1; i<btn_vec_.size(); ++i) assign_empty(btn_vec_[i]);
}

const ksi_cafe::menu_item_t* tree_buttons_t::select(const ksi_cafe::menu_item_t* menu_item)
{
	std::vector<wxButton*>::iterator It;
	for (std::vector<wxButton*>::iterator i=btn_vec_.begin(); i != btn_vec_.end(); ++i)
	{
		if (kv_status_[*i] == btn_status::menu_item && kv_menu_item_[*i] == menu_item)
		{
			for (std::vector<wxButton*>::iterator j=i+1; j != btn_vec_.end(); ++j) assign_empty(*j);
			return menu_item;
		}
		if (kv_status_[*i] == btn_status::root && kv_menu_item_[*i] == menu_item)
		{
			for (std::vector<wxButton*>::iterator j=i+1; j != btn_vec_.end(); ++j) assign_empty(*j);
			return menu_item;
		}
		if (kv_status_[*i] == btn_status::empty)
		{
			assign_menu_item(*i, menu_item);
			return menu_item;
		}
	}
	THROW_LOCATION(0, "const ksi_cafe::menu_item_t* tree_buttons_t::select(const ksi_cafe::menu_item_t*)");
}

const ksi_cafe::menu_item_t* tree_buttons_t::selected()
{
	const ksi_cafe::menu_item_t* res_item = 0;
	for (size_t i=0; i<btn_vec_.size(); ++i)
	{
		if (kv_status_[btn_vec_[i]] == btn_status::menu_item)
	    res_item = kv_menu_item_[btn_vec_[i]];
	}
	return res_item;
}

btn_status::status_t tree_buttons_t::get_status(wxButton* btn)
{
	std::vector<wxButton*>::iterator It;
	//It = std::find_if(btn_vec_.begin(), btn_vec_.end(), boost::bind(std::equal_to<wxButton*>(), btn, _1));
	It = std::find(btn_vec_.begin(), btn_vec_.end(), btn);
	if (It == btn_vec_.end()) return btn_status::error;
	return kv_status_[*It];
}

void menu_buttons_t::assign_empty(wxButton* btn)
{
	std::vector<wxButton*>::iterator It;
	//It = std::find_if(btn_vec_.begin(), btn_vec_.end(), boost::bind(std::equal_to<wxButton*>(), btn, _1));
	It = std::find(btn_vec_.begin(), btn_vec_.end(), btn);
	if (It == btn_vec_.end()) THROW_LOCATION(0, "void menu_buttons_t::assign_empty(wxButton*)");
	kv_status_[btn] = btn_status::empty;
	btn->Hide();
}

void menu_buttons_t::assign_next_page(wxButton* btn)
{
	std::vector<wxButton*>::iterator It;
	//It = std::find_if(btn_vec_.begin(), btn_vec_.end(), boost::bind(std::equal_to<wxButton*>(), btn, _1));
	It = std::find(btn_vec_.begin(), btn_vec_.end(), btn);
	if (It == btn_vec_.end()) THROW_LOCATION(0, "void menu_buttons_t::assign_next_page(wxButton*)");
	kv_status_[btn] = btn_status::next;
	btn->SetLabel(locale->get("global", "next_str", ">> next"));
	change_bg_colour(btn, next_page_color);
	wrap_label(btn);
	set_normal_font(btn);
	btn->Show();
}

void menu_buttons_t::assign_prev_page(wxButton* btn)
{
	std::vector<wxButton*>::iterator It;
	//It = std::find_if(btn_vec_.begin(), btn_vec_.end(), boost::bind(std::equal_to<wxButton*>(), btn, _1));
	It = std::find(btn_vec_.begin(), btn_vec_.end(), btn);
	if (It == btn_vec_.end()) THROW_LOCATION(0, "void menu_buttons_t::assign_prev_page(wxButton*)");
	kv_status_[btn] = btn_status::prev;
	btn->SetLabel(locale->get("global", "prev_str", "<< prev"));
	change_bg_colour(btn, prev_page_color);
	wrap_label(btn);
	set_normal_font(btn);
	btn->Show();
}

void menu_buttons_t::assign_parent(wxButton* btn, const ksi_cafe::menu_item_t* parent)
{
	std::vector<wxButton*>::iterator It;
	//It = std::find_if(btn_vec_.begin(), btn_vec_.end(), boost::bind(std::equal_to<wxButton*>(), btn, _1));
	It = std::find(btn_vec_.begin(), btn_vec_.end(), btn);
	if (It == btn_vec_.end()) THROW_LOCATION(0, "void menu_buttons_t::assign_parent(wxButton*, ksi_cafe::menu_item_t*)");
	kv_status_[btn] = btn_status::parent;
	kv_menu_item_[btn] = parent;
	btn->SetLabel(_("..."));
	change_bg_colour(btn, parent_color);
	wrap_label(btn);
	set_bold_font(btn);
	btn->Show();
	kv_menu_item_[btn] = parent;
}

void menu_buttons_t::assign_menu_item(wxButton* btn, const ksi_cafe::menu_item_t* item)
{
	std::vector<wxButton*>::iterator It;
	//It = std::find_if(btn_vec_.begin(), btn_vec_.end(), boost::bind(std::equal_to<wxButton*>(), btn, _1));
	It = std::find(btn_vec_.begin(), btn_vec_.end(), btn);
	if (It == btn_vec_.end()) THROW_LOCATION(0, "void menu_buttons_t::assign_menu_item(wxButton*, ksi_cafe::menu_item_t*)");
	kv_status_[btn] = btn_status::menu_item;
	std::string name = item->name();
	btn->SetLabel((to_uc(name)));
	int color = item->color();
	change_bg_colour(btn, wxColour(ksi_cafe::red(color), ksi_cafe::green(color), ksi_cafe::blue(color)));
	wrap_label(btn);
	set_bold_font(btn);
	btn->Show();
	kv_menu_item_[btn] = item;
}

void menu_buttons_t::assign_menu_dish(wxButton* btn, const ksi_cafe::menu_dish_t* dish)
{
	std::vector<wxButton*>::iterator It;
	//It = std::find_if(btn_vec_.begin(), btn_vec_.end(), boost::bind(std::equal_to<wxButton*>(), btn, _1));
	It = std::find(btn_vec_.begin(), btn_vec_.end(), btn);
	if (It == btn_vec_.end()) THROW_LOCATION(0, "void menu_buttons_t::assign_menu_dish(wxButton*, ksi_cafe::menu_dish_t*)");
	kv_status_[btn] = btn_status::menu_dish;
	std::string name = dish->name();
	btn->SetLabel((to_uc(name)));
	int color = dish->hotkey_color();
	change_bg_colour(btn, wxColour(ksi_cafe::red(color), ksi_cafe::green(color), ksi_cafe::blue(color)));
	wrap_label(btn);
	set_normal_font(btn);
	btn->Show();
	kv_menu_dish_[btn] = dish;
}

wxButton* menu_buttons_t::get_next()
{
	for (size_t i=0; i<btn_vec_.size(); ++i)
	{
		if (kv_status_[btn_vec_[i]] == btn_status::empty)
			return btn_vec_[i];
	}
	return 0;
}

void menu_buttons_t::show_prev_page(const ksi_cafe::menu_t* menu, const ksi_cafe::menu_item_t* menu_item)
{
	kv_status_.clear();
	cur_page_--;
	if (cur_page_ == 1)
	{
		show(menu, menu_item);
		return;
	}
	if (menu_item != 0 && get_next() != 0)
	{
		if (menu_item->parent_id() == 0) assign_parent(get_next(), 0);
		else
		{
			std::vector<const ksi_cafe::menu_item_t*> vec = menu->menu_item();
			std::vector<const ksi_cafe::menu_item_t*>::iterator It;
			It = std::find_if(vec.begin(), vec.end(), boost::bind(std::equal_to<int>(), menu_item->parent_id(), boost::bind(&ksi_cafe::menu_item_t::menu_item_id, _1)));
			if (It == vec.end()) THROW_LOCATION(0,"void menu_buttons_t::show_prev_page(const ksi_cafe::menu_t*, const ksi_cafe::menu_item_t*)");
			assign_parent(get_next(), *It);
		}
	}

	int menu_item_id = 0;
	if (menu_item != 0) menu_item_id = menu_item->menu_item_id();

	std::vector<const ksi_cafe::menu_item_t*> menu_item_vec = menu->menu_item(menu_item_id);
	std::vector<const ksi_cafe::menu_dish_t*> menu_dish_vec = menu->menu_dish(menu_item_id);

	size_t end = btn_vec_.size() - 2;
	int total_count = menu_item_vec.size()+menu_dish_vec.size();
	size_t first = btn_vec_.size()-2 + (btn_vec_.size()-3)*(cur_page_-2);
	if (total_count-first>end) end-=1;

	for (size_t i=0; i<end; ++i)
	{
		if (first+i<menu_item_vec.size() && get_next() != 0)
		{
			assign_menu_item(get_next(), menu_item_vec[first+i]);
		}
		else if (get_next() != 0 && (first+i-menu_item_vec.size() < menu_dish_vec.size()))
		{
			assign_menu_dish(get_next(), menu_dish_vec[first+i-menu_item_vec.size()]);
		}
	}
	assign_prev_page(get_next());
	if (total_count-first > end && get_next() != 0) assign_next_page(get_next());

	for (size_t i=0; i<btn_vec_.size(); ++i)
	{
		if (kv_status_[btn_vec_[i]] == btn_status::empty) assign_empty(btn_vec_[i]);
	}
}

void menu_buttons_t::show_next_page(const ksi_cafe::menu_t* menu, const ksi_cafe::menu_item_t* menu_item)
{
	kv_status_.clear();
	cur_page_++;
	if (menu_item != 0 && get_next() != 0)
	{
		if (menu_item->parent_id() == 0) assign_parent(get_next(), 0);
		else
		{
			std::vector<const ksi_cafe::menu_item_t*> vec = menu->menu_item();
			std::vector<const ksi_cafe::menu_item_t*>::iterator It;
			It = std::find_if(vec.begin(), vec.end(), boost::bind(std::equal_to<int>(), menu_item->parent_id(), boost::bind(&ksi_cafe::menu_item_t::menu_item_id, _1)));
			if (It == vec.end()) THROW_LOCATION(0,"void menu_buttons_t::show_next_page(const ksi_cafe::menu_t*, const ksi_cafe::menu_item_t*)");
			assign_parent(get_next(), *It);
		}
	}

	int menu_item_id = 0;
	if (menu_item != 0) menu_item_id = menu_item->menu_item_id();

	std::vector<const ksi_cafe::menu_item_t*> menu_item_vec = menu->menu_item(menu_item_id);
	std::vector<const ksi_cafe::menu_dish_t*> menu_dish_vec = menu->menu_dish(menu_item_id);

	size_t end = btn_vec_.size() - 2;
	size_t total_count = menu_item_vec.size() + menu_dish_vec.size();
	size_t first = (btn_vec_.size()-2) + (btn_vec_.size()-3)*(cur_page_-2);
	if (total_count - first > end) end -= 1;

	for (size_t i=0; i<end; ++i)
	{
		if (first+i<menu_item_vec.size() && get_next() != 0)
		{
			assign_menu_item(get_next(), menu_item_vec[first+i]);
		}
		else if (get_next() != 0 && (first+i-menu_item_vec.size() < menu_dish_vec.size()))
		{
			assign_menu_dish(get_next(), menu_dish_vec[first+i-menu_item_vec.size()]);
		}
	}
	assign_prev_page(get_next());
	if (total_count-first > end && get_next() != 0) assign_next_page(get_next());

	for (size_t i=0; i<btn_vec_.size(); ++i)
	{
		if (kv_status_[btn_vec_[i]] == btn_status::empty) assign_empty(btn_vec_[i]);
	}
}

void menu_buttons_t::show(const ksi_cafe::menu_t* menu, const ksi_cafe::menu_item_t* menu_item)
{
	kv_status_.clear();
	cur_page_ = 1;
	if (menu_item != 0 && get_next() != 0)
	{
		if (menu_item->parent_id() == 0) assign_parent(get_next(), 0);
		else
		{
			std::vector<const ksi_cafe::menu_item_t*> vec = menu->menu_item();
			std::vector<const ksi_cafe::menu_item_t*>::iterator It;
			It = std::find_if(vec.begin(), vec.end(), boost::bind(std::equal_to<int>(), menu_item->parent_id(), boost::bind(&ksi_cafe::menu_item_t::menu_item_id, _1)));
			if (It == vec.end()) THROW_LOCATION(0, "void menu_buttons_t::show(ksi_cafe::menu_t*, ksi_cafe::menu_item_t*)");
			assign_parent(get_next(), *It);
		}
	}

	int menu_item_id = 0;
	if (menu_item != 0) menu_item_id = menu_item->menu_item_id();

	std::vector<const ksi_cafe::menu_item_t*> menu_item_vec = menu->menu_item(menu_item_id);
	std::vector<const ksi_cafe::menu_dish_t*> menu_dish_vec = menu->menu_dish(menu_item_id);

	size_t end = btn_vec_.size() - 1;
	size_t total_count = menu_item_vec.size() + menu_dish_vec.size();
	if (total_count > end) end--;

	for (size_t i=0; i<end; ++i)
	{
		if (i<menu_item_vec.size() && get_next() != 0)
		{
			assign_menu_item(get_next(), menu_item_vec[i]);
		}
		else if (get_next() != 0 && (i-menu_item_vec.size() < menu_dish_vec.size()))
		{
			assign_menu_dish(get_next(), menu_dish_vec[i-menu_item_vec.size()]);
		}
	}

	if (total_count > end && get_next() != 0)
		assign_next_page(get_next());

	for (size_t i=0; i<btn_vec_.size(); ++i)
	{
		if (kv_status_[btn_vec_[i]] == btn_status::empty) assign_empty(btn_vec_[i]);
	}
}

btn_status::status_t menu_buttons_t::get_status(wxButton* btn)
{
	std::vector<wxButton*>::iterator It;
	//It = std::find_if(btn_vec_.begin(), btn_vec_.end(), boost::bind(std::equal_to<wxButton*>(), btn, _1));
	It = std::find(btn_vec_.begin(), btn_vec_.end(), btn);
	if (It == btn_vec_.end()) return btn_status::error;
	return kv_status_[*It];
}

void hot_buttons_t::assign_empty(wxButton* btn)
{
	std::vector<wxButton*>::iterator It;
	//It = std::find_if(btn_vec_.begin(), btn_vec_.end(), boost::bind(std::equal_to<wxButton*>(), btn, _1));
	It = std::find(btn_vec_.begin(), btn_vec_.end(), btn);
	if (It == btn_vec_.end()) THROW_LOCATION(0, "void hot_buttons_t::assign_empty(wxButton*)");
	kv_status_[btn] = btn_status::empty;
	btn->Hide();
}

void hot_buttons_t::assign_menu_dish(wxButton* btn, const ksi_cafe::menu_dish_t* dish)
{
	std::vector<wxButton*>::iterator It;
	//It = std::find_if(btn_vec_.begin(), btn_vec_.end(), boost::bind(std::equal_to<wxButton*>(), btn, _1));
	It = std::find(btn_vec_.begin(), btn_vec_.end(), btn);
	if (It == btn_vec_.end()) THROW_LOCATION(0, "void hot_buttons_t::assign_menu_dish(wxButton*, const ksi_cafe::menu_dish_t*)");
	kv_status_[btn] = btn_status::menu_dish;
	std::string name = dish->name();
	btn->SetLabel((to_uc(name)));
	int color = dish->hotkey_color();
	change_bg_colour(btn, wxColour(ksi_cafe::red(color), ksi_cafe::green(color), ksi_cafe::blue(color)));
	wrap_label(btn);
	set_normal_font(btn);
	btn->Show();
	kv_menu_dish_[btn] = dish;
}

btn_status::status_t hot_buttons_t::get_status(wxButton* btn)
{
	std::vector<wxButton*>::iterator It;
	//It = std::find_if(btn_vec_.begin(), btn_vec_.end(), boost::bind(std::equal_to<wxButton*>(), btn, _1));
	It = std::find(btn_vec_.begin(), btn_vec_.end(), btn);
	if (It == btn_vec_.end()) return btn_status::error;
	return kv_status_[btn];
}

void hot_buttons_t::show(const ksi_cafe::menu_t* menu)
{
	kv_status_.clear();
	std::vector<const ksi_cafe::menu_dish_t*> vec = menu->hot_dish();
	for (std::vector<wxButton*>::iterator i=btn_vec_.begin(); i!=btn_vec_.end(); ++i)
	{
		std::vector<const ksi_cafe::menu_dish_t*>::iterator It;
		It = std::find_if(vec.begin(), vec.end(),
		boost::bind(std::equal_to<int>(), boost::bind(&ksi_cafe::menu_dish_t::hotkey_id, _1), std::distance(btn_vec_.begin(), i)+1));
		if (It == vec.end()) assign_empty(*i);
		else assign_menu_dish(*i, *It);
	}
}

const ksi_cafe::menu_dish_t* move_up_t::get_menu_dish(wxButton* btn)
{
	if (menu_btn_.get_status(btn) == btn_status::menu_dish)
		return menu_btn_.get_menu_dish(btn);

	if (hot_btn_.get_status(btn) == btn_status::menu_dish)
		return hot_btn_.get_menu_dish(btn);

	THROW_LOCATION(0, "const ksi_cafe::menu_dish_t* menu_buttons_t::get_menu_dish(wxButton*)");
}

const ksi_cafe::menu_item_t* move_up_t::get_menu_item(wxButton* btn)
{
	if (menu_btn_.get_status(btn) == btn_status::menu_item || menu_btn_.get_status(btn) == btn_status::parent)
		return menu_btn_.get_menu_item(btn);

	if (tree_btn_.get_status(btn) == btn_status::menu_item || tree_btn_.get_status(btn) == btn_status::root)
		return tree_btn_.get_menu_item(btn);

	THROW_LOCATION(0, "const ksi_cafe::menu_item_t* menu_buttons_t::get_menu_item(wxButton*)");
}

void list_buttons_t::assign_empty(wxButton* btn)
{
	std::vector<wxButton*>::iterator It;
	//It = std::find_if(btn_vec_.begin(), btn_vec_.end(), boost::bind(std::equal_to<wxButton*>(), btn, _1));
	It = std::find(btn_vec_.begin(), btn_vec_.end(), btn);
	if (It == btn_vec_.end()) THROW_LOCATION(0, "void list_buttons_t::assign_empty(wxButton*)");
	kv_status_[btn] = btn_status::empty;
	btn->Hide();
}

void list_buttons_t::assign_next_page(wxButton* btn)
{
	std::vector<wxButton*>::iterator It;
	//It = std::find_if(btn_vec_.begin(), btn_vec_.end(), boost::bind(std::equal_to<wxButton*>(), btn, _1));
	It = std::find(btn_vec_.begin(), btn_vec_.end(), btn);
	if (It == btn_vec_.end()) THROW_LOCATION(0, "void list_buttons_t::assign_next_page(wxButton*)");
	kv_status_[btn] = btn_status::next;
	btn->SetLabel(locale->get("global", "next_str", ">> next"));
	change_bg_colour(btn, next_page_color);
	btn->Enable(true);
	wrap_label(btn);
	set_normal_font(btn);
	btn->Show();	
}

void list_buttons_t::assign_prev_page(wxButton* btn)
{
	std::vector<wxButton*>::iterator It;
	//It = std::find_if(btn_vec_.begin(), btn_vec_.end(), boost::bind(std::equal_to<wxButton*>(), btn, _1));
	It = std::find(btn_vec_.begin(), btn_vec_.end(), btn);
	if (It == btn_vec_.end()) THROW_LOCATION(0, "void list_buttons_t::assign_prev_page(wxButton*)");
	kv_status_[btn] = btn_status::prev;
	btn->SetLabel(locale->get("global", "prev_str", "<< prev"));
	change_bg_colour(btn, prev_page_color);
	btn->Enable(true);
	wrap_label(btn);
	set_normal_font(btn);
	btn->Show();
}

void list_buttons_t::assign_close(wxButton* btn)
{
	std::vector<wxButton*>::iterator It;
	//It = std::find_if(btn_vec_.begin(), btn_vec_.end(), boost::bind(std::equal_to<wxButton*>(), btn, _1));
	It = std::find(btn_vec_.begin(), btn_vec_.end(), btn);
	if (It == btn_vec_.end()) THROW_LOCATION(0, "void list_buttons_t::assign_close(wxButton*)");
	kv_status_[btn] = btn_status::close;
	btn->SetLabel(locale->get("global", "cancel_str", "Cancel"));
	change_bg_colour(btn, close_color);
	btn->Enable(true);
	wrap_label(btn);
	set_normal_font(btn);
	btn->Show();  
}

void list_buttons_t::assign_cap(wxButton* btn, const wxString& cap, const wxColour& color, bool checked, bool enabled, bool wrap)
{
	std::vector<wxButton*>::iterator It;
	//It = std::find_if(btn_vec_.begin(), btn_vec_.end(), boost::bind(std::equal_to<wxButton*>(), btn, _1));
	It = std::find(btn_vec_.begin(), btn_vec_.end(), btn);
	if (It == btn_vec_.end()) THROW_LOCATION(0, "void list_buttons_t::assign_cap(wxButton*, const wxString&, const wxColour, bool, bool)");
	kv_status_[btn] = btn_status::item;
	btn->SetLabel(cap);
	if (!checked) {
		change_bg_colour(btn, color);
		set_normal_font(btn);
	}
	else {
		change_bg_colour(btn, parent_color);
		set_bold_font(btn);
	}
	btn->Enable(enabled);
	if (wrap) wrap_label(btn);
	btn->Show();  
}

wxButton* list_buttons_t::get_next()
{
	for (size_t i=0; i<btn_vec_.size(); ++i)
	{
		if (kv_status_[btn_vec_[i]] == btn_status::empty)
			return btn_vec_[i];
	}
	return 0;
}

btn_status::status_t list_buttons_t::get_status(wxButton* btn)
{
	std::vector<wxButton*>::iterator It;
	//It = std::find_if(btn_vec_.begin(), btn_vec_.end(), boost::bind(std::equal_to<wxButton*>(), btn, _1));
	It = std::find(btn_vec_.begin(), btn_vec_.end(), btn);
	if (It == btn_vec_.end()) return btn_status::error;
	return kv_status_[*It];
}

int list_buttons_t::get_modal_code(wxButton* btn)
{
	std::vector<wxButton*>::iterator It;
	//It = std::find_if(btn_vec_.begin(), btn_vec_.end(), boost::bind(std::equal_to<wxButton*>(), btn, _1));
	It = std::find(btn_vec_.begin(), btn_vec_.end(), btn);
	if (It == btn_vec_.end()) THROW_LOCATION(0, "int list_buttons_t::get_modal_code(wxButton*)");
	if (kv_status_[btn] == btn_status::close) return wxID_CLOSE;
	else if (kv_status_[btn] == btn_status::item)
	{
		size_t first = 0;
		if (cur_page_ > 1) first = need_close_ ? (btn_vec_.size()-2) + (btn_vec_.size()-3)*(cur_page_-2) : (btn_vec_.size()-1) + (btn_vec_.size()-2)*(cur_page_-2);
		return wxID_HIGHEST+std::distance(btn_vec_.begin(), It) + first;
	}
	return wxID_NONE;
}

void list_buttons_t::refresh()
{
	for(size_t i=0; i<btn_vec_.size(); ++i)
	{
		if (get_status(btn_vec_[i]) == btn_status::item)
		{
			int index = get_modal_code(btn_vec_[i])-wxID_HIGHEST;
			assign_cap(btn_vec_[i], cap_vec_[index], color_vec_[index], checked_[index], enabled_[index], wrap_[index]);
		}	
	}
}

void list_buttons_t::show(int page)
{
	kv_status_.clear();
	cur_page_ = page;
	if (cur_page_ == 1) {
		size_t end = need_close_ ? btn_vec_.size()-1 : btn_vec_.size();
		if (cap_vec_.size() > end) end -= 1;
		if (cap_vec_.size() < end) end = cap_vec_.size();
		for (size_t i=0; i<end; ++i) if (get_next() != 0) assign_cap(get_next(), cap_vec_[i], color_vec_[i], checked_[i], enabled_[i], wrap_[i]);
		if (cap_vec_.size() > end && get_next() != 0) assign_next_page(get_next());
		if (need_close_ && get_next() != 0) assign_close(get_next());
	}
	else {
		size_t end = need_close_ ? btn_vec_.size()-2 : btn_vec_.size()-1;
		size_t first = need_close_ ? (btn_vec_.size()-2) + (btn_vec_.size()-3)*(cur_page_-2) : (btn_vec_.size()-1) + (btn_vec_.size()-2)*(cur_page_-2);
		if (cap_vec_.size() - first > end) end -= 1;
		if (cap_vec_.size() - first < end) end = cap_vec_.size() - first;
		for (size_t i=0; i<end; ++i) if (get_next() != 0) assign_cap(get_next(), cap_vec_[first+i], color_vec_[first+i], checked_[first+i], enabled_[first+i], wrap_[first+i]);
		assign_prev_page(get_next());
		if (cap_vec_.size()-first > end && get_next() != 0) assign_next_page(get_next());
		if (need_close_ && get_next() != 0) assign_close(get_next());	
	}
	
	for (size_t i=0; i<btn_vec_.size(); ++i) {
		if (kv_status_[btn_vec_[i]] == btn_status::empty) assign_empty(btn_vec_[i]);
	}
}

void list_buttons_t::show_next_page()
{
	kv_status_.clear();
	cur_page_++;
	size_t end = need_close_ ? btn_vec_.size()-2 : btn_vec_.size()-1;
	size_t first = need_close_ ? (btn_vec_.size()-2) + (btn_vec_.size()-3)*(cur_page_-2) : (btn_vec_.size()-1) + (btn_vec_.size()-2)*(cur_page_-2);
	if (cap_vec_.size() - first > end) end -= 1;
	if (cap_vec_.size() - first < end) end = cap_vec_.size() - first;
	for (size_t i=0; i<end; ++i) if (get_next() != 0) assign_cap(get_next(), cap_vec_[first+i], color_vec_[first+i], checked_[first+i], enabled_[first+i], wrap_[first+i]);
	assign_prev_page(get_next());
	if (cap_vec_.size()-first > end && get_next() != 0) assign_next_page(get_next());
	if (need_close_ && get_next() != 0) assign_close(get_next());
	
	for (size_t i=0; i<btn_vec_.size(); ++i)
	{
		if (kv_status_[btn_vec_[i]] == btn_status::empty) assign_empty(btn_vec_[i]);
	}
}

void list_buttons_t::show_prev_page()
{
	kv_status_.clear();
	cur_page_--;
	if (cur_page_ == 1)
	{
		show();
		return;
	}
	size_t end = need_close_ ? btn_vec_.size()-2 : btn_vec_.size()-1;
	//size_t first = (btn_vec_.size()-2) + (btn_vec_.size()-3)*(cur_page_-2);
	size_t first = need_close_ ? (btn_vec_.size()-2) + (btn_vec_.size()-3)*(cur_page_-2) : (btn_vec_.size()-2) + (btn_vec_.size()-2)*(cur_page_-2);
	if (cap_vec_.size() - first > end) end -= 1;
	for (size_t i=0; i<end; ++i) if (get_next() != 0) assign_cap(get_next(), cap_vec_[first+i], color_vec_[first+i], checked_[first+i], enabled_[first+i], wrap_[first+i]);
	assign_prev_page(get_next());
	if (cap_vec_.size()-first > end && get_next() != 0) assign_next_page(get_next());
	if (need_close_ && get_next() != 0) assign_close(get_next());
	
	for (size_t i=0; i<btn_vec_.size(); ++i)
	{
		if (kv_status_[btn_vec_[i]] == btn_status::empty) assign_empty(btn_vec_[i]);
	}
}

void table_show_t::simple_show(int page) {
	if (list_buttons_.get() == 0) {
		std::vector<ksi_cafe::table_t> table_vec = table_container_->get_tables(level_);
		std::vector<wxString> cap_vec;
		std::vector<wxColour> color_vec;
		std::vector<bool> checked_vec;
		std::vector<bool> enabled_vec;
		std::vector<bool> wrap_vec;
		if (level_ == 0) {
			for (size_t i=0; i<table_vec.size(); ++i) {
				cap_vec.push_back(to_uc(table_vec[i].table_group_name()));
				color_vec.push_back(table_group_color);
				checked_vec.push_back(false);
				enabled_vec.push_back(true);
				wrap_vec.push_back(true);
			}
		}
		else {
			cap_vec.push_back(_("..\n")+locale->get("global", "select_the_room_str", "<select the room>"));
			color_vec.push_back(parent_color);
			checked_vec.push_back(false);
			enabled_vec.push_back(true);
			wrap_vec.push_back(false);
			for (size_t i=0; i<table_vec.size(); ++i) {
				cap_vec.push_back(to_uc(table_vec[i].name()));
				color_vec.push_back(table_color);
				checked_vec.push_back(false);
				enabled_vec.push_back(true);
				wrap_vec.push_back(true);
			}
		}

		list_buttons_.reset(new list_buttons_t(btn_vec_, cap_vec, color_vec, checked_vec, enabled_vec, wrap_vec, need_close_));
	}
	list_buttons_->show();
}

void table_show_t::refresh() {
	int cur_page = list_buttons_->cur_page();
	list_buttons_.reset();
	show(cur_page);
}

void table_show_t::show(int page) {
	if (content_ == content::SIMPLE) simple_show(page);
	else if (content_ == content::REGULAR) regular_show(page);
	else if (content_ == content::BOOKING) booking_show(page);
	else throw std::runtime_error("Invalid content::content_t. value = " + boost::lexical_cast<std::string>(content_));
}

void table_show_t::regular_show(int page) {
	if (list_buttons_.get() == 0) {
		std::vector<ksi_cafe::table_t> table_vec = table_container_->get_tables(level_);
		std::vector<wxString> cap_vec;
		std::vector<wxColour> color_vec;
		std::vector<bool> checked_vec;
		std::vector<bool> enabled_vec;
		std::vector<bool> wrap_vec;
		if (level_ == 0) {
			for (size_t i=0; i<table_vec.size(); ++i) {
				cap_vec.push_back(to_uc(table_vec[i].table_group_name()));
				color_vec.push_back(table_group_color);
				checked_vec.push_back(false);
				enabled_vec.push_back(true);
				wrap_vec.push_back(true);
			}
		}
		else {
			std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();

			query->create_statement
			(
		    "select sum(self_count) as self_count, sum(total_count) as total_count, tables \
		    from \
		    ( \
		      select 0 as self_count, count(*) as total_count , tables \
		      from ksi.orders \
		      where cafe_id = ksi.util.get_cur_cafe_id \
		      and seance_id in (select seance_id from ksi.seance where shift_id = ksi.util.get_cur_shift_id) \
		      and status_id in (1,2,6) and orders.type_id in (1) \
		      and table_group_id = "+ boost::lexical_cast<std::string>(level_) +" \
		      group by tables \
		    union \
		      select count(*) as self_count, 0 as total_count , tables \
		      from ksi.orders \
		      where cafe_id = ksi.util.get_cur_cafe_id \
		      and seance_id in (select seance_id from ksi.seance where shift_id = ksi.util.get_cur_shift_id) \
		      and status_id in (1,2,6) and creator_id = ksi.util.get_cur_user_id and orders.type_id in (1) \
		      and table_group_id = "+ boost::lexical_cast<std::string>(level_) +" \
		      group by tables \
		    ) \
			where tables is not null \
		    group by tables"
			);
			query->execute_statement();
			
			std::map<std::string, int> self_;
			std::map<std::string, int> total_;
			while (!query->next()) {
				int self_count = static_cast<int>(boost::get<double>(query->get(0)));
				int total_count = static_cast<int>(boost::get<double>(query->get(1)));
				std::string table_name = boost::get<std::string>(query->get(2));
				self_[table_name] = self_count;
				total_[table_name] = total_count;
			}
			cap_vec.push_back(_("..\n") + locale->get("global", "select_the_room_str", "<select the room>"));
			color_vec.push_back(parent_color);
			checked_vec.push_back(false);
			enabled_vec.push_back(true);
			wrap_vec.push_back(false);
			for (size_t i=0; i<table_vec.size(); ++i) {
				
				if (self_[table_vec[i].name()]) {
					cap_vec.push_back(to_uc(table_vec[i].name()) + _("\n") + locale->get("global", "open_orders_str", "open orders: ") + to_uc(boost::lexical_cast<std::string>(self_[table_vec[i].name()])));
					color_vec.push_back(ok_color);
				}
				else if (total_[table_vec[i].name()]) {
					cap_vec.push_back(to_uc(table_vec[i].name()) + _("\n") + locale->get("global", "occupied_str", "occupied"));
					color_vec.push_back(close_color);
				}
				else {
					cap_vec.push_back(to_uc(table_vec[i].name()) + _("\n") + locale->get("global", "free_str", "free"));
					color_vec.push_back(table_color);
				}
				checked_vec.push_back(false);
				enabled_vec.push_back(true);
				wrap_vec.push_back(false);
			}
		}
		
		list_buttons_.reset(new list_buttons_t(btn_vec_, cap_vec, color_vec, checked_vec, enabled_vec, wrap_vec, need_close_));
	}
	list_buttons_->show(page);
}

void table_show_t::booking_show(int page) {
	if (list_buttons_.get() == 0) {
		std::vector<ksi_cafe::table_t> table_vec = table_container_->get_tables(level_);
		std::vector<wxString> cap_vec;
		std::vector<wxColour> color_vec;
		std::vector<bool> checked_vec;
		std::vector<bool> enabled_vec;
		std::vector<bool> wrap_vec;
		if (level_ == 0) {
			for (size_t i=0; i<table_vec.size(); ++i) {
				cap_vec.push_back(to_uc(table_vec[i].table_group_name()));
				color_vec.push_back(table_group_color);
				checked_vec.push_back(false);
				enabled_vec.push_back(true);
				wrap_vec.push_back(true);
			}
		}
		else {
			std::auto_ptr<ksi_client::oracle_query_i> query = ksi_cafe::session->create_query();

			query->create_statement
			(
			"SELECT "
			" ORDERS.ORDER_ID, "
			" ORDERS.TABLES, "
			" ORDER_DELIVERY.DELIVERY_DATE, "
			" (ORDER_DELIVERY.FIRSTNAME || ' ' || ORDER_DELIVERY.LASTNAME) AS fio, "
			" ORDERS.GUEST_COUNT "
			" FROM KSI.ORDERS "
			" INNER JOIN KSI.ORDER_DELIVERY ON ORDER_DELIVERY.DELIVERY_ID = ORDERS.DELIVERY_ID "
			"		AND ORDERS.TYPE_ID = 6 "
			"		AND ORDERS.CAFE_ID = KSI.UTIL.GET_CUR_CAFE_ID "
			"		AND :seldate > ORDER_DELIVERY.DELIVERY_DATE - 4/24 "
			"		AND :seldate < ORDER_DELIVERY.DELIVERY_DATE + 4/24 "
			"		AND ORDER_DELIVERY.CAFE_ID = ORDERS.CAFE_ID "
			" WHERE ORDERS.CAFE_ID = KSI.UTIL.GET_CUR_CAFE_ID "
			" AND ORDERS.STATUS_ID IN (1,2,6) "
			" AND TABLE_GROUP_ID = :table_group_id "
			);
			query->set("table_group_id", level_);
			query->set("seldate", seldate_);
			query->execute_statement();
			
			typedef std::map<std::string, std::pair<int, std::pair<boost::posix_time::ptime, std::pair<std::string,int> > > > tableNameVals_t;
			tableNameVals_t tableNameVals;
			while (!query->next()) {
				int order_id = static_cast<int>(boost::get<double>(query->get(0)));
				std::string tableName = boost::get<std::string>(query->get(1));
				boost::posix_time::ptime delivery_date = boost::get<boost::posix_time::ptime>(query->get(2));
				std::string customer_fio = boost::get<std::string>(query->get(3));
				int guest_count = static_cast<int>(boost::get<double>(query->get(4)));
				if (tableNameVals.find(tableName) != tableNameVals.end()) {
					boost::posix_time::ptime prev_delivery_date = tableNameVals[tableName].second.first;
					boost::posix_time::time_period prev_period(seldate_ < prev_delivery_date ? seldate_ : prev_delivery_date, seldate_ >= prev_delivery_date ? prev_delivery_date : seldate_);
					boost::posix_time::time_duration prev_duration = prev_period.length();
					boost::posix_time::time_period curr_period(seldate_ < delivery_date ? seldate_ : delivery_date, seldate_ >= delivery_date ? delivery_date : seldate_);
					boost::posix_time::time_duration curr_duration = curr_period.length();
					if (curr_duration.total_seconds() < prev_duration.total_seconds()) {
						tableNameVals[tableName] = std::make_pair(order_id, std::make_pair(delivery_date, std::make_pair(customer_fio, guest_count)));
					}
				}
				else {
					tableNameVals[tableName] = std::make_pair(order_id, std::make_pair(delivery_date, std::make_pair(customer_fio, guest_count)));
				}
			}
			
			cap_vec.push_back(_("..\n") + locale->get("global", "select_the_room_str", "<select the room>"));
			color_vec.push_back(parent_color);
			checked_vec.push_back(false);
			enabled_vec.push_back(true);
			wrap_vec.push_back(false);
			for (size_t i=0; i<table_vec.size(); ++i) {
				tableNameVals_t::const_iterator It = tableNameVals.find(table_vec[i].name());
				if (It != tableNameVals.end()) {
					int order_id = It->second.first;
					boost::posix_time::ptime delivery_date = It->second.second.first;
					std::string customer_fio = It->second.second.second.first;
					int guest_count = It->second.second.second.second;
					cap_vec.push_back(to_uc(table_vec[i].name()) + _("\n") + _("Order# ") + to_uc(boost::lexical_cast<std::string>(order_id)) + _("\n") + locale->get_date_time(delivery_date) + _("\n") + to_uc(customer_fio) + _("\nGuests: ") + to_uc(boost::lexical_cast<std::string>(guest_count)));
					color_vec.push_back(close_color);
				}
				else {
					cap_vec.push_back(to_uc(table_vec[i].name()) + _("\n") + locale->get("global", "free_str", "free"));
					color_vec.push_back(table_color);
				}
				checked_vec.push_back(false);
				enabled_vec.push_back(true);
				wrap_vec.push_back(false);
			}
		}
		
		list_buttons_.reset(new list_buttons_t(btn_vec_, cap_vec, color_vec, checked_vec, enabled_vec, wrap_vec, need_close_));
	}
	list_buttons_->show(page);
}

void table_show_t::up() {
	level_ = 0;
	list_buttons_.reset();
	show();
}

void table_show_t::down(int table_group_id) {
	level_ = table_group_id;
	list_buttons_.reset();
	show();
}

void table_show_t::left() {
	list_buttons_->show_prev_page();
}

void table_show_t::right() {
	list_buttons_->show_next_page();
}

btn_status::status_t table_show_t::get_status(wxButton* btn) {
	if (list_buttons_.get() == 0) return btn_status::error;
	btn_status::status_t cur_status = list_buttons_->get_status(btn);
	if (cur_status == btn_status::next) return btn_status::next;
	else if (cur_status == btn_status::prev) return btn_status::prev;
	else if (need_close_ && cur_status == btn_status::close) return btn_status::close;
	else if (cur_status != btn_status::item) return btn_status::error;
	std::vector<ksi_cafe::table_t> table_vec = table_container_->get_tables(level_);
	int modal_code = list_buttons_->get_modal_code(btn);
	if (modal_code == wxID_NONE) return btn_status::error;
	int index = modal_code - wxID_HIGHEST;
	if (level_ == 0) {
		if (index >= table_vec.size()) return btn_status::error;
		return btn_status::table_group;
	}
	else {
		if (index >= table_vec.size()+1) return btn_status::error;
		if (index == 0) return btn_status::parent;
		return btn_status::table;
	}
	return btn_status::error;
}

ksi_cafe::table_t table_show_t::get_table_group(wxButton* btn) {
	if (list_buttons_.get() == 0) THROW_LOCATION(0, "ksi_cafe::table_t table_show_t::get_table_group(wxButton*)");
	btn_status::status_t cur_status = list_buttons_->get_status(btn);
	if (cur_status != btn_status::item) THROW_LOCATION(5, "ksi_cafe::table_t table_show_t::get_table_group(wxButton*)");
	std::vector<ksi_cafe::table_t> table_vec = table_container_->get_tables(level_);
	int modal_code = list_buttons_->get_modal_code(btn);
	if (modal_code == wxID_NONE) THROW_LOCATION(1, "ksi_cafe::table_t table_show_t::get_table_group(wxButton*)");
	int index = modal_code - wxID_HIGHEST;
	if (level_ == 0) {
		if (index >= table_vec.size()) THROW_LOCATION(2, "ksi_cafe::table_t table_show_t::get_table_group(wxButton*)");
		return table_vec[index];
	}
	else THROW_LOCATION(3, "ksi_cafe::table_t table_show_t::get_table_group(wxButton*)");
	THROW_LOCATION(4, "ksi_cafe::table_t table_show_t::get_table_group(wxButton*)");
}

ksi_cafe::table_t table_show_t::get_table(wxButton* btn) {
	if (list_buttons_.get() == 0) THROW_LOCATION(0, "ksi_cafe::table_t table_show_t::get_table(wxButton*)");
	btn_status::status_t cur_status = list_buttons_->get_status(btn);
	if (cur_status != btn_status::item) THROW_LOCATION(5, "ksi_cafe::table_t table_show_t::get_table(wxButton*)");
	std::vector<ksi_cafe::table_t> table_vec = table_container_->get_tables(level_);
	int modal_code = list_buttons_->get_modal_code(btn);
	if (modal_code == wxID_NONE) THROW_LOCATION(1, "ksi_cafe::table_t table_show_t::get_table(wxButton*)");
	int index = modal_code - wxID_HIGHEST;
	if (level_ == 0) {
		THROW_LOCATION(2, "ksi_cafe::table_t table_show_t::get_table(wxButton*)");
	}
	else {
		if (index >= table_vec.size()+1) THROW_LOCATION(3, "ksi_cafe::table_t table_show_t::get_table(wxButton*)");
		if (index == 0) THROW_LOCATION(6, "ksi_cafe::table_t table_show_t::get_table(wxButton*)");
		return table_vec[index-1];
	}
	THROW_LOCATION(4, "ksi_cafe::table_t table_show_t::get_table(wxButton*)");
}

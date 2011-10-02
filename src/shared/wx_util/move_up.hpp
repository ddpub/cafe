#ifndef DMITRY_DERGACHEV_12_05_2008_MOVE_UP_H
#define DMITRY_DERGACHEV_12_05_2008_MOVE_UP_H

#include <wx/button.h>
#include <wx/colour.h>
#include <lang/xml_lang.h>
#include <ksi_order/menu_interface.hpp>
#include <ksi_order/menu_container.hpp>
#include <vector>
#include <map>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace btn_status
{
  enum status_t { empty=0, parent, menu_item, menu_dish, next, prev, root, error, close, item, table_group, table};
};

class tree_buttons_t
{
public:
  tree_buttons_t(const std::vector<wxButton*>& btn_vec_): btn_vec_(btn_vec_) { reset(); }
  void reset();
  const ksi_cafe::menu_item_t* select(const ksi_cafe::menu_item_t*);
  const ksi_cafe::menu_item_t* selected();
public:
  btn_status::status_t get_status(wxButton*);
  const ksi_cafe::menu_item_t* get_menu_item(wxButton* btn) { return kv_menu_item_[btn]; }
private:
  void assign_empty(wxButton*);
  void assign_root(wxButton*);
  void assign_menu_item(wxButton*, const ksi_cafe::menu_item_t*);
private:
  std::vector<wxButton*> btn_vec_;
  std::map<wxButton*, btn_status::status_t> kv_status_;
  std::map<wxButton*, const ksi_cafe::menu_item_t*> kv_menu_item_;
};

class menu_buttons_t
{
public:
  menu_buttons_t(const std::vector<wxButton*>& btn_vec_): btn_vec_(btn_vec_) {}
  void show(const ksi_cafe::menu_t*, const ksi_cafe::menu_item_t*);
  void show_next_page(const ksi_cafe::menu_t*, const ksi_cafe::menu_item_t*);
  void show_prev_page(const ksi_cafe::menu_t*, const ksi_cafe::menu_item_t*);
public:
  btn_status::status_t get_status(wxButton*);
  const ksi_cafe::menu_item_t* get_menu_item(wxButton* btn) { return kv_menu_item_[btn]; }
  const ksi_cafe::menu_dish_t* get_menu_dish(wxButton* btn) { return kv_menu_dish_[btn]; }
private:
  void assign_empty(wxButton*);
  void assign_next_page(wxButton*);
  void assign_prev_page(wxButton*);
  void assign_parent(wxButton*, const ksi_cafe::menu_item_t*);
  void assign_menu_item(wxButton*, const ksi_cafe::menu_item_t*);
  void assign_menu_dish(wxButton*, const ksi_cafe::menu_dish_t*);
  wxButton* get_next();
private:
  std::vector<wxButton*> btn_vec_;
  std::map<wxButton*, btn_status::status_t> kv_status_;
  std::map<wxButton*, const ksi_cafe::menu_item_t*> kv_menu_item_;
  std::map<wxButton*, const ksi_cafe::menu_dish_t*> kv_menu_dish_;
  int cur_page_;
};

class hot_buttons_t
{
public:
  hot_buttons_t(const std::vector<wxButton*>& btn_vec_): btn_vec_(btn_vec_) {}
  void show(const ksi_cafe::menu_t* menu);
  btn_status::status_t get_status(wxButton*);
  const ksi_cafe::menu_dish_t* get_menu_dish(wxButton* btn) { return kv_menu_dish_[btn]; }
private:
  void assign_empty(wxButton*);
  void assign_menu_dish(wxButton*, const ksi_cafe::menu_dish_t*);
private:
  std::vector<wxButton*> btn_vec_;
  std::map<wxButton*, btn_status::status_t> kv_status_;
  std::map<wxButton*, const ksi_cafe::menu_dish_t*> kv_menu_dish_;
};

class move_up_t
{
public:
  move_up_t(const std::vector<wxButton*>& t, const std::vector<wxButton*> m, const std::vector<wxButton*>& h): tree_btn_(t), menu_btn_(m), hot_btn_(h) {}
  void show(const ksi_cafe::menu_t* m) { menu_ = m; tree_btn_.reset(); menu_btn_.show(menu_, 0); hot_btn_.show(menu_); }
  void tree_click(const ksi_cafe::menu_item_t* item) { tree_btn_.select(item); menu_btn_.show(menu_, item); }
  void tree_click(wxButton* btn) { tree_click(get_menu_item(btn)); }
  void menu_item_click(const ksi_cafe::menu_item_t* item) { tree_btn_.select(item); menu_btn_.show(menu_, item); }
  void menu_item_click(wxButton* btn) { menu_item_click(get_menu_item(btn)); }
  void next_page_click(wxButton* btn) { menu_btn_.show_next_page(menu_, tree_btn_.selected()); }
  void prev_page_click(wxButton* btn) { menu_btn_.show_prev_page(menu_, tree_btn_.selected()); }
public:
  btn_status::status_t get_menu_bar_status(wxButton* btn) { return menu_btn_.get_status(btn); }
  btn_status::status_t get_tree_bar_status(wxButton* btn) { return tree_btn_.get_status(btn); }
  btn_status::status_t get_hot_bar_status(wxButton* btn) { return hot_btn_.get_status(btn); }
  const ksi_cafe::menu_item_t* get_menu_item(wxButton* btn);
  const ksi_cafe::menu_dish_t* get_menu_dish(wxButton* btn);
private:
  tree_buttons_t tree_btn_;
  menu_buttons_t menu_btn_;
  hot_buttons_t hot_btn_;
  const ksi_cafe::menu_t* menu_;
};

class list_buttons_t
{
public:
	list_buttons_t(const std::vector<wxButton*>& btn_vec_, const std::vector<wxString>& cap_vec_, std::vector<wxColour>& color_vec_, std::vector<bool>& checked_, std::vector<bool>& enabled_, std::vector<bool>& wrap_, bool need_close_=true)
	: btn_vec_(btn_vec_), cap_vec_(cap_vec_), color_vec_(color_vec_), checked_(checked_), enabled_(enabled_), wrap_(wrap_), need_close_(need_close_) {}
public:
	void refresh();
	void show(int page=1);
	void show_next_page();
	void show_prev_page();
	int cur_page() const { return cur_page_; }
public:
	btn_status::status_t get_status(wxButton*);
	int get_modal_code(wxButton*);
	void change_checked(int index, bool status) { checked_[index] = status; }
	void change_enabled(int index, bool status) { enabled_[index] = status; }
private:
	void assign_empty(wxButton*);
	void assign_next_page(wxButton*);
	void assign_prev_page(wxButton*);
	void assign_close(wxButton*);
	void assign_cap(wxButton*, const wxString&, const wxColour& color, bool checked, bool enabled, bool wrap);
	wxButton* get_next();
private:
	std::vector<wxButton*> btn_vec_;
	std::vector<wxString> cap_vec_;
	std::vector<wxColour> color_vec_;
	std::vector<bool> checked_;
	std::vector<bool> enabled_;
	std::vector<bool> wrap_;
	std::map<wxButton*, btn_status::status_t> kv_status_;
	int cur_page_;
	bool need_close_;
};

namespace content {
	enum content_t {
		SIMPLE=0,
		REGULAR=1,
		BOOKING=2
	};
}

class table_show_t {
public:
	table_show_t(const std::vector<wxButton*>& btn_vec_, const ksi_cafe::table_container_t* table_container_, content::content_t content_, bool need_close_=false, boost::posix_time::ptime seldate_ = boost::posix_time::not_a_date_time)
	: btn_vec_(btn_vec_), table_container_(table_container_), content_(content_), level_(0), need_close_(need_close_), seldate_(seldate_) {}
	void show(int page=1);
	void up();
	void down(int table_group_id);
	void left();
	void right();
	void refresh();
	btn_status::status_t get_status(wxButton* btn);
	ksi_cafe::table_t get_table_group(wxButton* btn);
	ksi_cafe::table_t get_table(wxButton* btn);
	int level() const { return level_; }
	void set_buttons(const std::vector<wxButton*>& ext_btn_vec) { btn_vec_ = ext_btn_vec; }
private:
	void simple_show(int page);
	void regular_show(int page);
	void booking_show(int page);
private:
	std::vector<wxButton*> btn_vec_;
	const ksi_cafe::table_container_t* table_container_;
	std::auto_ptr<list_buttons_t> list_buttons_;
	boost::posix_time::ptime seldate_;
	content::content_t content_;
	int level_;
	bool need_close_;
};

#endif

#ifndef DMITRY_DERGACHEV_04_03_2008_WX_UTIL_H
#define DMITRY_DERGACHEV_04_03_2008_WX_UTIL_H

#include <wx/window.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/colour.h>
#include <wx/event.h>
#include <wx/font.h>
#include <wx/timer.h>
#include <wx/utils.h>
#include <wx/sizer.h>
#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <ksi_include/ksi_exceptions.hpp>
#include <ksi_util/util.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <map>



#define parent_color wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER )
#define next_page_color wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER )
#define prev_page_color wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER )
#define close_color wxColour( 255, 204, 204 )
#define ok_color wxColour( 204, 255, 204 )
#define btn_face_color wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE )//wxColour( 236, 233, 216 )
#define yellow_color wxColour( 255, 255, 0 )
#define table_color wxColour( 208, 208, 208 )
#define table_group_color wxColour( 192, 192, 192 )
#define table_type_table_booking_color wxColour( 255, 255, 255 )
#define table_type_delivery_color wxColour( 115, 192, 238 )
#define table_type_pickup_color wxColour( 180, 221, 245 )

inline std::string to_mb(const wxString& source)
{
	if (source == _("")) return "";
	return std::string(wxConvCurrent->cWX2MB(source));
}

inline wxString to_uc(const std::string& source)
{
	if (source == "") return _("");
	return wxConvCurrent->cMB2WX(source.c_str());
}

void change_bg_colour(wxButton*, const wxColour&);

void change_font_weight(wxButton*, wxFontWeight weight);
void set_bold_font(wxButton*);
void set_normal_font(wxButton*);

class toggle_group_t
{
public:
  // Constructor
  toggle_group_t(wxButton*, const std::vector<wxButton*>&, const wxColour&, const wxColour&);
public:
  void toggle(wxButton*);
  void toggle(const wxCommandEvent&);
  wxButton* current() const { return current_; }
  void roll_back();
private:
  // Member function
private:
  // Member item
  wxButton* back_up_;
  wxButton* current_;
  std::vector<wxButton*> btn_vec_;
  wxColour toggle_clr_;
  wxColour untoggle_clr_;
private:
/*
  toggle_group_t();
  toggle_group_t(const toggle_group_t&);
  toggle_group_t& operator=(const toggle_group_t&);
*/
};

template <typename Object_t>
class obj_handler_t
{
public:
	obj_handler_t(Object_t* obj_, void (Object_t::*pm_)(wxTimerEvent&)): obj_(obj_), pm_(pm_) {}
	void operator()(wxTimerEvent& event) { (obj_->*pm_)(event); }
private:
	Object_t* obj_;
	void (Object_t::*pm_)(wxTimerEvent&);
};

template <typename Object_t>
class wx_timer_t: public wxEvtHandler
{
public:
	explicit wx_timer_t(obj_handler_t<Object_t> handler_): handler_(handler_), interval_(-1), OneShot_(false)
	{
		timerid_ = ::wxNewId();
		timer_.SetOwner(this, timerid_);
		Connect(timer_.GetId(), timerid_, wxEVT_TIMER, wxTimerEventHandler(wx_timer_t::OnTimer), 0, this);
	}
	~wx_timer_t() { Disconnect(-1, timerid_, wxEVT_TIMER); }
	bool start(int interval, bool OneShot = false) { interval_ = interval*1000; OneShot_ = OneShot; return timer_.Start(interval_, OneShot_); }
	bool start2(int interval, bool OneShot = false) { interval_ = interval*100; OneShot_ = OneShot; return timer_.Start(interval_, OneShot_); }
	bool start3(int interval, bool OneShot = false) { interval_ = interval*10; OneShot_ = OneShot; return timer_.Start(interval_, OneShot_); }
	bool start() { return timer_.Start(interval_, OneShot_); }
	void stop() { timer_.Stop(); }	
	void OnTimer(wxTimerEvent& event) { handler_(event); }
	long timer_id() const { return timerid_; }
	bool IsRunning() const { return timer_.IsRunning(); }
private:
	obj_handler_t<Object_t> handler_;
	long timerid_;
	wxTimer timer_;
	int interval_;
	bool OneShot_;
};

template <typename Object_t>
class pause_timer_t
{
public:
	pause_timer_t(wx_timer_t<Object_t>& timer_): timer_(timer_), owner_(timer_.IsRunning()) { if (owner_) timer_.stop(); }
	~pause_timer_t() { if (owner_) timer_.start(); }
private:
	wx_timer_t<Object_t>& timer_;
	bool owner_;
};

class wx_change_cursor_t
{
public:
	wx_change_cursor_t(wxWindow* win_, const wxCursor& origin_, const wxCursor& temp_): win_(win_), origin_(origin_), temp_(temp_) { win_->SetCursor(temp_); }
	~wx_change_cursor_t() { win_->SetCursor(origin_); }
private:
	wxWindow* win_;
	wxCursor origin_;
	wxCursor temp_;
};

template <typename T>
class wx_window_ptr
{
public:
  explicit wx_window_ptr(T* ptr_=0): ptr_(ptr_) {}
  ~wx_window_ptr() { if (ptr_) ptr_->Destroy(); }
  T* get() const { return ptr_; }
  T* operator->() const { return ptr_; }
  void reset(T* new_ptr = 0) { if (ptr_) ptr_->Destroy(); ptr_ = new_ptr; }
  T* release() { T* tmp = ptr_; ptr_ = 0; return tmp; }
private:
  T* ptr_;
private:
  wx_window_ptr(const wx_window_ptr&);
  wx_window_ptr& operator=(const wx_window_ptr&);
};

void wrap_label(wxButton* btn);

class wx_disable_t {
public:
	explicit wx_disable_t(wxWindow& object_): object_(object_) { lock(); }
	explicit wx_disable_t(wxWindow* pobject_): object_(*pobject_) { lock(); }
	~wx_disable_t() { unlock(); }
	static bool disable(wxWindow* pobject_) {
		return locked_[pobject_] != 0;
	}
private:
	void lock() {
		object_.Enable(false);
		++locked_[&object_];
	}
	void unlock() { 
		--locked_[&object_];
		if (locked_[&object_] == 0) object_.Enable(true); 
	}
	wxWindow& object_;
	static std::map<wxWindow*, size_t> locked_;
};

class wx_hide_t {
public:
	wx_hide_t(wxWindow* object_): sizer_(0), object_(object_), destroyed_(false) { hide(); }
	wx_hide_t(wxSizer* sizer_, wxWindow* object_): sizer_(sizer_), object_(object_), destroyed_(false) { hide(); }
	~wx_hide_t() { if (!destroyed_) show(); }
	void force() { show(); destroyed_ = true; }
private:
	void hide() { if (sizer_ && object_) sizer_->Show(object_, false, false); else if (object_) object_->Hide(); }
	void show() { if (sizer_ && object_) sizer_->Show(object_, true, false); else if (object_) object_->Show(); }
	wxWindow* object_;
	wxSizer* sizer_;
	bool destroyed_;
};
/*
template <typename Callable_1, typename Callable_2>
class virtual_pause_t {
	virtual_pause_t(Callable_1 pause_, Callable_2 resume_)
	: pause_(pause_), resume_(resume_) {
		pause_();
	}
	~virtual_pause_t(Callable_1 pause_, Callable_2 resume_)
	: pause_(pause_), resume_(resume_) {
		resume_();
	}
private:
	Callable_1 pause_;
	Callable_2 resume_;
};
*/
class vkbdEvent: public wxEvent {
public:
	vkbdEvent(const wxString& buff_, long v_code_);
	virtual ~vkbdEvent();
	virtual wxEvent* Clone() const;
	const wxString& buff() const;
	long v_code() const;
private:
	wxString buff_;
	long v_code_;
};

DECLARE_EVENT_TYPE(EVT_VKBD, -1)

#define EVT_VKBD(fn)                                             \
        DECLARE_EVENT_TABLE_ENTRY( EVT_VKBD, wxID_ANY, wxID_ANY,   \
        (wxObjectEventFunction)(wxEventFunction) & fn, (wxObject*) NULL ),


#endif

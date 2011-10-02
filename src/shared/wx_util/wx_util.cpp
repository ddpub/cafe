#include "wx_util.hpp"
#include <wx/arrstr.h>
#include <wx/tokenzr.h>
#include <wx/dcclient.h>
#include <ksi_util/util.hpp>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <ksi_include/algo_functor.hpp>
#include <boost/bind.hpp>


std::map<wxWindow*, size_t> wx_disable_t::locked_;

toggle_group_t::toggle_group_t(wxButton* current_, const std::vector<wxButton*>& btn_vec_, const wxColour& toggle_clr_, const wxColour& untoggle_clr_)
: back_up_(0), current_(current_), btn_vec_(btn_vec_), toggle_clr_(toggle_clr_), untoggle_clr_(untoggle_clr_)
{
	if (current_ != 0) {
		change_bg_colour(current_, toggle_clr_);
		set_bold_font(current_);
	}
}

void change_bg_colour(wxButton* btn, const wxColour& clr)
{
  btn->SetBackgroundColour(clr);
  if ((clr.Red() + clr.Green() + clr.Blue())/2 > 128)
    btn->SetForegroundColour( wxColour( 0, 0, 0 ) );
  else
    btn->SetForegroundColour( wxColour( 255, 255, 255 ) );
}

void change_font_weight(wxButton* btn, wxFontWeight weight) {
	wxFont font = btn->GetFont();
	font.SetWeight(weight);
	btn->SetFont(font);
}

void set_bold_font(wxButton* btn) {
	change_font_weight(btn, wxFONTWEIGHT_BOLD);
}

void set_normal_font(wxButton* btn) {
	change_font_weight(btn, wxFONTWEIGHT_NORMAL);
}

void toggle_group_t::toggle(wxButton* btn)
{
	if (btn == 0 && current_ != 0) {
		back_up_ = current_;
		change_bg_colour(current_, untoggle_clr_);
		set_normal_font(current_);
		current_ = 0;
	}
	if (current_ == btn) return;

	back_up_ = current_;
  
	change_bg_colour(current_, untoggle_clr_);
	set_normal_font(current_);

	current_ = *std::find_if(btn_vec_.begin(), btn_vec_.end(), boost::bind(identical_functor<wxButton*>(), _1) == btn );

	change_bg_colour(current_, toggle_clr_);
	set_bold_font(current_);
}

void toggle_group_t::toggle(const wxCommandEvent& event)
{
  wxObject* obj = event.GetEventObject();
  wxButton* btn = dynamic_cast<wxButton*>(obj);
  toggle(btn);
}

void toggle_group_t::roll_back() {
	toggle(back_up_);
}


void wrap_label(wxButton* btn)
{
  change_bg_colour(btn, btn->GetBackgroundColour());
  wxString title = btn->GetLabel();
  wxArrayString arr = wxStringTokenize(title);
  wxClientDC dc(btn);
  int max_len = dc.GetSize().x;
  int space_len = dc.GetTextExtent(_(" ")).x;
  std::vector<int> len_vec;
  for (size_t i=0; i<arr.Count(); ++i)
  {
    wxSize size = dc.GetTextExtent(arr[i]);
    len_vec.push_back(size.x);
  }
  wxString new_title;
  for (size_t i=0; i<len_vec.size(); )
  {
    int acc_len = len_vec[i];
    if (new_title.Length() != 0)
      new_title += _("\n") + arr[i];
    else
      new_title += arr[i];
    while (i+1<len_vec.size() && acc_len + space_len + len_vec[i+1] <= max_len)
    {
      new_title += _(" ") +arr[i+1];
      acc_len += space_len + len_vec[i+1];
      i++;
    }
    i++;
  }
  btn->SetLabel(new_title);
}


DEFINE_EVENT_TYPE(EVT_VKBD)
//IMPLEMENT_DYNAMIC_CLASS(vkbdEvent, wxEvent)

vkbdEvent::vkbdEvent(const wxString& buff_, long v_code_): wxEvent(wxID_ANY, EVT_VKBD), buff_(buff_), v_code_(v_code_) {}
vkbdEvent::~vkbdEvent() {}
wxEvent* vkbdEvent::Clone() const { return new vkbdEvent(buff_, v_code_); }

const wxString& vkbdEvent::buff() const { return buff_; }

long vkbdEvent::v_code() const { return v_code_; }

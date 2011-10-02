#include "select_time_f.h"
#include <wx/msgdlg.h>
#include <wx_util/wx_util.hpp>
#include <memory>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <string>
#include <boost/lexical_cast.hpp>
#include <lang/xml_lang.h>

extern std::auto_ptr<ksi_cafe::xml_lang_t> locale;

select_time_f::select_time_f( wxWindow* parent, const wxString& title_, const boost::posix_time::time_period& period_, int setup_minutes_)
:
base_select_time_f( parent ), title_(title_), period_(period_), setup_minutes_(setup_minutes_)
{
	SetLabel(title_);
	if (period_.is_null()) return;
	
	boost::posix_time::ptime curr = period_.begin();
	boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
	
	while (curr <= period_.end()) {
		boost::posix_time::ptime cap_time = boost::posix_time::ptime(now.date(), boost::posix_time::time_duration(curr.time_of_day().hours(), 0, 0, 0));
		wxString btn_cap = locale->get_time_short(cap_time);
		//wxString btn_cap = to_uc(boost::lexical_cast<std::string>(curr.time_of_day().hours())) + _(":00");
		wxButton* btn = new wxButton( this, wxID_ANY, btn_cap, wxDefaultPosition, wxSize( 100,40 ), 0 );
		btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
		btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( select_time_f::btn_click ), NULL, this );
		btn_date_time_map_[btn] = curr;
		
		change_bg_colour(btn, btn_face_color);
		

		if (now.date() == curr.date()) {
			if (curr.time_of_day().hours() < now.time_of_day().hours()) btn->Enable(false);
			else if (curr.time_of_day().hours() == now.time_of_day().hours() && now.time_of_day().minutes() > 45) btn->Enable(false);
		}
		
		if (btn->IsEnabled()) {
			m00_btn->Enable(true);
			m15_btn->Enable(true);
			m30_btn->Enable(true);
			m45_btn->Enable(true);
			
			if (now.date() == curr.date()) {
				if (curr.time_of_day().hours() < now.time_of_day().hours()) {
					m00_btn->Enable(false);
					m15_btn->Enable(false);
					m30_btn->Enable(false);
					m45_btn->Enable(false);
				}
				else if (curr.time_of_day().hours() == now.time_of_day().hours()) {
					m00_btn->Enable(false);
					if (now.time_of_day().minutes() > 16) m15_btn->Enable(false);
					if (now.time_of_day().minutes() > 31) m30_btn->Enable(false);
					if (now.time_of_day().minutes() > 46) m45_btn->Enable(false);
				}
			}
			
			if (m00_btn->IsEnabled() && period_.end().time_of_day() < curr.time_of_day()) m00_btn->Enable(false);
			if (m15_btn->IsEnabled() && period_.end().time_of_day() < curr.time_of_day() + boost::posix_time::minutes(15)) m15_btn->Enable(false);
			if (m30_btn->IsEnabled() && period_.end().time_of_day() < curr.time_of_day() + boost::posix_time::minutes(30)) m30_btn->Enable(false);
			if (m45_btn->IsEnabled() && period_.end().time_of_day() < curr.time_of_day() + boost::posix_time::minutes(45)) m45_btn->Enable(false);
			
			boost::posix_time::time_duration curr_start = (now.date() == curr.date()) ? now.time_of_day() : period_.begin().time_of_day();
			
			bool m00_btn_conform_ = false;
			bool m15_btn_conform_ = false;
			bool m30_btn_conform_ = false;
			bool m45_btn_conform_ = false;
			if (m00_btn->IsEnabled() && curr.time_of_day() < curr_start + boost::posix_time::minutes(setup_minutes_)) {
				m00_btn_conform_ = true;
			}
			if (m15_btn->IsEnabled() && curr.time_of_day() + boost::posix_time::minutes(15) < curr_start + boost::posix_time::minutes(setup_minutes_)) {
				m15_btn_conform_ = true;
			}
			if (m30_btn->IsEnabled() && curr.time_of_day() + boost::posix_time::minutes(30) < curr_start + boost::posix_time::minutes(setup_minutes_)) {
				m30_btn_conform_ = true;
			}
			if (m45_btn->IsEnabled() && curr.time_of_day() + boost::posix_time::minutes(45) < curr_start + boost::posix_time::minutes(setup_minutes_)) {
				m45_btn_conform_ = true;
			}
			
			if ( (!m00_btn->IsEnabled() || m00_btn_conform_)
			  && (!m15_btn->IsEnabled() || m15_btn_conform_)
			  && (!m30_btn->IsEnabled() || m30_btn_conform_)
			  && (!m45_btn->IsEnabled() || m45_btn_conform_) ) {
				if (m00_btn->IsEnabled() || m15_btn->IsEnabled() || m30_btn->IsEnabled() || m45_btn->IsEnabled()) {
					change_bg_colour(btn, yellow_color);
				}
			}
			
		}
		hgird->Add( btn, 0, wxALL, 5 );
		curr += boost::posix_time::hours(1);
	}
	change_bg_colour(m00_btn, btn_face_color);
	change_bg_colour(m15_btn, btn_face_color);
	change_bg_colour(m30_btn, btn_face_color);
	change_bg_colour(m45_btn, btn_face_color);
	m00_btn->Enable(false);
	m15_btn->Enable(false);
	m30_btn->Enable(false);
	m45_btn->Enable(false);
	
	this->Layout();
	main_sizer->Fit( this );
	CenterOnParent();
	ok_btn->Enable(false);
	btn_time_duration_map_[m00_btn] = boost::posix_time::minutes(0);
	btn_time_duration_map_[m15_btn] = boost::posix_time::minutes(15);
	btn_time_duration_map_[m30_btn] = boost::posix_time::minutes(30);
	btn_time_duration_map_[m45_btn] = boost::posix_time::minutes(45);

	ok_btn->SetLabel(locale->get("global", "OK_BNT", "Ok"));
	cancel_btn->SetLabel(locale->get("global", "CANCEL_BTN", "Cancel"));
	hours_sizer->GetStaticBox()->SetLabel(locale->get("global", "hours_str", "Hours"));
	minutes_sizer->GetStaticBox()->SetLabel(locale->get("global", "minutes_str", "Minutes"));
}

select_time_f::~select_time_f() {
	for (std::map<wxButton*, boost::posix_time::ptime>::const_iterator i=btn_date_time_map_.begin(); i!=btn_date_time_map_.end(); ++i) {
		if (i->first != 0) 
			i->first->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( select_time_f::btn_click ), NULL, this );
	}
}

void select_time_f::m_btnClick( wxCommandEvent& event )
{
	// TODO: Implement m00_btnClick
	wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
	if (btn == 0) return;

	wxString message = locale->get("global", "prep_order_str", "The time you're trying to set might be insufficient to prepare order. Are you sure?");
	wxMessageDialog dlg(this, message, _("Confirmation"), wxYES_NO);
	dlg.Centre(wxBOTH);
	if (!require_conform_[btn] || (require_conform_[btn] && dlg.ShowModal() == wxID_YES)) {
		selected_ += btn_time_duration_map_[btn];
		SetLabel(title_+_(": ") + locale->get_time(selected_));
		ok_btn->Enable(true);
		set_normal_font(m00_btn);
		set_normal_font(m15_btn);
		set_normal_font(m30_btn);
		set_normal_font(m45_btn);
		set_bold_font(btn);
	}
}

void select_time_f::ok_btnClick( wxCommandEvent& event )
{
	// TODO: Implement ok_btnClick
	EndModal(wxID_OK);
}

void select_time_f::cancel_btnClick( wxCommandEvent& event )
{
	// TODO: Implement cancel_btnClick
	EndModal(wxID_CANCEL);
}

void select_time_f::btn_click( wxCommandEvent& event ) {
	wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
	if (btn == 0) return;
	for (std::map<wxButton*, boost::posix_time::ptime>::const_iterator i=btn_date_time_map_.begin(); i!=btn_date_time_map_.end(); ++i) {
		if (i->first != 0) 
			set_normal_font(i->first);
	}
	set_bold_font(btn);
	m00_btn->Enable(true);
	m15_btn->Enable(true);
	m30_btn->Enable(true);
	m45_btn->Enable(true);
	change_bg_colour(m00_btn, btn_face_color);
	change_bg_colour(m15_btn, btn_face_color);
	change_bg_colour(m30_btn, btn_face_color);
	change_bg_colour(m45_btn, btn_face_color);
	set_normal_font(m00_btn);
	set_normal_font(m15_btn);
	set_normal_font(m30_btn);
	set_normal_font(m45_btn);
	ok_btn->Enable(false);
	SetLabel(title_);
	
	boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
	selected_ = btn_date_time_map_[btn];
	
	if (now.date() == selected_.date()) {
		if (selected_.time_of_day().hours() < now.time_of_day().hours()) {
			m00_btn->Enable(false);
			m15_btn->Enable(false);
			m30_btn->Enable(false);
			m45_btn->Enable(false);
		}
		else if (selected_.time_of_day().hours() == now.time_of_day().hours()) {
			m00_btn->Enable(false);
			if (now.time_of_day().minutes() > 16) m15_btn->Enable(false);
			if (now.time_of_day().minutes() > 31) m30_btn->Enable(false);
			if (now.time_of_day().minutes() > 46) m45_btn->Enable(false);
		}
	}
	
	if (m00_btn->IsEnabled() && period_.end().time_of_day() < selected_.time_of_day()) m00_btn->Enable(false);
	if (m15_btn->IsEnabled() && period_.end().time_of_day() < selected_.time_of_day() + boost::posix_time::minutes(15)) m15_btn->Enable(false);
	if (m30_btn->IsEnabled() && period_.end().time_of_day() < selected_.time_of_day() + boost::posix_time::minutes(30)) m30_btn->Enable(false);
	if (m45_btn->IsEnabled() && period_.end().time_of_day() < selected_.time_of_day() + boost::posix_time::minutes(45)) m45_btn->Enable(false);

	boost::posix_time::time_duration curr_start = (now.date() == selected_.date()) ? now.time_of_day() : period_.begin().time_of_day();
	if (m00_btn->IsEnabled() && selected_.time_of_day() < curr_start + boost::posix_time::minutes(setup_minutes_)) {
		require_conform_[m00_btn] = 1;
		change_bg_colour(m00_btn, yellow_color);
	}
	else require_conform_[m00_btn] = 0;
	if (m15_btn->IsEnabled() && selected_.time_of_day() + boost::posix_time::minutes(15) < curr_start + boost::posix_time::minutes(setup_minutes_)) {
		require_conform_[m15_btn] = 1;
		change_bg_colour(m15_btn, yellow_color);
	}
	else require_conform_[m15_btn] = 0;
	if (m30_btn->IsEnabled() && selected_.time_of_day() + boost::posix_time::minutes(30) < curr_start + boost::posix_time::minutes(setup_minutes_)) {
		require_conform_[m30_btn] = 1;
		change_bg_colour(m30_btn, yellow_color);
	}
	else require_conform_[m30_btn] = 0;
	if (m45_btn->IsEnabled() && selected_.time_of_day() + boost::posix_time::minutes(45) < curr_start + boost::posix_time::minutes(setup_minutes_)) {
		require_conform_[m45_btn] = 1;
		change_bg_colour(m45_btn, yellow_color);
	}	
	else require_conform_[m45_btn] = 0;
}

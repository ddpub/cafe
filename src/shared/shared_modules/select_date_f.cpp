
#include "select_date_f.h"
#include <wx_util/wx_util.hpp>

#include <memory>
#include <boost/date_time/posix_time/ptime.hpp>
#include <lang/xml_lang.h>

extern std::auto_ptr<ksi_cafe::xml_lang_t> locale;

select_date_f::select_date_f( wxWindow* parent, const wxString& title, const std::vector<boost::gregorian::date>& date_, const std::vector<int>& enable_, const boost::gregorian::date& selected_ )
:
base_select_date_f( parent ), date_(date_), enable_(enable_), selected_(selected_) {
	SetLabel(title);
	if (date_.size() == 0) return;
	for (size_t i=0; i<date_[0].day_of_week(); ++i) {
		grid->Add(0, 0, wxALL, 5);
	}
	for (size_t i=0; i<date_.size(); ++i) {
		wxString btn_cap = locale->get_date(boost::posix_time::ptime(date_[i], boost::posix_time::time_duration()));
		wxButton* btn = new wxButton( this, wxID_ANY, btn_cap, wxDefaultPosition, wxSize( 100,40 ), 0 );
		btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
		btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( select_date_f::btn_click ), NULL, this );
		if (date_[i] == selected_) set_bold_font(btn);
		btn_date_map_[btn] = date_[i];
		btn->Enable(enable_[i]);
		grid->Add( btn, 0, wxALL, 5 );
	}
	this->Layout();
	main_sizer->Fit( this );
	CenterOnParent();
	ok_btn->Enable(!selected_.is_not_a_date());

	ok_btn->SetLabel(locale->get("global", "OK_BNT", "Ok"));
	cancel_btn->SetLabel(locale->get("global", "CANCEL_BTN", "Cancel"));
	su_st->SetLabel(locale->get("global", "Su", "Su"));
	mo_st->SetLabel(locale->get("global", "Mo", "Mo"));
	tu_st->SetLabel(locale->get("global", "Tu", "Tu"));
	we_st->SetLabel(locale->get("global", "We", "We"));
	th_st->SetLabel(locale->get("global", "Th", "Th"));
	ft_st->SetLabel(locale->get("global", "Fr", "Fr"));
	sa_st->SetLabel(locale->get("global", "Sa", "Sa"));
}

select_date_f::~select_date_f() {
	for (std::map<wxButton*, boost::gregorian::date>::const_iterator i=btn_date_map_.begin(); i!=btn_date_map_.end(); ++i) {
		if (i->first != 0) 
			i->first->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( select_date_f::btn_click ), NULL, this );
	}
}

void select_date_f::btn_click( wxCommandEvent& event ) {
	wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
	if (btn == 0) return;
	for (std::map<wxButton*, boost::gregorian::date>::const_iterator i=btn_date_map_.begin(); i!=btn_date_map_.end(); ++i) {
		if (i->first != 0) 
			set_normal_font(i->first);
	}
	set_bold_font(btn);
	selected_ = btn_date_map_[btn];
	ok_btn->Enable(true);
}

void select_date_f::ok_btnClick( wxCommandEvent& event ) {
	EndModal(wxID_OK);
}

void select_date_f::cancel_btnClick( wxCommandEvent& event ) {
	EndModal(wxID_CANCEL);
}

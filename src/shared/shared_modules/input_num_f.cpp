#include "input_num_f.h"
#include <wx_util/wx_util.hpp>
#include <boost/lexical_cast.hpp>
#include <ksi_include/ksi_exceptions.hpp>
#include <lang/xml_lang.h>
#include <memory>
#include <wx/msgdlg.h>
#include <ksi_include/algo_functor.hpp>
#include <boost/function.hpp>

extern std::auto_ptr<ksi_cafe::xml_lang_t> locale;

input_num_f::input_num_f( wxWindow* parent, const wxString& title, const ksi_cafe::base_format_t& format )
:
base_input_num_f( parent ), currency_(false)
{
	SetLabel(title);
	clear_btn->SetLabel(locale->get("input_num_f", "clear_btn", "Clear"));
	ok_btn->SetLabel(locale->get("input_num_f", "ok_btn", "Ok"));
	cancel_btn->SetLabel(locale->get("input_num_f", "cancel_btn", "Cancel"));
	CentreOnParent();
	format_.reset(format.clone());
}

input_num_f::input_num_f( wxWindow* parent, const wxString& title, bool currency_)
:
base_input_num_f( parent ), currency_(currency_)
{
	SetLabel(title);
	clear_btn->SetLabel(locale->get("input_num_f", "clear_btn", "Clear"));
	ok_btn->SetLabel(locale->get("input_num_f", "ok_btn", "Ok"));
	cancel_btn->SetLabel(locale->get("input_num_f", "cancel_btn", "Cancel"));
	CentreOnParent();
	format_.reset(new ksi_cafe::identical_format_t());
	if (currency_) ed->SetValue(locale->get_currency(0));
	else ed->SetValue(_(""));
}

void input_num_f::onChar( wxKeyEvent& event ) {
	if (event.GetUnicodeKey() == WXK_TAB) {
	}
	else if (event.GetUnicodeKey() == WXK_ESCAPE) {
		wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
		evt.SetEventObject( clear_btn );
		clear_btn->GetEventHandler()->ProcessEvent( evt );
	}
	else if (event.GetUnicodeKey() == wxChar('0') && num0_btn->IsEnabled()) {
		wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
		evt.SetEventObject( num0_btn );
		num0_btn->GetEventHandler()->ProcessEvent( evt );
	}
	else if (event.GetUnicodeKey() == wxChar('1') && num1_btn->IsEnabled()) {
		wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
		evt.SetEventObject( num1_btn );
		num1_btn->GetEventHandler()->ProcessEvent( evt );
	}
	else if (event.GetUnicodeKey() == wxChar('2') && num2_btn->IsEnabled()) {
		wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
		evt.SetEventObject( num2_btn );
		num2_btn->GetEventHandler()->ProcessEvent( evt );
	}
	else if (event.GetUnicodeKey() == wxChar('3') && num3_btn->IsEnabled()) {
		wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
		evt.SetEventObject( num3_btn );
		num3_btn->GetEventHandler()->ProcessEvent( evt );
	}
	else if (event.GetUnicodeKey() == wxChar('4') && num4_btn->IsEnabled()) {
		wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
		evt.SetEventObject( num4_btn );
		num4_btn->GetEventHandler()->ProcessEvent( evt );
	}
	else if (event.GetUnicodeKey() == wxChar('5') && num5_btn->IsEnabled()) {
		wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
		evt.SetEventObject( num5_btn );
		num5_btn->GetEventHandler()->ProcessEvent( evt );
	}
	else if (event.GetUnicodeKey() == wxChar('6') && num6_btn->IsEnabled()) {
		wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
		evt.SetEventObject( num6_btn );
		num6_btn->GetEventHandler()->ProcessEvent( evt );
	}
	else if (event.GetUnicodeKey() == wxChar('7') && num7_btn->IsEnabled()) {
		wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
		evt.SetEventObject( num7_btn );
		num7_btn->GetEventHandler()->ProcessEvent( evt );
	}
	else if (event.GetUnicodeKey() == wxChar('8') && num8_btn->IsEnabled()) {
		wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
		evt.SetEventObject( num8_btn );
		num8_btn->GetEventHandler()->ProcessEvent( evt );
	}
	else if (event.GetUnicodeKey() == wxChar('9') && num9_btn->IsEnabled()) {
		wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
		evt.SetEventObject( num9_btn );
		num9_btn->GetEventHandler()->ProcessEvent( evt );
	}
}

void input_num_f::btnClick( wxCommandEvent& event )
{
	// TODO: Implement btnClick
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, clear_btn)));
	wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
	if (btn == 0) return;
	if (!currency_) ed->SetValue(to_uc(format_->format(to_mb(to_uc(format_->reverse(to_mb(ed->GetValue())))+btn->GetLabel()))));
	else {
		double touch_val = 0;
		if (btn == num1_btn) touch_val = 1;
		else if (btn == num2_btn) touch_val = 2;
		else if (btn == num3_btn) touch_val = 3;
		else if (btn == num4_btn) touch_val = 4;
		else if (btn == num5_btn) touch_val = 5;
		else if (btn == num6_btn) touch_val = 6;
		else if (btn == num7_btn) touch_val = 7;
		else if (btn == num8_btn) touch_val = 8;
		else if (btn == num9_btn) touch_val = 9;
		double val = locale->reverse_currency(ed->GetValue());
		ed->SetValue(locale->get_currency(val*10 + touch_val/100));
	}
	ok_btn->Enable(true);
}

void input_num_f::clear_btnClick( wxCommandEvent& event )
{
	// TODO: Implement clear_btnClick
	if (!currency_) ed->SetValue(_(""));
	else ed->SetValue(locale->get_currency(0));
	ok_btn->Enable(false);
}

void input_num_f::ok_btnClick( wxCommandEvent& event )
{
	// TODO: Implement ok_btnClick
	EndModal(wxID_OK);
}

void input_num_f::cancel_btnClick( wxCommandEvent& event )
{
	// TODO: Implement cancel_btnClick
	EndModal(wxID_CANCEL);
}

void input_num_f::set_format_value(const wxString& val) {
	ed->SetValue(val);
}

void input_num_f::set_unformat_value(const wxString& val) {
	ed->SetValue(to_uc(format_->format(to_mb(val))));
}

wxString input_num_f::format_text() const {
	return ed->GetValue();
}

wxString input_num_f::unformat_text() const {
	return to_uc(format_->reverse(to_mb(ed->GetValue())));
}

double input_num_f::value() const {
	if (!currency_) THROW_LOCATION(0, "double input_num_f::value() const");
	return locale->reverse_currency(ed->GetValue());
}

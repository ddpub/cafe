#include "auth_f.h"
#include <wx/msgdlg.h>
#include <wx/dcscreen.h>
#include <string>
#include <boost/lexical_cast.hpp>
#include <miracle/convert.hpp>
#include "cashier_ico.h"
#include <memory>
#include <lang/xml_lang.h>
extern std::auto_ptr<ksi_cafe::xml_lang_t> locale;

auth_f::auth_f( wxWindow* parent, const wxString& form_caption, const wxString& cc_main_caption_, const wxString& cc_other_caption_)
:base_auth_f( parent ), cc_main_caption_(cc_main_caption_), cc_other_caption_(cc_other_caption_), active_(0), kbd_showed_(false), req_version_(true)
{
	m_cc_bmp->SetBitmap(CashierBitmapsFunc(1));
	ok_btn->SetLabel(locale->get("auth_f", "ok_btn", "Ok"));
	cancel_btn->SetLabel(locale->get("auth_f", "cancel_btn", "Cancel"));
	message_lb->SetLabel(cc_main_caption_);
	show_kbd_btn->SetLabel(locale->get("auth_f", "show_kbd_btn1", "Show keyboard"));
	login_lb->SetLabel(locale->get("auth_f", "login_lb", "login"));
	password_lb->SetLabel(locale->get("auth_f", "password_lb", "password"));
	host_lb->SetLabel(locale->get("auth_f", "host_lb", "host"));
	SetLabel(form_caption);
	half_show();
	this->Connect( EVT_VKBD, wxObjectEventFunction( &auth_f::vkbdClick ), NULL, this );
}

auth_f::~auth_f() {
	this->Disconnect( EVT_VKBD, wxObjectEventFunction( &auth_f::vkbdClick ), NULL, this );
}

void auth_f::vkbdClick( vkbdEvent& event ) {
	if (event.v_code() == 0) {
		if (active_ == login_ed) {
			login_ed->WriteText(event.buff());
		}
		else if (active_ == password_ed) {
			password_ed->WriteText(event.buff());
		}
		else if (active_ == host_ed) {
			host_ed->WriteText(event.buff());
		}
	}
	else if (event.v_code() == WXK_BACK ) {
		if (active_ == login_ed) {
			long from = login_ed->GetInsertionPoint() - 1;
			long to = login_ed->GetInsertionPoint();
			login_ed->Remove(from, to);
		}
		else if (active_ == password_ed) {
			long from = password_ed->GetInsertionPoint() - 1;
			long to = password_ed->GetInsertionPoint();
			password_ed->Remove(from, to);
		}
		else if (active_ == host_ed) {
			long from = host_ed->GetInsertionPoint() - 1;
			long to = host_ed->GetInsertionPoint();
			host_ed->Remove(from, to);
		}		
	}
	else if (event.v_code() == WXK_RETURN) {
		wxCommandEvent btn_click_event( wxEVT_COMMAND_BUTTON_CLICKED, event.GetId());
		ok_btnClick(btn_click_event);		
	}
}

void auth_f::login_edSetFocus( wxFocusEvent& event ) {
	active_ = login_ed;
}

void auth_f::password_edSetFocus( wxFocusEvent& event ) {
	active_ = password_ed;
}

void auth_f::host_edSetFocus( wxFocusEvent& event ) {
	active_ = host_ed;
}

void auth_f::onChar( wxKeyEvent& event ) {
	// TODO: Implement onChar
	if (event.GetKeyCode() == WXK_TAB) {
		if (event.GetId() == ok_btn->GetId() || event.GetId() == cancel_btn->GetId() || event.GetId() == show_kbd_btn->GetId() || event.GetId() == advanced_btn->GetId()) {
			ok_btn->Navigate();
		}
	}
	else if (event.GetKeyCode() == WXK_RETURN || event.GetKeyCode() == WXK_NUMPAD_ENTER) {
		if (half_show_ && catch_return_) {
			catch_return_ = false;
			if (!req_version_) 
				wxMessageBox(_("This magnetic card has obsolete data format, and it is no longer supported.  Please renew card using HR Manager v1.0.1.15 or newer. Or use virtual keyboard to type login and password."), _("Message"), wxOK, this);
			if (req_version_ && check()) EndModal(wxID_OK);
		}
		else if (event.GetId() == ok_btn->GetId()) {
			wxCommandEvent btn_click_event( wxEVT_COMMAND_BUTTON_CLICKED, event.GetId());
			ok_btnClick(btn_click_event);
		}
		else if (event.GetId() == cancel_btn->GetId()) {
			wxCommandEvent btn_click_event( wxEVT_COMMAND_BUTTON_CLICKED, event.GetId());
			cancel_btnClick(btn_click_event);
		}
		else if (event.GetId() == show_kbd_btn->GetId()) {
			wxCommandEvent btn_click_event( wxEVT_COMMAND_BUTTON_CLICKED, event.GetId());
			show_kbd_btnClick(btn_click_event);
		}
		else if (event.GetId() == advanced_btn->GetId()) {
			wxCommandEvent btn_click_event( wxEVT_COMMAND_BUTTON_CLICKED, event.GetId());
			advanced_btnClick(btn_click_event);
		}
	}
	else if (half_show_ && card_reading_) {
		if (event.GetUnicodeKey() == wxChar('?')) card_reading_ = false;
		buff_ += event.GetUnicodeKey();
		if (!card_reading_) {
			catch_return_ = true;
			if (correct_ksi_record(to_mb(buff_))) {
				if (ksi_version(to_mb(buff_)) != 2) req_version_ = false;
				else {
					req_version_ = true;
					std::string login = to_mb(buff_.Mid(5, 14));
					login = unconv_login(login);
					std::string password = to_mb(buff_.Mid(19, buff_.Len()-19-1));
					password = unconv_password(password);
					login_ed->SetValue(to_uc(login));
					password_ed->SetValue(to_uc(password));
				}
			}
		}
	}
	else if (half_show_ && !card_reading_) {
		card_reading_ = true;
		buff_ = _("");
		buff_ += event.GetUnicodeKey();
	}
	else
		event.Skip();
}

void auth_f::onKeyDown( wxKeyEvent& event )
{
	// TODO: Implement onKeyDown
	if (event.GetKeyCode() == WXK_ESCAPE) {
		EndModal(wxID_CANCEL);
	}
	else
		event.Skip();
}

void auth_f::ok_btnClick( wxCommandEvent& event )
{
	// TODO: Implement ok_btnClick
	kbd_form.reset();
	if (check() && req_version_) EndModal(wxID_OK);
}

void auth_f::cancel_btnClick( wxCommandEvent& event )
{
	// TODO: Implement cancel_btnClick
	EndModal(wxID_CANCEL);
}

void auth_f::show_kbd_btnClick( wxCommandEvent& event )
{
	// TODO: Implement sk_btnClick
	if (!kbd_showed_) {
		kbd_form.reset(new text_f(this, _(""), _(""), 0, true));
		kbd_form->Show();
		wxScreenDC screen;
		if (GetPosition().y + GetSize().GetHeight() + kbd_form->GetSize().GetHeight() > screen.GetSize().GetHeight()) {
			Move(GetPosition().x, screen.GetSize().GetHeight() - GetSize().GetHeight() - kbd_form->GetSize().GetHeight());
		}
		kbd_form->Move(kbd_form->GetPosition().x, GetPosition().y + GetSize().GetHeight());
		show_kbd_btn->SetLabel(locale->get("auth_f", "show_kbd_btn2", "Hide keyboard"));
		//show_kbd_btn->SetLabel(_("Hide keyboard"));
		kbd_showed_ = true;
	}
	else {
		kbd_form.reset();
		show_kbd_btn->SetLabel(locale->get("auth_f", "show_kbd_btn1", "Show keyboard"));
		//show_kbd_btn->SetLabel(_("Show keyboard"));
		kbd_showed_ = false;
	}
}

void auth_f::advanced_btnClick( wxCommandEvent& event )
{
	// TODO: Implement
	curr_show();
}

void auth_f::full_show() {
	advanced_btn->SetLabel(locale->get("auth_f", "advanced_btn2", "Advanced <<"));
	//advanced_btn->SetLabel(_("Advanced <<"));
	ok_btn->Show(true);
	show_kbd_btn->Show(true);
	auth_sizerFG->Show(true);
	half_show_ = false;
	btn_sizeGB->SetItemPosition(cancel_btn, wxGBPosition(2,0));
	btn_sizeGB->SetItemPosition(ok_btn, wxGBPosition(0,0));
	btn_sizeGB->SetItemPosition(show_kbd_btn, wxGBPosition(1,0));
	btn_sizeGB->SetItemPosition(advanced_btn, wxGBPosition(1,1));
	btn_sizeGB->SetItemPosition(cancel_btn, wxGBPosition(0,1));
	message_lb->SetLabel(cc_other_caption_);
	main_sizerV->Fit(this);
	if (login_ed->GetValue() != _("") && host_ed->GetValue() != _("")) password_ed->SetFocus();
	else login_ed->SetFocus();
}

void auth_f::half_show() {
	advanced_btn->SetLabel(locale->get("auth_f", "advanced_btn1", "Advanced >>"));
	//advanced_btn->SetLabel(_("Advanced >>"));
	ok_btn->Show(false);
	show_kbd_btn->Show(false);
	auth_sizerFG->Show(false);
	half_show_ = true;
	btn_sizeGB->SetItemPosition(ok_btn, wxGBPosition(2,0));
	btn_sizeGB->SetItemPosition(cancel_btn, wxGBPosition(0,0));
	btn_sizeGB->SetItemPosition(advanced_btn, wxGBPosition(0,1));
	btn_sizeGB->SetItemPosition(show_kbd_btn, wxGBPosition(1,1));
	btn_sizeGB->SetItemPosition(ok_btn, wxGBPosition(1,0));
	message_lb->SetLabel(cc_main_caption_);
	main_sizerV->Fit(this); 
}

void auth_f::curr_show() {
	card_reading_ = false;
	if (half_show_) {
		full_show();
		return;
	}
	half_show();
}

bool auth_f::check() {
	if (login_ed->GetValue() == _("")) {
		wxMessageBox(locale->get("auth_f", "empty_login_msg", "Empty login is not allowed"), _("Message"), wxOK, this);
	}
	else if (password_ed->GetValue() == _("")) {
		wxMessageBox(locale->get("auth_f", "empty_password_msg", "Empty password is not allowed"), _("Message"), wxOK, this);
	}
	else if (host_ed->GetValue() == _("")) {
		wxMessageBox(locale->get("auth_f", "empty_host_msg", "Empty host is not allowed"), _("Message"), wxOK, this);
	}
	else {
		return true;
	}
	return false;
}

wxString auth_f::get_host(wxString host_str)
{
	int pos = host_str.Find(_(":"));
	if (pos == -1) return host_str;
	return host_str.Mid(0, pos);
}

int auth_f::get_port(wxString host_str)
{
	try
	{
		int pos = host_str.Find(_(":"));
		if (pos == -1) return 1521;
		int pos1 = host_str.find(_(":"), pos+1);
		if (pos1 == -1) return boost::lexical_cast<int>(to_mb(host_str.Mid(pos+1)));
		return boost::lexical_cast<int>(to_mb(host_str.Mid(pos+1, pos1-pos-1)));
	}
	catch(boost::bad_lexical_cast &e)
	{
		return 1521;
	}
}

wxString auth_f::get_sid(wxString host_str)
{
	int pos = host_str.Find(_(":"));
	if (pos == -1) return _("cafe");
	int pos1 = host_str.find(_(":"), pos+1);
	if (pos1 == -1) 
	{
		try
		{
			std::string sstr = to_mb(host_str.Mid(pos+1));
			int tmp = boost::lexical_cast<int>(to_mb(host_str.Mid(pos+1)));
			return _("cafe");
		}
		catch(boost::bad_lexical_cast &e)
		{
			return host_str.Mid(pos+1);
		}
	}
	return host_str.Mid(pos1+1);
}

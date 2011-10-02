#include "text_f.h"
#include <wx/msgdlg.h>
#include <wx/defs.h>
#include <lang/xml_lang.h>
#include <memory>

extern std::auto_ptr<ksi_cafe::xml_lang_t> locale;


text_f::text_f( wxWindow* parent, const wxString& title, const wxString& text_title, bool need_amount_, bool passwd_)
:
base_text_f( parent ), kbd_(1), kbd_case_(0), active_(text_ed), need_amount_(need_amount_), passwd_(passwd_)
{
	SetLabel(title);
	text_sizer->GetStaticBox()->SetLabel(text_title);
	amount_sizer->GetStaticBox()->SetLabel(locale->get("text_f", "amount_sizer", "Amount"));
	ok_btn->SetLabel(locale->get("text_f", "ok_btn", "Ok"));
	cancel_btn->SetLabel(locale->get("text_f", "cancel_btn", "Cancel"));
	amount_lb->SetLabel(locale->get("text_f", "amount_lb", "Amount"));
	space_btn->SetLabel(locale->get("text_f", "space_btn", "Space"));
	
	us_low_[m_button1] = _("1");
	us_up_[m_button1] = _("!");
	us_low_[m_button2] = _("2");
	us_up_[m_button2] = _("@");
	us_low_[m_button3] = _("3");
	us_up_[m_button3] = _("#");
	us_low_[m_button4] = _("4");
	us_up_[m_button4] = _("$");
	us_low_[m_button5] = _("5");
	us_up_[m_button5] = _("%");
	us_low_[m_button6] = _("6");
	us_up_[m_button6] = _("^");
	us_low_[m_button7] = _("7");
	us_up_[m_button7] = _("&&");
	us_low_[m_button8] = _("8");
	us_up_[m_button8] = _("*");
	us_low_[m_button9] = _("9");
	us_up_[m_button9] = _("(");
	us_low_[m_button0] = _("0");
	us_up_[m_button0] = _(")");
	us_low_[m_button10] = _("-");
	us_up_[m_button10] = _("_");
	us_low_[m_button11] = _("=");
	us_up_[m_button11] = _("+");
	us_low_[m_buttonq] = _("q");
	us_up_[m_buttonq] = _("Q");
	us_low_[m_buttonw] = _("w");
	us_up_[m_buttonw] = _("W");
	us_low_[m_buttone] = _("e");
	us_up_[m_buttone] = _("E");
	us_low_[m_buttonr] = _("r");
	us_up_[m_buttonr] = _("R");
	us_low_[m_buttont] = _("t");
	us_up_[m_buttont] = _("T");
	us_low_[m_buttony] = _("y");
	us_up_[m_buttony] = _("Y");
	us_low_[m_buttonu] = _("u");
	us_up_[m_buttonu] = _("U");
	us_low_[m_buttoni] = _("i");
	us_up_[m_buttoni] = _("I");
	us_low_[m_buttono] = _("o");
	us_up_[m_buttono] = _("O");
	us_low_[m_buttonp] = _("p");
	us_up_[m_buttonp] = _("P");
	us_low_[m_buttona] = _("a");
	us_up_[m_buttona] = _("A");
	us_low_[m_buttons] = _("s");
	us_up_[m_buttons] = _("S");
	us_low_[m_buttond] = _("d");
	us_up_[m_buttond] = _("D");
	us_low_[m_buttonf] = _("f");
	us_up_[m_buttonf] = _("F");
	us_low_[m_buttong] = _("g");
	us_up_[m_buttong] = _("G");
	us_low_[m_buttonh] = _("h");
	us_up_[m_buttonh] = _("H");
	us_low_[m_buttonj] = _("j");
	us_up_[m_buttonj] = _("J");
	us_low_[m_buttonk] = _("k");
	us_up_[m_buttonk] = _("K");
	us_low_[m_buttonl] = _("l");
	us_up_[m_buttonl] = _("L");
	us_low_[m_buttonz] = _("z");
	us_up_[m_buttonz] = _("Z");
	us_low_[m_buttonx] = _("x");
	us_up_[m_buttonx] = _("X");
	us_low_[m_buttonc] = _("c");
	us_up_[m_buttonc] = _("C");
	us_low_[m_buttonv] = _("v");
	us_up_[m_buttonv] = _("V");
	us_low_[m_buttonb] = _("b");
	us_up_[m_buttonb] = _("B");
	us_low_[m_buttonn] = _("n");
	us_up_[m_buttonn] = _("N");
	us_low_[m_buttonm] = _("m");
	us_up_[m_buttonm] = _("M");
	us_low_[m_button12] = _("[");
	us_up_[m_button12] = _("{");
	us_low_[m_button13] = _("]");
	us_up_[m_button13] = _("}");
	us_low_[m_button14] = _(";");
	us_up_[m_button14] = _(":");
	us_low_[m_button15] = _("'");
	us_up_[m_button15] = _("\"");
	us_low_[m_button16] = _("\\");
	us_up_[m_button16] = _("|");
	us_low_[m_button17] = _(",");
	us_up_[m_button17] = _("<");
	us_low_[m_button18] = _(".");
	us_up_[m_button18] = _(">");
	us_low_[m_button19] = _("/");
	us_up_[m_button19] = _("?");
	
	/*
	ru_low_[m_button1] = _("1");
	ru_up_[m_button1] = _("!");
	ru_low_[m_button2] = _("2");
	ru_up_[m_button2] = _("\"");
	ru_low_[m_button3] = _("3");
	ru_up_[m_button3] = _("№");
	ru_low_[m_button4] = _("4");
	ru_up_[m_button4] = _(";");
	ru_low_[m_button5] = _("5");
	ru_up_[m_button5] = _("%");
	ru_low_[m_button6] = _("6");
	ru_up_[m_button6] = _(":");
	ru_low_[m_button7] = _("7");
	ru_up_[m_button7] = _("?");
	ru_low_[m_button8] = _("8");
	ru_up_[m_button8] = _("*");
	ru_low_[m_button9] = _("9");
	ru_up_[m_button9] = _("(");
	ru_low_[m_button0] = _("0");
	ru_up_[m_button0] = _(")");
	ru_low_[m_button10] = _("-");
	ru_up_[m_button10] = _("_");
	ru_low_[m_button11] = _("=");
	ru_up_[m_button11] = _("+");
	ru_low_[m_buttonq] = _("й");
	ru_up_[m_buttonq] = _("Й");
	ru_low_[m_buttonw] = _("ц");
	ru_up_[m_buttonw] = _("Ц");
	ru_low_[m_buttone] = _("у");
	ru_up_[m_buttone] = _("У");
	ru_low_[m_buttonr] = _("к");
	ru_up_[m_buttonr] = _("К");
	ru_low_[m_buttont] = _("е");
	ru_up_[m_buttont] = _("Е");
	ru_low_[m_buttony] = _("н");
	ru_up_[m_buttony] = _("Н");
	ru_low_[m_buttonu] = _("г");
	ru_up_[m_buttonu] = _("Г");
	ru_low_[m_buttoni] = _("ш");
	ru_up_[m_buttoni] = _("Ш");
	ru_low_[m_buttono] = _("щ");
	ru_up_[m_buttono] = _("Щ");
	ru_low_[m_buttonp] = _("з");
	ru_up_[m_buttonp] = _("З");
	ru_low_[m_buttona] = _("ф");
	ru_up_[m_buttona] = _("Ф");
	ru_low_[m_buttons] = _("ы");
	ru_up_[m_buttons] = _("Ы");
	ru_low_[m_buttond] = _("в");
	ru_up_[m_buttond] = _("В");
	ru_low_[m_buttonf] = _("а");
	ru_up_[m_buttonf] = _("А");
	ru_low_[m_buttong] = _("п");
	ru_up_[m_buttong] = _("П");
	ru_low_[m_buttonh] = _("р");
	ru_up_[m_buttonh] = _("Р");
	ru_low_[m_buttonj] = _("о");
	ru_up_[m_buttonj] = _("О");
	ru_low_[m_buttonk] = _("л");
	ru_up_[m_buttonk] = _("Л");
	ru_low_[m_buttonl] = _("д");
	ru_up_[m_buttonl] = _("Д");
	ru_low_[m_buttonz] = _("я");
	ru_up_[m_buttonz] = _("Я");
	ru_low_[m_buttonx] = _("ч");
	ru_up_[m_buttonx] = _("Ч");
	ru_low_[m_buttonc] = _("с");
	ru_up_[m_buttonc] = _("С");
	ru_low_[m_buttonv] = _("м");
	ru_up_[m_buttonv] = _("М");
	ru_low_[m_buttonb] = _("и");
	ru_up_[m_buttonb] = _("�?");
	ru_low_[m_buttonn] = _("т");
	ru_up_[m_buttonn] = _("Т");
	ru_low_[m_buttonm] = _("ь");
	ru_up_[m_buttonm] = _("Ь");
	ru_low_[m_button12] = _("х");
	ru_up_[m_button12] = _("Х");
	ru_low_[m_button13] = _("ъ");
	ru_up_[m_button13] = _("Ъ");
	ru_low_[m_button14] = _("ж");
	ru_up_[m_button14] = _("Ж");
	ru_low_[m_button15] = _("э");
	ru_up_[m_button15] = _("Э");
	ru_low_[m_button16] = _("\\");
	ru_up_[m_button16] = _("/");
	ru_low_[m_button17] = _("б");
	ru_up_[m_button17] = _("Б");
	ru_low_[m_button18] = _("ю");
	ru_up_[m_button18] = _("Ю");
	ru_low_[m_button19] = _(".");
	ru_up_[m_button19] = _(",");
	*/
	
	apply_kbd();
	
	if (need_amount_) {
		amount_ed->SetValue(locale->get_currency(0));
		group_sizer->Show(amount_sizer, true, true);
		amount_sizer->GetStaticBox()->SetLabel(locale->get("text_f", "amount_sizer", "Amount"));
		amount_ed->Show();
		amount_lb->Show();
	}
	else {
		group_sizer->Show(amount_sizer, false, true);
		amount_sizer->GetStaticBox()->SetLabel(_(""));
		amount_ed->Hide();
		amount_lb->Hide();
	}
	if (passwd_) { 
		text_ed->SetWindowStyle(text_ed->GetWindowStyleFlag()|wxTE_PASSWORD);
		text_ed->Refresh();
		group_sizer->Show(text_sizer, false, true);
		text_ed->Hide();
		ok_btn->Hide();
		cancel_btn->SetLabel(locale->get("text_f", "close_btn", "Close"));
		bottom_sizer->Add( 0, 0, 1, wxEXPAND, 5 );
	}
	amount_sizer->Fit( this );
	text_sizer->Fit( this );
	main_bSizer->Fit( this );
	CentreOnParent();
}

void text_f::btnClick( wxCommandEvent& event )
{
	// TODO: Implement btnClick
	wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
	if (btn == 0) return;
	if (active_ == text_ed) {
		if (btn->GetLabel() == _("&&")) text_ed->WriteText(_("&"));
		else text_ed->WriteText(btn->GetLabel());
	}
	else if (active_ == amount_ed && need_amount_) {
		wxString sval = btn->GetLabel();
		amount_ed->SetInsertionPoint(amount_ed->GetLastPosition());
		int val = -1;
		if (sval == _("1")) val = 1;
		else if (sval == _("2")) val = 2;
		else if (sval == _("3")) val = 3;
		else if (sval == _("4")) val = 4;
		else if (sval == _("5")) val = 5;
		else if (sval == _("6")) val = 6;
		else if (sval == _("7")) val = 7;
		else if (sval == _("8")) val = 8;
		else if (sval == _("9")) val = 9;
		else if (sval == _("0")) val = 0;
		
		if (val != -1) {
			double amount = locale->reverse_currency(amount_ed->GetValue());
			if (amount > 999999) return;
			amount = amount*10+double(val)/100;
			amount_ed->SetValue(locale->get_currency(amount));
		}
	}
	if (passwd_) {
		vkbdEvent throw_event(btn->GetLabel(), 0);
		GetParent()->GetEventHandler()->ProcessEvent(throw_event);
	}
}

void text_f::bs_btnClick( wxCommandEvent& event )
{
	// TODO: Implement bs_btnClick
	if (active_ == text_ed) {
		long from = text_ed->GetInsertionPoint() - 1;
		long to = text_ed->GetInsertionPoint();
		text_ed->Remove(from, to);
	}
	else if (active_ ==  amount_ed && need_amount_) {
		amount_ed->SetValue(locale->get_currency(0));
	}
	if (passwd_) {
		vkbdEvent throw_event(_(""), WXK_BACK);
		GetParent()->GetEventHandler()->ProcessEvent(throw_event);
	}
}

void text_f::enter_btnClick( wxCommandEvent& event )
{
	// TODO: Implement enter_btnClick
	wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
	if (btn == 0) return;
	if (active_ == text_ed) {
		text_ed->WriteText(_("\n"));
	}
	if (passwd_) {
		vkbdEvent throw_event(_(""), WXK_RETURN);
		GetParent()->GetEventHandler()->ProcessEvent(throw_event);
	}
}

void text_f::shift_btnClick( wxCommandEvent& event )
{
	// TODO: Implement shift_btnClick
	wxToggleButton* btn = dynamic_cast<wxToggleButton*>(event.GetEventObject());
	if (btn == 0) return;
	if (btn->GetValue()) {
		shift1_btn->SetValue(true);
		shift2_btn->SetValue(true);
		kbd_case_ = 1;
	}
	else {
		shift1_btn->SetValue(false);
		shift2_btn->SetValue(false);
		kbd_case_ = 0;
	}
	apply_kbd();
}

void text_f::kbd_btnClick( wxCommandEvent& event )
{
	// TODO: Implement kbd_btnClick
	if (kbd_ == 1) {
		kbd_ = 2;
	}
	else {
		kbd_ = 1;
	}
	apply_kbd();
}

void text_f::space_btnClick( wxCommandEvent& event )
{
	// TODO: Implement space_btnClick
	wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
	if (btn == 0) return;
	if (active_ == text_ed) {
		text_ed->WriteText(_(" "));
	}
	if (passwd_) {
		vkbdEvent throw_event(_(" "), WXK_RETURN);
		GetParent()->GetEventHandler()->ProcessEvent(throw_event);
	}
}

void text_f::apply_kbd() {
  space_btn->SetLabel(locale->get("text_f", "space_btn", "Space"));
	if (kbd_ == 1 && kbd_case_ == 0) {
		for (std::map<wxButton*, wxString>::const_iterator i=us_low_.begin(); i!=us_low_.end(); ++i) {
			i->first->SetLabel(i->second);
		}
		kbd_btn->SetLabel(_("us"));
    space_btn->SetLabel(_("Space"));
	}
	else if (kbd_ == 1 && kbd_case_ == 1) {
		for (std::map<wxButton*, wxString>::const_iterator i=us_up_.begin(); i!=us_up_.end(); ++i) {
			i->first->SetLabel(i->second);
		}
		kbd_btn->SetLabel(_("US"));
    space_btn->SetLabel(_("Space"));
	}
	else if (kbd_ == 2 && kbd_case_ == 0) {
		for (std::map<wxButton*, wxString>::const_iterator i=us_low_.begin(); i!=us_low_.end(); ++i) {
			//wxMessageBox(i->second + _(" : ") + locale->get_key(to_mb(i->second)));
			i->first->SetLabel(locale->get_key(to_mb(i->second)));
		}
		wxString kbd_name = locale->get_keyboard_name();
		kbd_btn->SetLabel(kbd_name.MakeLower());
	}
	else if (kbd_ == 2 && kbd_case_ == 1) {
		for (std::map<wxButton*, wxString>::const_iterator i=us_up_.begin(); i!=us_up_.end(); ++i) {
			i->first->SetLabel(locale->get_key(to_mb(i->second)));
		}
		wxString kbd_name = locale->get_keyboard_name();
		kbd_btn->SetLabel(kbd_name.MakeUpper());
	}
}

void text_f::ok_btnClick( wxCommandEvent& event ) {
	EndModal(wxID_OK);
}

void text_f::cancel_btnClick( wxCommandEvent& event ) {
	if (passwd_) Close(); 
	else EndModal(wxID_CANCEL);
}

void text_f::text_edFocus( wxFocusEvent& event ) {
	active_ = text_ed;
}

void text_f::amount_edFocus( wxFocusEvent& event ) {
	active_ = amount_ed;
}

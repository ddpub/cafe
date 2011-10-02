#include "calc_f.h"
#include <wx_util/wx_util.hpp>
#include <wx/msgdlg.h>
#include <memory>
#include <ksi_util/util.hpp>
#include <lang/xml_lang.h>
#include <wx_util/xml_config.hpp>
#include <ksi_include/algo_functor.hpp>
#include <boost/function.hpp>

extern std::auto_ptr<ksi_cafe::xml_lang_t> locale;
extern std::auto_ptr<xml_config_t> config;

calc_f::calc_f( wxWindow* parent, const wxString& title, double total_, bool prepayment_ )
:
base_calc_f( parent ), total_(total_), prepayment_(prepayment_)
{
	frac_ = boost::lexical_cast<size_t>(config->get("app", "frac"));
	cash_tendered_lb->SetLabel(locale->get("calc_f", "cash_tendered_lb", "Cash tendered"));
	order_total_lb->SetLabel(locale->get("calc_f", "order_total_lb", "Order total"));
	change_lb->SetLabel(locale->get("calc_f", "change_lb", "Change"));
	exact_amount_btn->SetLabel(locale->get("calc_f", "exact_amount_btn", "Exact amount"));
	wrap_label(exact_amount_btn);
	process_btn->SetLabel(locale->get("calc_f", "process_btn", "Commit processing"));
	wrap_label(process_btn);
	cancel_btn->SetLabel(locale->get("calc_f", "cancel_btn", "Cancel"));
	wrap_label(cancel_btn);
	clear_btn->SetLabel(locale->get("calc_f", "clear_btn", "Clear"));
	wrap_label(clear_btn);
	
	if (locale->get_locale_name() != "") {
		speed_num100_btn->Hide();
		speed_num50_btn->Hide();
		speed_num20_btn->Hide();
		speed_num10_btn->Hide();
		speed_num5_btn->Hide();
		speed_num1_btn->Hide();
		speed_num05_btn->Hide();
		speed_num025_btn->Hide();
		speed_num01_btn->Hide();
		speed_num005_btn->Hide();
	}
	SetLabel(title);
	cash_tendered_ed->SetValue(locale->get_currency(0));
	order_total_ed->SetValue(locale->get_currency(total_));
	//change_ed->SetValue(currency->str(0));
	validControls(100000);
	this->SetSizer( main_sizer );
	this->Layout();
	main_sizer->Fit( this );
	
	CentreOnParent();
}

void calc_f::validControls(double maxPay) {
	double currPay = locale->reverse_currency(cash_tendered_ed->GetValue());
	num00_btn->Enable(currPay + diffMoney(num00_btn) < maxPay);
	num0_btn->Enable(currPay + diffMoney(num0_btn) < maxPay);
	num1_btn->Enable(currPay + diffMoney(num1_btn) < maxPay);
	num2_btn->Enable(currPay + diffMoney(num2_btn) < maxPay);
	num3_btn->Enable(currPay + diffMoney(num3_btn) < maxPay);
	num4_btn->Enable(currPay + diffMoney(num4_btn) < maxPay);
	num5_btn->Enable(currPay + diffMoney(num5_btn) < maxPay);
	num6_btn->Enable(currPay + diffMoney(num6_btn) < maxPay);
	num7_btn->Enable(currPay + diffMoney(num7_btn) < maxPay);
	num8_btn->Enable(currPay + diffMoney(num8_btn) < maxPay);
	num9_btn->Enable(currPay + diffMoney(num9_btn) < maxPay);
	speed_num005_btn->Enable(currPay + diffMoney(speed_num005_btn) < maxPay);
	speed_num01_btn->Enable(currPay + diffMoney(speed_num01_btn) < maxPay);
	speed_num025_btn->Enable(currPay + diffMoney(speed_num025_btn) < maxPay);
	speed_num05_btn->Enable(currPay + diffMoney(speed_num05_btn) < maxPay);
	speed_num1_btn->Enable(currPay + diffMoney(speed_num1_btn) < maxPay);
	speed_num5_btn->Enable(currPay + diffMoney(speed_num5_btn) < maxPay);
	speed_num10_btn->Enable(currPay + diffMoney(speed_num10_btn) < maxPay);
	speed_num20_btn->Enable(currPay + diffMoney(speed_num20_btn) < maxPay);
	speed_num50_btn->Enable(currPay + diffMoney(speed_num50_btn) < maxPay);
	speed_num100_btn->Enable(currPay + diffMoney(speed_num100_btn) < maxPay);
}

void calc_f::onChar( wxKeyEvent& event ) {
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
	else if (event.GetUnicodeKey() == wxChar('8') && num1_btn->IsEnabled()) {
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

void calc_f::onKeyDown( wxKeyEvent& event ) {
	event.Skip();
}

void calc_f::exact_amount_btnClick( wxCommandEvent& event )
{
	// TODO: Implement exact_amount_btnClick
	cash_tendered_ed->SetValue(locale->get_currency(total_));
}

void calc_f::process_btnClick( wxCommandEvent& event )
{
	// TODO: Implement process_btnClick
	if (prepayment_) {
		if (int(locale->reverse_currency(cash_tendered_ed->GetValue())*100) != 0) EndModal(wxID_OK);
	}
	else if (ksi_cafe::round(locale->reverse_currency(cash_tendered_ed->GetValue())) >= ksi_cafe::round(total_)) EndModal(wxID_OK);
}

void calc_f::cancel_btnClick( wxCommandEvent& event )
{
	// TODO: Implement cancel_btnClick
	EndModal(wxID_CANCEL);
}

double calc_f::diffMoney( wxButton* btn ) {
	double currVal = locale->reverse_currency(cash_tendered_ed->GetValue());
	double dem = 100; if (frac_ == 0) dem = 1; else if (frac_ == 1) dem = 10;
	
	if (btn == num00_btn) return currVal*100 - currVal;
	else if (btn == num0_btn) return currVal*10 - currVal;
	else if (btn == num1_btn) return (currVal*10 + 1/dem) - currVal;
	else if (btn == num2_btn) return (currVal*10 + 2/dem) - currVal;
	else if (btn == num3_btn) return (currVal*10 + 3/dem) - currVal;
	else if (btn == num4_btn) return (currVal*10 + 4/dem) - currVal;
	else if (btn == num5_btn) return (currVal*10 + 5/dem) - currVal;
	else if (btn == num6_btn) return (currVal*10 + 6/dem) - currVal;
	else if (btn == num7_btn) return (currVal*10 + 7/dem) - currVal;
	else if (btn == num8_btn) return (currVal*10 + 8/dem) - currVal;
	else if (btn == num9_btn) return (currVal*10 + 9/dem) - currVal;
	else if (btn == speed_num005_btn) return 0.05;
	else if (btn == speed_num01_btn) return 0.1;
	else if (btn == speed_num025_btn) return 0.25;
	else if (btn == speed_num05_btn) return 0.5;
	else if (btn == speed_num1_btn) return 1;
	else if (btn == speed_num5_btn) return 5;
	else if (btn == speed_num10_btn) return 10;
	else if (btn == speed_num20_btn) return 20;
	else if (btn == speed_num50_btn) return 50;
	else if (btn == speed_num100_btn) return 100;
	else return 0;
}

double calc_f::diffMoney( wxCommandEvent& event) {
	wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
	return diffMoney(btn);
}

void calc_f::num_btnClick( wxCommandEvent& event )
{
	// TODO: Implement num_btnClick
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, clear_btn)));
	cash_tendered_ed->SetValue(locale->get_currency(locale->reverse_currency(cash_tendered_ed->GetValue())+diffMoney(event)));
/*
	double val = locale->reverse_currency(cash_tendered_ed->GetValue());
	double touch_val = 0;
	wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
	if (btn == 0) return;
	if (btn == num00_btn) {
		cash_tendered_ed->SetValue(locale->get_currency(val*100));
	}
	else if (btn == num0_btn) {
		cash_tendered_ed->SetValue(locale->get_currency(val*10));
	}
	else if (btn == num1_btn) touch_val = 1;
	else if (btn == num2_btn) touch_val = 2;
	else if (btn == num3_btn) touch_val = 3;
	else if (btn == num4_btn) touch_val = 4;
	else if (btn == num5_btn) touch_val = 5;
	else if (btn == num6_btn) touch_val = 6;
	else if (btn == num7_btn) touch_val = 7;
	else if (btn == num8_btn) touch_val = 8;
	else if (btn == num9_btn) touch_val = 9;
	
	double dem = 100;
	if (frac_ == 0)  dem = 1;
	else if (frac_ == 1) dem = 10;
	if (touch_val != 0) cash_tendered_ed->SetValue(locale->get_currency(val*10 + touch_val/dem));
*/
}

void calc_f::clear_btnClick( wxCommandEvent& event )
{
	// TODO: Implement clear_btnClick
	cash_tendered_ed->SetValue(locale->get_currency(0));
	change_ed->SetValue(_(""));
}

void calc_f::speed_num_btnClick( wxCommandEvent& event )
{
	// TODO: Implement speed_num_btnClick
	cash_tendered_ed->SetValue(locale->get_currency(locale->reverse_currency(cash_tendered_ed->GetValue())+diffMoney(event)));
/*
	double val = locale->reverse_currency(cash_tendered_ed->GetValue());
	wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
	if (btn == 0) return;
	double touch_val = 0;
	if (btn == speed_num005_btn) touch_val = 0.05;
	else if (btn == speed_num01_btn) touch_val = 0.1;
	else if (btn == speed_num025_btn) touch_val = 0.25;
	else if (btn == speed_num05_btn) touch_val = 0.5;
	else if (btn == speed_num1_btn) touch_val = 1;
	else if (btn == speed_num5_btn) touch_val = 5;
	else if (btn == speed_num10_btn) touch_val = 10;
	else if (btn == speed_num20_btn) touch_val = 20;
	else if (btn == speed_num50_btn) touch_val = 50;
	else if (btn == speed_num100_btn) touch_val = 100;
	
	cash_tendered_ed->SetValue(locale->get_currency(val+touch_val));
*/
}

void calc_f::cash_tendered_edChange( wxCommandEvent& event )
{
	double val = locale->reverse_currency(cash_tendered_ed->GetValue());
	if (prepayment_) {
		if (ksi_cafe::round(val) == 0) process_btn->Enable(false);
		else {
			if (ksi_cafe::round(val) > ksi_cafe::round(total_)) cash_tendered_ed->SetValue(locale->get_currency(total_));
			process_btn->Enable(true);
		}
	}
	else {
		if (ksi_cafe::round(val) >= ksi_cafe::round(total_)) {
			change_ed->SetValue(locale->get_currency(ksi_cafe::round(val)-ksi_cafe::round(total_)));
			process_btn->Enable(true);
		}
		else {
			change_ed->SetValue(_(""));
			process_btn->Enable(false);
		}
	}
	validControls(100000);
}

double calc_f::amount() const {
	return ksi_cafe::round(locale->reverse_currency(cash_tendered_ed->GetValue()));
}

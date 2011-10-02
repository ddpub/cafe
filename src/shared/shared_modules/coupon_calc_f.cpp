#include "coupon_calc_f.h"
#include <wx/settings.h>
#include <wx/msgdlg.h>
#include <ksi_include/ksi_exceptions.hpp>
#include <ksi_util/util.hpp>
#include <lang/xml_lang.h>
#include <ksi_util/currency.hpp>
#include <ksi_include/algo_functor.hpp>
#include <boost/function.hpp>


extern std::auto_ptr<ksi_cafe::xml_lang_t> locale;

coupon_calc_f::coupon_calc_f( wxWindow* parent, double subtotal_, const wxString& cap1, const wxString& cap2 )
:
base_coupon_calc_f( parent ), subtotal_(subtotal_)
{
	procent_btn->SetLabel(cap1);
	money_btn->SetLabel(cap2);
	ok_btn->SetLabel(locale->get("coupon_calc_f", "ok_btn", "Ok"));
	cancel_btn->SetLabel(locale->get("coupon_calc_f", "cancel_btn", "Cancel"));
	clear_btn->SetLabel(locale->get("coupon_calc_f", "clear_btn", "Clear"));
	try
	{
		std::vector<wxButton*> type_group_vec;
		type_group_vec.push_back(procent_btn);
		type_group_vec.push_back(money_btn);

		type_toggle_group_ptr.reset
		(
		new toggle_group_t(procent_btn
	                   , type_group_vec
	                   , wxSystemSettings::GetColour( wxSYS_COLOUR_BTNSHADOW )
	                   , wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER )
	                   )
		);
		press_ = procent_btn;
		CentreOnParent();
		procent_ed->SetValue(to_uc(ksi_cafe::percent_t().format(0)));
		money_ed->SetValue(locale->get_currency(0));
	}
	catch (exception_t& ex)
	{
		wxMessageBox(to_uc(ex.comp_message()));
	}
	CentreOnParent();	
}

void coupon_calc_f::onChar( wxKeyEvent& event ) {
	if (event.GetUnicodeKey() == WXK_TAB) {
	}
	else if (event.GetUnicodeKey() == WXK_ESCAPE) {
		wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
		evt.SetEventObject( clear_btn );
		clear_btn->GetEventHandler()->ProcessEvent( evt );
	}
	else if (event.GetUnicodeKey() == wxChar('0') && num00_btn->IsEnabled()) {
		wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
		evt.SetEventObject( num00_btn );
		num00_btn->GetEventHandler()->ProcessEvent( evt );
	}
	else if (event.GetUnicodeKey() == wxChar('1') && num01_btn->IsEnabled()) {
		wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
		evt.SetEventObject( num01_btn );
		num01_btn->GetEventHandler()->ProcessEvent( evt );
	}
	else if (event.GetUnicodeKey() == wxChar('2') && num02_btn->IsEnabled()) {
		wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
		evt.SetEventObject( num02_btn );
		num02_btn->GetEventHandler()->ProcessEvent( evt );
	}
	else if (event.GetUnicodeKey() == wxChar('3') && num03_btn->IsEnabled()) {
		wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
		evt.SetEventObject( num03_btn );
		num03_btn->GetEventHandler()->ProcessEvent( evt );
	}
	else if (event.GetUnicodeKey() == wxChar('4') && num04_btn->IsEnabled()) {
		wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
		evt.SetEventObject( num04_btn );
		num04_btn->GetEventHandler()->ProcessEvent( evt );
	}
	else if (event.GetUnicodeKey() == wxChar('5') && num05_btn->IsEnabled()) {
		wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
		evt.SetEventObject( num05_btn );
		num05_btn->GetEventHandler()->ProcessEvent( evt );
	}
	else if (event.GetUnicodeKey() == wxChar('6') && num06_btn->IsEnabled()) {
		wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
		evt.SetEventObject( num06_btn );
		num06_btn->GetEventHandler()->ProcessEvent( evt );
	}
	else if (event.GetUnicodeKey() == wxChar('7') && num07_btn->IsEnabled()) {
		wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
		evt.SetEventObject( num07_btn );
		num07_btn->GetEventHandler()->ProcessEvent( evt );
	}
	else if (event.GetUnicodeKey() == wxChar('8') && num08_btn->IsEnabled()) {
		wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
		evt.SetEventObject( num08_btn );
		num08_btn->GetEventHandler()->ProcessEvent( evt );
	}
	else if (event.GetUnicodeKey() == wxChar('9') && num09_btn->IsEnabled()) {
		wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, GetId());
		evt.SetEventObject( num09_btn );
		num09_btn->GetEventHandler()->ProcessEvent( evt );
	}
}

void coupon_calc_f::type_btnClick( wxCommandEvent& event )
{
	// TODO: Implement type_btnClick
	type_toggle_group_ptr->toggle(event);
	press_ = event.GetEventObject();
	clear_btnClick(event);
}

void coupon_calc_f::editOnChar( wxKeyEvent& event )
{
	// TODO: Implement editKeyDown
	event.Skip();
}

void coupon_calc_f::num_btnClick( wxCommandEvent& event )
{
	// TODO: Implement num_btnClick
	destroyProcessor_t<boost::function<void ()> > restoreFocus(boost::function<void ()>(boost::bind(&wxWindow::SetFocus, clear_btn)));
	wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
	if (btn == 0) return;
	int num = boost::lexical_cast<int>(to_mb(btn->GetLabel()));
	try
	{
		if (press_ == procent_btn)
		{
			wxString str = procent_ed->GetValue();
			
			double val = ksi_cafe::percent_t().reverse(to_mb(str));
			val = val*10 + num;
			procent_ed->SetValue(to_uc(ksi_cafe::percent_t().format(val)));
			money_ed->SetValue(locale->get_currency(val*subtotal_/100));
			if (val*10 + 0 > 100) num00_btn->Enable(false); else num00_btn->Enable(true);
			if (val*10 + 1 > 100) num01_btn->Enable(false); else num01_btn->Enable(true);
			if (val*10 + 2 > 100) num02_btn->Enable(false); else num02_btn->Enable(true);
			if (val*10 + 3 > 100) num03_btn->Enable(false); else num03_btn->Enable(true);
			if (val*10 + 4 > 100) num04_btn->Enable(false); else num04_btn->Enable(true);
			if (val*10 + 5 > 100) num05_btn->Enable(false); else num05_btn->Enable(true);
			if (val*10 + 6 > 100) num06_btn->Enable(false); else num06_btn->Enable(true);
			if (val*10 + 7 > 100) num07_btn->Enable(false); else num07_btn->Enable(true);
			if (val*10 + 8 > 100) num08_btn->Enable(false); else num08_btn->Enable(true);
			if (val*10 + 9 > 100) num09_btn->Enable(false); else num09_btn->Enable(true);
		}
		else
		{
			double val = locale->reverse_currency(money_ed->GetValue());
			val = val*10 + double(num)/100;
			money_ed->SetValue(locale->get_currency(val));
			double percent = 100*val/subtotal_;
			procent_ed->SetValue(to_uc(ksi_cafe::percent_t().format(percent)));
			if (val*10 + double(0)/100 > subtotal_) num00_btn->Enable(false); else num00_btn->Enable(true);
			if (val*10 + double(1)/100 > subtotal_) num01_btn->Enable(false); else num01_btn->Enable(true);
			if (val*10 + double(2)/100 > subtotal_) num02_btn->Enable(false); else num02_btn->Enable(true);
			if (val*10 + double(3)/100 > subtotal_) num03_btn->Enable(false); else num03_btn->Enable(true);
			if (val*10 + double(4)/100 > subtotal_) num04_btn->Enable(false); else num04_btn->Enable(true);
			if (val*10 + double(5)/100 > subtotal_) num05_btn->Enable(false); else num05_btn->Enable(true);
			if (val*10 + double(6)/100 > subtotal_) num06_btn->Enable(false); else num06_btn->Enable(true);
			if (val*10 + double(7)/100 > subtotal_) num07_btn->Enable(false); else num07_btn->Enable(true);
			if (val*10 + double(8)/100 > subtotal_) num08_btn->Enable(false); else num08_btn->Enable(true);
			if (val*10 + double(9)/100 > subtotal_) num09_btn->Enable(false); else num09_btn->Enable(true);
		}
		wxString str = money_ed->GetValue();
		if (int(locale->reverse_currency(str)*100) != 0) ok_btn->Enable(true); else ok_btn->Enable(false);
	}
	catch (exception_t& ex)
	{
		wxMessageBox(to_uc(ex.comp_message()));
	}	
}

void coupon_calc_f::clear_btnClick( wxCommandEvent& event )
{
	// TODO: Implement clear_btnClick
	try
	{
		procent_ed->SetValue(to_uc(ksi_cafe::percent_t().format(0)));
		money_ed->SetValue(locale->get_currency(0));
		num00_btn->Enable(true);
		num01_btn->Enable(true);
		num02_btn->Enable(true);
		num03_btn->Enable(true);
		num04_btn->Enable(true);
		num05_btn->Enable(true);
		num06_btn->Enable(true);
		num07_btn->Enable(true);
		num08_btn->Enable(true);
		num09_btn->Enable(true);
		wxString str = money_ed->GetValue();
		if (int(locale->reverse_currency(str)*100) != 0) ok_btn->Enable(true); else ok_btn->Enable(false);
	}
	catch (exception_t& ex)
	{
		wxMessageBox(to_uc(ex.comp_message()));
	}
}

void coupon_calc_f::ok_btnClick( wxCommandEvent& event )
{
	// TODO: Implement ok_btnClick
	EndModal(wxID_OK);
}

void coupon_calc_f::cancel_btnClick( wxCommandEvent& event )
{
	// TODO: Implement cancel_btnClick
	EndModal(wxID_CANCEL);
}

double coupon_calc_f::discount_amount() const
{
	return locale->reverse_currency(money_ed->GetValue());
}

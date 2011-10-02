#include "tip_amount_f.h"
#include <wx/msgdlg.h>
#include <wx_util/wx_util.hpp>
#include <memory>
#include <boost/lexical_cast.hpp>
#include <lang/xml_lang.h>

extern std::auto_ptr<ksi_cafe::xml_lang_t> locale;

tip_amount_f::tip_amount_f( wxWindow* parent, const wxString& title, double amount, bool hide )
:
base_tip_amount_f( parent )
{
	SetLabel(title);
	amount_ed->SetValue(locale->get_currency(amount));
	total_ed->SetValue(locale->get_currency(amount));
	tip_ed->SetValue(locale->get_currency(0));
	
	amount_lb->SetLabel(locale->get("tip_amount_f", "amount_lb", "Amount:"));
	tip_lb->SetLabel(locale->get("tip_amount_f", "tip_lb", "Tip:"));
	total_lb->SetLabel(locale->get("tip_amount_f", "total_lb", "Total:"));
	clear_btn->SetLabel(locale->get("tip_amount_f", "clear_btn", "Clear"));
	ok_btn->SetLabel(locale->get("tip_amount_f", "ok_btn", "Ok"));
	cancel_btn->SetLabel(locale->get("tip_amount_f", "cancel_btn", "Cancel"));
	hide_btn->SetLabel(locale->get("tip_amount_f", "hide_btn", "Hide"));	
	hide_btn->Show(!hide);
}

void tip_amount_f::btnClick( wxCommandEvent& event )
{
	// TODO: Implement btnClick
	wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
	if (btn == 0) return;
	int curr_val = boost::lexical_cast<int>(to_mb(btn->GetLabel())); 
	double tip = locale->reverse_currency(tip_ed->GetValue())*10 + double(curr_val)/100;
	tip_ed->SetValue(locale->get_currency(tip));
	total_ed->SetValue(locale->get_currency(tip+locale->reverse_currency(amount_ed->GetValue())));
}

void tip_amount_f::clear_btnClick( wxCommandEvent& event )
{
	// TODO: Implement clear_btnClick
	tip_ed->SetValue(locale->get_currency(0));
	total_ed->SetValue(amount_ed->GetValue());	
}

void tip_amount_f::ok_btnClick( wxCommandEvent& event ) {
	EndModal(wxID_OK);
}

void tip_amount_f::hide_btnClick( wxCommandEvent& event )
{
	// TODO: Implement hide_btnClick
	EndModal(wxID_MORE);
}

void tip_amount_f::cancel_btnClick( wxCommandEvent& event )
{
	// TODO: Implement cancel_btnClick
	wxString message = locale->get("global", "cancel_auth_msg", "This will void credit card authorization. Continue?");
	wxMessageDialog dlg(this, message, _("Confirmation"), wxYES_NO);
	dlg.Centre(wxBOTH);
	if (dlg.ShowModal() == wxID_YES) EndModal(wxID_CANCEL);
}

double tip_amount_f::tip() const {
	return locale->reverse_currency(tip_ed->GetValue());
}

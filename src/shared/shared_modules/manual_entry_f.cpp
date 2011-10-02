#include "manual_entry_f.h"
#include <lang/xml_lang.h>
#include <memory>

extern std::auto_ptr<ksi_cafe::xml_lang_t> locale;

manual_entry_f::manual_entry_f( wxWindow* parent, const wxString& title )
:
base_manual_entry_f( parent ), active_(card_num_ed)
{
	SetLabel(title);
	clear_btn->SetLabel(locale->get("manual_entry_f", "clear_btn", "Clear"));
	ok_btn->SetLabel(locale->get("manual_entry_f", "ok_btn", "Ok"));
	cancel_btn->SetLabel(locale->get("manual_entry_f", "cancel_btn", "Cancel"));
}

void manual_entry_f::num_btnClick( wxCommandEvent& event )
{
	// TODO: Implement num_btnClick
	wxTextCtrl* ed = dynamic_cast<wxTextCtrl*>(active_);
	if (ed == 0) return;
	wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
	if (btn == 0) return;
	ed->SetValue(ed->GetValue() + btn->GetLabel());
	if (card_num_ed->GetValue() != _("") && exp_month_ed->GetValue() != _("") && exp_year_ed->GetValue() != _("") && cvv2_ed->GetValue() != _(""))
		ok_btn->Enable(true);
	else
		ok_btn->Enable(false);
}

void manual_entry_f::clear_btnClick( wxCommandEvent& event )
{
	// TODO: Implement clear_btnClick
	wxTextCtrl* ed = dynamic_cast<wxTextCtrl*>(active_);
	if (ed == 0) return;
	ed->SetValue(_(""));
}

void manual_entry_f::ok_btnClick( wxCommandEvent& event )
{
	// TODO: Implement ok_btnClick
	EndModal(wxID_OK);
}

void manual_entry_f::cancel_btnClick( wxCommandEvent& event )
{
	// TODO: Implement cancel_btnClick
	EndModal(wxID_CANCEL);
}

void manual_entry_f::card_numFocus( wxFocusEvent& event ) {
	active_ = card_num_ed;
}

void manual_entry_f::monthFocus( wxFocusEvent& event ) {
	active_ = exp_month_ed;
}

void manual_entry_f::yearFocus( wxFocusEvent& event ) {
	active_ = exp_year_ed;
}

void manual_entry_f::cvv2Focus( wxFocusEvent& event ) {
	active_ = cvv2_ed;
}


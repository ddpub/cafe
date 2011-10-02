#include "info_f.h"

#include <lang/xml_lang.h>

extern std::auto_ptr<ksi_cafe::xml_lang_t> locale;

info_f::info_f( wxWindow* parent, const wxString& title, const wxString& label, const wxString& change, bool close_ )
:
base_info_f( parent ), timer_(obj_handler_t<info_f>(this, &info_f::OnTimer))
{
	SetLabel(title);
	if (label != _("")) {
		label_lb->SetLabel(label);
		label_lb->Show(true);
	}
	if (change != _("")) {
		change_lb->SetLabel(change);
		change_lb->Show(true);
	}
	close_btn->Show(close_);
	if (!close_) {
		timer_.start2(25);
	}
	this->Layout();
	main_sizer->Fit( this );
	close_btn->SetLabel(locale->get("info_f", "close_btn", "Close"));
	wrap_label(close_btn);	
	CenterOnParent();
}

void info_f::close_btnClick( wxCommandEvent& event )
{
	// TODO: Implement close_btnClick
	EndModal(wxID_OK);
}

void info_f::OnTimer (wxTimerEvent& event) {
	EndModal(wxID_OK);
}

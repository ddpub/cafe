#include "load_f.h"
#include <boost/date_time/posix_time/ptime.hpp>
#include <wx_util/wx_util.hpp>
#include <lang/xml_lang.h>
#include <memory>

extern std::auto_ptr<ksi_cafe::xml_lang_t> locale;

load_f::load_f( wxWindow* parent, const wxString& title, const wxString& label)
: base_load_f( parent ), timer_(obj_handler_t<load_f>(this, &load_f::OnTimer))
{
	SetLabel(title);
	if (label != _("")) {
		label_lb->SetLabel(label);
		label_lb->Show(true);
	}
	cancel_btn->SetLabel(locale->get("load_f", "cancel_btn", "Cancel"));
	cancel_btn->Show(true);
	this->Layout();
	main_sizer->Fit( this );
	CenterOnParent();
}

void load_f::OnTimer (wxTimerEvent& event) {
	wx_disable_t disable_window(this);
	if (thr_.timed_join(boost::posix_time::milliseconds(0))) {
		timer_.stop();
		thr_.join();
		EndModal(wxID_OK);
	}
}

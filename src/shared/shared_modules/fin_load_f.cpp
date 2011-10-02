#include "fin_load_f.h"
#include <boost/date_time/posix_time/ptime.hpp>
#include <wx_util/wx_util.hpp>

fin_load_f::fin_load_f( wxWindow* parent, const wxString& title, const wxString& label )
:
base_load_f( parent ), timer_(obj_handler_t<fin_load_f>(this, &fin_load_f::OnTimer))
{
	cancel_btn->Show(false);
	SetLabel(title);
	label_lb->SetLabel(label);
	label_lb->Show(true);
	this->Layout();
	main_sizer->Fit( this );
	CenterOnParent();
}

void fin_load_f::cancel_btnClick( wxCommandEvent& event )
{
	// TODO: Implement cancel_btnClick
}

void fin_load_f::OnTimer (wxTimerEvent& event) {
	wx_disable_t disable_window(this);
	if (thr_.timed_join(boost::posix_time::milliseconds(0))) {
		timer_.stop();
		thr_.join();
		EndModal(wxID_OK);
	}
}

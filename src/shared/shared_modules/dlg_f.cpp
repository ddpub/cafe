#include "dlg_f.h"
#include <ksi_include/ksi_exceptions.hpp>
#include <wx_util/wx_util.hpp>
#include <algorithm>
#include <iterator>
#include <ksi_include/algo_functor.hpp>

dlg_f::dlg_f( wxWindow* parent, const wxString& cap_, const wxString& lab_,  std::vector<wxString>& cap_vec_, std::vector<wxColour>& color_vec_ )
:
base_dlg_f( parent ) {
	SetLabel(cap_);
	message_ed->SetLabel(lab_ + _("\n"));
	btn_sizer->Add( 0, 0, 1, wxEXPAND, 5 );
	for (size_t i=0; i<cap_vec_.size(); ++i) {
		wxButton* btn = new wxButton( this, wxID_ANY, cap_vec_[i], wxDefaultPosition, wxSize( 100, 30 ), 0);
		btn->SetFont( wxFont( 10, 74, 90, 90, false, _("Arial Narrow") ) );
		change_bg_colour( btn, color_vec_[i] );
		btn_sizer->Add( btn, 0, wxALL, 5 );
		btn_sizer->Add( 0, 0, 1, wxEXPAND, 5 );
		// Connect Events
		btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dlg_f::OnClick ), NULL, this );
		btn_vec_.push_back( btn );
	}
	
	this->SetSizer( main_sizer );
	this->Layout();
	main_sizer->Fit( this );
	
	this->Centre( wxBOTH );	
}

dlg_f::~dlg_f() {
	// Disconnect Events
	for (size_t i=0; i<btn_vec_.size(); ++i) { 
		btn_vec_[i]->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( dlg_f::OnClick ), NULL, this );
	}
}

void dlg_f::OnClick( wxCommandEvent& event ) {
	wxObject* obj = event.GetEventObject();
	wxButton* btn = dynamic_cast<wxButton*>(obj);
	if (btn == 0) return;
	std::vector<wxButton*>::iterator It = std::find(btn_vec_.begin(), btn_vec_.end(), btn);
	if (It == btn_vec_.end()) return;
	EndModal(wxID_HIGHEST + std::distance(btn_vec_.begin(), It));
}

///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "base_dlg_f.h"

///////////////////////////////////////////////////////////////////////////

base_dlg_f::base_dlg_f( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	main_sizer = new wxBoxSizer( wxVERTICAL );
	
	message_ed = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 400,-1 ), wxALIGN_LEFT );
	message_ed->Wrap( 300 );
	message_ed->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	main_sizer->Add( message_ed, 1, wxALL|wxEXPAND, 5 );
	
	btn_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	main_sizer->Add( btn_sizer, 0, wxEXPAND, 0 );
	
	this->SetSizer( main_sizer );
	this->Layout();
	main_sizer->Fit( this );
	
	this->Centre( wxBOTH );
}

base_dlg_f::~base_dlg_f()
{
}

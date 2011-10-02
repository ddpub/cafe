///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "base_load_f.h"

///////////////////////////////////////////////////////////////////////////

base_load_f::base_load_f( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	main_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	
	main_sizer->Add( 50, 50, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	label_lb = new wxStaticText( this, wxID_ANY, wxT("Communicating with processing server, please wait."), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	label_lb->Wrap( -1 );
	label_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer4->Add( label_lb, 0, wxALL|wxEXPAND, 5 );
	
	cancel_btn = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	cancel_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	cancel_btn->SetBackgroundColour( wxColour( 255, 204, 204 ) );
	cancel_btn->Hide();
	
	bSizer4->Add( cancel_btn, 0, wxALIGN_CENTER|wxALL, 5 );
	
	main_sizer->Add( bSizer4, 0, wxEXPAND, 5 );
	
	
	main_sizer->Add( 50, 50, 1, wxEXPAND, 5 );
	
	this->SetSizer( main_sizer );
	this->Layout();
	main_sizer->Fit( this );
	
	// Connect Events
	cancel_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_load_f::cancel_btnClick ), NULL, this );
}

base_load_f::~base_load_f()
{
	// Disconnect Events
	cancel_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_load_f::cancel_btnClick ), NULL, this );
}

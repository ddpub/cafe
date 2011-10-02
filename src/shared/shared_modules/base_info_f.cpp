///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "base_info_f.h"

///////////////////////////////////////////////////////////////////////////

base_info_f::base_info_f( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	main_sizer = new wxBoxSizer( wxVERTICAL );
	
	label_lb = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	label_lb->Wrap( 320 );
	label_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	label_lb->Hide();
	
	main_sizer->Add( label_lb, 0, wxALL|wxEXPAND, 5 );
	
	change_lb = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	change_lb->Wrap( -1 );
	change_lb->SetFont( wxFont( 14, 74, 90, 92, false, wxT("Arial Narrow") ) );
	change_lb->Hide();
	
	main_sizer->Add( change_lb, 0, wxALL|wxEXPAND, 5 );
	
	close_btn = new wxButton( this, wxID_ANY, wxT("Close"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	close_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	close_btn->SetBackgroundColour( wxColour( 204, 255, 204 ) );
	
	main_sizer->Add( close_btn, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	this->SetSizer( main_sizer );
	this->Layout();
	
	// Connect Events
	close_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_info_f::close_btnClick ), NULL, this );
}

base_info_f::~base_info_f()
{
	// Disconnect Events
	close_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_info_f::close_btnClick ), NULL, this );
}

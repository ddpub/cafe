///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "base_select_time_f.h"

///////////////////////////////////////////////////////////////////////////

base_select_time_f::base_select_time_f( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	main_sizer = new wxBoxSizer( wxVERTICAL );
	
	hours_sizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Hours") ), wxVERTICAL );
	
	hgird = new wxGridSizer( 2, 4, 0, 0 );
	
	hours_sizer->Add( hgird, 1, wxEXPAND, 5 );
	
	main_sizer->Add( hours_sizer, 1, wxEXPAND, 5 );
	
	minutes_sizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Minues") ), wxVERTICAL );
	
	mgrid = new wxGridSizer( 1, 4, 0, 0 );
	
	m00_btn = new wxButton( this, wxID_ANY, wxT(":00"), wxDefaultPosition, wxSize( 100,40 ), 0 );
	m00_btn->SetFont( wxFont( 14, 74, 90, 90, false, wxT("Arial Narrow") ) );
	m00_btn->Enable( false );
	
	mgrid->Add( m00_btn, 0, wxALL, 5 );
	
	m15_btn = new wxButton( this, wxID_ANY, wxT(":15"), wxDefaultPosition, wxSize( 100,40 ), 0 );
	m15_btn->SetFont( wxFont( 14, 74, 90, 90, false, wxT("Arial Narrow") ) );
	m15_btn->Enable( false );
	
	mgrid->Add( m15_btn, 0, wxALL, 5 );
	
	m30_btn = new wxButton( this, wxID_ANY, wxT(":30"), wxDefaultPosition, wxSize( 100,40 ), 0 );
	m30_btn->SetFont( wxFont( 14, 74, 90, 90, false, wxT("Arial Narrow") ) );
	m30_btn->Enable( false );
	
	mgrid->Add( m30_btn, 0, wxALL, 5 );
	
	m45_btn = new wxButton( this, wxID_ANY, wxT(":45"), wxDefaultPosition, wxSize( 100,40 ), 0 );
	m45_btn->SetFont( wxFont( 14, 74, 90, 90, false, wxT("Arial Narrow") ) );
	m45_btn->Enable( false );
	
	mgrid->Add( m45_btn, 0, wxALL, 5 );
	
	minutes_sizer->Add( mgrid, 1, wxEXPAND, 5 );
	
	main_sizer->Add( minutes_sizer, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	ok_btn = new wxButton( this, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	ok_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	ok_btn->SetBackgroundColour( wxColour( 204, 255, 204 ) );
	ok_btn->Enable( false );
	
	bSizer2->Add( ok_btn, 0, wxALL, 5 );
	
	
	bSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	cancel_btn = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	cancel_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	cancel_btn->SetBackgroundColour( wxColour( 255, 204, 204 ) );
	
	bSizer2->Add( cancel_btn, 0, wxALL, 5 );
	
	
	bSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	main_sizer->Add( bSizer2, 0, wxEXPAND, 5 );
	
	this->SetSizer( main_sizer );
	this->Layout();
	main_sizer->Fit( this );
	
	// Connect Events
	m00_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_select_time_f::m_btnClick ), NULL, this );
	m15_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_select_time_f::m_btnClick ), NULL, this );
	m30_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_select_time_f::m_btnClick ), NULL, this );
	m45_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_select_time_f::m_btnClick ), NULL, this );
	ok_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_select_time_f::ok_btnClick ), NULL, this );
	cancel_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_select_time_f::cancel_btnClick ), NULL, this );
}

base_select_time_f::~base_select_time_f()
{
	// Disconnect Events
	m00_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_select_time_f::m_btnClick ), NULL, this );
	m15_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_select_time_f::m_btnClick ), NULL, this );
	m30_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_select_time_f::m_btnClick ), NULL, this );
	m45_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_select_time_f::m_btnClick ), NULL, this );
	ok_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_select_time_f::ok_btnClick ), NULL, this );
	cancel_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_select_time_f::cancel_btnClick ), NULL, this );
}

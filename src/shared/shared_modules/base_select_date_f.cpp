///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "base_select_date_f.h"

///////////////////////////////////////////////////////////////////////////

base_select_date_f::base_select_date_f( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	main_sizer = new wxBoxSizer( wxVERTICAL );
	
	grid = new wxGridSizer( 2, 7, 0, 0 );
	
	su_st = new wxStaticText( this, wxID_ANY, wxT("Su"), wxDefaultPosition, wxSize( -1,-1 ), wxALIGN_CENTRE );
	su_st->Wrap( -1 );
	su_st->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	grid->Add( su_st, 0, wxALL, 5 );
	
	mo_st = new wxStaticText( this, wxID_ANY, wxT("Mo"), wxDefaultPosition, wxSize( -1,-1 ), wxALIGN_CENTRE );
	mo_st->Wrap( -1 );
	mo_st->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	grid->Add( mo_st, 0, wxALL, 5 );
	
	tu_st = new wxStaticText( this, wxID_ANY, wxT("Tu"), wxDefaultPosition, wxSize( -1,-1 ), wxALIGN_CENTRE );
	tu_st->Wrap( -1 );
	tu_st->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	grid->Add( tu_st, 0, wxALL, 5 );
	
	we_st = new wxStaticText( this, wxID_ANY, wxT("We"), wxDefaultPosition, wxSize( -1,-1 ), wxALIGN_CENTRE );
	we_st->Wrap( -1 );
	we_st->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	grid->Add( we_st, 0, wxALL, 5 );
	
	th_st = new wxStaticText( this, wxID_ANY, wxT("Th"), wxDefaultPosition, wxSize( -1,-1 ), wxALIGN_CENTRE );
	th_st->Wrap( -1 );
	th_st->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	grid->Add( th_st, 0, wxALL, 5 );
	
	ft_st = new wxStaticText( this, wxID_ANY, wxT("Fr"), wxDefaultPosition, wxSize( -1,-1 ), wxALIGN_CENTRE );
	ft_st->Wrap( -1 );
	ft_st->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	grid->Add( ft_st, 0, wxALL, 5 );
	
	sa_st = new wxStaticText( this, wxID_ANY, wxT("Sa"), wxDefaultPosition, wxSize( -1,-1 ), wxALIGN_CENTRE );
	sa_st->Wrap( -1 );
	sa_st->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	grid->Add( sa_st, 0, wxALL, 5 );
	
	main_sizer->Add( grid, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	ok_btn = new wxButton( this, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	ok_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	ok_btn->SetBackgroundColour( wxColour( 204, 255, 204 ) );
	ok_btn->Enable( false );
	
	bSizer3->Add( ok_btn, 0, wxALL, 5 );
	
	
	bSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	cancel_btn = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	cancel_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	cancel_btn->SetBackgroundColour( wxColour( 255, 204, 204 ) );
	
	bSizer3->Add( cancel_btn, 0, wxALL, 5 );
	
	
	bSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	main_sizer->Add( bSizer3, 0, wxEXPAND, 5 );
	
	this->SetSizer( main_sizer );
	this->Layout();
	main_sizer->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	ok_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_select_date_f::ok_btnClick ), NULL, this );
	cancel_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_select_date_f::cancel_btnClick ), NULL, this );
}

base_select_date_f::~base_select_date_f()
{
	// Disconnect Events
	ok_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_select_date_f::ok_btnClick ), NULL, this );
	cancel_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_select_date_f::cancel_btnClick ), NULL, this );
}

///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "base_options_f.h"

///////////////////////////////////////////////////////////////////////////

base_options_f::base_options_f( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 3, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_printer_port_lb = new wxStaticText( this, wxID_ANY, wxT("Printer port"), wxDefaultPosition, wxDefaultSize, 0 );
	m_printer_port_lb->Wrap( -1 );
	fgSizer1->Add( m_printer_port_lb, 0, wxALL, 5 );
	
	wxArrayString m_printer_port_cbChoices;
	m_printer_port_cb = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_printer_port_cbChoices, 0 );
	m_printer_port_cb->SetSelection( 0 );
	fgSizer1->Add( m_printer_port_cb, 0, wxALL, 5 );
	
	m_command_set_lb = new wxStaticText( this, wxID_ANY, wxT("Command set"), wxDefaultPosition, wxDefaultSize, 0 );
	m_command_set_lb->Wrap( -1 );
	fgSizer1->Add( m_command_set_lb, 0, wxALL, 5 );
	
	wxArrayString m_command_set_cbChoices;
	m_command_set_cb = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_command_set_cbChoices, 0 );
	m_command_set_cb->SetSelection( 0 );
	fgSizer1->Add( m_command_set_cb, 0, wxALL, 5 );
	
	m_baud_rate_lb = new wxStaticText( this, wxID_ANY, wxT("Baud rate"), wxDefaultPosition, wxDefaultSize, 0 );
	m_baud_rate_lb->Wrap( -1 );
	fgSizer1->Add( m_baud_rate_lb, 0, wxALL, 5 );
	
	wxArrayString m_baud_rate_cbChoices;
	m_baud_rate_cb = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_baud_rate_cbChoices, 0 );
	m_baud_rate_cb->SetSelection( 0 );
	fgSizer1->Add( m_baud_rate_cb, 0, wxALL, 5 );
	
	bSizer1->Add( fgSizer1, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	ok_btn = new wxButton( this, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	ok_btn->Enable( false );
	
	bSizer3->Add( ok_btn, 0, wxALL, 5 );
	
	
	bSizer3->Add( 0, 0, 2, wxEXPAND, 5 );
	
	cancel_btn = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( cancel_btn, 0, wxALL, 5 );
	
	
	bSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer1->Add( bSizer3, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_printer_port_cb->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( base_options_f::m_printer_portChoice ), NULL, this );
	m_command_set_cb->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( base_options_f::m_command_setChoice ), NULL, this );
	m_baud_rate_cb->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( base_options_f::m_baud_rateChoice ), NULL, this );
	ok_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_options_f::ok_btnClick ), NULL, this );
	cancel_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_options_f::cancel_btnClick ), NULL, this );
}

base_options_f::~base_options_f()
{
	// Disconnect Events
	m_printer_port_cb->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( base_options_f::m_printer_portChoice ), NULL, this );
	m_command_set_cb->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( base_options_f::m_command_setChoice ), NULL, this );
	m_baud_rate_cb->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( base_options_f::m_baud_rateChoice ), NULL, this );
	ok_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_options_f::ok_btnClick ), NULL, this );
	cancel_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_options_f::cancel_btnClick ), NULL, this );
}

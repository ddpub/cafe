///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "base_manual_entry_f.h"

///////////////////////////////////////////////////////////////////////////

base_manual_entry_f::base_manual_entry_f( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* m_staticText3;
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Card #:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	m_staticText3->SetFont( wxFont( 14, 74, 90, 92, false, wxT("Arial Narrow") ) );
	
	bSizer2->Add( m_staticText3, 0, wxALL, 5 );
	
	card_num_ed = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_READONLY );
	card_num_ed->SetFont( wxFont( 14, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer2->Add( card_num_ed, 1, wxALL, 5 );
	
	bSizer1->Add( bSizer2, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("Exp. month:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	m_staticText5->SetFont( wxFont( 14, 74, 90, 92, false, wxT("Arial Narrow") ) );
	
	bSizer4->Add( m_staticText5, 0, wxALL, 5 );
	
	exp_month_ed = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 100,-1 ), wxTE_READONLY );
	exp_month_ed->SetFont( wxFont( 14, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer4->Add( exp_month_ed, 1, wxALL, 5 );
	
	bSizer3->Add( bSizer4, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("Exp. year:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	m_staticText6->SetFont( wxFont( 14, 74, 90, 92, false, wxT("Arial Narrow") ) );
	
	bSizer5->Add( m_staticText6, 0, wxALL, 5 );
	
	exp_year_ed = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 100,-1 ), wxTE_READONLY );
	exp_year_ed->SetFont( wxFont( 14, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer5->Add( exp_year_ed, 1, wxALL, 5 );
	
	bSizer3->Add( bSizer5, 1, wxEXPAND, 5 );
	
	bSizer1->Add( bSizer3, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* m_staticText7;
	m_staticText7 = new wxStaticText( this, wxID_ANY, wxT("CVV2:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	m_staticText7->SetFont( wxFont( 14, 74, 90, 92, false, wxT("Arial Narrow") ) );
	
	bSizer6->Add( m_staticText7, 0, wxALL, 5 );
	
	cvv2_ed = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_READONLY );
	cvv2_ed->SetFont( wxFont( 14, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer6->Add( cvv2_ed, 1, wxALL, 5 );
	
	bSizer1->Add( bSizer6, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer8->Add( 0, 0, 1, wxEXPAND, 5 );
	
	num7_btn = new wxButton( this, wxID_ANY, wxT("7"), wxDefaultPosition, wxSize( 70,70 ), 0 );
	num7_btn->SetFont( wxFont( 20, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer8->Add( num7_btn, 0, 0, 5 );
	
	num8_btn = new wxButton( this, wxID_ANY, wxT("8"), wxDefaultPosition, wxSize( 70,70 ), 0 );
	num8_btn->SetFont( wxFont( 20, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer8->Add( num8_btn, 0, 0, 5 );
	
	num9_btn = new wxButton( this, wxID_ANY, wxT("9"), wxDefaultPosition, wxSize( 70,70 ), 0 );
	num9_btn->SetFont( wxFont( 20, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer8->Add( num9_btn, 0, 0, 5 );
	
	
	bSizer8->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer7->Add( bSizer8, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer9->Add( 0, 0, 1, wxEXPAND, 5 );
	
	num4_btn = new wxButton( this, wxID_ANY, wxT("4"), wxDefaultPosition, wxSize( 70,70 ), 0 );
	num4_btn->SetFont( wxFont( 20, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer9->Add( num4_btn, 0, 0, 5 );
	
	num5_btn = new wxButton( this, wxID_ANY, wxT("5"), wxDefaultPosition, wxSize( 70,70 ), 0 );
	num5_btn->SetFont( wxFont( 20, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer9->Add( num5_btn, 0, 0, 5 );
	
	num6_btn = new wxButton( this, wxID_ANY, wxT("6"), wxDefaultPosition, wxSize( 70,70 ), 0 );
	num6_btn->SetFont( wxFont( 20, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer9->Add( num6_btn, 0, 0, 5 );
	
	
	bSizer9->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer7->Add( bSizer9, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer10->Add( 0, 0, 1, wxEXPAND, 5 );
	
	num1_btn = new wxButton( this, wxID_ANY, wxT("1"), wxDefaultPosition, wxSize( 70,70 ), 0 );
	num1_btn->SetFont( wxFont( 20, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer10->Add( num1_btn, 0, 0, 5 );
	
	num2_btn = new wxButton( this, wxID_ANY, wxT("2"), wxDefaultPosition, wxSize( 70,70 ), 0 );
	num2_btn->SetFont( wxFont( 20, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer10->Add( num2_btn, 0, 0, 5 );
	
	num3_btn = new wxButton( this, wxID_ANY, wxT("3"), wxDefaultPosition, wxSize( 70,70 ), 0 );
	num3_btn->SetFont( wxFont( 20, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer10->Add( num3_btn, 0, 0, 5 );
	
	
	bSizer10->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer7->Add( bSizer10, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	
	bSizer11->Add( 0, 0, 1, wxEXPAND, 5 );
	
	num0_btn = new wxButton( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( 70,70 ), 0 );
	num0_btn->SetFont( wxFont( 20, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer11->Add( num0_btn, 0, wxALIGN_CENTER, 5 );
	
	
	bSizer11->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer7->Add( bSizer11, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	
	bSizer12->Add( 0, 0, 1, wxEXPAND, 5 );
	
	clear_btn = new wxButton( this, wxID_ANY, wxT("Clear"), wxDefaultPosition, wxSize( 100,40 ), 0 );
	clear_btn->SetFont( wxFont( 14, 74, 90, 92, false, wxT("Arial Narrow") ) );
	
	bSizer12->Add( clear_btn, 0, wxALIGN_CENTER, 0 );
	
	
	bSizer12->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer7->Add( bSizer12, 1, wxEXPAND, 5 );
	
	bSizer1->Add( bSizer7, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer13->Add( 0, 0, 1, wxEXPAND, 5 );
	
	ok_btn = new wxButton( this, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	ok_btn->SetDefault(); 
	ok_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	ok_btn->SetBackgroundColour( wxColour( 204, 255, 204 ) );
	ok_btn->Enable( false );
	
	bSizer13->Add( ok_btn, 0, 0, 5 );
	
	
	bSizer13->Add( 0, 0, 1, wxEXPAND, 5 );
	
	cancel_btn = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	cancel_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	cancel_btn->SetBackgroundColour( wxColour( 255, 204, 204 ) );
	
	bSizer13->Add( cancel_btn, 0, 0, 5 );
	
	
	bSizer13->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer1->Add( bSizer13, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	card_num_ed->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( base_manual_entry_f::card_numFocus ), NULL, this );
	exp_month_ed->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( base_manual_entry_f::monthFocus ), NULL, this );
	exp_year_ed->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( base_manual_entry_f::yearFocus ), NULL, this );
	cvv2_ed->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( base_manual_entry_f::cvv2Focus ), NULL, this );
	num7_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_manual_entry_f::num_btnClick ), NULL, this );
	num8_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_manual_entry_f::num_btnClick ), NULL, this );
	num9_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_manual_entry_f::num_btnClick ), NULL, this );
	num4_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_manual_entry_f::num_btnClick ), NULL, this );
	num5_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_manual_entry_f::num_btnClick ), NULL, this );
	num6_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_manual_entry_f::num_btnClick ), NULL, this );
	num1_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_manual_entry_f::num_btnClick ), NULL, this );
	num2_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_manual_entry_f::num_btnClick ), NULL, this );
	num3_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_manual_entry_f::num_btnClick ), NULL, this );
	num0_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_manual_entry_f::num_btnClick ), NULL, this );
	clear_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_manual_entry_f::clear_btnClick ), NULL, this );
	ok_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_manual_entry_f::ok_btnClick ), NULL, this );
	cancel_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_manual_entry_f::cancel_btnClick ), NULL, this );
}

base_manual_entry_f::~base_manual_entry_f()
{
	// Disconnect Events
	card_num_ed->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( base_manual_entry_f::card_numFocus ), NULL, this );
	exp_month_ed->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( base_manual_entry_f::monthFocus ), NULL, this );
	exp_year_ed->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( base_manual_entry_f::yearFocus ), NULL, this );
	cvv2_ed->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( base_manual_entry_f::cvv2Focus ), NULL, this );
	num7_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_manual_entry_f::num_btnClick ), NULL, this );
	num8_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_manual_entry_f::num_btnClick ), NULL, this );
	num9_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_manual_entry_f::num_btnClick ), NULL, this );
	num4_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_manual_entry_f::num_btnClick ), NULL, this );
	num5_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_manual_entry_f::num_btnClick ), NULL, this );
	num6_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_manual_entry_f::num_btnClick ), NULL, this );
	num1_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_manual_entry_f::num_btnClick ), NULL, this );
	num2_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_manual_entry_f::num_btnClick ), NULL, this );
	num3_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_manual_entry_f::num_btnClick ), NULL, this );
	num0_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_manual_entry_f::num_btnClick ), NULL, this );
	clear_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_manual_entry_f::clear_btnClick ), NULL, this );
	ok_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_manual_entry_f::ok_btnClick ), NULL, this );
	cancel_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_manual_entry_f::cancel_btnClick ), NULL, this );
}

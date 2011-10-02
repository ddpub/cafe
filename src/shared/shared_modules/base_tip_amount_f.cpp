///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "base_tip_amount_f.h"

///////////////////////////////////////////////////////////////////////////

base_tip_amount_f::base_tip_amount_f( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	mainbSizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 2, 2, 0, 0 );
	
	amount_lb = new wxStaticText( this, wxID_ANY, wxT("Amount:"), wxDefaultPosition, wxDefaultSize, 0 );
	amount_lb->Wrap( -1 );
	amount_lb->SetFont( wxFont( 14, 74, 90, 92, false, wxT("Arial Narrow") ) );
	
	gSizer1->Add( amount_lb, 0, wxALIGN_LEFT|wxALL, 5 );
	
	amount_ed = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	amount_ed->SetFont( wxFont( 14, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	gSizer1->Add( amount_ed, 0, wxALIGN_RIGHT|wxALL|wxEXPAND, 5 );
	
	tip_lb = new wxStaticText( this, wxID_ANY, wxT("Tip:"), wxDefaultPosition, wxDefaultSize, 0 );
	tip_lb->Wrap( -1 );
	tip_lb->SetFont( wxFont( 14, 74, 90, 92, false, wxT("Arial Narrow") ) );
	
	gSizer1->Add( tip_lb, 0, wxALIGN_LEFT|wxALL, 5 );
	
	tip_ed = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	tip_ed->SetFont( wxFont( 14, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	gSizer1->Add( tip_ed, 0, wxALL|wxEXPAND|wxRIGHT, 5 );
	
	total_lb = new wxStaticText( this, wxID_ANY, wxT("Total:"), wxDefaultPosition, wxDefaultSize, 0 );
	total_lb->Wrap( -1 );
	total_lb->SetFont( wxFont( 14, 74, 90, 92, false, wxT("Arial Narrow") ) );
	
	gSizer1->Add( total_lb, 0, wxALIGN_LEFT|wxALL, 5 );
	
	total_ed = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	total_ed->SetFont( wxFont( 14, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	gSizer1->Add( total_ed, 0, wxALIGN_RIGHT|wxALL|wxEXPAND, 5 );
	
	bSizer3->Add( gSizer1, 0, wxEXPAND, 5 );
	
	bSizer2->Add( bSizer3, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer8->Add( 0, 0, 1, wxEXPAND, 5 );
	
	num7_btn = new wxButton( this, wxID_ANY, wxT("7"), wxDefaultPosition, wxSize( 70,70 ), 0 );
	num7_btn->SetFont( wxFont( 20, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer8->Add( num7_btn, 0, 0, 0 );
	
	num8_btn = new wxButton( this, wxID_ANY, wxT("8"), wxDefaultPosition, wxSize( 70,70 ), 0 );
	num8_btn->SetFont( wxFont( 20, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer8->Add( num8_btn, 0, 0, 5 );
	
	num9_btn = new wxButton( this, wxID_ANY, wxT("9"), wxDefaultPosition, wxSize( 70,70 ), 0 );
	num9_btn->SetFont( wxFont( 20, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer8->Add( num9_btn, 0, 0, 0 );
	
	
	bSizer8->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer4->Add( bSizer8, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer9->Add( 0, 0, 1, wxEXPAND, 5 );
	
	num4_btn = new wxButton( this, wxID_ANY, wxT("4"), wxDefaultPosition, wxSize( 70,70 ), 0 );
	num4_btn->SetFont( wxFont( 20, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer9->Add( num4_btn, 0, 0, 0 );
	
	num5_btn = new wxButton( this, wxID_ANY, wxT("5"), wxDefaultPosition, wxSize( 70,70 ), 0 );
	num5_btn->SetFont( wxFont( 20, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer9->Add( num5_btn, 0, 0, 0 );
	
	num6_btn = new wxButton( this, wxID_ANY, wxT("6"), wxDefaultPosition, wxSize( 70,70 ), 0 );
	num6_btn->SetFont( wxFont( 20, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer9->Add( num6_btn, 0, 0, 0 );
	
	
	bSizer9->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer4->Add( bSizer9, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer10->Add( 0, 0, 1, wxEXPAND, 5 );
	
	num1_btn = new wxButton( this, wxID_ANY, wxT("1"), wxDefaultPosition, wxSize( 70,70 ), 0 );
	num1_btn->SetFont( wxFont( 20, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer10->Add( num1_btn, 0, 0, 0 );
	
	num2_btn = new wxButton( this, wxID_ANY, wxT("2"), wxDefaultPosition, wxSize( 70,70 ), 0 );
	num2_btn->SetFont( wxFont( 20, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer10->Add( num2_btn, 0, 0, 0 );
	
	num3_btn = new wxButton( this, wxID_ANY, wxT("3"), wxDefaultPosition, wxSize( 70,70 ), 0 );
	num3_btn->SetFont( wxFont( 20, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer10->Add( num3_btn, 0, 0, 0 );
	
	
	bSizer10->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer4->Add( bSizer10, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer11->Add( 0, 0, 1, wxEXPAND, 5 );
	
	num0_btn = new wxButton( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( 70,70 ), 0 );
	num0_btn->SetFont( wxFont( 20, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer11->Add( num0_btn, 0, 0, 0 );
	
	
	bSizer11->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer4->Add( bSizer11, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	
	bSizer12->Add( 0, 0, 1, wxEXPAND, 5 );
	
	clear_btn = new wxButton( this, wxID_ANY, wxT("Clear"), wxDefaultPosition, wxSize( 100,40 ), 0 );
	clear_btn->SetFont( wxFont( 14, 74, 90, 92, false, wxT("Arial Narrow") ) );
	
	bSizer12->Add( clear_btn, 0, wxALIGN_CENTER, 0 );
	
	
	bSizer12->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer4->Add( bSizer12, 1, wxEXPAND, 5 );
	
	bSizer2->Add( bSizer4, 1, wxEXPAND, 5 );
	
	mainbSizer->Add( bSizer2, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer13->Add( 0, 0, 1, wxEXPAND, 5 );
	
	ok_btn = new wxButton( this, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	ok_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	ok_btn->SetBackgroundColour( wxColour( 204, 255, 204 ) );
	
	bSizer13->Add( ok_btn, 0, 0, 0 );
	
	
	bSizer13->Add( 0, 0, 1, wxEXPAND, 5 );
	
	hide_btn = new wxButton( this, wxID_ANY, wxT("Hide"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	hide_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer13->Add( hide_btn, 0, 0, 0 );
	
	
	bSizer13->Add( 0, 0, 1, wxEXPAND, 5 );
	
	cancel_btn = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	cancel_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	cancel_btn->SetBackgroundColour( wxColour( 255, 204, 204 ) );
	
	bSizer13->Add( cancel_btn, 0, 0, 0 );
	
	
	bSizer13->Add( 0, 0, 1, wxEXPAND, 5 );
	
	mainbSizer->Add( bSizer13, 0, wxEXPAND, 5 );
	
	this->SetSizer( mainbSizer );
	this->Layout();
	
	// Connect Events
	num7_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_tip_amount_f::btnClick ), NULL, this );
	num8_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_tip_amount_f::btnClick ), NULL, this );
	num9_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_tip_amount_f::btnClick ), NULL, this );
	num4_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_tip_amount_f::btnClick ), NULL, this );
	num5_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_tip_amount_f::btnClick ), NULL, this );
	num6_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_tip_amount_f::btnClick ), NULL, this );
	num1_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_tip_amount_f::btnClick ), NULL, this );
	num2_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_tip_amount_f::btnClick ), NULL, this );
	num3_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_tip_amount_f::btnClick ), NULL, this );
	num0_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_tip_amount_f::btnClick ), NULL, this );
	clear_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_tip_amount_f::clear_btnClick ), NULL, this );
	ok_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_tip_amount_f::ok_btnClick ), NULL, this );
	hide_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_tip_amount_f::hide_btnClick ), NULL, this );
	cancel_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_tip_amount_f::cancel_btnClick ), NULL, this );
}

base_tip_amount_f::~base_tip_amount_f()
{
	// Disconnect Events
	num7_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_tip_amount_f::btnClick ), NULL, this );
	num8_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_tip_amount_f::btnClick ), NULL, this );
	num9_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_tip_amount_f::btnClick ), NULL, this );
	num4_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_tip_amount_f::btnClick ), NULL, this );
	num5_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_tip_amount_f::btnClick ), NULL, this );
	num6_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_tip_amount_f::btnClick ), NULL, this );
	num1_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_tip_amount_f::btnClick ), NULL, this );
	num2_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_tip_amount_f::btnClick ), NULL, this );
	num3_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_tip_amount_f::btnClick ), NULL, this );
	num0_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_tip_amount_f::btnClick ), NULL, this );
	clear_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_tip_amount_f::clear_btnClick ), NULL, this );
	ok_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_tip_amount_f::ok_btnClick ), NULL, this );
	hide_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_tip_amount_f::hide_btnClick ), NULL, this );
	cancel_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_tip_amount_f::cancel_btnClick ), NULL, this );
}

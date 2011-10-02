///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "base_input_num_f.h"

///////////////////////////////////////////////////////////////////////////

base_input_num_f::base_input_num_f( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	ed = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT|wxTAB_TRAVERSAL|wxWANTS_CHARS );
	ed->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer1->Add( ed, 0, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	num7_btn = new wxButton( this, wxID_ANY, wxT("7"), wxDefaultPosition, wxSize( 70,70 ), 0|wxWANTS_CHARS );
	num7_btn->SetFont( wxFont( 20, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer2->Add( num7_btn, 0, 0, 0 );
	
	num8_btn = new wxButton( this, wxID_ANY, wxT("8"), wxDefaultPosition, wxSize( 70,70 ), 0|wxWANTS_CHARS );
	num8_btn->SetFont( wxFont( 20, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer2->Add( num8_btn, 0, 0, 0 );
	
	num9_btn = new wxButton( this, wxID_ANY, wxT("9"), wxDefaultPosition, wxSize( 70,70 ), 0|wxWANTS_CHARS );
	num9_btn->SetFont( wxFont( 20, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer2->Add( num9_btn, 0, 0, 0 );
	
	
	bSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer1->Add( bSizer2, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	num4_btn = new wxButton( this, wxID_ANY, wxT("4"), wxDefaultPosition, wxSize( 70,70 ), 0|wxWANTS_CHARS );
	num4_btn->SetFont( wxFont( 20, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer3->Add( num4_btn, 0, 0, 0 );
	
	num5_btn = new wxButton( this, wxID_ANY, wxT("5"), wxDefaultPosition, wxSize( 70,70 ), 0|wxWANTS_CHARS );
	num5_btn->SetFont( wxFont( 20, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer3->Add( num5_btn, 0, 0, 0 );
	
	num6_btn = new wxButton( this, wxID_ANY, wxT("6"), wxDefaultPosition, wxSize( 70,70 ), 0|wxWANTS_CHARS );
	num6_btn->SetFont( wxFont( 20, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer3->Add( num6_btn, 0, 0, 0 );
	
	
	bSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer1->Add( bSizer3, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer4->Add( 0, 0, 1, wxEXPAND, 5 );
	
	num1_btn = new wxButton( this, wxID_ANY, wxT("1"), wxDefaultPosition, wxSize( 70,70 ), 0|wxWANTS_CHARS );
	num1_btn->SetFont( wxFont( 20, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer4->Add( num1_btn, 0, 0, 0 );
	
	num2_btn = new wxButton( this, wxID_ANY, wxT("2"), wxDefaultPosition, wxSize( 70,70 ), 0|wxWANTS_CHARS );
	num2_btn->SetFont( wxFont( 20, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer4->Add( num2_btn, 0, 0, 0 );
	
	num3_btn = new wxButton( this, wxID_ANY, wxT("3"), wxDefaultPosition, wxSize( 70,70 ), 0|wxWANTS_CHARS );
	num3_btn->SetFont( wxFont( 20, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer4->Add( num3_btn, 0, 0, 0 );
	
	
	bSizer4->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer1->Add( bSizer4, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer5->Add( 0, 0, 1, wxEXPAND, 5 );
	
	num0_btn = new wxButton( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( 70,70 ), 0|wxWANTS_CHARS );
	num0_btn->SetFont( wxFont( 20, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer5->Add( num0_btn, 0, 0, 0 );
	
	
	bSizer5->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer1->Add( bSizer5, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	clear_btn = new wxButton( this, wxID_ANY, wxT("Clear"), wxDefaultPosition, wxSize( 100,40 ), 0|wxWANTS_CHARS );
	clear_btn->SetFont( wxFont( 14, 74, 90, 92, false, wxT("Arial Narrow") ) );
	
	bSizer6->Add( clear_btn, 0, wxALIGN_CENTER, 5 );
	
	
	bSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer1->Add( bSizer6, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer7->Add( 0, 0, 1, wxEXPAND, 5 );
	
	ok_btn = new wxButton( this, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxSize( 150,40 ), 0|wxWANTS_CHARS );
	ok_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	ok_btn->SetBackgroundColour( wxColour( 204, 255, 204 ) );
	ok_btn->Enable( false );
	
	bSizer7->Add( ok_btn, 0, 0, 0 );
	
	
	bSizer7->Add( 0, 0, 1, wxEXPAND, 5 );
	
	cancel_btn = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxSize( 150,40 ), 0|wxWANTS_CHARS );
	cancel_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	cancel_btn->SetBackgroundColour( wxColour( 255, 204, 204 ) );
	
	bSizer7->Add( cancel_btn, 0, 0, 0 );
	
	
	bSizer7->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer1->Add( bSizer7, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CHAR, wxKeyEventHandler( base_input_num_f::onChar ) );
	ed->Connect( wxEVT_CHAR, wxKeyEventHandler( base_input_num_f::onChar ), NULL, this );
	num7_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_input_num_f::btnClick ), NULL, this );
	num7_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_input_num_f::onChar ), NULL, this );
	num8_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_input_num_f::btnClick ), NULL, this );
	num8_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_input_num_f::onChar ), NULL, this );
	num9_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_input_num_f::btnClick ), NULL, this );
	num9_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_input_num_f::onChar ), NULL, this );
	num4_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_input_num_f::btnClick ), NULL, this );
	num4_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_input_num_f::onChar ), NULL, this );
	num5_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_input_num_f::btnClick ), NULL, this );
	num5_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_input_num_f::onChar ), NULL, this );
	num6_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_input_num_f::btnClick ), NULL, this );
	num6_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_input_num_f::onChar ), NULL, this );
	num1_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_input_num_f::btnClick ), NULL, this );
	num1_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_input_num_f::onChar ), NULL, this );
	num2_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_input_num_f::btnClick ), NULL, this );
	num2_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_input_num_f::onChar ), NULL, this );
	num3_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_input_num_f::btnClick ), NULL, this );
	num3_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_input_num_f::onChar ), NULL, this );
	num0_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_input_num_f::btnClick ), NULL, this );
	num0_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_input_num_f::onChar ), NULL, this );
	clear_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_input_num_f::clear_btnClick ), NULL, this );
	clear_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_input_num_f::onChar ), NULL, this );
	ok_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_input_num_f::ok_btnClick ), NULL, this );
	ok_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_input_num_f::onChar ), NULL, this );
	cancel_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_input_num_f::cancel_btnClick ), NULL, this );
	cancel_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_input_num_f::onChar ), NULL, this );
}

base_input_num_f::~base_input_num_f()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_input_num_f::onChar ) );
	ed->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_input_num_f::onChar ), NULL, this );
	num7_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_input_num_f::btnClick ), NULL, this );
	num7_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_input_num_f::onChar ), NULL, this );
	num8_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_input_num_f::btnClick ), NULL, this );
	num8_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_input_num_f::onChar ), NULL, this );
	num9_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_input_num_f::btnClick ), NULL, this );
	num9_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_input_num_f::onChar ), NULL, this );
	num4_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_input_num_f::btnClick ), NULL, this );
	num4_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_input_num_f::onChar ), NULL, this );
	num5_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_input_num_f::btnClick ), NULL, this );
	num5_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_input_num_f::onChar ), NULL, this );
	num6_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_input_num_f::btnClick ), NULL, this );
	num6_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_input_num_f::onChar ), NULL, this );
	num1_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_input_num_f::btnClick ), NULL, this );
	num1_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_input_num_f::onChar ), NULL, this );
	num2_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_input_num_f::btnClick ), NULL, this );
	num2_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_input_num_f::onChar ), NULL, this );
	num3_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_input_num_f::btnClick ), NULL, this );
	num3_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_input_num_f::onChar ), NULL, this );
	num0_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_input_num_f::btnClick ), NULL, this );
	num0_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_input_num_f::onChar ), NULL, this );
	clear_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_input_num_f::clear_btnClick ), NULL, this );
	clear_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_input_num_f::onChar ), NULL, this );
	ok_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_input_num_f::ok_btnClick ), NULL, this );
	ok_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_input_num_f::onChar ), NULL, this );
	cancel_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_input_num_f::cancel_btnClick ), NULL, this );
	cancel_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_input_num_f::onChar ), NULL, this );
}

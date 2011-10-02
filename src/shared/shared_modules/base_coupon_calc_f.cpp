///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "base_coupon_calc_f.h"

///////////////////////////////////////////////////////////////////////////

base_coupon_calc_f::base_coupon_calc_f( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxVERTICAL );
	
	procent_btn = new wxButton( this, wxID_ANY, wxT("Discount in percent"), wxDefaultPosition, wxSize( -1,30 ), 0|wxTAB_TRAVERSAL|wxWANTS_CHARS );
	procent_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	procent_btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER ) );
	
	bSizer16->Add( procent_btn, 0, wxEXPAND, 5 );
	
	
	bSizer16->Add( 0, 5, 0, wxEXPAND, 5 );
	
	money_btn = new wxButton( this, wxID_ANY, wxT("Discount in $"), wxDefaultPosition, wxSize( -1,30 ), 0|wxTAB_TRAVERSAL|wxWANTS_CHARS );
	money_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	money_btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER ) );
	
	bSizer16->Add( money_btn, 0, wxEXPAND, 5 );
	
	
	bSizer16->Add( 0, 5, 0, wxEXPAND, 0 );
	
	procent_ed = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT|wxTAB_TRAVERSAL|wxWANTS_CHARS );
	procent_ed->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	procent_ed->Enable( false );
	
	bSizer16->Add( procent_ed, 0, wxEXPAND, 5 );
	
	
	bSizer16->Add( 0, 5, 0, wxEXPAND, 0 );
	
	money_ed = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT|wxTAB_TRAVERSAL|wxWANTS_CHARS );
	money_ed->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	money_ed->Enable( false );
	
	bSizer16->Add( money_ed, 0, wxEXPAND, 5 );
	
	
	bSizer16->Add( 0, 5, 0, wxEXPAND, 5 );
	
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 4, 3, 0, 0 );
	
	num07_btn = new wxButton( this, wxID_ANY, wxT("7"), wxDefaultPosition, wxSize( 70,70 ), 0|wxWANTS_CHARS );
	num07_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	gSizer1->Add( num07_btn, 0, wxALIGN_CENTER|wxALL, 0 );
	
	num08_btn = new wxButton( this, wxID_ANY, wxT("8"), wxDefaultPosition, wxSize( 70,70 ), 0|wxWANTS_CHARS );
	num08_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	gSizer1->Add( num08_btn, 0, wxALIGN_CENTER|wxALL, 0 );
	
	num09_btn = new wxButton( this, wxID_ANY, wxT("9"), wxDefaultPosition, wxSize( 70,70 ), 0|wxWANTS_CHARS );
	num09_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	gSizer1->Add( num09_btn, 0, wxALIGN_CENTER|wxALL, 0 );
	
	num04_btn = new wxButton( this, wxID_ANY, wxT("4"), wxDefaultPosition, wxSize( 70,70 ), 0|wxWANTS_CHARS );
	num04_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	gSizer1->Add( num04_btn, 0, wxALIGN_CENTER|wxALL, 0 );
	
	num05_btn = new wxButton( this, wxID_ANY, wxT("5"), wxDefaultPosition, wxSize( 70,70 ), 0|wxWANTS_CHARS );
	num05_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	gSizer1->Add( num05_btn, 0, wxALIGN_CENTER|wxALL, 0 );
	
	num06_btn = new wxButton( this, wxID_ANY, wxT("6"), wxDefaultPosition, wxSize( 70,70 ), 0|wxWANTS_CHARS );
	num06_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	gSizer1->Add( num06_btn, 0, wxALIGN_CENTER|wxALL, 0 );
	
	num01_btn = new wxButton( this, wxID_ANY, wxT("1"), wxDefaultPosition, wxSize( 70,70 ), 0|wxWANTS_CHARS );
	num01_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	gSizer1->Add( num01_btn, 0, wxALIGN_CENTER, 0 );
	
	num02_btn = new wxButton( this, wxID_ANY, wxT("2"), wxDefaultPosition, wxSize( 70,70 ), 0|wxWANTS_CHARS );
	num02_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	gSizer1->Add( num02_btn, 0, wxALIGN_CENTER|wxALL, 0 );
	
	num03_btn = new wxButton( this, wxID_ANY, wxT("3"), wxDefaultPosition, wxSize( 70,70 ), 0|wxWANTS_CHARS );
	num03_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	gSizer1->Add( num03_btn, 0, wxALIGN_CENTER|wxALL, 0 );
	
	wxButton* m_button46;
	m_button46 = new wxButton( this, wxID_ANY, wxT("MyButton"), wxDefaultPosition, wxSize( 70,70 ), 0 );
	m_button46->Hide();
	
	gSizer1->Add( m_button46, 0, wxALL, 5 );
	
	num00_btn = new wxButton( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( 70,70 ), 0|wxWANTS_CHARS );
	num00_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	gSizer1->Add( num00_btn, 0, wxALIGN_CENTER|wxALL, 0 );
	
	wxButton* m_button48;
	m_button48 = new wxButton( this, wxID_ANY, wxT("MyButton"), wxDefaultPosition, wxSize( 70,70 ), 0 );
	m_button48->Hide();
	
	gSizer1->Add( m_button48, 0, wxALL, 5 );
	
	bSizer16->Add( gSizer1, 0, wxALIGN_CENTER, 0 );
	
	clear_btn = new wxButton( this, wxID_ANY, wxT("Clear"), wxDefaultPosition, wxSize( 100,40 ), 0|wxWANTS_CHARS );
	clear_btn->SetFont( wxFont( 16, 74, 90, 92, false, wxT("Arial Narrow") ) );
	
	bSizer16->Add( clear_btn, 0, wxALIGN_CENTER|wxALL, 5 );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer17->Add( 0, 0, 1, wxEXPAND, 5 );
	
	ok_btn = new wxButton( this, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxSize( 100,50 ), 0|wxWANTS_CHARS );
	ok_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	ok_btn->SetBackgroundColour( wxColour( 204, 255, 204 ) );
	ok_btn->Enable( false );
	
	bSizer17->Add( ok_btn, 0, wxEXPAND, 5 );
	
	
	bSizer17->Add( 0, 0, 2, wxEXPAND, 5 );
	
	cancel_btn = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxSize( 100,50 ), 0|wxWANTS_CHARS );
	cancel_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	cancel_btn->SetBackgroundColour( wxColour( 255, 204, 204 ) );
	
	bSizer17->Add( cancel_btn, 0, wxEXPAND, 5 );
	
	
	bSizer17->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer16->Add( bSizer17, 0, wxEXPAND, 0 );
	
	this->SetSizer( bSizer16 );
	this->Layout();
	bSizer16->Fit( this );
	
	// Connect Events
	this->Connect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ) );
	procent_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_coupon_calc_f::type_btnClick ), NULL, this );
	procent_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	money_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_coupon_calc_f::type_btnClick ), NULL, this );
	money_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	procent_ed->Connect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	money_ed->Connect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	num07_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_coupon_calc_f::num_btnClick ), NULL, this );
	num07_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	num08_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_coupon_calc_f::num_btnClick ), NULL, this );
	num08_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	num09_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_coupon_calc_f::num_btnClick ), NULL, this );
	num09_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	num04_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_coupon_calc_f::num_btnClick ), NULL, this );
	num04_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	num05_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_coupon_calc_f::num_btnClick ), NULL, this );
	num05_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	num06_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_coupon_calc_f::num_btnClick ), NULL, this );
	num06_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	num01_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_coupon_calc_f::num_btnClick ), NULL, this );
	num01_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	num02_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_coupon_calc_f::num_btnClick ), NULL, this );
	num02_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	num03_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_coupon_calc_f::num_btnClick ), NULL, this );
	num03_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	num00_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_coupon_calc_f::num_btnClick ), NULL, this );
	num00_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	clear_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_coupon_calc_f::clear_btnClick ), NULL, this );
	clear_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	ok_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_coupon_calc_f::ok_btnClick ), NULL, this );
	ok_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	cancel_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_coupon_calc_f::cancel_btnClick ), NULL, this );
	cancel_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
}

base_coupon_calc_f::~base_coupon_calc_f()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ) );
	procent_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_coupon_calc_f::type_btnClick ), NULL, this );
	procent_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	money_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_coupon_calc_f::type_btnClick ), NULL, this );
	money_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	procent_ed->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	money_ed->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	num07_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_coupon_calc_f::num_btnClick ), NULL, this );
	num07_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	num08_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_coupon_calc_f::num_btnClick ), NULL, this );
	num08_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	num09_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_coupon_calc_f::num_btnClick ), NULL, this );
	num09_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	num04_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_coupon_calc_f::num_btnClick ), NULL, this );
	num04_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	num05_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_coupon_calc_f::num_btnClick ), NULL, this );
	num05_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	num06_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_coupon_calc_f::num_btnClick ), NULL, this );
	num06_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	num01_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_coupon_calc_f::num_btnClick ), NULL, this );
	num01_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	num02_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_coupon_calc_f::num_btnClick ), NULL, this );
	num02_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	num03_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_coupon_calc_f::num_btnClick ), NULL, this );
	num03_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	num00_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_coupon_calc_f::num_btnClick ), NULL, this );
	num00_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	clear_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_coupon_calc_f::clear_btnClick ), NULL, this );
	clear_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	ok_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_coupon_calc_f::ok_btnClick ), NULL, this );
	ok_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
	cancel_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_coupon_calc_f::cancel_btnClick ), NULL, this );
	cancel_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_coupon_calc_f::onChar ), NULL, this );
}

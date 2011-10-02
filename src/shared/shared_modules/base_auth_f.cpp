///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "base_auth_f.h"

///////////////////////////////////////////////////////////////////////////

base_auth_f::base_auth_f( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxDialog( parent, id, title, pos, size, style, name )
{
	this->SetSizeHints( wxSize( -1,-1 ), wxSize( -1,-1 ) );
	this->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	this->SetBackgroundColour( wxColour( 0, 0, 255 ) );
	
	main_sizerV = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* top_sizerH;
	top_sizerH = new wxBoxSizer( wxHORIZONTAL );
	
	m_cc_bmp = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	top_sizerH->Add( m_cc_bmp, 0, wxALL, 5 );
	
	message_lb = new wxStaticText( this, wxID_ANY, wxT("swipe your magnetic card to sign in"), wxDefaultPosition, wxDefaultSize, 0 );
	message_lb->Wrap( -1 );
	message_lb->SetFont( wxFont( 11, 74, 90, 90, false, wxT("Arial Narrow") ) );
	message_lb->SetForegroundColour( wxColour( 255, 255, 255 ) );
	
	top_sizerH->Add( message_lb, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	main_sizerV->Add( top_sizerH, 0, wxALIGN_TOP|wxSHAPED|wxEXPAND, 0 );
	
	auth_sizerFG = new wxFlexGridSizer( 3, 2, 0, 10 );
	auth_sizerFG->SetFlexibleDirection( wxBOTH );
	auth_sizerFG->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	login_lb = new wxStaticText( this, wxID_ANY, wxT("login"), wxDefaultPosition, wxDefaultSize, 0 );
	login_lb->Wrap( -1 );
	login_lb->SetFont( wxFont( 11, 74, 90, 90, false, wxT("Arial Narrow") ) );
	login_lb->SetForegroundColour( wxColour( 255, 255, 255 ) );
	
	auth_sizerFG->Add( login_lb, 0, wxALL, 5 );
	
	login_ed = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	login_ed->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	auth_sizerFG->Add( login_ed, 0, wxALL, 5 );
	
	password_lb = new wxStaticText( this, wxID_ANY, wxT("password"), wxDefaultPosition, wxDefaultSize, 0 );
	password_lb->Wrap( -1 );
	password_lb->SetFont( wxFont( 11, 74, 90, 90, false, wxT("Arial Narrow") ) );
	password_lb->SetForegroundColour( wxColour( 255, 255, 255 ) );
	
	auth_sizerFG->Add( password_lb, 0, wxALL, 5 );
	
	password_ed = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
	password_ed->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	auth_sizerFG->Add( password_ed, 0, wxALL, 5 );
	
	host_lb = new wxStaticText( this, wxID_ANY, wxT("host"), wxDefaultPosition, wxDefaultSize, 0 );
	host_lb->Wrap( -1 );
	host_lb->SetFont( wxFont( 11, 74, 90, 90, false, wxT("Arial Narrow") ) );
	host_lb->SetForegroundColour( wxColour( 255, 255, 255 ) );
	
	auth_sizerFG->Add( host_lb, 0, wxALL, 5 );
	
	host_ed = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	host_ed->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	auth_sizerFG->Add( host_ed, 0, wxALL, 5 );
	
	main_sizerV->Add( auth_sizerFG, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_TOP|wxALL, 0 );
	
	btn_sizeGB = new wxGridBagSizer( 0, 0 );
	btn_sizeGB->AddGrowableCol( 0 );
	btn_sizeGB->AddGrowableRow( 2 );
	btn_sizeGB->SetFlexibleDirection( wxBOTH );
	btn_sizeGB->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	ok_btn = new wxButton( this, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxSize( 120,25 ), 0|wxWANTS_CHARS );
	ok_btn->SetDefault(); 
	ok_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	btn_sizeGB->Add( ok_btn, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	cancel_btn = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxSize( 120,25 ), 0|wxWANTS_CHARS );
	cancel_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	btn_sizeGB->Add( cancel_btn, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	show_kbd_btn = new wxButton( this, wxID_ANY, wxT("Show keyboard"), wxDefaultPosition, wxSize( 120,25 ), 0|wxWANTS_CHARS );
	show_kbd_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	btn_sizeGB->Add( show_kbd_btn, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	advanced_btn = new wxButton( this, wxID_ANY, wxT("Advanced >>"), wxDefaultPosition, wxSize( 120,25 ), 0|wxWANTS_CHARS );
	advanced_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	btn_sizeGB->Add( advanced_btn, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
	main_sizerV->Add( btn_sizeGB, 0, wxALIGN_CENTER_HORIZONTAL, 0 );
	
	this->SetSizer( main_sizerV );
	this->Layout();
	main_sizerV->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CHAR, wxKeyEventHandler( base_auth_f::onChar ) );
	this->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( base_auth_f::onKeyDown ) );
	login_ed->Connect( wxEVT_CHAR, wxKeyEventHandler( base_auth_f::onChar ), NULL, this );
	login_ed->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( base_auth_f::onKeyDown ), NULL, this );
	login_ed->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( base_auth_f::login_edSetFocus ), NULL, this );
	password_ed->Connect( wxEVT_CHAR, wxKeyEventHandler( base_auth_f::onChar ), NULL, this );
	password_ed->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( base_auth_f::onKeyDown ), NULL, this );
	password_ed->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( base_auth_f::password_edSetFocus ), NULL, this );
	host_ed->Connect( wxEVT_CHAR, wxKeyEventHandler( base_auth_f::onChar ), NULL, this );
	host_ed->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( base_auth_f::onKeyDown ), NULL, this );
	host_ed->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( base_auth_f::host_edSetFocus ), NULL, this );
	ok_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_auth_f::ok_btnClick ), NULL, this );
	ok_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_auth_f::onChar ), NULL, this );
	ok_btn->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( base_auth_f::onKeyDown ), NULL, this );
	cancel_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_auth_f::cancel_btnClick ), NULL, this );
	cancel_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_auth_f::onChar ), NULL, this );
	cancel_btn->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( base_auth_f::onKeyDown ), NULL, this );
	show_kbd_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_auth_f::show_kbd_btnClick ), NULL, this );
	show_kbd_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_auth_f::onChar ), NULL, this );
	show_kbd_btn->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( base_auth_f::onKeyDown ), NULL, this );
	advanced_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_auth_f::advanced_btnClick ), NULL, this );
	advanced_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_auth_f::onChar ), NULL, this );
	advanced_btn->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( base_auth_f::onKeyDown ), NULL, this );
}

base_auth_f::~base_auth_f()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_auth_f::onChar ) );
	this->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( base_auth_f::onKeyDown ) );
	login_ed->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_auth_f::onChar ), NULL, this );
	login_ed->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( base_auth_f::onKeyDown ), NULL, this );
	login_ed->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( base_auth_f::login_edSetFocus ), NULL, this );
	password_ed->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_auth_f::onChar ), NULL, this );
	password_ed->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( base_auth_f::onKeyDown ), NULL, this );
	password_ed->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( base_auth_f::password_edSetFocus ), NULL, this );
	host_ed->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_auth_f::onChar ), NULL, this );
	host_ed->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( base_auth_f::onKeyDown ), NULL, this );
	host_ed->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( base_auth_f::host_edSetFocus ), NULL, this );
	ok_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_auth_f::ok_btnClick ), NULL, this );
	ok_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_auth_f::onChar ), NULL, this );
	ok_btn->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( base_auth_f::onKeyDown ), NULL, this );
	cancel_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_auth_f::cancel_btnClick ), NULL, this );
	cancel_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_auth_f::onChar ), NULL, this );
	cancel_btn->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( base_auth_f::onKeyDown ), NULL, this );
	show_kbd_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_auth_f::show_kbd_btnClick ), NULL, this );
	show_kbd_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_auth_f::onChar ), NULL, this );
	show_kbd_btn->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( base_auth_f::onKeyDown ), NULL, this );
	advanced_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_auth_f::advanced_btnClick ), NULL, this );
	advanced_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_auth_f::onChar ), NULL, this );
	advanced_btn->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( base_auth_f::onKeyDown ), NULL, this );
}

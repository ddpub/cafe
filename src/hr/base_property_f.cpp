///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "base_property_f.h"

///////////////////////////////////////////////////////////////////////////

base_property_f::base_property_f( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* msr_sizer;
	msr_sizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Settings for MSR206") ), wxVERTICAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	port_lp = new wxStaticText( this, wxID_ANY, wxT("Com port"), wxDefaultPosition, wxDefaultSize, 0 );
	port_lp->Wrap( -1 );
	bSizer2->Add( port_lp, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	port_cb = new wxComboBox( this, wxID_ANY, wxT("COM1"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	port_cb->Append( wxT("COM1") );
	port_cb->Append( wxT("COM2") );
	port_cb->Append( wxT("COM3") );
	port_cb->Append( wxT("COM4") );
	port_cb->Append( wxT("COM5") );
	port_cb->Append( wxT("COM6") );
	port_cb->Append( wxT("COM7") );
	port_cb->Append( wxT("COM8") );
	port_cb->Append( wxT("COM9") );
	port_cb->Append( wxT("COM10") );
	bSizer2->Add( port_cb, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	btn_test = new wxButton( this, wxID_ANY, wxT("Test"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( btn_test, 0, wxALL, 5 );
	
	msr_sizer->Add( bSizer2, 1, wxEXPAND, 0 );
	
	bSizer1->Add( msr_sizer, 0, wxEXPAND, 0 );
	
	wxStaticBoxSizer* locale_sizer;
	locale_sizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Locale settings") ), wxVERTICAL );
	
	ck_lang = new wxCheckBox( this, wxID_ANY, wxT("Load lanuage"), wxDefaultPosition, wxDefaultSize, 0 );
	
	locale_sizer->Add( ck_lang, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );
	
	lb_file = new wxStaticText( this, wxID_ANY, wxT("File name"), wxDefaultPosition, wxSize( 80,-1 ), wxALIGN_RIGHT );
	lb_file->Wrap( -1 );
	bSizer10->Add( lb_file, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 4 );
	
	
	bSizer10->Add( 3, 0, 0, wxEXPAND, 5 );
	
	ed_lang = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 140,-1 ), 0 );
	ed_lang->Enable( false );
	
	bSizer10->Add( ed_lang, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 0 );
	
	btn_load_lang = new wxButton( this, wxID_ANY, wxT("..."), wxDefaultPosition, wxSize( 20,20 ), 0 );
	btn_load_lang->Enable( false );
	btn_load_lang->SetMinSize( wxSize( 20,20 ) );
	btn_load_lang->SetMaxSize( wxSize( 20,20 ) );
	
	bSizer10->Add( btn_load_lang, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 1 );
	
	locale_sizer->Add( bSizer10, 0, wxEXPAND, 0 );
	
	
	locale_sizer->Add( 0, 5, 0, wxEXPAND, 1 );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxHORIZONTAL );
	
	lb_locale_name = new wxStaticText( this, wxID_ANY, wxT("Locale name"), wxDefaultPosition, wxSize( 80,-1 ), wxALIGN_RIGHT );
	lb_locale_name->Wrap( -1 );
	bSizer11->Add( lb_locale_name, 0, wxALIGN_CENTER_VERTICAL, 4 );
	
	
	bSizer11->Add( 3, 0, 0, wxEXPAND, 5 );
	
	cb_locale = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 160,-1 ), 0, NULL, 0 ); 
	cb_locale->Enable( false );
	
	bSizer11->Add( cb_locale, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 0 );
	
	locale_sizer->Add( bSizer11, 0, wxEXPAND, 0 );
	
	bSizer1->Add( locale_sizer, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer8->Add( 0, 0, 1, wxEXPAND, 5 );
	
	btn_ok = new wxButton( this, ID_BTNOK, wxT("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( btn_ok, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	bSizer8->Add( 0, 0, 1, wxEXPAND, 5 );
	
	btn_cancel = new wxButton( this, ID_BTNCANCEL, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( btn_cancel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	bSizer8->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer1->Add( bSizer8, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	// Connect Events
	port_cb->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( base_property_f::OnComChanged ), NULL, this );
	btn_test->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_property_f::OnTest ), NULL, this );
	ck_lang->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( base_property_f::OnCheckLang ), NULL, this );
	ed_lang->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( base_property_f::OnFileChange ), NULL, this );
	btn_load_lang->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_property_f::OnLoadFile ), NULL, this );
	cb_locale->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( base_property_f::OnLocaleLoad ), NULL, this );
	btn_ok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_property_f::OnClose ), NULL, this );
	btn_cancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_property_f::OnClose ), NULL, this );
}

base_property_f::~base_property_f()
{
	// Disconnect Events
	port_cb->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( base_property_f::OnComChanged ), NULL, this );
	btn_test->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_property_f::OnTest ), NULL, this );
	ck_lang->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( base_property_f::OnCheckLang ), NULL, this );
	ed_lang->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( base_property_f::OnFileChange ), NULL, this );
	btn_load_lang->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_property_f::OnLoadFile ), NULL, this );
	cb_locale->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( base_property_f::OnLocaleLoad ), NULL, this );
	btn_ok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_property_f::OnClose ), NULL, this );
	btn_cancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_property_f::OnClose ), NULL, this );
}

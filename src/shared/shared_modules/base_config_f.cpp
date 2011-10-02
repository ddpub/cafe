///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "base_config_f.h"

///////////////////////////////////////////////////////////////////////////

base_config_f::base_config_f( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_notebook = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP );
	m_notebook->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	m_panel1 = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 2, 2, 0, 300 );
	
	lock_timeout_lb = new wxStaticText( m_panel1, wxID_ANY, wxT("Application lock timeout"), wxDefaultPosition, wxDefaultSize, 0 );
	lock_timeout_lb->Wrap( -1 );
	lock_timeout_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	gSizer1->Add( lock_timeout_lb, 0, wxALL|wxEXPAND, 5 );
	
	wxString lock_timeout_cbChoices[] = { wxT("never"), wxT("15 sec"), wxT("30 sec"), wxT("1 min"), wxT("5 min"), wxT("10 min") };
	int lock_timeout_cbNChoices = sizeof( lock_timeout_cbChoices ) / sizeof( wxString );
	lock_timeout_cb = new wxChoice( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, lock_timeout_cbNChoices, lock_timeout_cbChoices, 0 );
	lock_timeout_cb->SetSelection( 3 );
	lock_timeout_cb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	gSizer1->Add( lock_timeout_cb, 0, wxALL|wxEXPAND, 5 );
	
	server_status_check_lb = new wxStaticText( m_panel1, wxID_ANY, wxT("Server status check"), wxDefaultPosition, wxDefaultSize, 0 );
	server_status_check_lb->Wrap( -1 );
	server_status_check_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	gSizer1->Add( server_status_check_lb, 0, wxALL|wxEXPAND, 5 );
	
	wxString server_status_check_cbChoices[] = { wxT("never"), wxT("5 sec"), wxT("10 sec"), wxT("15 sec"), wxT("30 sec"), wxT("60 sec") };
	int server_status_check_cbNChoices = sizeof( server_status_check_cbChoices ) / sizeof( wxString );
	server_status_check_cb = new wxChoice( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, server_status_check_cbNChoices, server_status_check_cbChoices, 0 );
	server_status_check_cb->SetSelection( 4 );
	server_status_check_cb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	gSizer1->Add( server_status_check_cb, 0, wxALL|wxEXPAND, 5 );
	
	frac_lb = new wxStaticText( m_panel1, wxID_ANY, wxT("Number of decimals to be typed in cash payment"), wxDefaultPosition, wxDefaultSize, 0 );
	frac_lb->Wrap( -1 );
	frac_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	gSizer1->Add( frac_lb, 0, wxALL|wxEXPAND, 5 );
	
	wxString frac_cbChoices[] = { wxT("0"), wxT("1"), wxT("2") };
	int frac_cbNChoices = sizeof( frac_cbChoices ) / sizeof( wxString );
	frac_cb = new wxChoice( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, frac_cbNChoices, frac_cbChoices, 0 );
	frac_cb->SetSelection( 2 );
	frac_cb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	gSizer1->Add( frac_cb, 0, wxALL|wxEXPAND, 5 );
	
	bSizer2->Add( gSizer1, 0, wxEXPAND, 5 );
	
	vfd_chb = new wxCheckBox( m_panel1, wxID_ANY, wxT("Enable VFD customer display"), wxDefaultPosition, wxDefaultSize, 0 );
	
	vfd_chb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer2->Add( vfd_chb, 0, wxALL|wxEXPAND, 5 );
	
	vfd_sizer = new wxStaticBoxSizer( new wxStaticBox( m_panel1, wxID_ANY, wxT("Customer display welcome message") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 3, 3, 0, 0 );
	fgSizer2->AddGrowableCol( 2 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	vfd_align_lb = new wxStaticText( m_panel1, wxID_ANY, wxT("Align"), wxDefaultPosition, wxDefaultSize, 0 );
	vfd_align_lb->Wrap( -1 );
	vfd_align_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	fgSizer2->Add( vfd_align_lb, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer2->Add( 0, 0, 0, wxEXPAND, 5 );
	
	vfd_text_lb = new wxStaticText( m_panel1, wxID_ANY, wxT("Text"), wxDefaultPosition, wxDefaultSize, 0 );
	vfd_text_lb->Wrap( -1 );
	vfd_text_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	fgSizer2->Add( vfd_text_lb, 0, wxALL|wxEXPAND, 5 );
	
	wxString fline_choiceChoices[] = { wxT("Left"), wxT("Center"), wxT("Right") };
	int fline_choiceNChoices = sizeof( fline_choiceChoices ) / sizeof( wxString );
	fline_choice = new wxChoice( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, fline_choiceNChoices, fline_choiceChoices, 0 );
	fline_choice->SetSelection( 1 );
	fline_choice->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	fgSizer2->Add( fline_choice, 0, wxALL, 5 );
	
	fline_lb = new wxStaticText( m_panel1, wxID_ANY, wxT("Line 1"), wxDefaultPosition, wxDefaultSize, 0 );
	fline_lb->Wrap( -1 );
	fline_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	fgSizer2->Add( fline_lb, 0, wxALL, 5 );
	
	fline_ed = new wxTextCtrl( m_panel1, wxID_ANY, wxT("Welcome"), wxDefaultPosition, wxDefaultSize, 0 );
	fline_ed->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	fgSizer2->Add( fline_ed, 0, wxALL|wxEXPAND, 5 );
	
	wxString sline_choiceChoices[] = { wxT("Left"), wxT("Center"), wxT("Right") };
	int sline_choiceNChoices = sizeof( sline_choiceChoices ) / sizeof( wxString );
	sline_choice = new wxChoice( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, sline_choiceNChoices, sline_choiceChoices, 0 );
	sline_choice->SetSelection( 1 );
	sline_choice->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	fgSizer2->Add( sline_choice, 0, wxALL, 5 );
	
	sline_lb = new wxStaticText( m_panel1, wxID_ANY, wxT("Line 2"), wxDefaultPosition, wxDefaultSize, 0 );
	sline_lb->Wrap( -1 );
	sline_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	fgSizer2->Add( sline_lb, 0, wxALL, 5 );
	
	sline_ed = new wxTextCtrl( m_panel1, wxID_ANY, wxT("to Laparilla Grill"), wxDefaultPosition, wxDefaultSize, 0 );
	sline_ed->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	fgSizer2->Add( sline_ed, 0, wxALL|wxEXPAND, 5 );
	
	vfd_sizer->Add( fgSizer2, 0, wxEXPAND, 5 );
	
	bSizer2->Add( vfd_sizer, 0, wxEXPAND, 5 );
	
	auto_resolution_chb = new wxCheckBox( m_panel1, wxID_ANY, wxT("Auto resolution full screen"), wxDefaultPosition, wxDefaultSize, 0 );
	auto_resolution_chb->SetValue(true);
	
	auto_resolution_chb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer2->Add( auto_resolution_chb, 0, wxALL|wxEXPAND, 5 );
	
	resolution_sizer = new wxStaticBoxSizer( new wxStaticBox( m_panel1, wxID_ANY, wxT("Window Resolution") ), wxVERTICAL );
	
	wxGridSizer* gSizer5;
	gSizer5 = new wxGridSizer( 2, 2, 0, 0 );
	
	screen_width_lb = new wxStaticText( m_panel1, wxID_ANY, wxT("Width"), wxDefaultPosition, wxDefaultSize, 0 );
	screen_width_lb->Wrap( -1 );
	screen_width_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	gSizer5->Add( screen_width_lb, 0, wxALL|wxEXPAND, 5 );
	
	wxString resolution_width_cbChoices[] = { wxT("800"), wxT("1024"), wxT("1280") };
	int resolution_width_cbNChoices = sizeof( resolution_width_cbChoices ) / sizeof( wxString );
	resolution_width_cb = new wxChoice( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, resolution_width_cbNChoices, resolution_width_cbChoices, 0 );
	resolution_width_cb->SetSelection( 1 );
	resolution_width_cb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	resolution_width_cb->Enable( false );
	
	gSizer5->Add( resolution_width_cb, 0, wxALL|wxEXPAND, 5 );
	
	screen_height_lb = new wxStaticText( m_panel1, wxID_ANY, wxT("Height"), wxDefaultPosition, wxDefaultSize, 0 );
	screen_height_lb->Wrap( -1 );
	screen_height_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	gSizer5->Add( screen_height_lb, 0, wxALL|wxEXPAND, 5 );
	
	wxString resolution_height_cbChoices[] = { wxT("600"), wxT("768"), wxT("1024") };
	int resolution_height_cbNChoices = sizeof( resolution_height_cbChoices ) / sizeof( wxString );
	resolution_height_cb = new wxChoice( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, resolution_height_cbNChoices, resolution_height_cbChoices, 0 );
	resolution_height_cb->SetSelection( 1 );
	resolution_height_cb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	resolution_height_cb->Enable( false );
	
	gSizer5->Add( resolution_height_cb, 0, wxALL|wxEXPAND, 5 );
	
	resolution_sizer->Add( gSizer5, 1, wxEXPAND, 5 );
	
	stay_on_top_chb = new wxCheckBox( m_panel1, wxID_ANY, wxT("Always On Top"), wxDefaultPosition, wxDefaultSize, 0 );
	stay_on_top_chb->SetValue(true);
	
	resolution_sizer->Add( stay_on_top_chb, 0, wxALL|wxEXPAND, 5 );
	
	bSizer2->Add( resolution_sizer, 0, wxEXPAND, 5 );
	
	
	bSizer2->Add( 1, 0, 0, wxEXPAND, 5 );
	
	terminate_app_btn = new wxButton( m_panel1, wxID_ANY, wxT("Terminate application"), wxDefaultPosition, wxSize( -1,35 ), 0 );
	terminate_app_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer2->Add( terminate_app_btn, 0, wxALIGN_CENTER_HORIZONTAL|wxBOTTOM|wxSHAPED, 5 );
	
	
	bSizer2->Add( 1, 0, 0, wxEXPAND, 5 );
	
	m_panel1->SetSizer( bSizer2 );
	m_panel1->Layout();
	bSizer2->Fit( m_panel1 );
	m_notebook->AddPage( m_panel1, wxT("Main options"), true );
	m_panel2 = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	cc_heartland_rb = new wxRadioButton( m_panel2, wxID_ANY, wxT("Use Heartland Payment System merchant account"), wxDefaultPosition, wxDefaultSize, 0 );
	cc_heartland_rb->SetValue( true ); 
	bSizer3->Add( cc_heartland_rb, 0, wxALL|wxEXPAND, 5 );
	
	cc_account_sizer = new wxStaticBoxSizer( new wxStaticBox( m_panel2, wxID_ANY, wxT("Heartland merchant account settings") ), wxVERTICAL );
	
	cc_fg_sizer = new wxFlexGridSizer( 5, 2, 0, 0 );
	cc_fg_sizer->SetFlexibleDirection( wxBOTH );
	cc_fg_sizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	License_id_lb = new wxStaticText( m_panel2, wxID_ANY, wxT("License_id"), wxDefaultPosition, wxDefaultSize, 0 );
	License_id_lb->Wrap( -1 );
	License_id_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	cc_fg_sizer->Add( License_id_lb, 0, wxALL|wxEXPAND, 5 );
	
	license_ed = new wxTextCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 150,-1 ), wxTE_READONLY|wxTE_RIGHT );
	license_ed->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	license_ed->Enable( false );
	
	cc_fg_sizer->Add( license_ed, 0, wxALL|wxEXPAND, 5 );
	
	site_id_lb = new wxStaticText( m_panel2, wxID_ANY, wxT("Site_id"), wxDefaultPosition, wxDefaultSize, 0 );
	site_id_lb->Wrap( -1 );
	site_id_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	cc_fg_sizer->Add( site_id_lb, 0, wxALL|wxEXPAND, 5 );
	
	site_ed = new wxTextCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 150,-1 ), wxTE_READONLY|wxTE_RIGHT );
	site_ed->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	site_ed->Enable( false );
	
	cc_fg_sizer->Add( site_ed, 0, wxALL, 5 );
	
	device_id_lb = new wxStaticText( m_panel2, wxID_ANY, wxT("Device_id"), wxDefaultPosition, wxDefaultSize, 0 );
	device_id_lb->Wrap( -1 );
	device_id_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	cc_fg_sizer->Add( device_id_lb, 0, wxALL|wxEXPAND, 5 );
	
	device_ed = new wxTextCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 150,-1 ), wxTE_READONLY|wxTE_RIGHT );
	device_ed->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	device_ed->Enable( false );
	
	cc_fg_sizer->Add( device_ed, 0, wxALL, 5 );
	
	user_lb = new wxStaticText( m_panel2, wxID_ANY, wxT("User"), wxDefaultPosition, wxDefaultSize, 0 );
	user_lb->Wrap( -1 );
	user_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	cc_fg_sizer->Add( user_lb, 0, wxALL|wxEXPAND, 5 );
	
	user_ed = new wxTextCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 150,-1 ), wxTE_READONLY );
	user_ed->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	user_ed->Enable( false );
	
	cc_fg_sizer->Add( user_ed, 0, wxALL, 5 );
	
	password_lb = new wxStaticText( m_panel2, wxID_ANY, wxT("Password"), wxDefaultPosition, wxDefaultSize, 0 );
	password_lb->Wrap( -1 );
	password_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	cc_fg_sizer->Add( password_lb, 0, wxALL|wxEXPAND, 5 );
	
	password_ed = new wxTextCtrl( m_panel2, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 150,-1 ), wxTE_PASSWORD|wxTE_READONLY );
	password_ed->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	password_ed->Enable( false );
	
	cc_fg_sizer->Add( password_ed, 0, wxALL, 5 );
	
	cc_account_sizer->Add( cc_fg_sizer, 1, wxEXPAND, 5 );
	
	test_credentials_btn = new wxButton( m_panel2, wxID_ANY, wxT("Test credentials"), wxDefaultPosition, wxSize( -1,40 ), 0 );
	test_credentials_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	cc_account_sizer->Add( test_credentials_btn, 0, wxALIGN_CENTER|wxALL, 5 );
	
	tip_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	max_tip_val_lb = new wxStaticText( m_panel2, wxID_ANY, wxT("Max. tips value:"), wxDefaultPosition, wxDefaultSize, 0 );
	max_tip_val_lb->Wrap( -1 );
	max_tip_val_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	tip_sizer->Add( max_tip_val_lb, 0, wxALL|wxEXPAND, 5 );
	
	wxString max_tips_value_cbChoices[] = { wxT("0%"), wxT("5%"), wxT("10%"), wxT("15%"), wxT("20%"), wxT("25%"), wxT("30%"), wxT("35%"), wxT("40%"), wxT("50%") };
	int max_tips_value_cbNChoices = sizeof( max_tips_value_cbChoices ) / sizeof( wxString );
	max_tips_value_cb = new wxChoice( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, max_tips_value_cbNChoices, max_tips_value_cbChoices, 0 );
	max_tips_value_cb->SetSelection( 4 );
	max_tips_value_cb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	tip_sizer->Add( max_tips_value_cb, 0, wxALL|wxLEFT, 5 );
	
	cc_account_sizer->Add( tip_sizer, 0, wxEXPAND, 5 );
	
	bSizer3->Add( cc_account_sizer, 0, wxEXPAND, 5 );
	
	cc_stand_alone_rb = new wxRadioButton( m_panel2, wxID_ANY, wxT("Use standalone CC processing device"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( cc_stand_alone_rb, 0, wxALL|wxEXPAND, 5 );
	
	cc_disable_rb = new wxRadioButton( m_panel2, wxID_ANY, wxT("Disable credit card processing"), wxDefaultPosition, wxDefaultSize, 0 );
	cc_disable_rb->SetValue( true ); 
	bSizer3->Add( cc_disable_rb, 0, wxALL|wxEXPAND, 5 );
	
	m_panel2->SetSizer( bSizer3 );
	m_panel2->Layout();
	bSizer3->Fit( m_panel2 );
	m_notebook->AddPage( m_panel2, wxT("CC Settings"), false );
	m_panel3 = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	printer_sizer = new wxStaticBoxSizer( new wxStaticBox( m_panel3, wxID_ANY, wxT("Receipt printer") ), wxVERTICAL );
	
	local_printer_chb = new wxRadioButton( m_panel3, wxID_ANY, wxT("Local printer"), wxDefaultPosition, wxDefaultSize, 0 );
	local_printer_chb->SetValue( true ); 
	printer_sizer->Add( local_printer_chb, 0, wxALL|wxEXPAND, 5 );
	
	local_printer_sizer = new wxStaticBoxSizer( new wxStaticBox( m_panel3, wxID_ANY, wxT("Hardware settings") ), wxVERTICAL );
	
	wxGridSizer* gSizer2;
	gSizer2 = new wxGridSizer( 2, 2, 0, 0 );
	
	printer_port_lb = new wxStaticText( m_panel3, wxID_ANY, wxT("Printer port"), wxDefaultPosition, wxDefaultSize, 0 );
	printer_port_lb->Wrap( -1 );
	printer_port_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	gSizer2->Add( printer_port_lb, 0, wxALL|wxEXPAND, 5 );
	
	wxString printer_port_cbChoices[] = { wxT("COM1"), wxT("COM2"), wxT("COM3"), wxT("COM4") };
	int printer_port_cbNChoices = sizeof( printer_port_cbChoices ) / sizeof( wxString );
	printer_port_cb = new wxChoice( m_panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, printer_port_cbNChoices, printer_port_cbChoices, 0 );
	printer_port_cb->SetSelection( 0 );
	printer_port_cb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	gSizer2->Add( printer_port_cb, 0, wxALL|wxEXPAND, 5 );
	
	command_set_lb = new wxStaticText( m_panel3, wxID_ANY, wxT("Command set"), wxDefaultPosition, wxDefaultSize, 0 );
	command_set_lb->Wrap( -1 );
	command_set_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	gSizer2->Add( command_set_lb, 0, wxALL|wxEXPAND, 5 );
	
	wxString command_set_cbChoices[] = { wxT("STAR"), wxT("EPSON"), wxT("FISCAL REGISTRAR") };
	int command_set_cbNChoices = sizeof( command_set_cbChoices ) / sizeof( wxString );
	command_set_cb = new wxChoice( m_panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, command_set_cbNChoices, command_set_cbChoices, 0 );
	command_set_cb->SetSelection( 0 );
	command_set_cb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	gSizer2->Add( command_set_cb, 0, wxALL|wxEXPAND, 5 );
	
	baud_rate_lb = new wxStaticText( m_panel3, wxID_ANY, wxT("Baud rate"), wxDefaultPosition, wxDefaultSize, 0 );
	baud_rate_lb->Wrap( -1 );
	baud_rate_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	gSizer2->Add( baud_rate_lb, 0, wxALL|wxEXPAND, 5 );
	
	wxString baud_rate_cbChoices[] = { wxT("9600"), wxT("19200"), wxT("57600") };
	int baud_rate_cbNChoices = sizeof( baud_rate_cbChoices ) / sizeof( wxString );
	baud_rate_cb = new wxChoice( m_panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, baud_rate_cbNChoices, baud_rate_cbChoices, 0 );
	baud_rate_cb->SetSelection( 0 );
	baud_rate_cb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	gSizer2->Add( baud_rate_cb, 0, wxALL|wxEXPAND, 5 );
	
	local_printer_sizer->Add( gSizer2, 0, wxEXPAND, 0 );
	
	printer_sizer->Add( local_printer_sizer, 0, wxEXPAND, 0 );
	
	net_printer_chb = new wxRadioButton( m_panel3, wxID_ANY, wxT("Net printer (needs remote print server)"), wxDefaultPosition, wxDefaultSize, 0 );
	printer_sizer->Add( net_printer_chb, 0, wxALL|wxEXPAND, 5 );
	
	host_settings_sizer = new wxStaticBoxSizer( new wxStaticBox( m_panel3, wxID_ANY, wxT("Host settings") ), wxHORIZONTAL );
	
	host_lb = new wxStaticText( m_panel3, wxID_ANY, wxT("Host"), wxDefaultPosition, wxDefaultSize, 0 );
	host_lb->Wrap( -1 );
	host_settings_sizer->Add( host_lb, 0, wxALL|wxEXPAND, 5 );
	
	
	host_settings_sizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	printer_host_ed = new wxTextCtrl( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), 0 );
	printer_host_ed->Enable( false );
	
	host_settings_sizer->Add( printer_host_ed, 0, wxALL|wxEXPAND, 5 );
	
	printer_sizer->Add( host_settings_sizer, 0, wxEXPAND, 0 );
	
	no_printer_chb = new wxRadioButton( m_panel3, wxID_ANY, wxT("No printer"), wxDefaultPosition, wxDefaultSize, 0 );
	printer_sizer->Add( no_printer_chb, 0, wxALL|wxEXPAND, 5 );
	
	bSizer5->Add( printer_sizer, 0, wxEXPAND, 0 );
	
	printings_settings_sizer = new wxStaticBoxSizer( new wxStaticBox( m_panel3, wxID_ANY, wxT("Printing settings") ), wxVERTICAL );
	
	group_by_chb = new wxCheckBox( m_panel3, wxID_ANY, wxT("Group entrees by kitchen groups"), wxDefaultPosition, wxDefaultSize, 0 );
	
	group_by_chb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	printings_settings_sizer->Add( group_by_chb, 0, wxALL|wxEXPAND, 5 );
	
	print_logo_chb = new wxCheckBox( m_panel3, wxID_ANY, wxT("Print logo"), wxDefaultPosition, wxDefaultSize, 0 );
	
	print_logo_chb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	printings_settings_sizer->Add( print_logo_chb, 0, wxALL|wxEXPAND, 5 );
	
	test_printer_btn = new wxButton( m_panel3, wxID_ANY, wxT("Test printer"), wxDefaultPosition, wxSize( -1,40 ), 0 );
	test_printer_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	printings_settings_sizer->Add( test_printer_btn, 0, wxALIGN_CENTER|wxALL|wxSHAPED, 5 );
	
	bSizer5->Add( printings_settings_sizer, 0, wxEXPAND, 5 );
	
	
	bSizer5->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_panel3->SetSizer( bSizer5 );
	m_panel3->Layout();
	bSizer5->Fit( m_panel3 );
	m_notebook->AddPage( m_panel3, wxT("Printer settings "), false );
	m_panel5 = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );
	
	securty_sizer = new wxStaticBoxSizer( new wxStaticBox( m_panel5, wxID_ANY, wxT("Operations needed manager's authorization") ), wxVERTICAL );
	
	auth_reciept_duplicate_chb = new wxCheckBox( m_panel5, wxID_ANY, wxT("Print receipt duplicate"), wxDefaultPosition, wxDefaultSize, 0 );
	auth_reciept_duplicate_chb->SetValue(true);
	
	securty_sizer->Add( auth_reciept_duplicate_chb, 0, wxALL|wxEXPAND, 5 );
	
	auth_refund_chb = new wxCheckBox( m_panel5, wxID_ANY, wxT("Refund order"), wxDefaultPosition, wxDefaultSize, 0 );
	auth_refund_chb->SetValue(true);
	
	securty_sizer->Add( auth_refund_chb, 0, wxALL|wxEXPAND, 5 );
	
	auth_app_settings_chb = new wxCheckBox( m_panel5, wxID_ANY, wxT("Access to application settings"), wxDefaultPosition, wxDefaultSize, 0 );
	auth_app_settings_chb->SetValue(true);
	
	securty_sizer->Add( auth_app_settings_chb, 0, wxALL|wxEXPAND, 5 );
	
	auth_misc_charges_chb = new wxCheckBox( m_panel5, wxID_ANY, wxT("Create misc charges"), wxDefaultPosition, wxDefaultSize, 0 );
	auth_misc_charges_chb->SetValue(true);
	
	securty_sizer->Add( auth_misc_charges_chb, 0, wxALL|wxEXPAND, 5 );
	
	auth_reload_all_menus_chb = new wxCheckBox( m_panel5, wxID_ANY, wxT("Reload all menus"), wxDefaultPosition, wxDefaultSize, 0 );
	auth_reload_all_menus_chb->SetValue(true);
	
	securty_sizer->Add( auth_reload_all_menus_chb, 0, wxALL|wxEXPAND, 5 );
	
	auth_table_booking_chb = new wxCheckBox( m_panel5, wxID_ANY, wxT("Table booking"), wxDefaultPosition, wxDefaultSize, 0 );
	auth_table_booking_chb->SetValue(true);
	
	securty_sizer->Add( auth_table_booking_chb, 0, wxALL|wxEXPAND, 5 );
	
	auth_shift_chb = new wxCheckBox( m_panel5, wxID_ANY, wxT("Seances"), wxDefaultPosition, wxDefaultSize, 0 );
	auth_shift_chb->SetValue(true);
	
	securty_sizer->Add( auth_shift_chb, 0, wxALL|wxEXPAND, 5 );
	
	bSizer8->Add( securty_sizer, 0, wxEXPAND, 5 );
	
	m_panel5->SetSizer( bSizer8 );
	m_panel5->Layout();
	bSizer8->Fit( m_panel5 );
	m_notebook->AddPage( m_panel5, wxT("Security policy"), false );
	m_panel6 = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	entree_codes_chb = new wxCheckBox( m_panel6, wxID_ANY, wxT("Entree codes"), wxDefaultPosition, wxDefaultSize, 0 );
	entree_codes_chb->SetValue(true);
	
	entree_codes_chb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer9->Add( entree_codes_chb, 0, wxALL|wxEXPAND, 2 );
	
	misc_charges_chb = new wxCheckBox( m_panel6, wxID_ANY, wxT("Misc charges"), wxDefaultPosition, wxDefaultSize, 0 );
	misc_charges_chb->SetValue(true);
	
	misc_charges_chb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer9->Add( misc_charges_chb, 0, wxALL|wxEXPAND, 2 );
	
	tokens_chb = new wxCheckBox( m_panel6, wxID_ANY, wxT("Tokens"), wxDefaultPosition, wxDefaultSize, 0 );
	tokens_chb->SetValue(true);
	
	tokens_chb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer9->Add( tokens_chb, 0, wxALL|wxEXPAND, 2 );
	
	cash_input_chb = new wxCheckBox( m_panel6, wxID_ANY, wxT("Show cash input window on order payment"), wxDefaultPosition, wxDefaultSize, 0 );
	cash_input_chb->SetValue(true);
	
	cash_input_chb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer9->Add( cash_input_chb, 0, wxALL|wxEXPAND, 2 );
	
	close_order_operation_auto_close_chb = new wxCheckBox( m_panel6, wxID_ANY, wxT("Close closed orders window after first action"), wxDefaultPosition, wxDefaultSize, 0 );
	close_order_operation_auto_close_chb->SetValue(true);
	
	close_order_operation_auto_close_chb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer9->Add( close_order_operation_auto_close_chb, 0, wxALL|wxEXPAND, 2 );
	
	auto_lock_chb = new wxCheckBox( m_panel6, wxID_ANY, wxT("Lock application after each action"), wxDefaultPosition, wxDefaultSize, 0 );
	auto_lock_chb->SetValue(true);
	
	auto_lock_chb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer9->Add( auto_lock_chb, 0, wxALL|wxEXPAND, 2 );
	
	cash_advance_chb = new wxCheckBox( m_panel6, wxID_ANY, wxT("Cash advance"), wxDefaultPosition, wxDefaultSize, 0 );
	cash_advance_chb->SetValue(true);
	
	cash_advance_chb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer9->Add( cash_advance_chb, 0, wxALL|wxEXPAND, 2 );
	
	comment_entree_chb = new wxCheckBox( m_panel6, wxID_ANY, wxT("Comment entree"), wxDefaultPosition, wxDefaultSize, 0 );
	comment_entree_chb->SetValue(true);
	
	comment_entree_chb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer9->Add( comment_entree_chb, 0, wxALL|wxEXPAND, 2 );
	
	discount_chb = new wxCheckBox( m_panel6, wxID_ANY, wxT("Discount"), wxDefaultPosition, wxDefaultSize, 0 );
	discount_chb->SetValue(true);
	
	discount_chb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer9->Add( discount_chb, 0, wxALL|wxEXPAND, 2 );
	
	trainig_mode_chb = new wxCheckBox( m_panel6, wxID_ANY, wxT("Traning mode"), wxDefaultPosition, wxDefaultSize, 0 );
	trainig_mode_chb->SetValue(true);
	
	trainig_mode_chb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer9->Add( trainig_mode_chb, 0, wxALL|wxEXPAND, 2 );
	
	us_mode_chb = new wxCheckBox( m_panel6, wxID_ANY, wxT("US mode"), wxDefaultPosition, wxDefaultSize, 0 );
	us_mode_chb->SetValue(true);
	
	us_mode_chb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer9->Add( us_mode_chb, 0, wxALL|wxEXPAND, 2 );
	
	allow_remove_entrees_chb = new wxCheckBox( m_panel6, wxID_ANY, wxT("Allow to remove entrees which already has been sent to kitchen"), wxDefaultPosition, wxDefaultSize, 0 );
	allow_remove_entrees_chb->SetValue(true);
	
	bSizer9->Add( allow_remove_entrees_chb, 0, wxALL|wxEXPAND, 2 );
	
	order_type_sizer = new wxStaticBoxSizer( new wxStaticBox( m_panel6, wxID_ANY, wxT("Order types") ), wxVERTICAL );
	
	for_here_chb = new wxCheckBox( m_panel6, wxID_ANY, wxT("For here"), wxDefaultPosition, wxDefaultSize, 0 );
	for_here_chb->SetValue(true);
	
	for_here_chb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	order_type_sizer->Add( for_here_chb, 0, wxALL|wxEXPAND, 2 );
	
	to_go_chb = new wxCheckBox( m_panel6, wxID_ANY, wxT("To go"), wxDefaultPosition, wxDefaultSize, 0 );
	to_go_chb->SetValue(true);
	
	to_go_chb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	order_type_sizer->Add( to_go_chb, 0, wxALL|wxEXPAND, 2 );
	
	delivery_chb = new wxCheckBox( m_panel6, wxID_ANY, wxT("Delivery"), wxDefaultPosition, wxDefaultSize, 0 );
	delivery_chb->SetValue(true);
	
	delivery_chb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	order_type_sizer->Add( delivery_chb, 0, wxALL|wxEXPAND, 2 );
	
	pickup_chb = new wxCheckBox( m_panel6, wxID_ANY, wxT("Pickup"), wxDefaultPosition, wxDefaultSize, 0 );
	pickup_chb->SetValue(true);
	
	pickup_chb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	order_type_sizer->Add( pickup_chb, 0, wxALL|wxEXPAND, 2 );
	
	employee_meal_chb = new wxCheckBox( m_panel6, wxID_ANY, wxT("Employee meal"), wxDefaultPosition, wxDefaultSize, 0 );
	employee_meal_chb->SetValue(true);
	
	employee_meal_chb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	order_type_sizer->Add( employee_meal_chb, 0, wxALL|wxEXPAND, 2 );
	
	donation_chb = new wxCheckBox( m_panel6, wxID_ANY, wxT("Donation"), wxDefaultPosition, wxDefaultSize, 0 );
	
	donation_chb->Enable( false );
	
	order_type_sizer->Add( donation_chb, 0, wxALL|wxEXPAND, 2 );
	
	table_booking_chb = new wxCheckBox( m_panel6, wxID_ANY, wxT("Table booking"), wxDefaultPosition, wxDefaultSize, 0 );
	table_booking_chb->SetValue(true);
	
	table_booking_chb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	order_type_sizer->Add( table_booking_chb, 0, wxALL|wxEXPAND, 2 );
	
	bSizer9->Add( order_type_sizer, 1, wxEXPAND, 3 );
	
	m_panel6->SetSizer( bSizer9 );
	m_panel6->Layout();
	bSizer9->Fit( m_panel6 );
	m_notebook->AddPage( m_panel6, wxT("Functionality"), false );
	m_panel4 = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer9;
	sbSizer9 = new wxStaticBoxSizer( new wxStaticBox( m_panel4, wxID_ANY, wxT("Locale") ), wxVERTICAL );
	
	load_locale_chb = new wxCheckBox( m_panel4, wxID_ANY, wxT("Load language"), wxDefaultPosition, wxDefaultSize, 0 );
	
	sbSizer9->Add( load_locale_chb, 0, wxALL, 5 );
	
	wxFlexGridSizer* langd_sizer;
	langd_sizer = new wxFlexGridSizer( 2, 3, 0, 0 );
	langd_sizer->SetFlexibleDirection( wxBOTH );
	langd_sizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	file_lb = new wxStaticText( m_panel4, wxID_ANY, wxT("File:"), wxDefaultPosition, wxDefaultSize, 0 );
	file_lb->Wrap( -1 );
	file_lb->Enable( false );
	
	langd_sizer->Add( file_lb, 0, wxALL|wxEXPAND, 5 );
	
	file_ed = new wxTextCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), wxTE_READONLY );
	file_ed->Enable( false );
	
	langd_sizer->Add( file_ed, 0, wxALL|wxEXPAND, 5 );
	
	select_file_btn = new wxButton( m_panel4, wxID_ANY, wxT("Select File"), wxDefaultPosition, wxDefaultSize, 0 );
	select_file_btn->Enable( false );
	
	langd_sizer->Add( select_file_btn, 0, wxALL|wxEXPAND, 5 );
	
	locales_lb = new wxStaticText( m_panel4, wxID_ANY, wxT("Available locales:"), wxDefaultPosition, wxDefaultSize, 0 );
	locales_lb->Wrap( -1 );
	locales_lb->Enable( false );
	
	langd_sizer->Add( locales_lb, 0, wxALL, 5 );
	
	wxArrayString locale_cbChoices;
	locale_cb = new wxChoice( m_panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, locale_cbChoices, 0 );
	locale_cb->SetSelection( 0 );
	locale_cb->Enable( false );
	
	langd_sizer->Add( locale_cb, 0, wxALL, 5 );
	
	sbSizer9->Add( langd_sizer, 1, wxEXPAND, 5 );
	
	bSizer7->Add( sbSizer9, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer10;
	sbSizer10 = new wxStaticBoxSizer( new wxStaticBox( m_panel4, wxID_ANY, wxEmptyString ), wxVERTICAL );
	
	bSizer7->Add( sbSizer10, 1, wxEXPAND, 5 );
	
	m_panel4->SetSizer( bSizer7 );
	m_panel4->Layout();
	bSizer7->Fit( m_panel4 );
	m_notebook->AddPage( m_panel4, wxT("Other options"), false );
	
	bSizer1->Add( m_notebook, 1, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );
	
	close_btn = new wxButton( this, wxID_ANY, wxT("Save and close"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	close_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	close_btn->SetBackgroundColour( wxColour( 204, 255, 204 ) );
	
	bSizer10->Add( close_btn, 0, wxALL, 5 );
	
	
	bSizer10->Add( 0, 0, 1, wxEXPAND, 5 );
	
	cancel_btn = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	cancel_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	cancel_btn->SetBackgroundColour( wxColour( 255, 204, 204 ) );
	
	bSizer10->Add( cancel_btn, 0, wxALL, 5 );
	
	bSizer1->Add( bSizer10, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	// Connect Events
	fline_choice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( base_config_f::fline_choiceChange ), NULL, this );
	fline_ed->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( base_config_f::fline_edClick ), NULL, this );
	sline_choice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( base_config_f::sline_choiceChange ), NULL, this );
	sline_ed->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( base_config_f::sline_edClick ), NULL, this );
	auto_resolution_chb->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( base_config_f::auto_resolution_chbCheck ), NULL, this );
	stay_on_top_chb->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( base_config_f::stay_on_top_chbCheck ), NULL, this );
	terminate_app_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_config_f::terminate_app_btnClick ), NULL, this );
	cc_heartland_rb->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( base_config_f::cc_processing_rgClick ), NULL, this );
	license_ed->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( base_config_f::license_edClick ), NULL, this );
	license_ed->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( base_config_f::EditFocus ), NULL, this );
	site_ed->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( base_config_f::site_edClick ), NULL, this );
	site_ed->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( base_config_f::EditFocus ), NULL, this );
	device_ed->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( base_config_f::device_edClick ), NULL, this );
	device_ed->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( base_config_f::EditFocus ), NULL, this );
	user_ed->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( base_config_f::user_edClick ), NULL, this );
	user_ed->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( base_config_f::EditFocus ), NULL, this );
	password_ed->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( base_config_f::password_edClick ), NULL, this );
	password_ed->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( base_config_f::EditFocus ), NULL, this );
	test_credentials_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_config_f::test_credentials_btnClick ), NULL, this );
	cc_stand_alone_rb->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( base_config_f::cc_processing_rgClick ), NULL, this );
	cc_disable_rb->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( base_config_f::cc_processing_rgClick ), NULL, this );
	local_printer_chb->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( base_config_f::local_printer_chbCheck ), NULL, this );
	net_printer_chb->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( base_config_f::net_printer_chbCheck ), NULL, this );
	no_printer_chb->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( base_config_f::no_printer_chbCheck ), NULL, this );
	print_logo_chb->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( base_config_f::print_logoCheck ), NULL, this );
	test_printer_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_config_f::test_printer_btnClick ), NULL, this );
	for_here_chb->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( base_config_f::order_typeCheck ), NULL, this );
	to_go_chb->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( base_config_f::order_typeCheck ), NULL, this );
	delivery_chb->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( base_config_f::order_typeCheck ), NULL, this );
	pickup_chb->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( base_config_f::order_typeCheck ), NULL, this );
	employee_meal_chb->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( base_config_f::order_typeCheck ), NULL, this );
	donation_chb->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( base_config_f::order_typeCheck ), NULL, this );
	table_booking_chb->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( base_config_f::order_typeCheck ), NULL, this );
	load_locale_chb->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( base_config_f::load_locale_chbCheck ), NULL, this );
	select_file_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_config_f::select_file_btnClick ), NULL, this );
	locale_cb->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( base_config_f::locale_chbClick ), NULL, this );
	close_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_config_f::close_btnClick ), NULL, this );
	cancel_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_config_f::cancel_btnClick ), NULL, this );
}

base_config_f::~base_config_f()
{
	// Disconnect Events
	fline_choice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( base_config_f::fline_choiceChange ), NULL, this );
	fline_ed->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( base_config_f::fline_edClick ), NULL, this );
	sline_choice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( base_config_f::sline_choiceChange ), NULL, this );
	sline_ed->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( base_config_f::sline_edClick ), NULL, this );
	auto_resolution_chb->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( base_config_f::auto_resolution_chbCheck ), NULL, this );
	stay_on_top_chb->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( base_config_f::stay_on_top_chbCheck ), NULL, this );
	terminate_app_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_config_f::terminate_app_btnClick ), NULL, this );
	cc_heartland_rb->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( base_config_f::cc_processing_rgClick ), NULL, this );
	license_ed->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( base_config_f::license_edClick ), NULL, this );
	license_ed->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( base_config_f::EditFocus ), NULL, this );
	site_ed->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( base_config_f::site_edClick ), NULL, this );
	site_ed->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( base_config_f::EditFocus ), NULL, this );
	device_ed->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( base_config_f::device_edClick ), NULL, this );
	device_ed->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( base_config_f::EditFocus ), NULL, this );
	user_ed->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( base_config_f::user_edClick ), NULL, this );
	user_ed->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( base_config_f::EditFocus ), NULL, this );
	password_ed->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( base_config_f::password_edClick ), NULL, this );
	password_ed->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( base_config_f::EditFocus ), NULL, this );
	test_credentials_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_config_f::test_credentials_btnClick ), NULL, this );
	cc_stand_alone_rb->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( base_config_f::cc_processing_rgClick ), NULL, this );
	cc_disable_rb->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( base_config_f::cc_processing_rgClick ), NULL, this );
	local_printer_chb->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( base_config_f::local_printer_chbCheck ), NULL, this );
	net_printer_chb->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( base_config_f::net_printer_chbCheck ), NULL, this );
	no_printer_chb->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( base_config_f::no_printer_chbCheck ), NULL, this );
	print_logo_chb->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( base_config_f::print_logoCheck ), NULL, this );
	test_printer_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_config_f::test_printer_btnClick ), NULL, this );
	for_here_chb->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( base_config_f::order_typeCheck ), NULL, this );
	to_go_chb->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( base_config_f::order_typeCheck ), NULL, this );
	delivery_chb->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( base_config_f::order_typeCheck ), NULL, this );
	pickup_chb->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( base_config_f::order_typeCheck ), NULL, this );
	employee_meal_chb->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( base_config_f::order_typeCheck ), NULL, this );
	donation_chb->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( base_config_f::order_typeCheck ), NULL, this );
	table_booking_chb->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( base_config_f::order_typeCheck ), NULL, this );
	load_locale_chb->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( base_config_f::load_locale_chbCheck ), NULL, this );
	select_file_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_config_f::select_file_btnClick ), NULL, this );
	locale_cb->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( base_config_f::locale_chbClick ), NULL, this );
	close_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_config_f::close_btnClick ), NULL, this );
	cancel_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_config_f::cancel_btnClick ), NULL, this );
}

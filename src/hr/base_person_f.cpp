///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "base_person_f.h"

///////////////////////////////////////////////////////////////////////////

base_person_f::base_person_f( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer33;
	bSizer33 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer27;
	bSizer27 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxHORIZONTAL );
	
	wxGridSizer* gSizer18;
	gSizer18 = new wxGridSizer( 2, 2, 0, 0 );
	
	lb_login = new wxStaticText( this, wxID_ANY, wxT("Login"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	lb_login->Wrap( -1 );
	gSizer18->Add( lb_login, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	t_login = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer18->Add( t_login, 0, wxALL, 5 );
	
	lb_firstname = new wxStaticText( this, wxID_ANY, wxT("Firstname"), wxDefaultPosition, wxDefaultSize, 0 );
	lb_firstname->Wrap( -1 );
	gSizer18->Add( lb_firstname, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	t_firstname = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer18->Add( t_firstname, 0, wxALL, 5 );
	
	lb_lastname = new wxStaticText( this, wxID_ANY, wxT("Lastname"), wxDefaultPosition, wxDefaultSize, 0 );
	lb_lastname->Wrap( -1 );
	gSizer18->Add( lb_lastname, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	t_lastname = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer18->Add( t_lastname, 0, wxALIGN_LEFT|wxALL, 5 );
	
	lb_surname = new wxStaticText( this, wxID_ANY, wxT("Surname"), wxDefaultPosition, wxDefaultSize, 0 );
	lb_surname->Wrap( -1 );
	gSizer18->Add( lb_surname, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	t_surname = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer18->Add( t_surname, 0, wxALL, 5 );
	
	lb_position = new wxStaticText( this, wxID_ANY, wxT("Position"), wxDefaultPosition, wxDefaultSize, 0 );
	lb_position->Wrap( -1 );
	gSizer18->Add( lb_position, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	bx_position = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 100,-1 ), 0, NULL, wxCB_DROPDOWN|wxCB_READONLY ); 
	gSizer18->Add( bx_position, 0, wxALL, 5 );
	
	lb_hire = new wxStaticText( this, wxID_ANY, wxT("Hire"), wxDefaultPosition, wxDefaultSize, 0 );
	lb_hire->Wrap( -1 );
	gSizer18->Add( lb_hire, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	dt_hire = new wxDatePickerCtrl( this, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxSize( 100,-1 ), wxDP_DEFAULT|wxDP_DROPDOWN );
	gSizer18->Add( dt_hire, 0, wxALL, 5 );
	
	lb_ssn = new wxStaticText( this, wxID_ANY, wxT("SSN#"), wxDefaultPosition, wxDefaultSize, 0 );
	lb_ssn->Wrap( -1 );
	gSizer18->Add( lb_ssn, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	t_ssn = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer18->Add( t_ssn, 0, wxALL, 5 );
	
	lb_birthday = new wxStaticText( this, wxID_ANY, wxT("birthday"), wxDefaultPosition, wxDefaultSize, 0 );
	lb_birthday->Wrap( -1 );
	gSizer18->Add( lb_birthday, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	dt_birthday = new wxDatePickerCtrl( this, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxSize( 100,-1 ), wxDP_ALLOWNONE|wxDP_DEFAULT|wxDP_DROPDOWN );
	gSizer18->Add( dt_birthday, 0, wxALL, 5 );
	
	lb_state = new wxStaticText( this, wxID_ANY, wxT("State"), wxDefaultPosition, wxDefaultSize, 0 );
	lb_state->Wrap( -1 );
	gSizer18->Add( lb_state, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	bx_state = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 100,-1 ), 0, NULL, wxCB_DROPDOWN|wxCB_READONLY ); 
	gSizer18->Add( bx_state, 0, wxALL, 5 );
	
	lb_city = new wxStaticText( this, wxID_ANY, wxT("City"), wxDefaultPosition, wxDefaultSize, 0 );
	lb_city->Wrap( -1 );
	gSizer18->Add( lb_city, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	t_city = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer18->Add( t_city, 0, wxALL, 5 );
	
	lb_address1 = new wxStaticText( this, wxID_ANY, wxT("Address1"), wxDefaultPosition, wxDefaultSize, 0 );
	lb_address1->Wrap( -1 );
	gSizer18->Add( lb_address1, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	t_address1 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer18->Add( t_address1, 0, wxALL, 5 );
	
	lb_address2 = new wxStaticText( this, wxID_ANY, wxT("Address2"), wxDefaultPosition, wxDefaultSize, 0 );
	lb_address2->Wrap( -1 );
	gSizer18->Add( lb_address2, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	t_address2 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer18->Add( t_address2, 0, wxALL, 5 );
	
	lb_zip = new wxStaticText( this, wxID_ANY, wxT("Zip"), wxDefaultPosition, wxDefaultSize, 0 );
	lb_zip->Wrap( -1 );
	gSizer18->Add( lb_zip, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	t_zip = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer18->Add( t_zip, 0, wxALL, 5 );
	
	lb_business_mail = new wxStaticText( this, wxID_ANY, wxT("Business e-mail"), wxDefaultPosition, wxDefaultSize, 0 );
	lb_business_mail->Wrap( -1 );
	gSizer18->Add( lb_business_mail, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	t_mail_business = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer18->Add( t_mail_business, 0, wxALL, 5 );
	
	lb_private_mail = new wxStaticText( this, wxID_ANY, wxT("Private e-mail"), wxDefaultPosition, wxDefaultSize, 0 );
	lb_private_mail->Wrap( -1 );
	gSizer18->Add( lb_private_mail, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	t_mail_private = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer18->Add( t_mail_private, 0, wxALL, 5 );
	
	lb_phone_home = new wxStaticText( this, wxID_ANY, wxT("Home phone"), wxDefaultPosition, wxDefaultSize, 0 );
	lb_phone_home->Wrap( -1 );
	gSizer18->Add( lb_phone_home, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	t_phone_home = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer18->Add( t_phone_home, 0, wxALL, 5 );
	
	lb_phone_cell = new wxStaticText( this, wxID_ANY, wxT("Cell phone"), wxDefaultPosition, wxDefaultSize, 0 );
	lb_phone_cell->Wrap( -1 );
	gSizer18->Add( lb_phone_cell, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	t_phone_cell = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer18->Add( t_phone_cell, 0, wxALL, 5 );
	
	lb_phone_business = new wxStaticText( this, wxID_ANY, wxT("Business phone"), wxDefaultPosition, wxDefaultSize, 0 );
	lb_phone_business->Wrap( -1 );
	gSizer18->Add( lb_phone_business, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	t_phone_business = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer18->Add( t_phone_business, 0, wxALL, 5 );
	
	lb_ename = new wxStaticText( this, wxID_ANY, wxT("Emergency name"), wxDefaultPosition, wxDefaultSize, 0 );
	lb_ename->Wrap( -1 );
	gSizer18->Add( lb_ename, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	t_ename = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer18->Add( t_ename, 0, wxALL, 5 );
	
	lb_ephone = new wxStaticText( this, wxID_ANY, wxT("Emergency phone"), wxDefaultPosition, wxDefaultSize, 0 );
	lb_ephone->Wrap( -1 );
	gSizer18->Add( lb_ephone, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	t_ephone = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gSizer18->Add( t_ephone, 0, wxALL, 5 );
	
	bSizer25->Add( gSizer18, 1, wxALIGN_RIGHT|wxEXPAND, 5 );
	
	bSizer24->Add( bSizer25, 1, wxEXPAND, 5 );
	
	bSizer27->Add( bSizer24, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxVERTICAL );
	
	wxGridSizer* gSizer15;
	gSizer15 = new wxGridSizer( 2, 2, 0, 0 );
	
	lb_pwd = new wxStaticText( this, wxID_ANY, wxT("Password"), wxDefaultPosition, wxDefaultSize, 0 );
	lb_pwd->Wrap( -1 );
	gSizer15->Add( lb_pwd, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	t_password = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
	gSizer15->Add( t_password, 0, wxALL, 5 );
	
	lb_confpwd = new wxStaticText( this, wxID_ANY, wxT("Confirm password"), wxDefaultPosition, wxDefaultSize, 0 );
	lb_confpwd->Wrap( -1 );
	gSizer15->Add( lb_confpwd, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	t_confpassword = new wxTextCtrl( this, wID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
	gSizer15->Add( t_confpassword, 0, wxALL, 5 );
	
	bSizer26->Add( gSizer15, 0, wxEXPAND, 5 );
	
	t_magncard = new wxCheckBox( this, wxID_ANY, wxT("Write to magnetic card"), wxDefaultPosition, wxDefaultSize, 0 );
	
	bSizer26->Add( t_magncard, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer32;
	bSizer32 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer32->Add( 10, 0, 0, wxEXPAND, 5 );
	
	rb_hi = new wxRadioButton( this, wxID_ANY, wxT("hi-co"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	rb_hi->SetValue( true ); 
	bSizer32->Add( rb_hi, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );
	
	
	bSizer32->Add( 30, 0, 0, wxEXPAND, 5 );
	
	rb_low = new wxRadioButton( this, wxID_ANY, wxT("low-co"), wxDefaultPosition, wxSize( 80,-1 ), 0 );
	bSizer32->Add( rb_low, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	bSizer32->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer26->Add( bSizer32, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 0 );
	
	lb_descr = new wxStaticText( this, wxID_ANY, wxT("Description"), wxDefaultPosition, wxDefaultSize, 0 );
	lb_descr->Wrap( -1 );
	bSizer26->Add( lb_descr, 0, wxALL, 5 );
	
	t_descr = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_MULTILINE );
	bSizer26->Add( t_descr, 1, wxALL|wxEXPAND, 5 );
	
	lb_photo = new wxStaticText( this, wxID_ANY, wxT("Photo"), wxDefaultPosition, wxDefaultSize, 0 );
	lb_photo->Wrap( -1 );
	bSizer26->Add( lb_photo, 0, wxALL, 5 );
	
	t_photo = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	bSizer26->Add( t_photo, 1, wxALIGN_CENTER|wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 5 );
	
	bSizer27->Add( bSizer26, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer28;
	bSizer28 = new wxBoxSizer( wxVERTICAL );
	
	lb_cafe = new wxStaticText( this, wxID_ANY, wxT("Person's cafe"), wxDefaultPosition, wxDefaultSize, 0 );
	lb_cafe->Wrap( -1 );
	bSizer28->Add( lb_cafe, 0, wxALL, 5 );
	
	wxArrayString ck_cafeChoices;
	ck_cafe = new wxCheckListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, ck_cafeChoices, 0 );
	ck_cafe->SetMinSize( wxSize( 220,275 ) );
	
	bSizer28->Add( ck_cafe, 0, wxALL, 5 );
	
	lb_role = new wxStaticText( this, wxID_ANY, wxT("Person's role"), wxDefaultPosition, wxDefaultSize, 0 );
	lb_role->Wrap( -1 );
	bSizer28->Add( lb_role, 0, wxALL, 5 );
	
	wxArrayString ck_roleChoices;
	ck_role = new wxCheckListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, ck_roleChoices, 0 );
	ck_role->SetMinSize( wxSize( 220,275 ) );
	
	bSizer28->Add( ck_role, 0, wxALL, 5 );
	
	bSizer27->Add( bSizer28, 1, wxEXPAND, 5 );
	
	bSizer33->Add( bSizer27, 1, wxEXPAND, 5 );
	
	
	bSizer33->Add( 0, 10, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer36;
	bSizer36 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer36->Add( 0, 0, 2, wxEXPAND, 5 );
	
	btn_ok = new wxButton( this, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer36->Add( btn_ok, 0, wxALL, 5 );
	
	
	bSizer36->Add( 0, 0, 1, wxEXPAND, 5 );
	
	btn_cancel = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer36->Add( btn_cancel, 0, wxALL, 5 );
	
	
	bSizer36->Add( 0, 0, 2, wxEXPAND, 5 );
	
	bSizer33->Add( bSizer36, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer33 );
	this->Layout();
	bSizer33->Fit( this );
	
	// Connect Events
	t_login->Connect( wxEVT_CHAR, wxKeyEventHandler( base_person_f::OnLoginCtrl ), NULL, this );
	bx_position->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( base_person_f::OnCheckPos ), NULL, this );
	t_password->Connect( wxEVT_CHAR, wxKeyEventHandler( base_person_f::OnPwdCtrl ), NULL, this );
	t_confpassword->Connect( wxEVT_CHAR, wxKeyEventHandler( base_person_f::OnConfPwdCtrl ), NULL, this );
	t_zip->Connect( wxEVT_CHAR, wxKeyEventHandler( base_person_f::OnZipCtrl ), NULL, this );
	t_magncard->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( base_person_f::OnCheckCard ), NULL, this );
	rb_hi->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( base_person_f::OnClickHi ), NULL, this );
	rb_low->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( base_person_f::OnClickLow ), NULL, this );
	t_photo->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( base_person_f::OnLoadBmp ), NULL, this );
	t_photo->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( base_person_f::OnClearBmp ), NULL, this );
	btn_ok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_person_f::OnClose ), NULL, this );
	btn_cancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_person_f::OnCancel ), NULL, this );
}

base_person_f::~base_person_f()
{
	// Disconnect Events
	t_login->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_person_f::OnLoginCtrl ), NULL, this );
	bx_position->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( base_person_f::OnCheckPos ), NULL, this );
	t_password->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_person_f::OnPwdCtrl ), NULL, this );
	t_confpassword->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_person_f::OnConfPwdCtrl ), NULL, this );
	t_zip->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_person_f::OnZipCtrl ), NULL, this );
	t_magncard->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( base_person_f::OnCheckCard ), NULL, this );
	rb_hi->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( base_person_f::OnClickHi ), NULL, this );
	rb_low->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( base_person_f::OnClickLow ), NULL, this );
	t_photo->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( base_person_f::OnLoadBmp ), NULL, this );
	t_photo->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( base_person_f::OnClearBmp ), NULL, this );
	btn_ok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_person_f::OnClose ), NULL, this );
	btn_cancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_person_f::OnCancel ), NULL, this );
}

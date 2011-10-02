///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "base_delayed_order_f.h"

///////////////////////////////////////////////////////////////////////////

base_delayed_order_f::base_delayed_order_f( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	main_sizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	main_input_item_sizer = new wxBoxSizer( wxVERTICAL );
	
	input_item_sizer = new wxFlexGridSizer( 7, 2, 0, 0 );
	input_item_sizer->SetFlexibleDirection( wxVERTICAL );
	input_item_sizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	firstname_lb = new wxStaticText( this, wxID_ANY, wxT("First name"), wxDefaultPosition, wxDefaultSize, 0 );
	firstname_lb->Wrap( -1 );
	firstname_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	input_item_sizer->Add( firstname_lb, 0, wxALL|wxEXPAND, 5 );
	
	fname_ed = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 150,-1 ), wxTE_READONLY );
	fname_ed->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	fname_ed->SetBackgroundColour( wxColour( 255, 255, 128 ) );
	
	input_item_sizer->Add( fname_ed, 0, wxALL, 5 );
	
	lastname_lb = new wxStaticText( this, wxID_ANY, wxT("Last name"), wxDefaultPosition, wxDefaultSize, 0 );
	lastname_lb->Wrap( -1 );
	lastname_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	input_item_sizer->Add( lastname_lb, 0, wxALL|wxEXPAND, 5 );
	
	lname_ed = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 150,-1 ), wxTE_READONLY );
	lname_ed->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	input_item_sizer->Add( lname_ed, 0, wxALL, 5 );
	
	street_address_lb = new wxStaticText( this, wxID_ANY, wxT("Street address"), wxDefaultPosition, wxDefaultSize, 0 );
	street_address_lb->Wrap( -1 );
	street_address_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	input_item_sizer->Add( street_address_lb, 0, wxALL|wxEXPAND, 5 );
	
	address_ed = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 150,-1 ), wxTE_READONLY );
	address_ed->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	input_item_sizer->Add( address_ed, 0, wxALL, 5 );
	
	city_lb = new wxStaticText( this, wxID_ANY, wxT("City"), wxDefaultPosition, wxDefaultSize, 0 );
	city_lb->Wrap( -1 );
	city_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	input_item_sizer->Add( city_lb, 0, wxALL|wxEXPAND, 5 );
	
	city_ed = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 150,-1 ), wxTE_READONLY );
	city_ed->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	input_item_sizer->Add( city_ed, 0, wxALL, 5 );
	
	zip_lb = new wxStaticText( this, wxID_ANY, wxT("ZIP"), wxDefaultPosition, wxDefaultSize, 0 );
	zip_lb->Wrap( -1 );
	zip_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	input_item_sizer->Add( zip_lb, 0, wxALL|wxEXPAND, 5 );
	
	zip_ed = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 150,-1 ), wxTE_READONLY|wxTE_RIGHT );
	zip_ed->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	zip_ed->SetBackgroundColour( wxColour( 255, 255, 128 ) );
	
	input_item_sizer->Add( zip_ed, 0, wxALL, 5 );
	
	state_lb = new wxStaticText( this, wxID_ANY, wxT("State"), wxDefaultPosition, wxDefaultSize, 0 );
	state_lb->Wrap( -1 );
	state_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	input_item_sizer->Add( state_lb, 0, wxALL|wxEXPAND, 5 );
	
	wxArrayString state_cbChoices;
	state_cb = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 150,-1 ), state_cbChoices, 0 );
	state_cb->SetSelection( 0 );
	state_cb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	input_item_sizer->Add( state_cb, 0, wxALL, 5 );
	
	phone_lb = new wxStaticText( this, wxID_ANY, wxT("Phone"), wxDefaultPosition, wxDefaultSize, 0 );
	phone_lb->Wrap( -1 );
	phone_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	input_item_sizer->Add( phone_lb, 0, wxALL|wxEXPAND, 5 );
	
	phone_ed = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 150,-1 ), wxTE_READONLY|wxTE_RIGHT );
	phone_ed->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	phone_ed->SetBackgroundColour( wxColour( 255, 255, 128 ) );
	
	input_item_sizer->Add( phone_ed, 0, wxALL, 5 );
	
	main_input_item_sizer->Add( input_item_sizer, 0, wxEXPAND, 0 );
	
	yellow_lb = new wxStaticText( this, wxID_ANY, wxT("Mandatory fields are marked yellow"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	yellow_lb->Wrap( -1 );
	yellow_lb->SetFont( wxFont( 11, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	main_input_item_sizer->Add( yellow_lb, 0, wxALL|wxEXPAND, 5 );
	
	select_table_btn = new wxButton( this, wxID_ANY, wxT("Table: not selected"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	select_table_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	select_table_btn->Enable( false );
	select_table_btn->Hide();
	
	main_input_item_sizer->Add( select_table_btn, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 30 );
	
	bSizer2->Add( main_input_item_sizer, 1, wxEXPAND, 0 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	delivery_pickup_cap = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Date and time") ), wxHORIZONTAL );
	
	set_date_btn = new wxButton( this, wxID_ANY, wxT("Set the date"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	set_date_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	set_date_btn->Enable( false );
	
	delivery_pickup_cap->Add( set_date_btn, 0, wxALL, 5 );
	
	set_time_btn = new wxButton( this, wxID_ANY, wxT("Set the time"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	set_time_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	set_time_btn->Enable( false );
	
	delivery_pickup_cap->Add( set_time_btn, 0, wxALL, 5 );
	
	bSizer3->Add( delivery_pickup_cap, 0, wxEXPAND, 5 );
	
	
	bSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	prepayment_cap = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Prepayment") ), wxVERTICAL );
	
	wxGridSizer* gSizer2;
	gSizer2 = new wxGridSizer( 2, 2, 0, 0 );
	
	prepayment_btn = new wxButton( this, wxID_ANY, wxT("Prepayment"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	prepayment_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	prepayment_btn->SetBackgroundColour( wxColour( 204, 255, 204 ) );
	prepayment_btn->Enable( false );
	
	gSizer2->Add( prepayment_btn, 0, wxALL, 5 );
	
	cancel_prepayment_btn = new wxButton( this, wxID_ANY, wxT("Cancel prepayment"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	cancel_prepayment_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	cancel_prepayment_btn->SetBackgroundColour( wxColour( 255, 204, 204 ) );
	cancel_prepayment_btn->Enable( false );
	
	gSizer2->Add( cancel_prepayment_btn, 0, wxALL, 5 );
	
	auth_cc_btn = new wxButton( this, wxID_ANY, wxT("Authorize CC"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	auth_cc_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	auth_cc_btn->SetBackgroundColour( wxColour( 204, 255, 204 ) );
	auth_cc_btn->Enable( false );
	
	gSizer2->Add( auth_cc_btn, 0, wxALL, 5 );
	
	cancel_auth_cc_btn = new wxButton( this, wxID_ANY, wxT("Cancel authorize CC"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	cancel_auth_cc_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	cancel_auth_cc_btn->SetBackgroundColour( wxColour( 255, 204, 204 ) );
	cancel_auth_cc_btn->Enable( false );
	
	gSizer2->Add( cancel_auth_cc_btn, 0, wxALL, 5 );
	
	print_slip_btn = new wxButton( this, wxID_ANY, wxT("Print slip"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	print_slip_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	print_slip_btn->SetBackgroundColour( wxColour( 204, 255, 204 ) );
	print_slip_btn->Enable( false );
	
	gSizer2->Add( print_slip_btn, 0, wxALL, 5 );
	
	cancel_order_btn = new wxButton( this, wxID_ANY, wxT("Cancel order"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	cancel_order_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	cancel_order_btn->SetBackgroundColour( wxColour( 255, 204, 204 ) );
	
	gSizer2->Add( cancel_order_btn, 0, wxALL, 5 );
	
	prepayment_cap->Add( gSizer2, 1, wxEXPAND, 5 );
	
	bSizer3->Add( prepayment_cap, 0, wxEXPAND, 5 );
	
	bSizer2->Add( bSizer3, 1, wxEXPAND, 5 );
	
	main_sizer->Add( bSizer2, 1, wxEXPAND, 5 );
	
	note_cap = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Order notes") ), wxVERTICAL );
	
	order_notes_ed = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,80 ), wxTE_READONLY );
	note_cap->Add( order_notes_ed, 1, wxALL|wxEXPAND, 5 );
	
	main_sizer->Add( note_cap, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer5->Add( 0, 0, 1, wxEXPAND, 5 );
	
	save_btn = new wxButton( this, wxID_ANY, wxT("Save"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	save_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	save_btn->SetBackgroundColour( wxColour( 204, 255, 204 ) );
	save_btn->Enable( false );
	
	bSizer5->Add( save_btn, 0, wxALL, 5 );
	
	
	bSizer5->Add( 0, 0, 1, wxEXPAND, 5 );
	
	save_and_print_btn = new wxButton( this, wxID_ANY, wxT("Save and print \nreceipt copy"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	save_and_print_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	save_and_print_btn->Enable( false );
	
	bSizer5->Add( save_and_print_btn, 0, wxALL, 5 );
	
	
	bSizer5->Add( 0, 0, 1, wxEXPAND, 5 );
	
	save_and_hide_btn = new wxButton( this, wxID_ANY, wxT("Save and hide order"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	save_and_hide_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	save_and_hide_btn->Enable( false );
	
	bSizer5->Add( save_and_hide_btn, 0, wxALL, 5 );
	
	
	bSizer5->Add( 0, 0, 1, wxEXPAND, 5 );
	
	cancel_btn = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	cancel_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	cancel_btn->SetBackgroundColour( wxColour( 255, 204, 204 ) );
	
	bSizer5->Add( cancel_btn, 0, wxALL, 5 );
	
	
	bSizer5->Add( 0, 0, 1, wxEXPAND, 5 );
	
	main_sizer->Add( bSizer5, 0, wxEXPAND, 5 );
	
	this->SetSizer( main_sizer );
	this->Layout();
	main_sizer->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	fname_ed->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( base_delayed_order_f::fname_edClick ), NULL, this );
	lname_ed->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( base_delayed_order_f::lname_edClick ), NULL, this );
	address_ed->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( base_delayed_order_f::address_edClick ), NULL, this );
	city_ed->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( base_delayed_order_f::city_edClick ), NULL, this );
	zip_ed->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( base_delayed_order_f::zip_edClick ), NULL, this );
	state_cb->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( base_delayed_order_f::state_cbChoice ), NULL, this );
	phone_ed->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( base_delayed_order_f::phone_edClick ), NULL, this );
	select_table_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_delayed_order_f::select_table_btnClick ), NULL, this );
	set_date_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_delayed_order_f::set_date_btnClick ), NULL, this );
	set_time_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_delayed_order_f::set_time_btnClick ), NULL, this );
	prepayment_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_delayed_order_f::prepayment_btnClick ), NULL, this );
	cancel_prepayment_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_delayed_order_f::cancel_prepayment_btnClick ), NULL, this );
	auth_cc_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_delayed_order_f::auth_cc_btnClick ), NULL, this );
	cancel_auth_cc_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_delayed_order_f::cancel_auth_btnClick ), NULL, this );
	print_slip_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_delayed_order_f::print_sleep_btnClick ), NULL, this );
	cancel_order_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_delayed_order_f::cancel_order_btnClick ), NULL, this );
	order_notes_ed->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( base_delayed_order_f::order_notes_edClick ), NULL, this );
	save_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_delayed_order_f::save_btnClick ), NULL, this );
	save_and_print_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_delayed_order_f::save_and_print_btnClick ), NULL, this );
	save_and_hide_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_delayed_order_f::save_and_hide_btnClick ), NULL, this );
	cancel_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_delayed_order_f::cancel_btnClick ), NULL, this );
}

base_delayed_order_f::~base_delayed_order_f()
{
	// Disconnect Events
	fname_ed->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( base_delayed_order_f::fname_edClick ), NULL, this );
	lname_ed->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( base_delayed_order_f::lname_edClick ), NULL, this );
	address_ed->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( base_delayed_order_f::address_edClick ), NULL, this );
	city_ed->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( base_delayed_order_f::city_edClick ), NULL, this );
	zip_ed->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( base_delayed_order_f::zip_edClick ), NULL, this );
	state_cb->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( base_delayed_order_f::state_cbChoice ), NULL, this );
	phone_ed->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( base_delayed_order_f::phone_edClick ), NULL, this );
	select_table_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_delayed_order_f::select_table_btnClick ), NULL, this );
	set_date_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_delayed_order_f::set_date_btnClick ), NULL, this );
	set_time_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_delayed_order_f::set_time_btnClick ), NULL, this );
	prepayment_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_delayed_order_f::prepayment_btnClick ), NULL, this );
	cancel_prepayment_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_delayed_order_f::cancel_prepayment_btnClick ), NULL, this );
	auth_cc_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_delayed_order_f::auth_cc_btnClick ), NULL, this );
	cancel_auth_cc_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_delayed_order_f::cancel_auth_btnClick ), NULL, this );
	print_slip_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_delayed_order_f::print_sleep_btnClick ), NULL, this );
	cancel_order_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_delayed_order_f::cancel_order_btnClick ), NULL, this );
	order_notes_ed->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( base_delayed_order_f::order_notes_edClick ), NULL, this );
	save_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_delayed_order_f::save_btnClick ), NULL, this );
	save_and_print_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_delayed_order_f::save_and_print_btnClick ), NULL, this );
	save_and_hide_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_delayed_order_f::save_and_hide_btnClick ), NULL, this );
	cancel_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_delayed_order_f::cancel_btnClick ), NULL, this );
}

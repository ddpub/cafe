///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "base_main_f.h"

///////////////////////////////////////////////////////////////////////////

base_main_f::base_main_f( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	main_sizer = new wxBoxSizer( wxVERTICAL );
	
	top_pnl = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	top_pnl->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	top_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	room_lb = new wxStaticText( top_pnl, wxID_ANY, wxT("Main room"), wxDefaultPosition, wxDefaultSize, 0 );
	room_lb->Wrap( -1 );
	top_sizer->Add( room_lb, 0, wxALL, 5 );
	
	
	top_sizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	current_dt_lb = new wxStaticText( top_pnl, wxID_ANY, wxT("03/13/2008 18:05:00"), wxDefaultPosition, wxDefaultSize, 0 );
	current_dt_lb->Wrap( -1 );
	current_dt_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	top_sizer->Add( current_dt_lb, 0, wxALL, 5 );
	
	
	top_sizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	current_user_lb = new wxStaticText( top_pnl, wxID_ANY, wxT("cafe manager: Dmitry Dergachev"), wxDefaultPosition, wxDefaultSize, 0 );
	current_user_lb->Wrap( -1 );
	current_user_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	top_sizer->Add( current_user_lb, 0, wxALL, 5 );
	
	
	top_sizer->Add( 1, 0, 0, wxEXPAND, 5 );
	
	browse_delayed_orders_btn = new wxButton( top_pnl, wxID_ANY, wxT("Browse delayed orders (xx)"), wxDefaultPosition, wxSize( -1,30 ), 0 );
	top_sizer->Add( browse_delayed_orders_btn, 0, 0, 5 );
	
	
	top_sizer->Add( 1, 0, 0, wxEXPAND, 5 );
	
	open_orders_btn = new wxButton( top_pnl, wxID_ANY, wxT("Browse my open orders (xx)"), wxDefaultPosition, wxSize( -1,30 ), 0 );
	top_sizer->Add( open_orders_btn, 0, 0, 5 );
	
	
	top_sizer->Add( 1, 0, 0, wxEXPAND, 5 );
	
	logout_btn = new wxButton( top_pnl, wxID_ANY, wxT("Log out"), wxDefaultPosition, wxSize( -1,30 ), 0 );
	logout_btn->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHTTEXT ) );
	logout_btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNTEXT ) );
	
	top_sizer->Add( logout_btn, 0, 0, 5 );
	
	top_pnl->SetSizer( top_sizer );
	top_pnl->Layout();
	top_sizer->Fit( top_pnl );
	main_sizer->Add( top_pnl, 0, wxEXPAND, 0 );
	
	top_line = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	main_sizer->Add( top_line, 0, wxEXPAND, 5 );
	
	body_pnl = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	body_pnl->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	body_sizer = new wxBoxSizer( wxVERTICAL );
	
	btn_pnl = new wxPanel( body_pnl, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	body_sizer->Add( btn_pnl, 1, wxEXPAND, 0 );
	
	body_line = new wxStaticLine( body_pnl, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	body_sizer->Add( body_line, 0, wxEXPAND, 5 );
	
	control_btn_pnl = new wxPanel( body_pnl, wxID_ANY, wxDefaultPosition, wxSize( -1,50 ), wxTAB_TRAVERSAL );
	control_btn_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	open_cash_drawer_btn = new wxButton( control_btn_pnl, wxID_ANY, wxT("Open cash drawer"), wxDefaultPosition, wxSize( 100,50 ), 0 );
	open_cash_drawer_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	control_btn_sizer->Add( open_cash_drawer_btn, 0, 0, 0 );
	
	
	control_btn_sizer->Add( 0, 0, 0, wxEXPAND, 5 );
	
	close_order_operation_btn = new wxButton( control_btn_pnl, wxID_ANY, wxT("Closed orders operations"), wxDefaultPosition, wxSize( 100,50 ), 0 );
	close_order_operation_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	control_btn_sizer->Add( close_order_operation_btn, 0, 0, 0 );
	
	
	control_btn_sizer->Add( 0, 0, 0, wxEXPAND, 0 );
	
	cash_advance_btn = new wxButton( control_btn_pnl, wxID_ANY, wxT("Cash advance"), wxDefaultPosition, wxSize( 100,50 ), 0 );
	cash_advance_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	control_btn_sizer->Add( cash_advance_btn, 0, 0, 0 );
	
	
	control_btn_sizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	create_table_booking_order_btn = new wxButton( control_btn_pnl, wxID_ANY, wxT("Table booking"), wxDefaultPosition, wxSize( 100,50 ), 0 );
	create_table_booking_order_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	control_btn_sizer->Add( create_table_booking_order_btn, 0, 0, 0 );
	
	
	control_btn_sizer->Add( 0, 0, 0, wxEXPAND, 0 );
	
	create_to_go_order_btn = new wxButton( control_btn_pnl, wxID_ANY, wxT("To go"), wxDefaultPosition, wxSize( 100,50 ), 0 );
	create_to_go_order_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	control_btn_sizer->Add( create_to_go_order_btn, 0, 0, 0 );
	
	
	control_btn_sizer->Add( 0, 0, 0, wxEXPAND, 0 );
	
	create_delivery_order_btn = new wxButton( control_btn_pnl, wxID_ANY, wxT("Delivery"), wxDefaultPosition, wxSize( 100,50 ), 0 );
	create_delivery_order_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	control_btn_sizer->Add( create_delivery_order_btn, 0, 0, 0 );
	
	
	control_btn_sizer->Add( 0, 0, 0, wxEXPAND, 0 );
	
	create_pickup_order_btn = new wxButton( control_btn_pnl, wxID_ANY, wxT("Pickup"), wxDefaultPosition, wxSize( 100,50 ), 0 );
	create_pickup_order_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	control_btn_sizer->Add( create_pickup_order_btn, 0, 0, 0 );
	
	
	control_btn_sizer->Add( 0, 0, 0, wxEXPAND, 0 );
	
	create_employee_meal_order_btn = new wxButton( control_btn_pnl, wxID_ANY, wxT("Employee meal"), wxDefaultPosition, wxSize( 100,50 ), 0 );
	create_employee_meal_order_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	control_btn_sizer->Add( create_employee_meal_order_btn, 0, 0, 0 );
	
	control_btn_pnl->SetSizer( control_btn_sizer );
	control_btn_pnl->Layout();
	body_sizer->Add( control_btn_pnl, 0, wxEXPAND, 5 );
	
	body_pnl->SetSizer( body_sizer );
	body_pnl->Layout();
	body_sizer->Fit( body_pnl );
	main_sizer->Add( body_pnl, 1, wxEXPAND, 0 );
	
	bottom_line = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	main_sizer->Add( bottom_line, 0, wxEXPAND, 0 );
	
	bottom_pnl = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bottom_pnl->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bottom_sizer = new wxBoxSizer( wxVERTICAL );
	
	status_lb = new wxStaticText( bottom_pnl, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	status_lb->Wrap( -1 );
	status_lb->SetFont( wxFont( 8, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bottom_sizer->Add( status_lb, 0, wxEXPAND, 0 );
	
	bottom_pnl->SetSizer( bottom_sizer );
	bottom_pnl->Layout();
	bottom_sizer->Fit( bottom_pnl );
	main_sizer->Add( bottom_pnl, 0, wxEXPAND, 0 );
	
	this->SetSizer( main_sizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	browse_delayed_orders_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_main_f::browse_delayed_orders_btnClick ), NULL, this );
	open_orders_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_main_f::open_orders_btnClick ), NULL, this );
	logout_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_main_f::logout_btnClick ), NULL, this );
	open_cash_drawer_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_main_f::open_cash_drawer_btnClick ), NULL, this );
	close_order_operation_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_main_f::close_order_operation_btnClick ), NULL, this );
	cash_advance_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_main_f::cash_advance_btnClick ), NULL, this );
	create_table_booking_order_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_main_f::create_table_booking_order_btnClick ), NULL, this );
	create_to_go_order_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_main_f::create_to_go_order_btnClick ), NULL, this );
	create_delivery_order_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_main_f::create_delivery_order_btnClick ), NULL, this );
	create_pickup_order_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_main_f::create_pickup_order_btnClick ), NULL, this );
	create_employee_meal_order_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_main_f::create_employee_meal_order_btnClick ), NULL, this );
	status_lb->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( base_main_f::OnConfigDbClick ), NULL, this );
}

base_main_f::~base_main_f()
{
	// Disconnect Events
	browse_delayed_orders_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_main_f::browse_delayed_orders_btnClick ), NULL, this );
	open_orders_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_main_f::open_orders_btnClick ), NULL, this );
	logout_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_main_f::logout_btnClick ), NULL, this );
	open_cash_drawer_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_main_f::open_cash_drawer_btnClick ), NULL, this );
	close_order_operation_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_main_f::close_order_operation_btnClick ), NULL, this );
	cash_advance_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_main_f::cash_advance_btnClick ), NULL, this );
	create_table_booking_order_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_main_f::create_table_booking_order_btnClick ), NULL, this );
	create_to_go_order_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_main_f::create_to_go_order_btnClick ), NULL, this );
	create_delivery_order_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_main_f::create_delivery_order_btnClick ), NULL, this );
	create_pickup_order_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_main_f::create_pickup_order_btnClick ), NULL, this );
	create_employee_meal_order_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_main_f::create_employee_meal_order_btnClick ), NULL, this );
	status_lb->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( base_main_f::OnConfigDbClick ), NULL, this );
}

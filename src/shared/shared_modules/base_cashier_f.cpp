///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "base_cashier_f.h"
#include <ksi_include/custom_treelistctrl.hpp>
///////////////////////////////////////////////////////////////////////////

base_cashier_f::base_cashier_f( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	main_sizer = new wxBoxSizer( wxVERTICAL );
	
	top_pnl = new wxPanel( this, wxID_ANY, wxPoint( -1,-1 ), wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	top_pnl->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	top_bar_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	room_lb = new wxStaticText( top_pnl, wxID_ANY, wxT("Main room"), wxDefaultPosition, wxDefaultSize, 0 );
	room_lb->Wrap( -1 );
	top_bar_sizer->Add( room_lb, 0, wxALL, 5 );
	
	
	top_bar_sizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	current_dt_lb = new wxStaticText( top_pnl, wxID_ANY, wxT("03/13/2008 18:05:00"), wxDefaultPosition, wxDefaultSize, 0 );
	current_dt_lb->Wrap( -1 );
	current_dt_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	top_bar_sizer->Add( current_dt_lb, 0, wxALIGN_CENTER_VERTICAL, 0 );
	
	
	top_bar_sizer->Add( 10, 0, 1, wxEXPAND, 0 );
	
	current_user_lb = new wxStaticText( top_pnl, wxID_ANY, wxT("cafe manager: Dmitry Dergachev"), wxDefaultPosition, wxDefaultSize, 0 );
	current_user_lb->Wrap( -1 );
	current_user_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	top_bar_sizer->Add( current_user_lb, 0, wxALIGN_CENTER_VERTICAL, 0 );
	
	
	top_bar_sizer->Add( 5, 0, 0, wxEXPAND, 0 );
	
	browse_delayed_orders_btn = new wxButton( top_pnl, wxID_ANY, wxT("Browse delayed orders (xx)"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	browse_delayed_orders_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	top_bar_sizer->Add( browse_delayed_orders_btn, 0, wxALIGN_CENTER_VERTICAL|wxEXPAND, 0 );
	
	open_orders_btn = new wxButton( top_pnl, wxID_ANY, wxT("Browse my open orders (xx)"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	open_orders_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	top_bar_sizer->Add( open_orders_btn, 0, wxALIGN_CENTER_VERTICAL|wxEXPAND, 0 );
	
	logout_btn = new wxButton( top_pnl, wxID_ANY, wxT("Log out"), wxDefaultPosition, wxSize( -1,30 ), 0 );
	logout_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	logout_btn->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHTTEXT ) );
	logout_btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNTEXT ) );
	
	top_bar_sizer->Add( logout_btn, 0, wxALIGN_CENTER_VERTICAL|wxEXPAND, 0 );
	
	top_pnl->SetSizer( top_bar_sizer );
	top_pnl->Layout();
	top_bar_sizer->Fit( top_pnl );
	main_sizer->Add( top_pnl, 0, wxEXPAND, 0 );
	
	wxStaticLine* top_hline;
	top_hline = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	main_sizer->Add( top_hline, 0, wxEXPAND | wxALL, 0 );
	
	body_pnl = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	body_pnl->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	btn_pnl = new wxPanel( body_pnl, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	btn_pnl->SetForegroundColour( wxColour( 255, 255, 255 ) );
	
	bSizer6->Add( btn_pnl, 1, wxEXPAND | wxALL, 0 );
	
	rpanel = new wxPanel( body_pnl, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	rsizer = new wxBoxSizer( wxVERTICAL );
	
	sg = new wxOrderTreeListCtrl( rpanel, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTR_EXTENDED|wxTR_FULL_ROW_HIGHLIGHT|wxTR_HIDE_ROOT|wxTR_VIRTUAL );
	sg->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	sg->AddColumn( wxT("Entree"), 210, wxALIGN_LEFT, -1, true, false ); 
	sg->AddColumn( wxT("Price"), 85, wxALIGN_RIGHT, -1, true, false ); 
	sg->AddColumn( wxT("Qty"), 40, wxALIGN_RIGHT, -1, true, false ); 
	sg->AddColumn( wxT("Amount"), 85, wxALIGN_RIGHT, -1, true, false ); 
	
	rsizer->Add( sg, 1, wxEXPAND | wxALL, 0 );
	
	total_pnl = new wxPanel( rpanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	total_gSizer = new wxGridSizer( 5, 2, 0, 0 );
	
	m_subtotal_lb = new wxStaticText( total_pnl, wxID_ANY, wxT("Subtotal:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_subtotal_lb->Wrap( -1 );
	m_subtotal_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	total_gSizer->Add( m_subtotal_lb, 1, wxALIGN_LEFT|wxALL|wxEXPAND, 5 );
	
	m_subtotal_ed = new wxStaticText( total_pnl, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_subtotal_ed->Wrap( -1 );
	m_subtotal_ed->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	total_gSizer->Add( m_subtotal_ed, 1, wxALIGN_RIGHT|wxALL|wxEXPAND, 5 );
	
	m_sales_tax_lb = new wxStaticText( total_pnl, wxID_ANY, wxT("Sales tax:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sales_tax_lb->Wrap( -1 );
	m_sales_tax_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	total_gSizer->Add( m_sales_tax_lb, 0, wxALIGN_LEFT|wxALL|wxEXPAND, 5 );
	
	m_sales_tax_ed = new wxStaticText( total_pnl, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_sales_tax_ed->Wrap( -1 );
	m_sales_tax_ed->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	total_gSizer->Add( m_sales_tax_ed, 1, wxALIGN_RIGHT|wxALL|wxEXPAND, 5 );
	
	m_gratuity_lb = new wxStaticText( total_pnl, wxID_ANY, wxT("Gratuity:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_gratuity_lb->Wrap( -1 );
	m_gratuity_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	total_gSizer->Add( m_gratuity_lb, 0, wxALIGN_LEFT|wxALL|wxEXPAND, 5 );
	
	m_gratuity_ed = new wxStaticText( total_pnl, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_gratuity_ed->Wrap( -1 );
	m_gratuity_ed->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	total_gSizer->Add( m_gratuity_ed, 1, wxALIGN_RIGHT|wxALL|wxEXPAND, 5 );
	
	m_tools_lb = new wxStaticText( total_pnl, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_tools_lb->Wrap( -1 );
	m_tools_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	total_gSizer->Add( m_tools_lb, 0, wxALIGN_LEFT|wxALL|wxEXPAND, 5 );
	
	m_tools_ed = new wxStaticText( total_pnl, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_tools_ed->Wrap( -1 );
	m_tools_ed->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	total_gSizer->Add( m_tools_ed, 1, wxALIGN_RIGHT|wxALL|wxEXPAND, 5 );
	
	m_total_lb = new wxStaticText( total_pnl, wxID_ANY, wxT("TOTAL:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_total_lb->Wrap( -1 );
	m_total_lb->SetFont( wxFont( 16, 74, 90, 92, false, wxT("Arial Narrow") ) );
	
	total_gSizer->Add( m_total_lb, 0, wxALIGN_LEFT|wxALL|wxEXPAND, 5 );
	
	m_total_ed = new wxStaticText( total_pnl, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_total_ed->Wrap( -1 );
	m_total_ed->SetFont( wxFont( 16, 74, 90, 92, false, wxT("Arial Narrow") ) );
	
	total_gSizer->Add( m_total_ed, 1, wxALIGN_RIGHT|wxALL|wxEXPAND, 5 );
	
	total_pnl->SetSizer( total_gSizer );
	total_pnl->Layout();
	total_gSizer->Fit( total_pnl );
	rsizer->Add( total_pnl, 0, wxEXPAND | wxALL, 0 );
	
	ctrl_btn_pnl = new wxPanel( rpanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	ctrl_sizer = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	ctrl_sizer->Add( fgSizer1, 0, wxEXPAND, 0 );
	
	wxBoxSizer* tool1_sizer;
	tool1_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	misc_charges_btn = new wxButton( ctrl_btn_pnl, wxID_ANY, wxT("Misc\ncharges"), wxDefaultPosition, wxSize( -1,50 ), 0 );
	misc_charges_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	misc_charges_btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	
	tool1_sizer->Add( misc_charges_btn, 0, wxEXPAND, 0 );
	
	
	tool1_sizer->Add( 0, 0, 0, wxEXPAND, 0 );
	
	entree_code_btn = new wxButton( ctrl_btn_pnl, wxID_ANY, wxT("Entree\ncode"), wxDefaultPosition, wxSize( -1,50 ), 0 );
	entree_code_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	entree_code_btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	
	tool1_sizer->Add( entree_code_btn, 0, wxEXPAND, 0 );
	
	
	tool1_sizer->Add( 0, 0, 0, wxEXPAND, 0 );
	
	comment_entree_btn = new wxButton( ctrl_btn_pnl, wxID_ANY, wxT("Comment\nentree"), wxDefaultPosition, wxSize( -1,50 ), 0 );
	comment_entree_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	comment_entree_btn->SetBackgroundColour( wxColour( 133, 187, 88 ) );
	comment_entree_btn->Enable( false );
	
	tool1_sizer->Add( comment_entree_btn, 0, wxEXPAND, 0 );
	
	
	tool1_sizer->Add( 0, 0, 1, wxEXPAND, 0 );
	
	plus_btn = new wxBitmapButton( ctrl_btn_pnl, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 50,50 ), wxBU_AUTODRAW );
	plus_btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	plus_btn->Enable( false );
	
	plus_btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	plus_btn->Enable( false );
	
	tool1_sizer->Add( plus_btn, 0, wxEXPAND, 0 );
	
	
	tool1_sizer->Add( 5, 0, 0, wxEXPAND, 0 );
	
	minus_btn = new wxBitmapButton( ctrl_btn_pnl, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 50,50 ), wxBU_AUTODRAW );
	minus_btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	minus_btn->Enable( false );
	
	minus_btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	minus_btn->Enable( false );
	
	tool1_sizer->Add( minus_btn, 0, wxEXPAND, 0 );
	
	ctrl_sizer->Add( tool1_sizer, 0, wxEXPAND, 0 );
	
	wxGridBagSizer* gbSizer2;
	gbSizer2 = new wxGridBagSizer( 0, 0 );
	gbSizer2->SetFlexibleDirection( wxBOTH );
	gbSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	order_type_btn = new wxButton( ctrl_btn_pnl, wxID_ANY, wxT("Order type: \nfor here"), wxDefaultPosition, wxSize( -1,50 ), 0 );
	order_type_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	order_type_btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	
	gbSizer2->Add( order_type_btn, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 0 );
	
	guest_btn = new wxButton( ctrl_btn_pnl, wxID_ANY, wxT("Guests: x"), wxDefaultPosition, wxSize( -1,50 ), 0 );
	guest_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	guest_btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	
	gbSizer2->Add( guest_btn, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxEXPAND, 0 );
	
	discount_btn = new wxButton( ctrl_btn_pnl, wxID_ANY, wxT("Discounts"), wxDefaultPosition, wxSize( -1,50 ), 0 );
	discount_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	discount_btn->SetBackgroundColour( wxColour( 166, 202, 240 ) );
	
	gbSizer2->Add( discount_btn, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxEXPAND, 0 );
	
	token_btn = new wxButton( ctrl_btn_pnl, wxID_ANY, wxT("Token:\nnot selected"), wxDefaultPosition, wxSize( -1,50 ), 0 );
	token_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	token_btn->SetBackgroundColour( wxColour( 58, 110, 165 ) );
	
	gbSizer2->Add( token_btn, wxGBPosition( 0, 3 ), wxGBSpan( 1, 1 ), wxEXPAND, 0 );
	
	delayed_order_details_btn = new wxButton( ctrl_btn_pnl, wxID_ANY, wxT("Delayed\norder details"), wxDefaultPosition, wxSize( -1,50 ), 0 );
	delayed_order_details_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	delayed_order_details_btn->SetBackgroundColour( wxColour( 251, 247, 239 ) );
	
	gbSizer2->Add( delayed_order_details_btn, wxGBPosition( 0, 5 ), wxGBSpan( 1, 1 ), wxEXPAND, 0 );
	
	save_changes_btn = new wxButton( ctrl_btn_pnl, wxID_ANY, wxT("Save\nchanges"), wxDefaultPosition, wxSize( -1,50 ), 0 );
	save_changes_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	save_changes_btn->SetBackgroundColour( wxColour( 204, 255, 204 ) );
	
	gbSizer2->Add( save_changes_btn, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 0 );
	
	close_order_btn = new wxButton( ctrl_btn_pnl, wxID_ANY, wxT("Close order"), wxDefaultPosition, wxSize( -1,50 ), 0 );
	close_order_btn->SetFont( wxFont( 16, 74, 90, 92, false, wxT("Arial Narrow") ) );
	close_order_btn->SetBackgroundColour( wxColour( 204, 255, 204 ) );
	
	gbSizer2->Add( close_order_btn, wxGBPosition( 1, 4 ), wxGBSpan( 1, 2 ), wxEXPAND, 0 );
	
	cancel_changes_btn = new wxButton( ctrl_btn_pnl, wxID_ANY, wxT("Cancel\nchanges"), wxDefaultPosition, wxSize( -1,50 ), 0 );
	cancel_changes_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	cancel_changes_btn->SetBackgroundColour( wxColour( 255, 204, 204 ) );
	
	gbSizer2->Add( cancel_changes_btn, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxEXPAND, 0 );
	
	print_receipt_duplicate_btn = new wxButton( ctrl_btn_pnl, wxID_ANY, wxT("Print\nreceipt"), wxDefaultPosition, wxSize( -1,50 ), 0 );
	print_receipt_duplicate_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	print_receipt_duplicate_btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	
	gbSizer2->Add( print_receipt_duplicate_btn, wxGBPosition( 1, 2 ), wxGBSpan( 1, 1 ), wxEXPAND, 0 );
	
	other_orders_operations_btn = new wxButton( ctrl_btn_pnl, wxID_ANY, wxT("Other\noperations"), wxDefaultPosition, wxSize( -1,50 ), 0 );
	other_orders_operations_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	other_orders_operations_btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	
	gbSizer2->Add( other_orders_operations_btn, wxGBPosition( 1, 3 ), wxGBSpan( 1, 1 ), wxEXPAND, 0 );
	
	ctrl_sizer->Add( gbSizer2, 0, wxSHAPED, 0 );
	
	ctrl_btn_pnl->SetSizer( ctrl_sizer );
	ctrl_btn_pnl->Layout();
	ctrl_sizer->Fit( ctrl_btn_pnl );
	rsizer->Add( ctrl_btn_pnl, 0, wxFIXED_MINSIZE, 0 );
	
	rpanel->SetSizer( rsizer );
	rpanel->Layout();
	rsizer->Fit( rpanel );
	bSizer6->Add( rpanel, 0, wxEXPAND | wxALL, 0 );
	
	body_pnl->SetSizer( bSizer6 );
	body_pnl->Layout();
	bSizer6->Fit( body_pnl );
	main_sizer->Add( body_pnl, 1, wxEXPAND | wxALL, 0 );
	
	this->SetSizer( main_sizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	browse_delayed_orders_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::browse_delayed_orders_btnClick ), NULL, this );
	open_orders_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::open_orders_btnClick ), NULL, this );
	logout_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::logout_btnClick ), NULL, this );
	misc_charges_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::misc_charges_btnClick ), NULL, this );
	entree_code_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::entree_code_btnClick ), NULL, this );
	comment_entree_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::comment_entree_btnClick ), NULL, this );
	plus_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::plus_btnClick ), NULL, this );
	minus_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::minus_btnClick ), NULL, this );
	order_type_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::order_type_btnClick ), NULL, this );
	guest_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::guest_btnClick ), NULL, this );
	discount_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::discount_btnClick ), NULL, this );
	token_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::token_btnClick ), NULL, this );
	delayed_order_details_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::delayed_order_details_btnClick ), NULL, this );
	save_changes_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::save_changes_btnClick ), NULL, this );
	close_order_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::close_order_btnClick ), NULL, this );
	cancel_changes_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::cancel_changes_btnClick ), NULL, this );
	print_receipt_duplicate_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::print_receipt_duplicate_btnClick ), NULL, this );
	other_orders_operations_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::other_orders_operations_btnClick ), NULL, this );
}

base_cashier_f::~base_cashier_f()
{
	// Disconnect Events
	browse_delayed_orders_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::browse_delayed_orders_btnClick ), NULL, this );
	open_orders_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::open_orders_btnClick ), NULL, this );
	logout_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::logout_btnClick ), NULL, this );
	misc_charges_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::misc_charges_btnClick ), NULL, this );
	entree_code_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::entree_code_btnClick ), NULL, this );
	comment_entree_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::comment_entree_btnClick ), NULL, this );
	plus_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::plus_btnClick ), NULL, this );
	minus_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::minus_btnClick ), NULL, this );
	order_type_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::order_type_btnClick ), NULL, this );
	guest_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::guest_btnClick ), NULL, this );
	discount_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::discount_btnClick ), NULL, this );
	token_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::token_btnClick ), NULL, this );
	delayed_order_details_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::delayed_order_details_btnClick ), NULL, this );
	save_changes_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::save_changes_btnClick ), NULL, this );
	close_order_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::close_order_btnClick ), NULL, this );
	cancel_changes_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::cancel_changes_btnClick ), NULL, this );
	print_receipt_duplicate_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::print_receipt_duplicate_btnClick ), NULL, this );
	other_orders_operations_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::other_orders_operations_btnClick ), NULL, this );
}

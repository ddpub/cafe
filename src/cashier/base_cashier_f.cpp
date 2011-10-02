///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "base_cashier_f.h"
#include <ksi_include/custom_treelistctrl.hpp>
///////////////////////////////////////////////////////////////////////////

base_cashier_f::base_cashier_f( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	main_sizer = new wxBoxSizer( wxVERTICAL );
	
	top_pnl = new wxPanel( this, wxID_ANY, wxPoint( -1,-1 ), wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	top_pnl->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	top_bar_sizer = new wxBoxSizer( wxHORIZONTAL );
	
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
	
	open_cash_drawer_btn = new wxButton( top_pnl, wxID_ANY, wxT("Open cash drawer"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	open_cash_drawer_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	top_bar_sizer->Add( open_cash_drawer_btn, 0, wxALIGN_CENTER_VERTICAL|wxEXPAND, 0 );
	
	browse_delayed_orders_btn = new wxButton( top_pnl, wxID_ANY, wxT("Browse delayed orders (xx)"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	browse_delayed_orders_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	top_bar_sizer->Add( browse_delayed_orders_btn, 0, wxALIGN_CENTER_VERTICAL|wxEXPAND, 0 );
	
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
	
	m_panel61 = new wxPanel( body_pnl, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	m_panel61->SetMaxSize( wxSize( 500,-1 ) );
	
	r_sizer = new wxBoxSizer( wxVERTICAL );
	
	sg = new wxOrderTreeListCtrl( m_panel61, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_EXTENDED|wxTR_FULL_ROW_HIGHLIGHT|wxTR_HIDE_ROOT|wxTR_VIRTUAL );
	sg->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	sg->AddColumn( wxT("Entree"), 210, wxALIGN_LEFT, -1, true, false ); 
	sg->AddColumn( wxT("Price"), 85, wxALIGN_RIGHT, -1, true, false ); 
	sg->AddColumn( wxT("Qty"), 40, wxALIGN_RIGHT, -1, true, false ); 
	sg->AddColumn( wxT("Amount"), 85, wxALIGN_RIGHT, -1, true, false ); 
	
	r_sizer->Add( sg, 1, wxEXPAND | wxALL, 0 );
	
	total_pnl = new wxPanel( m_panel61, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	total_pnl_sizer = new wxBoxSizer( wxVERTICAL );
	
	subtotal_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_subtotal_lb = new wxStaticText( total_pnl, wxID_ANY, wxT("Subtotal:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_subtotal_lb->Wrap( -1 );
	m_subtotal_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	subtotal_sizer->Add( m_subtotal_lb, 1, wxALIGN_LEFT|wxALL|wxEXPAND, 5 );
	
	m_subtotal_ed = new wxStaticText( total_pnl, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_subtotal_ed->Wrap( -1 );
	m_subtotal_ed->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	subtotal_sizer->Add( m_subtotal_ed, 1, wxALIGN_RIGHT|wxALL|wxEXPAND, 5 );
	
	total_pnl_sizer->Add( subtotal_sizer, 0, wxEXPAND, 0 );
	
	tax_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_sales_tax_lb = new wxStaticText( total_pnl, wxID_ANY, wxT("Sales tax:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_sales_tax_lb->Wrap( -1 );
	m_sales_tax_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	tax_sizer->Add( m_sales_tax_lb, 1, wxALIGN_LEFT|wxALL|wxEXPAND, 5 );
	
	m_sales_tax_ed = new wxStaticText( total_pnl, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_sales_tax_ed->Wrap( -1 );
	m_sales_tax_ed->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	tax_sizer->Add( m_sales_tax_ed, 1, wxALIGN_RIGHT|wxALL|wxEXPAND, 5 );
	
	total_pnl_sizer->Add( tax_sizer, 0, wxEXPAND, 0 );
	
	gratuity_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_gratuity_lb = new wxStaticText( total_pnl, wxID_ANY, wxT("Gratuity:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_gratuity_lb->Wrap( -1 );
	m_gratuity_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	gratuity_sizer->Add( m_gratuity_lb, 1, wxALL, 5 );
	
	m_gratuity_ed = new wxStaticText( total_pnl, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_gratuity_ed->Wrap( -1 );
	m_gratuity_ed->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	gratuity_sizer->Add( m_gratuity_ed, 1, wxALL, 5 );
	
	total_pnl_sizer->Add( gratuity_sizer, 0, wxEXPAND, 0 );
	
	wxBoxSizer* tools_sizer;
	tools_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_tools_lb = new wxStaticText( total_pnl, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_tools_lb->Wrap( -1 );
	m_tools_lb->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	tools_sizer->Add( m_tools_lb, 1, wxALIGN_LEFT|wxALL|wxEXPAND, 5 );
	
	m_tools_ed = new wxStaticText( total_pnl, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_tools_ed->Wrap( -1 );
	m_tools_ed->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	tools_sizer->Add( m_tools_ed, 1, wxALIGN_RIGHT|wxALL|wxEXPAND, 5 );
	
	total_pnl_sizer->Add( tools_sizer, 0, wxEXPAND, 0 );
	
	wxBoxSizer* total_sizer;
	total_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_total_lb = new wxStaticText( total_pnl, wxID_ANY, wxT("TOTAL:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_total_lb->Wrap( -1 );
	m_total_lb->SetFont( wxFont( 16, 74, 90, 92, false, wxT("Arial Narrow") ) );
	
	total_sizer->Add( m_total_lb, 0, wxALIGN_LEFT|wxALL|wxEXPAND, 5 );
	
	m_total_ed = new wxStaticText( total_pnl, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT|wxST_NO_AUTORESIZE );
	m_total_ed->Wrap( -1 );
	m_total_ed->SetFont( wxFont( 16, 74, 90, 92, false, wxT("Arial Narrow") ) );
	
	total_sizer->Add( m_total_ed, 1, wxALIGN_RIGHT|wxALL|wxEXPAND, 5 );
	
	total_pnl_sizer->Add( total_sizer, 0, wxEXPAND, 0 );
	
	total_pnl->SetSizer( total_pnl_sizer );
	total_pnl->Layout();
	total_pnl_sizer->Fit( total_pnl );
	r_sizer->Add( total_pnl, 0, wxEXPAND | wxALL, 0 );
	
	ctrl_btn_pnl = new wxPanel( m_panel61, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	ctrl_sizer = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	ctrl_sizer->Add( fgSizer1, 1, wxEXPAND, 0 );
	
	wxBoxSizer* tool1_sizer;
	tool1_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	comment_entree_btn = new wxButton( ctrl_btn_pnl, wxID_ANY, wxT("Comment\nentree"), wxDefaultPosition, wxSize( -1,50 ), 0 );
	comment_entree_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	comment_entree_btn->SetBackgroundColour( wxColour( 133, 187, 88 ) );
	comment_entree_btn->Enable( false );
	
	tool1_sizer->Add( comment_entree_btn, 0, wxALIGN_CENTER_VERTICAL|wxEXPAND, 0 );
	
	
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
	
	
	tool1_sizer->Add( 5, 0, 0, wxEXPAND, 0 );
	
	delete_btn = new wxBitmapButton( ctrl_btn_pnl, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 50,50 ), wxBU_AUTODRAW );
	delete_btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	delete_btn->Enable( false );
	
	delete_btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	delete_btn->Enable( false );
	
	tool1_sizer->Add( delete_btn, 0, wxEXPAND, 0 );
	
	ctrl_sizer->Add( tool1_sizer, 0, wxEXPAND, 0 );
	
	grid_btnSizer = new wxGridBagSizer( 0, 0 );
	grid_btnSizer->SetFlexibleDirection( wxBOTH );
	grid_btnSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	for_here_btn = new wxButton( ctrl_btn_pnl, wxID_ANY, wxT("For here"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	for_here_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	for_here_btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER ) );
	for_here_btn->SetMinSize( wxSize( -1,50 ) );
	
	grid_btnSizer->Add( for_here_btn, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 0 );
	
	to_go_btn = new wxButton( ctrl_btn_pnl, wxID_ANY, wxT("To go"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	to_go_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	to_go_btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER ) );
	to_go_btn->SetMinSize( wxSize( -1,50 ) );
	
	grid_btnSizer->Add( to_go_btn, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxEXPAND, 0 );
	
	delivery_btn = new wxButton( ctrl_btn_pnl, wxID_ANY, wxT("Delivery"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	delivery_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	delivery_btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER ) );
	delivery_btn->SetMinSize( wxSize( -1,50 ) );
	
	grid_btnSizer->Add( delivery_btn, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxEXPAND, 0 );
	
	pickup_btn = new wxButton( ctrl_btn_pnl, wxID_ANY, wxT("Pickup"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	pickup_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	pickup_btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER ) );
	pickup_btn->SetMinSize( wxSize( -1,50 ) );
	
	grid_btnSizer->Add( pickup_btn, wxGBPosition( 0, 3 ), wxGBSpan( 1, 1 ), wxEXPAND, 0 );
	
	employee_meal_btn = new wxButton( ctrl_btn_pnl, wxID_ANY, wxT("Employee\nmeal."), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	employee_meal_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	employee_meal_btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER ) );
	employee_meal_btn->SetMinSize( wxSize( -1,50 ) );
	
	grid_btnSizer->Add( employee_meal_btn, wxGBPosition( 0, 4 ), wxGBSpan( 1, 1 ), wxEXPAND, 0 );
	
	donation_btn = new wxButton( ctrl_btn_pnl, wxID_ANY, wxT("Donation"), wxDefaultPosition, wxSize( -1,50 ), 0 );
	donation_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	donation_btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_ACTIVEBORDER ) );
	donation_btn->Enable( false );
	
	grid_btnSizer->Add( donation_btn, wxGBPosition( 0, 5 ), wxGBSpan( 1, 1 ), wxEXPAND, 0 );
	
	misc_charges_btn = new wxButton( ctrl_btn_pnl, wxID_ANY, wxT("Misc\ncharges"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	misc_charges_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	misc_charges_btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	misc_charges_btn->SetMinSize( wxSize( -1,50 ) );
	
	grid_btnSizer->Add( misc_charges_btn, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 0 );
	
	entree_code_btn = new wxButton( ctrl_btn_pnl, wxID_ANY, wxT("Entree\ncode"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	entree_code_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	entree_code_btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	entree_code_btn->SetMinSize( wxSize( -1,50 ) );
	
	grid_btnSizer->Add( entree_code_btn, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxEXPAND, 0 );
	
	discount_btn = new wxButton( ctrl_btn_pnl, wxID_ANY, wxT("Discounts"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	discount_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	discount_btn->SetBackgroundColour( wxColour( 166, 202, 240 ) );
	discount_btn->SetMinSize( wxSize( -1,50 ) );
	
	grid_btnSizer->Add( discount_btn, wxGBPosition( 1, 2 ), wxGBSpan( 1, 1 ), wxEXPAND, 0 );
	
	save_and_hide_btn = new wxButton( ctrl_btn_pnl, wxID_ANY, wxT("Save and \nhide order"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	save_and_hide_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	save_and_hide_btn->SetBackgroundColour( wxColour( 204, 255, 204 ) );
	save_and_hide_btn->SetMinSize( wxSize( -1,50 ) );
	
	grid_btnSizer->Add( save_and_hide_btn, wxGBPosition( 1, 4 ), wxGBSpan( 1, 1 ), wxEXPAND, 0 );
	
	token_btn = new wxButton( ctrl_btn_pnl, wxID_ANY, wxT("Token:\nnot selected"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	token_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	token_btn->SetBackgroundColour( wxColour( 58, 110, 165 ) );
	token_btn->SetMinSize( wxSize( -1,50 ) );
	
	grid_btnSizer->Add( token_btn, wxGBPosition( 1, 3 ), wxGBSpan( 1, 1 ), wxEXPAND, 0 );
	
	delayed_order_details_btn = new wxButton( ctrl_btn_pnl, wxID_ANY, wxT("Delayed\norder details"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	delayed_order_details_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	delayed_order_details_btn->SetBackgroundColour( wxColour( 251, 247, 239 ) );
	delayed_order_details_btn->SetMinSize( wxSize( -1,50 ) );
	
	grid_btnSizer->Add( delayed_order_details_btn, wxGBPosition( 1, 5 ), wxGBSpan( 1, 1 ), wxEXPAND, 0 );
	
	refund_order_btn = new wxButton( ctrl_btn_pnl, wxID_ANY, wxT("Closed orders\noperations"), wxDefaultPosition, wxDefaultSize, 0 );
	refund_order_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	refund_order_btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	refund_order_btn->SetMinSize( wxSize( -1,50 ) );
	
	grid_btnSizer->Add( refund_order_btn, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxEXPAND, 0 );
	
	process_btn = new wxButton( ctrl_btn_pnl, wxID_ANY, wxT("Process"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	process_btn->SetFont( wxFont( 16, 74, 90, 92, false, wxT("Arial Narrow") ) );
	process_btn->SetBackgroundColour( wxColour( 204, 255, 204 ) );
	process_btn->SetMinSize( wxSize( -1,50 ) );
	
	grid_btnSizer->Add( process_btn, wxGBPosition( 2, 3 ), wxGBSpan( 1, 3 ), wxEXPAND, 0 );
	
	cash_advance_btn = new wxButton( ctrl_btn_pnl, wxID_ANY, wxT("Cash\nadvance\n(0)"), wxDefaultPosition, wxDefaultSize, 0 );
	cash_advance_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	cash_advance_btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	cash_advance_btn->SetMinSize( wxSize( -1,50 ) );
	
	grid_btnSizer->Add( cash_advance_btn, wxGBPosition( 2, 1 ), wxGBSpan( 1, 1 ), wxEXPAND, 0 );
	
	shift_btn = new wxButton( ctrl_btn_pnl, wxID_ANY, wxT("Seances"), wxDefaultPosition, wxDefaultSize, 0 );
	shift_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	shift_btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	shift_btn->SetMinSize( wxSize( -1,50 ) );
	
	grid_btnSizer->Add( shift_btn, wxGBPosition( 2, 2 ), wxGBSpan( 1, 1 ), wxEXPAND, 0 );
	
	ctrl_sizer->Add( grid_btnSizer, 0, wxSHAPED, 0 );
	
	ctrl_btn_pnl->SetSizer( ctrl_sizer );
	ctrl_btn_pnl->Layout();
	ctrl_sizer->Fit( ctrl_btn_pnl );
	r_sizer->Add( ctrl_btn_pnl, 0, wxFIXED_MINSIZE, 0 );
	
	m_panel61->SetSizer( r_sizer );
	m_panel61->Layout();
	r_sizer->Fit( m_panel61 );
	bSizer6->Add( m_panel61, 0, wxEXPAND | wxALL, 0 );
	
	body_pnl->SetSizer( bSizer6 );
	body_pnl->Layout();
	bSizer6->Fit( body_pnl );
	main_sizer->Add( body_pnl, 1, wxEXPAND | wxALL, 0 );
	
	buttom_hline = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	main_sizer->Add( buttom_hline, 0, wxEXPAND | wxALL, 0 );
	
	status_pnl = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	status_pnl->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	status_lb = new wxStaticText( status_pnl, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	status_lb->Wrap( -1 );
	status_lb->SetFont( wxFont( 8, 74, 90, 90, false, wxT("Tahoma") ) );
	status_lb->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNSHADOW ) );
	
	bSizer7->Add( status_lb, 0, wxALL|wxEXPAND, 5 );
	
	status_pnl->SetSizer( bSizer7 );
	status_pnl->Layout();
	bSizer7->Fit( status_pnl );
	main_sizer->Add( status_pnl, 0, wxEXPAND|wxALL|wxALIGN_CENTER_HORIZONTAL, 0 );
	
	this->SetSizer( main_sizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	open_cash_drawer_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::open_cash_drawer_btnClick ), NULL, this );
	browse_delayed_orders_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::browse_delayed_orders_btnClick ), NULL, this );
	logout_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::logout_btnClick ), NULL, this );
	comment_entree_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::comment_entree_btnClick ), NULL, this );
	plus_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::plus_btnClick ), NULL, this );
	minus_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::minus_btnClick ), NULL, this );
	delete_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::delete_btnClick ), NULL, this );
	for_here_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::order_type_btnClick ), NULL, this );
	to_go_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::order_type_btnClick ), NULL, this );
	delivery_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::order_type_btnClick ), NULL, this );
	pickup_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::order_type_btnClick ), NULL, this );
	employee_meal_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::order_type_btnClick ), NULL, this );
	donation_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::order_type_btnClick ), NULL, this );
	misc_charges_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::misc_charges_btnClick ), NULL, this );
	entree_code_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::entree_code_btnClick ), NULL, this );
	discount_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::discount_btnClick ), NULL, this );
	save_and_hide_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::save_and_hide_btnClick ), NULL, this );
	token_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::token_btnClick ), NULL, this );
	delayed_order_details_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::delayed_order_details_btnClick ), NULL, this );
	refund_order_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::refund_order_btnClick ), NULL, this );
	process_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::process_btnClick ), NULL, this );
	cash_advance_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::cash_advance_btnClick ), NULL, this );
	shift_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::shift_btnClick ), NULL, this );
	status_pnl->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( base_cashier_f::OnConfigDbClick ), NULL, this );
	status_lb->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( base_cashier_f::OnConfigDbClick ), NULL, this );
}

base_cashier_f::~base_cashier_f()
{
	// Disconnect Events
	open_cash_drawer_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::open_cash_drawer_btnClick ), NULL, this );
	browse_delayed_orders_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::browse_delayed_orders_btnClick ), NULL, this );
	logout_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::logout_btnClick ), NULL, this );
	comment_entree_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::comment_entree_btnClick ), NULL, this );
	plus_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::plus_btnClick ), NULL, this );
	minus_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::minus_btnClick ), NULL, this );
	delete_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::delete_btnClick ), NULL, this );
	for_here_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::order_type_btnClick ), NULL, this );
	to_go_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::order_type_btnClick ), NULL, this );
	delivery_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::order_type_btnClick ), NULL, this );
	pickup_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::order_type_btnClick ), NULL, this );
	employee_meal_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::order_type_btnClick ), NULL, this );
	donation_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::order_type_btnClick ), NULL, this );
	misc_charges_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::misc_charges_btnClick ), NULL, this );
	entree_code_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::entree_code_btnClick ), NULL, this );
	discount_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::discount_btnClick ), NULL, this );
	save_and_hide_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::save_and_hide_btnClick ), NULL, this );
	token_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::token_btnClick ), NULL, this );
	delayed_order_details_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::delayed_order_details_btnClick ), NULL, this );
	refund_order_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::refund_order_btnClick ), NULL, this );
	process_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::process_btnClick ), NULL, this );
	cash_advance_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::cash_advance_btnClick ), NULL, this );
	shift_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_cashier_f::shift_btnClick ), NULL, this );
	status_pnl->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( base_cashier_f::OnConfigDbClick ), NULL, this );
	status_lb->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( base_cashier_f::OnConfigDbClick ), NULL, this );
}

///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "base_close_order_manager_f.h"
#include <ksi_include/custom_treelistctrl.hpp>
///////////////////////////////////////////////////////////////////////////

base_close_order_manager_f::base_close_order_manager_f( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	sg = new wxRefundTreeListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_EXTENDED|wxTR_FULL_ROW_HIGHLIGHT|wxTR_HIDE_ROOT|wxTR_VIRTUAL );
	sg->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	sg->AddColumn( wxT("ID"), 80, wxALIGN_RIGHT, -1, true, false ); 
	sg->AddColumn( wxT("Created"), 140, wxALIGN_LEFT, -1, true, false ); 
	sg->AddColumn( wxT("Amount"), 100, wxALIGN_RIGHT, -1, true, false ); 
	sg->AddColumn( wxT("Payment type"), 100, wxALIGN_LEFT, -1, true, false ); 
	sg->AddColumn( wxT("Room"), 100, wxALIGN_LEFT, -1, true, false ); 
	sg->AddColumn( wxT("POS"), 100, wxALIGN_LEFT, -1, true, false ); 
	sg->AddColumn( wxT("Operator"), 120, wxALIGN_LEFT, -1, true, false ); 
	
	bSizer1->Add( sg, 1, wxALL|wxEXPAND, 5 );
	
	buttom_bar_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	
	buttom_bar_sizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	find_order_btn = new wxButton( this, wxID_ANY, wxT("Type order_id"), wxDefaultPosition, wxSize( -1,50 ), 0 );
	find_order_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	buttom_bar_sizer->Add( find_order_btn, 0, 0, 5 );
	
	
	buttom_bar_sizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	print_receipt_duplicate_btn = new wxButton( this, wxID_ANY, wxT("Print reciept  duplicate"), wxDefaultPosition, wxSize( -1,50 ), 0 );
	print_receipt_duplicate_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	print_receipt_duplicate_btn->Enable( false );
	
	buttom_bar_sizer->Add( print_receipt_duplicate_btn, 0, 0, 5 );
	
	
	buttom_bar_sizer->Add( 0, 0, 1, wxEXPAND, 0 );
	
	refund_order_btn = new wxButton( this, wxID_ANY, wxT("Refund order"), wxDefaultPosition, wxSize( -1,50 ), 0 );
	refund_order_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	refund_order_btn->Enable( false );
	
	buttom_bar_sizer->Add( refund_order_btn, 0, 0, 5 );
	
	
	buttom_bar_sizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	change_payment_type_btn = new wxButton( this, wxID_ANY, wxT("Change payment type"), wxDefaultPosition, wxSize( -1,50 ), 0 );
	change_payment_type_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	change_payment_type_btn->Enable( false );
	
	buttom_bar_sizer->Add( change_payment_type_btn, 0, 0, 5 );
	
	
	buttom_bar_sizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	reopen_order_btn = new wxButton( this, wxID_ANY, wxT("Reopen order"), wxDefaultPosition, wxSize( -1,50 ), 0 );
	reopen_order_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	reopen_order_btn->Enable( false );
	
	buttom_bar_sizer->Add( reopen_order_btn, 0, 0, 5 );
	
	
	buttom_bar_sizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	close_btn = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxSize( -1,50 ), 0 );
	close_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	close_btn->SetBackgroundColour( wxColour( 255, 204, 204 ) );
	
	buttom_bar_sizer->Add( close_btn, 0, 0, 5 );
	
	
	buttom_bar_sizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer1->Add( buttom_bar_sizer, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	find_order_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_close_order_manager_f::find_order_btnClick ), NULL, this );
	print_receipt_duplicate_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_close_order_manager_f::print_receipt_duplicate_btnClick ), NULL, this );
	refund_order_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_close_order_manager_f::refund_order_btnClick ), NULL, this );
	change_payment_type_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_close_order_manager_f::change_payment_type_btnClick ), NULL, this );
	reopen_order_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_close_order_manager_f::reopen_order_btnClick ), NULL, this );
	close_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_close_order_manager_f::close_btnClick ), NULL, this );
}

base_close_order_manager_f::~base_close_order_manager_f()
{
	// Disconnect Events
	find_order_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_close_order_manager_f::find_order_btnClick ), NULL, this );
	print_receipt_duplicate_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_close_order_manager_f::print_receipt_duplicate_btnClick ), NULL, this );
	refund_order_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_close_order_manager_f::refund_order_btnClick ), NULL, this );
	change_payment_type_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_close_order_manager_f::change_payment_type_btnClick ), NULL, this );
	reopen_order_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_close_order_manager_f::reopen_order_btnClick ), NULL, this );
	close_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_close_order_manager_f::close_btnClick ), NULL, this );
}

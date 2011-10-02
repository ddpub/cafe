///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "base_split_order_f.h"
#include <ksi_include/custom_treelistctrl.hpp>
///////////////////////////////////////////////////////////////////////////

base_split_order_f::base_split_order_f( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxHORIZONTAL );
	
	source_size = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("order #xx-xxx") ), wxVERTICAL );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	source_sg = new wxSplitOrderTreeListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_EXTENDED|wxTR_FULL_ROW_HIGHLIGHT|wxTR_HIDE_ROOT|wxTR_VIRTUAL );
	source_sg->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	source_sg->AddColumn( wxT("Entree"), 160, wxALIGN_LEFT, -1, true, false ); 
	source_sg->AddColumn( wxT("price"), 60, wxALIGN_RIGHT, -1, true, false ); 
	source_sg->AddColumn( wxT("qty"), 50, wxALIGN_RIGHT, -1, true, false ); 
	source_sg->AddColumn( wxT("amount"), 60, wxALIGN_RIGHT, -1, true, false ); 
	
	bSizer5->Add( source_sg, 1, wxEXPAND | wxALL, 0 );
	
	source_size->Add( bSizer5, 1, wxEXPAND, 0 );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer7->Add( 0, 0, 1, wxEXPAND, 5 );
	
	source_guests_ed = new wxTextCtrl( this, wxID_ANY, wxT("99"), wxDefaultPosition, wxSize( 50,50 ), wxTE_RIGHT );
	source_guests_ed->SetFont( wxFont( 28, 74, 90, 90, false, wxT("Arial Narrow") ) );
	source_guests_ed->Enable( false );
	
	bSizer7->Add( source_guests_ed, 0, wxBOTTOM|wxTOP, 5 );
	
	m_spinBtn1 = new wxSpinButton( this, wxID_ANY, wxDefaultPosition, wxSize( -1,50 ), 0 );
	bSizer7->Add( m_spinBtn1, 0, wxBOTTOM|wxTOP, 5 );
	
	
	bSizer7->Add( 0, 0, 1, wxEXPAND, 5 );
	
	source_table_btn = new wxButton( this, wxID_ANY, wxT("Table: XXX"), wxDefaultPosition, wxSize( 100,50 ), 0 );
	source_table_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	source_table_btn->Enable( false );
	
	bSizer7->Add( source_table_btn, 0, wxTOP, 5 );
	
	
	bSizer7->Add( 0, 0, 1, wxEXPAND, 5 );
	
	source_size->Add( bSizer7, 0, wxEXPAND, 0 );
	
	bSizer9->Add( source_size, 1, wxEXPAND, 0 );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	
	bSizer12->Add( 0, 0, 1, wxEXPAND, 5 );
	
	move_from_source_btn = new wxButton( this, wxID_ANY, wxT(">"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	move_from_source_btn->SetFont( wxFont( 28, 74, 90, 90, false, wxT("Arial Narrow") ) );
	move_from_source_btn->Enable( false );
	
	bSizer12->Add( move_from_source_btn, 0, wxALL, 0 );
	
	
	bSizer12->Add( 0, 0, 1, wxEXPAND, 5 );
	
	move_to_source_btn = new wxButton( this, wxID_ANY, wxT("<"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	move_to_source_btn->SetFont( wxFont( 28, 74, 90, 90, false, wxT("Arial Narrow") ) );
	move_to_source_btn->Enable( false );
	
	bSizer12->Add( move_to_source_btn, 0, wxALL, 0 );
	
	
	bSizer12->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer9->Add( bSizer12, 0, wxEXPAND, 0 );
	
	destincation_sizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("order #xx-xxx") ), wxVERTICAL );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	destination_sg = new wxSplitOrderTreeListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_EXTENDED|wxTR_FULL_ROW_HIGHLIGHT|wxTR_HIDE_ROOT|wxTR_VIRTUAL );
	destination_sg->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	destination_sg->AddColumn( wxT("Entree"), 160, wxALIGN_LEFT, -1, true, false ); 
	destination_sg->AddColumn( wxT("price"), 60, wxALIGN_RIGHT, -1, true, false ); 
	destination_sg->AddColumn( wxT("qty"), 50, wxALIGN_RIGHT, -1, true, false ); 
	destination_sg->AddColumn( wxT("amount"), 60, wxALIGN_RIGHT, -1, true, false ); 
	
	bSizer6->Add( destination_sg, 1, wxEXPAND | wxALL, 0 );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer8->Add( 0, 0, 1, wxEXPAND, 5 );
	
	destination_guests_ed = new wxTextCtrl( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( 50,50 ), wxTE_RIGHT );
	destination_guests_ed->SetFont( wxFont( 28, 74, 90, 90, false, wxT("Arial Narrow") ) );
	destination_guests_ed->Enable( false );
	
	bSizer8->Add( destination_guests_ed, 0, wxBOTTOM|wxTOP, 5 );
	
	m_spinBtn2 = new wxSpinButton( this, wxID_ANY, wxDefaultPosition, wxSize( -1,50 ), 0 );
	bSizer8->Add( m_spinBtn2, 0, wxBOTTOM|wxTOP, 5 );
	
	
	bSizer8->Add( 0, 0, 1, wxEXPAND, 5 );
	
	destination_table_btn = new wxButton( this, wxID_ANY, wxT("Table: XXX"), wxDefaultPosition, wxSize( 100,50 ), 0 );
	destination_table_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer8->Add( destination_table_btn, 0, wxTOP, 5 );
	
	
	bSizer8->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer6->Add( bSizer8, 0, wxEXPAND, 0 );
	
	destincation_sizer->Add( bSizer6, 1, wxEXPAND, 0 );
	
	bSizer9->Add( destincation_sizer, 1, wxEXPAND, 0 );
	
	bSizer1->Add( bSizer9, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer10->Add( 0, 0, 1, wxEXPAND, 5 );
	
	ok_btn = new wxButton( this, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	ok_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	ok_btn->SetBackgroundColour( wxColour( 204, 255, 204 ) );
	ok_btn->Enable( false );
	
	bSizer10->Add( ok_btn, 0, wxTOP, 5 );
	
	
	bSizer10->Add( 0, 0, 2, wxEXPAND, 5 );
	
	cancel_btn = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	cancel_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	cancel_btn->SetBackgroundColour( wxColour( 255, 204, 204 ) );
	
	bSizer10->Add( cancel_btn, 0, wxTOP, 5 );
	
	
	bSizer10->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer1->Add( bSizer10, 0, wxEXPAND, 0 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	// Connect Events
	m_spinBtn1->Connect( wxEVT_SCROLL_LINEDOWN, wxSpinEventHandler( base_split_order_f::sourceSpinDownClick ), NULL, this );
	m_spinBtn1->Connect( wxEVT_SCROLL_LINEUP, wxSpinEventHandler( base_split_order_f::sourceSpinUpClick ), NULL, this );
	move_from_source_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_split_order_f::move_from_source_btnClick ), NULL, this );
	move_to_source_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_split_order_f::move_to_source_btnClick ), NULL, this );
	m_spinBtn2->Connect( wxEVT_SCROLL_LINEDOWN, wxSpinEventHandler( base_split_order_f::destinationSpinDownClick ), NULL, this );
	m_spinBtn2->Connect( wxEVT_SCROLL_LINEUP, wxSpinEventHandler( base_split_order_f::destionationSpinUpClick ), NULL, this );
	destination_table_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_split_order_f::destination_table_btnClick ), NULL, this );
	ok_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_split_order_f::ok_btnClick ), NULL, this );
	cancel_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_split_order_f::cancel_btnClick ), NULL, this );
}

base_split_order_f::~base_split_order_f()
{
	// Disconnect Events
	m_spinBtn1->Disconnect( wxEVT_SCROLL_LINEDOWN, wxSpinEventHandler( base_split_order_f::sourceSpinDownClick ), NULL, this );
	m_spinBtn1->Disconnect( wxEVT_SCROLL_LINEUP, wxSpinEventHandler( base_split_order_f::sourceSpinUpClick ), NULL, this );
	move_from_source_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_split_order_f::move_from_source_btnClick ), NULL, this );
	move_to_source_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_split_order_f::move_to_source_btnClick ), NULL, this );
	m_spinBtn2->Disconnect( wxEVT_SCROLL_LINEDOWN, wxSpinEventHandler( base_split_order_f::destinationSpinDownClick ), NULL, this );
	m_spinBtn2->Disconnect( wxEVT_SCROLL_LINEUP, wxSpinEventHandler( base_split_order_f::destionationSpinUpClick ), NULL, this );
	destination_table_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_split_order_f::destination_table_btnClick ), NULL, this );
	ok_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_split_order_f::ok_btnClick ), NULL, this );
	cancel_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_split_order_f::cancel_btnClick ), NULL, this );
}

///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "base_add_modifiers_f.h"
#include <ksi_include/custom_treelistctrl.hpp>
///////////////////////////////////////////////////////////////////////////

base_add_modifiers_f::base_add_modifiers_f( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );
	
	btn_pnl = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	btn_pnl->SetMaxSize( wxSize( 430,-1 ) );
	
	bSizer8->Add( btn_pnl, 1, wxEXPAND | wxALL, 0 );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer6->Add( 0, 0, 1, wxALIGN_BOTTOM|wxEXPAND, 5 );
	
	prev_page_btn = new wxButton( this, wxID_ANY, wxT("<< previous page"), wxDefaultPosition, wxSize( 100,40 ), 0 );
	prev_page_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	prev_page_btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	prev_page_btn->Enable( false );
	
	bSizer6->Add( prev_page_btn, 0, wxALL, 5 );
	
	
	bSizer6->Add( 0, 0, 2, wxALIGN_BOTTOM|wxEXPAND, 5 );
	
	next_page_btn = new wxButton( this, wxID_ANY, wxT(">> next page"), wxDefaultPosition, wxSize( 100,40 ), 0 );
	next_page_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	next_page_btn->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	next_page_btn->Enable( false );
	
	bSizer6->Add( next_page_btn, 0, wxALL, 5 );
	
	
	bSizer6->Add( 0, 0, 1, wxALIGN_BOTTOM|wxEXPAND, 5 );
	
	bSizer8->Add( bSizer6, 0, wxEXPAND, 5 );
	
	bSizer5->Add( bSizer8, 1, wxEXPAND, 5 );
	
	m_panel21 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	sg = new wxModifierTreeListCtrl( m_panel21, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTR_EXTENDED|wxTR_FULL_ROW_HIGHLIGHT|wxTR_HIDE_ROOT|wxTR_VIRTUAL );
	sg->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	sg->SetMinSize( wxSize( 300,-1 ) );
	
	sg->AddColumn( wxT("Group/Modifier"), 220, wxALIGN_LEFT, -1, true, false ); 
	sg->AddColumn( wxT("Qty"), 50, wxALIGN_RIGHT, -1, true, false ); 
	
	bSizer4->Add( sg, 1, wxALL|wxEXPAND, 0 );
	
	m_panel4 = new wxPanel( m_panel21, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	m_panel4->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	wxBoxSizer* bSizer51;
	bSizer51 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer51->Add( 0, 0, 1, wxEXPAND, 0 );
	
	plus_btn = new wxBitmapButton( m_panel4, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 50,50 ), wxBU_AUTODRAW );
	plus_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	plus_btn->Enable( false );
	
	plus_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	plus_btn->Enable( false );
	
	bSizer51->Add( plus_btn, 0, wxALL, 0 );
	
	
	bSizer51->Add( 0, 0, 0, wxEXPAND, 0 );
	
	minus_btn = new wxBitmapButton( m_panel4, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 50,50 ), wxBU_AUTODRAW );
	minus_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	minus_btn->Enable( false );
	
	minus_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	minus_btn->Enable( false );
	
	bSizer51->Add( minus_btn, 0, wxALL, 0 );
	
	
	bSizer51->Add( 0, 0, 0, wxEXPAND, 0 );
	
	delete_btn = new wxBitmapButton( m_panel4, wxID_ANY, wxNullBitmap, wxPoint( -1,-1 ), wxSize( 50,50 ), wxBU_AUTODRAW );
	delete_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	delete_btn->Enable( false );
	
	delete_btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	delete_btn->Enable( false );
	
	bSizer51->Add( delete_btn, 0, wxALL, 0 );
	
	m_panel4->SetSizer( bSizer51 );
	m_panel4->Layout();
	bSizer51->Fit( m_panel4 );
	bSizer4->Add( m_panel4, 0, wxEXPAND | wxALL, 0 );
	
	m_panel21->SetSizer( bSizer4 );
	m_panel21->Layout();
	bSizer4->Fit( m_panel21 );
	bSizer5->Add( m_panel21, 0, wxALL|wxEXPAND, 0 );
	
	bSizer1->Add( bSizer5, 1, wxEXPAND, 0 );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	ok_btn = new wxButton( this, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	ok_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	ok_btn->SetBackgroundColour( wxColour( 204, 255, 204 ) );
	
	bSizer2->Add( ok_btn, 0, wxTOP, 5 );
	
	
	bSizer2->Add( 0, 0, 2, wxEXPAND, 5 );
	
	cancel_btn = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	cancel_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	cancel_btn->SetBackgroundColour( wxColour( 255, 204, 204 ) );
	
	bSizer2->Add( cancel_btn, 0, wxTOP, 5 );
	
	
	bSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer1->Add( bSizer2, 0, wxEXPAND, 0 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	// Connect Events
	prev_page_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_add_modifiers_f::prev_page_btnClick ), NULL, this );
	next_page_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_add_modifiers_f::next_page_btnClick ), NULL, this );
	plus_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_add_modifiers_f::plus_btnClick ), NULL, this );
	minus_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_add_modifiers_f::minus_btnClick ), NULL, this );
	delete_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_add_modifiers_f::delete_btnClick ), NULL, this );
	ok_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_add_modifiers_f::ok_btnClick ), NULL, this );
	cancel_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_add_modifiers_f::cancel_btnClick ), NULL, this );
}

base_add_modifiers_f::~base_add_modifiers_f()
{
	// Disconnect Events
	prev_page_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_add_modifiers_f::prev_page_btnClick ), NULL, this );
	next_page_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_add_modifiers_f::next_page_btnClick ), NULL, this );
	plus_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_add_modifiers_f::plus_btnClick ), NULL, this );
	minus_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_add_modifiers_f::minus_btnClick ), NULL, this );
	delete_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_add_modifiers_f::delete_btnClick ), NULL, this );
	ok_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_add_modifiers_f::ok_btnClick ), NULL, this );
	cancel_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_add_modifiers_f::cancel_btnClick ), NULL, this );
}

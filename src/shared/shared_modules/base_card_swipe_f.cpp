///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "base_card_swipe_f.h"

///////////////////////////////////////////////////////////////////////////

base_card_swipe_f::base_card_swipe_f( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_label = new wxStaticText( this, wxID_ANY, wxT("Swipe the credit card"), wxDefaultPosition, wxDefaultSize, 0|wxWANTS_CHARS );
	m_label->Wrap( 200 );
	m_label->SetFont( wxFont( 14, 74, 90, 90, false, wxT("Arial Narrow") ) );
	m_label->SetMinSize( wxSize( 200,-1 ) );
	
	bSizer1->Add( m_label, 0, wxALL, 5 );
	
	cancel_btn = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0|wxWANTS_CHARS );
	cancel_btn->SetFont( wxFont( 14, 74, 90, 90, false, wxT("Arial Narrow") ) );
	cancel_btn->SetMinSize( wxSize( 100,30 ) );
	
	bSizer1->Add( cancel_btn, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );
	
	// Connect Events
	this->Connect( wxEVT_CHAR, wxKeyEventHandler( base_card_swipe_f::OnChar ) );
	m_label->Connect( wxEVT_CHAR, wxKeyEventHandler( base_card_swipe_f::OnChar ), NULL, this );
	cancel_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_card_swipe_f::cancel_btnClick ), NULL, this );
	cancel_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_card_swipe_f::OnChar ), NULL, this );
	cancel_btn->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( base_card_swipe_f::OnLeftMouseUp ), NULL, this );
}

base_card_swipe_f::~base_card_swipe_f()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_card_swipe_f::OnChar ) );
	m_label->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_card_swipe_f::OnChar ), NULL, this );
	cancel_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_card_swipe_f::cancel_btnClick ), NULL, this );
	cancel_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_card_swipe_f::OnChar ), NULL, this );
	cancel_btn->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( base_card_swipe_f::OnLeftMouseUp ), NULL, this );
}

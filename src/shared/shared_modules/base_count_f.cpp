///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "base_count_f.h"

///////////////////////////////////////////////////////////////////////////

base_count_f::base_count_f( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	val_ed = new wxTextCtrl( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( -1,100 ), wxTE_RIGHT );
	val_ed->SetMaxLength( 2 ); 
	val_ed->SetFont( wxFont( 60, 74, 90, 90, false, wxT("Arial Narrow") ) );
	val_ed->Enable( false );
	
	bSizer2->Add( val_ed, 0, wxALL, 5 );
	
	spin_btn = new wxSpinButton( this, wxID_ANY, wxDefaultPosition, wxSize( 100,100 ), 0 );
	bSizer2->Add( spin_btn, 0, wxALL, 5 );
	
	bSizer1->Add( bSizer2, 1, wxEXPAND, 5 );
	
	ok_btn = new wxButton( this, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	ok_btn->SetDefault(); 
	ok_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Arial Narrow") ) );
	ok_btn->SetBackgroundColour( wxColour( 204, 255, 204 ) );
	
	bSizer1->Add( ok_btn, 0, wxALIGN_CENTER, 0 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );
	
	// Connect Events
	spin_btn->Connect( wxEVT_SCROLL_LINEDOWN, wxSpinEventHandler( base_count_f::spin_btnDown ), NULL, this );
	spin_btn->Connect( wxEVT_SCROLL_LINEUP, wxSpinEventHandler( base_count_f::spin_btnUp ), NULL, this );
	ok_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_count_f::ok_btnClick ), NULL, this );
}

base_count_f::~base_count_f()
{
	// Disconnect Events
	spin_btn->Disconnect( wxEVT_SCROLL_LINEDOWN, wxSpinEventHandler( base_count_f::spin_btnDown ), NULL, this );
	spin_btn->Disconnect( wxEVT_SCROLL_LINEUP, wxSpinEventHandler( base_count_f::spin_btnUp ), NULL, this );
	ok_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_count_f::ok_btnClick ), NULL, this );
}

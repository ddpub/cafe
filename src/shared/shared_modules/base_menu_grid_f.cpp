///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 17 2007)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "base_menu_grid_f.h"

///////////////////////////////////////////////////////////////////////////

base_menu_grid_f::base_menu_grid_f( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	menu_list = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	menu_list->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer7->Add( menu_list, 1, wxEXPAND, 0 );
	
	this->SetSizer( bSizer7 );
	this->Layout();
	bSizer7->Fit( this );
	
	// Connect Events
	menu_list->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( base_menu_grid_f::menu_click ), NULL, this );
}

base_menu_grid_f::~base_menu_grid_f()
{
	// Disconnect Events
	menu_list->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( base_menu_grid_f::menu_click ), NULL, this );
}

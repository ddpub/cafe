///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "base_position_f.h"

///////////////////////////////////////////////////////////////////////////

base_position_f::base_position_f( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	lb_name = new wxStaticText( this, wxID_ANY, wxT("Position name"), wxDefaultPosition, wxDefaultSize, 0 );
	lb_name->Wrap( -1 );
	bSizer2->Add( lb_name, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	tc_posname = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( tc_posname, 1, wxALL, 5 );
	
	bSizer1->Add( bSizer2, 0, wxEXPAND, 5 );
	
	lb_role = new wxStaticText( this, wxID_ANY, wxT("Roles"), wxDefaultPosition, wxDefaultSize, 0 );
	lb_role->Wrap( -1 );
	bSizer1->Add( lb_role, 0, wxALL, 5 );
	
	wxArrayString cl_roleChoices;
	cl_role = new wxCheckListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, cl_roleChoices, 0 );
	bSizer1->Add( cl_role, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	btn_ok = new wxButton( this, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( btn_ok, 0, wxALL, 5 );
	
	
	bSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	btn_cancel = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( btn_cancel, 0, wxALL, 5 );
	
	
	bSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	bSizer1->Add( bSizer3, 0, wxEXPAND, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	// Connect Events
	btn_ok->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_position_f::OnClose ), NULL, this );
	btn_cancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_position_f::OnEnd ), NULL, this );
}

base_position_f::~base_position_f()
{
	// Disconnect Events
	btn_ok->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_position_f::OnClose ), NULL, this );
	btn_cancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_position_f::OnEnd ), NULL, this );
}

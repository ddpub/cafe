///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "base_lm_f.h"

///////////////////////////////////////////////////////////////////////////

base_lm_f::base_lm_f( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	bmpSizer_ = new wxBoxSizer( wxVERTICAL );
	
	bmp_ = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bmpSizer_->Add( bmp_, 1, wxALL|wxEXPAND, 0 );
	
	this->SetSizer( bmpSizer_ );
	this->Layout();
	m_menubar = new wxMenuBar( 0 );
	m_file = new wxMenu();
	wxMenuItem* m_open;
	m_open = new wxMenuItem( m_file, wxID_ANY, wxString( wxT("Open") ) , wxEmptyString, wxITEM_NORMAL );
	m_file->Append( m_open );
	
	m_save_as = new wxMenuItem( m_file, wxID_ANY, wxString( wxT("Save As...") ) , wxEmptyString, wxITEM_NORMAL );
	m_file->Append( m_save_as );
	
	m_file->AppendSeparator();
	
	wxMenuItem* m_exit;
	m_exit = new wxMenuItem( m_file, wxID_ANY, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	m_file->Append( m_exit );
	
	m_menubar->Append( m_file, wxT("File") );
	
	m_tools = new wxMenu();
	m_register_logo = new wxMenuItem( m_tools, wxID_ANY, wxString( wxT("Send logo to printer") ) , wxEmptyString, wxITEM_NORMAL );
	m_tools->Append( m_register_logo );
	
	m_center = new wxMenuItem( m_tools, wxID_ANY, wxString( wxT("Center") ) , wxEmptyString, wxITEM_NORMAL );
	m_tools->Append( m_center );
	m_center->Enable( false );
	
	m_tools->AppendSeparator();
	
	wxMenuItem* m_options;
	m_options = new wxMenuItem( m_tools, wxID_ANY, wxString( wxT("Printer settings...") ) , wxEmptyString, wxITEM_NORMAL );
	m_tools->Append( m_options );
	
	m_menubar->Append( m_tools, wxT("Tools") );
	
	m_help = new wxMenu();
	wxMenuItem* m_menuItem4;
	m_menuItem4 = new wxMenuItem( m_help, wxID_ANY, wxString( wxT("About...") ) , wxEmptyString, wxITEM_NORMAL );
	m_help->Append( m_menuItem4 );
	
	m_menubar->Append( m_help, wxT("Help") );
	
	this->SetMenuBar( m_menubar );
	
	m_statusBar = this->CreateStatusBar( 3, 0, wxID_ANY );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	bmp_->Connect( wxEVT_PAINT, wxPaintEventHandler( base_lm_f::bmpOnPaint ), NULL, this );
	bmp_->Connect( wxEVT_SIZE, wxSizeEventHandler( base_lm_f::bmpOnSize ), NULL, this );
	this->Connect( m_open->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( base_lm_f::m_OpenClick ) );
	this->Connect( m_save_as->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( base_lm_f::m_save_asClick ) );
	this->Connect( m_save_as->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler( base_lm_f::m_save_asOnPopUp ) );
	this->Connect( m_exit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( base_lm_f::m_exitClick ) );
	this->Connect( m_register_logo->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( base_lm_f::m_register_logoClick ) );
	this->Connect( m_register_logo->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler( base_lm_f::m_register_logoOnPopUp ) );
	this->Connect( m_center->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( base_lm_f::m_centerClick ) );
	this->Connect( m_center->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler( base_lm_f::m_centerOnPopUp ) );
	this->Connect( m_options->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( base_lm_f::m_optionsClick ) );
	this->Connect( m_menuItem4->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( base_lm_f::m_aboutClick ) );
	m_statusBar->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( base_lm_f::statusBarDClick ), NULL, this );
	m_statusBar->Connect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( base_lm_f::statusBarUpdateUI ), NULL, this );
}

base_lm_f::~base_lm_f()
{
	// Disconnect Events
	bmp_->Disconnect( wxEVT_PAINT, wxPaintEventHandler( base_lm_f::bmpOnPaint ), NULL, this );
	bmp_->Disconnect( wxEVT_SIZE, wxSizeEventHandler( base_lm_f::bmpOnSize ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( base_lm_f::m_OpenClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( base_lm_f::m_save_asClick ) );
	this->Disconnect( wxID_ANY, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( base_lm_f::m_save_asOnPopUp ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( base_lm_f::m_exitClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( base_lm_f::m_register_logoClick ) );
	this->Disconnect( wxID_ANY, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( base_lm_f::m_register_logoOnPopUp ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( base_lm_f::m_centerClick ) );
	this->Disconnect( wxID_ANY, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( base_lm_f::m_centerOnPopUp ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( base_lm_f::m_optionsClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( base_lm_f::m_aboutClick ) );
	m_statusBar->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( base_lm_f::statusBarDClick ), NULL, this );
	m_statusBar->Disconnect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( base_lm_f::statusBarUpdateUI ), NULL, this );
}

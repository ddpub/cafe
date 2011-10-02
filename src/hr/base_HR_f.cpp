///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "base_HR_f.h"
#include "HR_ico.h"

///////////////////////////////////////////////////////////////////////////

base_HR_f::base_HR_f( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,-1 ), wxDefaultSize );
	
	wxBoxSizer* bSizer37;
	bSizer37 = new wxBoxSizer( wxVERTICAL );
	
	m_panel5 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	nb_HR = new wxNotebook( m_panel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	wnd_pers = new wxPanel( nb_HR, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer42;
	bSizer42 = new wxBoxSizer( wxVERTICAL );
	
	tb_person = new wxToolBar( wnd_pers, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL|wxTB_TEXT );
	tb_person->SetToolBitmapSize( wxSize( 30,30 ) );
	tb_person->AddTool( ADDPERS_ID, wxT("Hire"), HR_tool(4));
	tb_person->AddTool( EDITPERS_ID, wxT("Edit"), HR_tool(1));
	tb_person->AddTool( FIREPERS_ID, wxT("Fire"), HR_tool(5));
	tb_person->AddTool( REHIREPERS_ID, wxT("Rehire"), HR_tool(4));
	tb_person->AddTool( UNLOCKPERS_ID, wxT("Unlock"), HR_tool(3));
	tb_person->AddTool( DELPERS_ID, wxT("Delete"), HR_tool(0));
	tb_person->AddSeparator();
	
	ppp = new wxPanel(tb_person, wxID_ANY, wxDefaultPosition, wxSize(310, 50));
	
	ck_cafefilter = new wxCheckBox(ppp, wxID_ANY, wxT("Cafe filter"), wxPoint(0,5), wxSize(110, 20));
	ck_rolefilter = new wxCheckBox(ppp, wxID_ANY, wxT("Role filter"), wxPoint(0,30), wxSize(110, 20));
	ck_cafefilter->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(base_HR_f::OnCheckCafeFilter), NULL, this );
	ck_rolefilter->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(base_HR_f::OnCheckRoleFilter), NULL, this );
	
	ed_cafefilter = new wxTextCtrl(ppp, wxID_ANY, _T(""), wxPoint(110,2), wxSize(150, 20), wxTE_READONLY );
	ed_rolefilter = new wxTextCtrl(ppp, wxID_ANY, _T(""), wxPoint(110,27), wxSize(150, 20), wxTE_READONLY );
	btn_cafe = new wxButton(ed_cafefilter, wxID_ANY, _T("..."), wxPoint(130,0), wxSize(16,17));
	btn_role = new wxButton(ed_rolefilter, wxID_ANY, _T("..."), wxPoint(130,0), wxSize(16,17));
	btn_cafe->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(base_HR_f::OnTextCafeFilter), NULL, this );
	btn_role->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(base_HR_f::OnTextRoleFilter), NULL, this );
	
	tb_person->AddControl((wxControl*) ppp);
	
	tb_person->Realize();
	
	bSizer42->Add( tb_person, 0, wxALL|wxEXPAND, 1 );
	
	nb_person = new wxNotebook( wnd_pers, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_LEFT );
	t_active = new wxPanel( nb_person, wxID_ANY, wxDefaultPosition, wxSize( -1,300 ), 0, wxT("Active") );
	t_active->SetMaxSize( wxSize( -1,300 ) );
	
	wxGridSizer* gSizer19;
	gSizer19 = new wxGridSizer( 1, 1, 0, 0 );
	
	gr_pers_a = new wxGrid( t_active, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	
	// Grid
	gr_pers_a->CreateGrid( 1, 5 );
	gr_pers_a->EnableEditing( false );
	gr_pers_a->EnableGridLines( true );
	gr_pers_a->EnableDragGridSize( false );
	gr_pers_a->SetMargins( 0, 0 );
	
	// Columns
	gr_pers_a->SetColSize( 0, 127 );
	gr_pers_a->SetColSize( 1, 108 );
	gr_pers_a->SetColSize( 2, 135 );
	gr_pers_a->SetColSize( 3, 75 );
	gr_pers_a->SetColSize( 4, 119 );
	gr_pers_a->EnableDragColMove( false );
	gr_pers_a->EnableDragColSize( true );
	gr_pers_a->SetColLabelSize( 20 );
	gr_pers_a->SetColLabelValue( 0, wxT("login") );
	gr_pers_a->SetColLabelValue( 1, wxT("firstname") );
	gr_pers_a->SetColLabelValue( 2, wxT("lastname") );
	gr_pers_a->SetColLabelValue( 3, wxT("surname") );
	gr_pers_a->SetColLabelValue( 4, wxT("birthday") );
	gr_pers_a->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	gr_pers_a->EnableDragRowSize( true );
	gr_pers_a->SetRowLabelSize( 1 );
	gr_pers_a->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Label Appearance
	
	// Cell Defaults
	gr_pers_a->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	gSizer19->Add( gr_pers_a, 0, wxALL|wxEXPAND, 2 );
	
	t_active->SetSizer( gSizer19 );
	t_active->Layout();
	nb_person->AddPage( t_active, wxT("Active"), true );
	t_fired = new wxPanel( nb_person, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	wxGridSizer* gSizer191;
	gSizer191 = new wxGridSizer( 1, 1, 0, 0 );
	
	gr_pers_f = new wxGrid( t_fired, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	
	// Grid
	gr_pers_f->CreateGrid( 1, 5 );
	gr_pers_f->EnableEditing( false );
	gr_pers_f->EnableGridLines( true );
	gr_pers_f->EnableDragGridSize( false );
	gr_pers_f->SetMargins( 0, 0 );
	
	// Columns
	gr_pers_f->SetColSize( 0, 110 );
	gr_pers_f->SetColSize( 1, 114 );
	gr_pers_f->SetColSize( 2, 110 );
	gr_pers_f->SetColSize( 3, 112 );
	gr_pers_f->SetColSize( 4, 80 );
	gr_pers_f->EnableDragColMove( false );
	gr_pers_f->EnableDragColSize( true );
	gr_pers_f->SetColLabelSize( 20 );
	gr_pers_f->SetColLabelValue( 0, wxT("login") );
	gr_pers_f->SetColLabelValue( 1, wxT("firstname") );
	gr_pers_f->SetColLabelValue( 2, wxT("lastname") );
	gr_pers_f->SetColLabelValue( 3, wxT("surname") );
	gr_pers_f->SetColLabelValue( 4, wxT("birthday") );
	gr_pers_f->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	gr_pers_f->EnableDragRowSize( true );
	gr_pers_f->SetRowLabelSize( 1 );
	gr_pers_f->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Label Appearance
	
	// Cell Defaults
	gr_pers_f->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_CENTRE );
	gSizer191->Add( gr_pers_f, 0, wxALL|wxEXPAND, 2 );
	
	t_fired->SetSizer( gSizer191 );
	t_fired->Layout();
	gSizer191->Fit( t_fired );
	nb_person->AddPage( t_fired, wxT("Fired"), false );
	
	bSizer42->Add( nb_person, 1, wxEXPAND | wxALL, 2 );
	
	wnd_pers->SetSizer( bSizer42 );
	wnd_pers->Layout();
	bSizer42->Fit( wnd_pers );
	nb_HR->AddPage( wnd_pers, wxT("Personnel"), true );
	wnd_pos = new wxPanel( nb_HR, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer53;
	bSizer53 = new wxBoxSizer( wxVERTICAL );
	
	tb_position = new wxToolBar( wnd_pos, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL|wxTB_TEXT );
	tb_position->SetToolBitmapSize( wxSize( 27,27 ) );
	tb_position->AddTool( ADDPOS_ID, wxT("Add"), HR_tool(2));
	tb_position->AddTool( EDITPOS_ID, wxT("Edit"), HR_tool(1));
	tb_position->AddTool( DELPOS_ID, wxT("Delete"), HR_tool(0));
	tb_position->Realize();
	
	bSizer53->Add( tb_position, 0, wxALL|wxEXPAND, 1 );
	
	wxGridSizer* gSizer24;
	gSizer24 = new wxGridSizer( 0, 2, 0, 0 );
	
	lb_position = new wxListBox( wnd_pos, wxID_ANY, wxDefaultPosition, wxSize( -1,370 ), 0, NULL, 0 ); 
	gSizer24->Add( lb_position, 1, wxALL|wxEXPAND, 2 );
	
	wxArrayString cl_roleChoices;
	cl_role = new wxCheckListBox( wnd_pos, wxID_ANY, wxDefaultPosition, wxDefaultSize, cl_roleChoices, 0 );
	cl_role->Enable( false );
	
	gSizer24->Add( cl_role, 1, wxALL|wxEXPAND, 2 );
	
	bSizer53->Add( gSizer24, 1, wxEXPAND, 5 );
	
	wnd_pos->SetSizer( bSizer53 );
	wnd_pos->Layout();
	bSizer53->Fit( wnd_pos );
	nb_HR->AddPage( wnd_pos, wxT("Positions"), false );
	
	bSizer4->Add( nb_HR, 1, wxALL|wxEXPAND, 2 );
	
	st_bar = new wxStaticText( m_panel5, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	st_bar->Wrap( -1 );
	bSizer4->Add( st_bar, 0, wxALIGN_BOTTOM|wxALL|wxEXPAND, 1 );
	
	m_panel5->SetSizer( bSizer4 );
	m_panel5->Layout();
	bSizer4->Fit( m_panel5 );
	bSizer37->Add( m_panel5, 1, wxEXPAND | wxALL, 0 );
	
	this->SetSizer( bSizer37 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( base_HR_f::OnResize ) );
	this->Connect( ADDPERS_ID, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( base_HR_f::OnAdd ) );
	this->Connect( EDITPERS_ID, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( base_HR_f::OnEdit ) );
	this->Connect( FIREPERS_ID, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( base_HR_f::OnFire ) );
	this->Connect( REHIREPERS_ID, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( base_HR_f::OnRehire ) );
	this->Connect( UNLOCKPERS_ID, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( base_HR_f::OnUnlock ) );
	this->Connect( DELPERS_ID, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( base_HR_f::OnDelete ) );
	nb_person->Connect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( base_HR_f::OnChangeActive ), NULL, this );
	gr_pers_a->Connect( wxEVT_GRID_CELL_LEFT_CLICK, wxGridEventHandler( base_HR_f::OnGridClick ), NULL, this );
	gr_pers_a->Connect( wxEVT_GRID_CELL_LEFT_DCLICK, wxGridEventHandler( base_HR_f::OnGridDblClick ), NULL, this );
	gr_pers_a->Connect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( base_HR_f::OnPopupMenuClick ), NULL, this );
	gr_pers_a->Connect( wxEVT_GRID_LABEL_LEFT_CLICK, wxGridEventHandler( base_HR_f::OnSort ), NULL, this );
	gr_pers_f->Connect( wxEVT_GRID_CELL_LEFT_CLICK, wxGridEventHandler( base_HR_f::OnGridClick ), NULL, this );
	gr_pers_f->Connect( wxEVT_GRID_CELL_LEFT_DCLICK, wxGridEventHandler( base_HR_f::OnGridDblClick ), NULL, this );
	gr_pers_f->Connect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( base_HR_f::OnPopupMenuClick ), NULL, this );
	gr_pers_f->Connect( wxEVT_GRID_LABEL_LEFT_CLICK, wxGridEventHandler( base_HR_f::OnSort ), NULL, this );
	this->Connect( ADDPOS_ID, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( base_HR_f::OnAddpos ) );
	this->Connect( EDITPOS_ID, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( base_HR_f::OnEditpos ) );
	this->Connect( DELPOS_ID, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( base_HR_f::OnDelpos ) );
	lb_position->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( base_HR_f::OnClickPos ), NULL, this );
	lb_position->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( base_HR_f::OnEditpos ), NULL, this );
	st_bar->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( base_HR_f::OnPropertyClick ), NULL, this );
}

base_HR_f::~base_HR_f()
{
	// Disconnect Events
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( base_HR_f::OnResize ) );
	this->Disconnect( ADDPERS_ID, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( base_HR_f::OnAdd ) );
	this->Disconnect( EDITPERS_ID, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( base_HR_f::OnEdit ) );
	this->Disconnect( FIREPERS_ID, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( base_HR_f::OnFire ) );
	this->Disconnect( REHIREPERS_ID, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( base_HR_f::OnRehire ) );
	this->Disconnect( UNLOCKPERS_ID, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( base_HR_f::OnUnlock ) );
	this->Disconnect( DELPERS_ID, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( base_HR_f::OnDelete ) );
	nb_person->Disconnect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( base_HR_f::OnChangeActive ), NULL, this );
	gr_pers_a->Disconnect( wxEVT_GRID_CELL_LEFT_CLICK, wxGridEventHandler( base_HR_f::OnGridClick ), NULL, this );
	gr_pers_a->Disconnect( wxEVT_GRID_CELL_LEFT_DCLICK, wxGridEventHandler( base_HR_f::OnGridDblClick ), NULL, this );
	gr_pers_a->Disconnect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( base_HR_f::OnPopupMenuClick ), NULL, this );
	gr_pers_a->Disconnect( wxEVT_GRID_LABEL_LEFT_CLICK, wxGridEventHandler( base_HR_f::OnSort ), NULL, this );
	gr_pers_f->Disconnect( wxEVT_GRID_CELL_LEFT_CLICK, wxGridEventHandler( base_HR_f::OnGridClick ), NULL, this );
	gr_pers_f->Disconnect( wxEVT_GRID_CELL_LEFT_DCLICK, wxGridEventHandler( base_HR_f::OnGridDblClick ), NULL, this );
	gr_pers_f->Disconnect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( base_HR_f::OnPopupMenuClick ), NULL, this );
	gr_pers_f->Disconnect( wxEVT_GRID_LABEL_LEFT_CLICK, wxGridEventHandler( base_HR_f::OnSort ), NULL, this );
	this->Disconnect( ADDPOS_ID, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( base_HR_f::OnAddpos ) );
	this->Disconnect( EDITPOS_ID, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( base_HR_f::OnEditpos ) );
	this->Disconnect( DELPOS_ID, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( base_HR_f::OnDelpos ) );
	lb_position->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( base_HR_f::OnClickPos ), NULL, this );
	lb_position->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( base_HR_f::OnEditpos ), NULL, this );
	st_bar->Disconnect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( base_HR_f::OnPropertyClick ), NULL, this );
	
	ck_cafefilter->Disconnect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(base_HR_f::OnCheckCafeFilter), NULL, this );
	ck_rolefilter->Disconnect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(base_HR_f::OnCheckRoleFilter), NULL, this );
	btn_cafe->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(base_HR_f::OnTextCafeFilter), NULL, this );
	btn_role->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(base_HR_f::OnTextRoleFilter), NULL, this );
}

///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "base_text_f.h"

///////////////////////////////////////////////////////////////////////////

base_text_f::base_text_f( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	main_bSizer = new wxBoxSizer( wxVERTICAL );
	
	group_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	text_sizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxEmptyString ), wxVERTICAL );
	
	text_ed = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,100 ), wxTE_LEFT|wxTE_MULTILINE|wxTE_PROCESS_ENTER|wxTE_READONLY|wxWANTS_CHARS );
	text_ed->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	text_sizer->Add( text_ed, 1, wxEXPAND, 0 );
	
	group_sizer->Add( text_sizer, 1, wxEXPAND, 5 );
	
	amount_sizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Amount") ), wxHORIZONTAL );
	
	amount_lb = new wxStaticText( this, wxID_ANY, wxT("Amount:"), wxDefaultPosition, wxDefaultSize, 0 );
	amount_lb->Wrap( -1 );
	amount_lb->SetFont( wxFont( 14, 74, 90, 90, false, wxT("Tahoma") ) );
	
	amount_sizer->Add( amount_lb, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	amount_ed = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 120,-1 ), wxTE_READONLY|wxTE_RIGHT|wxWANTS_CHARS );
	amount_ed->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	amount_sizer->Add( amount_ed, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	group_sizer->Add( amount_sizer, 0, wxEXPAND, 5 );
	
	main_bSizer->Add( group_sizer, 0, wxEXPAND, 0 );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer2->Add( 25, 0, 0, wxEXPAND, 5 );
	
	m_button1 = new wxButton( this, wxID_ANY, wxT("1"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_button1->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer2->Add( m_button1, 0, 0, 5 );
	
	m_button2 = new wxButton( this, wxID_ANY, wxT("2"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_button2->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer2->Add( m_button2, 0, 0, 5 );
	
	m_button3 = new wxButton( this, wxID_ANY, wxT("3"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_button3->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer2->Add( m_button3, 0, 0, 5 );
	
	m_button4 = new wxButton( this, wxID_ANY, wxT("4"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_button4->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer2->Add( m_button4, 0, 0, 5 );
	
	m_button5 = new wxButton( this, wxID_ANY, wxT("5"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_button5->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer2->Add( m_button5, 0, 0, 5 );
	
	m_button6 = new wxButton( this, wxID_ANY, wxT("6"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_button6->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer2->Add( m_button6, 0, 0, 5 );
	
	m_button7 = new wxButton( this, wxID_ANY, wxT("7"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_button7->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer2->Add( m_button7, 0, 0, 5 );
	
	m_button8 = new wxButton( this, wxID_ANY, wxT("8"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_button8->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer2->Add( m_button8, 0, 0, 5 );
	
	m_button9 = new wxButton( this, wxID_ANY, wxT("9"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_button9->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer2->Add( m_button9, 0, 0, 5 );
	
	m_button0 = new wxButton( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_button0->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer2->Add( m_button0, 0, 0, 5 );
	
	m_button10 = new wxButton( this, wxID_ANY, wxT("-"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_button10->SetFont( wxFont( 14, 74, 93, 90, false, wxT("Tahoma") ) );
	
	bSizer2->Add( m_button10, 0, 0, 5 );
	
	m_button11 = new wxButton( this, wxID_ANY, wxT("="), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_button11->SetFont( wxFont( 14, 74, 93, 90, false, wxT("Tahoma") ) );
	
	bSizer2->Add( m_button11, 0, 0, 5 );
	
	bs_btn = new wxButton( this, wxID_ANY, wxT("<-"), wxDefaultPosition, wxSize( 100,50 ), 0 );
	bs_btn->SetFont( wxFont( 14, 74, 93, 90, false, wxT("Tahoma") ) );
	
	bSizer2->Add( bs_btn, 0, 0, 5 );
	
	main_bSizer->Add( bSizer2, 0, 0, 0 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer3->Add( 50, 0, 0, wxEXPAND, 0 );
	
	m_buttonq = new wxButton( this, wxID_ANY, wxT("q"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_buttonq->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer3->Add( m_buttonq, 0, 0, 5 );
	
	m_buttonw = new wxButton( this, wxID_ANY, wxT("w"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_buttonw->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer3->Add( m_buttonw, 0, 0, 5 );
	
	m_buttone = new wxButton( this, wxID_ANY, wxT("e"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_buttone->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer3->Add( m_buttone, 0, 0, 5 );
	
	m_buttonr = new wxButton( this, wxID_ANY, wxT("r"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_buttonr->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer3->Add( m_buttonr, 0, 0, 5 );
	
	m_buttont = new wxButton( this, wxID_ANY, wxT("t"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_buttont->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer3->Add( m_buttont, 0, 0, 5 );
	
	m_buttony = new wxButton( this, wxID_ANY, wxT("y"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_buttony->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer3->Add( m_buttony, 0, 0, 5 );
	
	m_buttonu = new wxButton( this, wxID_ANY, wxT("u"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_buttonu->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer3->Add( m_buttonu, 0, 0, 5 );
	
	m_buttoni = new wxButton( this, wxID_ANY, wxT("i"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_buttoni->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer3->Add( m_buttoni, 0, 0, 5 );
	
	m_buttono = new wxButton( this, wxID_ANY, wxT("o"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_buttono->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer3->Add( m_buttono, 0, 0, 5 );
	
	m_buttonp = new wxButton( this, wxID_ANY, wxT("p"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_buttonp->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer3->Add( m_buttonp, 0, 0, 5 );
	
	m_button12 = new wxButton( this, wxID_ANY, wxT("["), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_button12->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer3->Add( m_button12, 0, 0, 5 );
	
	m_button13 = new wxButton( this, wxID_ANY, wxT("]"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_button13->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer3->Add( m_button13, 0, 0, 5 );
	
	enter_btn = new wxButton( this, wxID_ANY, wxT("Enter"), wxDefaultPosition, wxSize( 75,50 ), 0 );
	enter_btn->SetFont( wxFont( 14, 74, 90, 92, false, wxT("Tahoma") ) );
	
	bSizer3->Add( enter_btn, 0, 0, 0 );
	
	main_bSizer->Add( bSizer3, 0, 0, 0 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer4->Add( 75, 0, 0, wxEXPAND, 5 );
	
	m_buttona = new wxButton( this, wxID_ANY, wxT("a"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_buttona->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer4->Add( m_buttona, 0, 0, 5 );
	
	m_buttons = new wxButton( this, wxID_ANY, wxT("s"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_buttons->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer4->Add( m_buttons, 0, 0, 5 );
	
	m_buttond = new wxButton( this, wxID_ANY, wxT("d"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_buttond->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer4->Add( m_buttond, 0, 0, 5 );
	
	m_buttonf = new wxButton( this, wxID_ANY, wxT("f"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_buttonf->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer4->Add( m_buttonf, 0, 0, 5 );
	
	m_buttong = new wxButton( this, wxID_ANY, wxT("g"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_buttong->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer4->Add( m_buttong, 0, 0, 5 );
	
	m_buttonh = new wxButton( this, wxID_ANY, wxT("h"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_buttonh->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer4->Add( m_buttonh, 0, 0, 5 );
	
	m_buttonj = new wxButton( this, wxID_ANY, wxT("j"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_buttonj->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer4->Add( m_buttonj, 0, 0, 5 );
	
	m_buttonk = new wxButton( this, wxID_ANY, wxT("k"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_buttonk->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer4->Add( m_buttonk, 0, 0, 5 );
	
	m_buttonl = new wxButton( this, wxID_ANY, wxT("l"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_buttonl->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer4->Add( m_buttonl, 0, 0, 5 );
	
	m_button14 = new wxButton( this, wxID_ANY, wxT(";"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_button14->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer4->Add( m_button14, 0, 0, 5 );
	
	m_button15 = new wxButton( this, wxID_ANY, wxT("'"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_button15->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer4->Add( m_button15, 0, 0, 5 );
	
	m_button16 = new wxButton( this, wxID_ANY, wxT("\\"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_button16->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer4->Add( m_button16, 0, 0, 5 );
	
	main_bSizer->Add( bSizer4, 0, 0, 0 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	shift1_btn = new wxToggleButton( this, wxID_ANY, wxT("Shift"), wxDefaultPosition, wxSize( 100,50 ), 0 );
	shift1_btn->SetFont( wxFont( 14, 74, 90, 92, false, wxT("Tahoma") ) );
	
	bSizer5->Add( shift1_btn, 0, 0, 0 );
	
	m_buttonz = new wxButton( this, wxID_ANY, wxT("z"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_buttonz->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer5->Add( m_buttonz, 0, 0, 5 );
	
	m_buttonx = new wxButton( this, wxID_ANY, wxT("x"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_buttonx->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer5->Add( m_buttonx, 0, 0, 5 );
	
	m_buttonc = new wxButton( this, wxID_ANY, wxT("c"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_buttonc->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer5->Add( m_buttonc, 0, 0, 5 );
	
	m_buttonv = new wxButton( this, wxID_ANY, wxT("v"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_buttonv->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer5->Add( m_buttonv, 0, 0, 5 );
	
	m_buttonb = new wxButton( this, wxID_ANY, wxT("b"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_buttonb->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer5->Add( m_buttonb, 0, 0, 5 );
	
	m_buttonn = new wxButton( this, wxID_ANY, wxT("n"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_buttonn->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer5->Add( m_buttonn, 0, 0, 5 );
	
	m_buttonm = new wxButton( this, wxID_ANY, wxT("m"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_buttonm->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer5->Add( m_buttonm, 0, 0, 5 );
	
	m_button17 = new wxButton( this, wxID_ANY, wxT(","), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_button17->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer5->Add( m_button17, 0, 0, 5 );
	
	m_button18 = new wxButton( this, wxID_ANY, wxT("."), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_button18->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer5->Add( m_button18, 0, 0, 5 );
	
	m_button19 = new wxButton( this, wxID_ANY, wxT("/"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	m_button19->SetFont( wxFont( 18, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer5->Add( m_button19, 0, 0, 5 );
	
	shift2_btn = new wxToggleButton( this, wxID_ANY, wxT("Shift"), wxDefaultPosition, wxSize( 125,50 ), 0 );
	shift2_btn->SetFont( wxFont( 14, 74, 90, 92, false, wxT("Tahoma") ) );
	
	bSizer5->Add( shift2_btn, 0, 0, 0 );
	
	main_bSizer->Add( bSizer5, 0, 0, 0 );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	kbd_btn = new wxButton( this, wxID_ANY, wxT("US"), wxDefaultPosition, wxSize( 50,50 ), 0 );
	kbd_btn->SetFont( wxFont( 14, 74, 93, 90, false, wxT("Tahoma") ) );
	
	bSizer6->Add( kbd_btn, 0, 0, 5 );
	
	
	bSizer6->Add( 125, 0, 0, wxEXPAND, 5 );
	
	space_btn = new wxButton( this, wxID_ANY, wxT("Space"), wxDefaultPosition, wxSize( 300,50 ), 0 );
	space_btn->SetFont( wxFont( 14, 74, 90, 90, false, wxT("Tahoma") ) );
	
	bSizer6->Add( space_btn, 0, 0, 5 );
	
	
	bSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	main_bSizer->Add( bSizer6, 0, 0, 0 );
	
	bottom_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	
	bottom_sizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	ok_btn = new wxButton( this, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	ok_btn->SetDefault(); 
	ok_btn->SetBackgroundColour( wxColour( 204, 255, 204 ) );
	
	bottom_sizer->Add( ok_btn, 0, 0, 5 );
	
	
	bottom_sizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	cancel_btn = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxSize( 150,40 ), 0 );
	cancel_btn->SetBackgroundColour( wxColour( 255, 204, 204 ) );
	
	bottom_sizer->Add( cancel_btn, 0, 0, 5 );
	
	
	bottom_sizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	main_bSizer->Add( bottom_sizer, 0, wxEXPAND, 0 );
	
	this->SetSizer( main_bSizer );
	this->Layout();
	main_bSizer->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	text_ed->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( base_text_f::text_edFocus ), NULL, this );
	amount_ed->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( base_text_f::amount_edFocus ), NULL, this );
	m_button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button4->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button5->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button6->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button7->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button8->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button9->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button0->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button10->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button11->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	bs_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::bs_btnClick ), NULL, this );
	m_buttonq->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttonw->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttone->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttonr->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttont->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttony->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttonu->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttoni->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttono->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttonp->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button12->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button13->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	enter_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::enter_btnClick ), NULL, this );
	m_buttona->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttons->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttond->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttonf->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttong->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttonh->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttonj->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttonk->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttonl->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button14->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button15->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button16->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	shift1_btn->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( base_text_f::shift_btnClick ), NULL, this );
	m_buttonz->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttonx->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttonc->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttonv->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttonb->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttonn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttonm->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button17->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button18->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button19->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	shift2_btn->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( base_text_f::shift_btnClick ), NULL, this );
	kbd_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::kbd_btnClick ), NULL, this );
	space_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::space_btnClick ), NULL, this );
	ok_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::ok_btnClick ), NULL, this );
	cancel_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::cancel_btnClick ), NULL, this );
}

base_text_f::~base_text_f()
{
	// Disconnect Events
	text_ed->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( base_text_f::text_edFocus ), NULL, this );
	amount_ed->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( base_text_f::amount_edFocus ), NULL, this );
	m_button1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button3->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button4->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button5->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button6->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button7->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button8->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button9->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button0->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button10->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button11->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	bs_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::bs_btnClick ), NULL, this );
	m_buttonq->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttonw->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttone->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttonr->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttont->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttony->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttonu->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttoni->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttono->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttonp->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button12->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button13->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	enter_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::enter_btnClick ), NULL, this );
	m_buttona->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttons->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttond->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttonf->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttong->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttonh->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttonj->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttonk->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttonl->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button14->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button15->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button16->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	shift1_btn->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( base_text_f::shift_btnClick ), NULL, this );
	m_buttonz->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttonx->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttonc->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttonv->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttonb->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttonn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_buttonm->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button17->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button18->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	m_button19->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::btnClick ), NULL, this );
	shift2_btn->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( base_text_f::shift_btnClick ), NULL, this );
	kbd_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::kbd_btnClick ), NULL, this );
	space_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::space_btnClick ), NULL, this );
	ok_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::ok_btnClick ), NULL, this );
	cancel_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_text_f::cancel_btnClick ), NULL, this );
}

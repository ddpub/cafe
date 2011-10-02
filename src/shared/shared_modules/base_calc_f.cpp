///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "base_calc_f.h"

///////////////////////////////////////////////////////////////////////////

base_calc_f::base_calc_f( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	main_sizer = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 3, 2, 0, 0 );
	
	cash_tendered_lb = new wxStaticText( this, wxID_ANY, wxT("Cash tendered"), wxDefaultPosition, wxDefaultSize, 0 );
	cash_tendered_lb->Wrap( -1 );
	cash_tendered_lb->SetFont( wxFont( 14, 74, 90, 92, false, wxT("Arial Narrow") ) );
	
	gSizer1->Add( cash_tendered_lb, 0, wxALL, 5 );
	
	cash_tendered_ed = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 120,-1 ), wxTE_RIGHT|wxTAB_TRAVERSAL|wxWANTS_CHARS );
	cash_tendered_ed->SetFont( wxFont( 18, 74, 90, 92, false, wxT("Arial Narrow") ) );
	
	gSizer1->Add( cash_tendered_ed, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	order_total_lb = new wxStaticText( this, wxID_ANY, wxT("Order total"), wxDefaultPosition, wxDefaultSize, 0 );
	order_total_lb->Wrap( -1 );
	order_total_lb->SetFont( wxFont( 14, 74, 90, 92, false, wxT("Arial Narrow") ) );
	
	gSizer1->Add( order_total_lb, 0, wxALL, 5 );
	
	order_total_ed = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 120,-1 ), wxTE_READONLY|wxTE_RIGHT|wxTAB_TRAVERSAL|wxWANTS_CHARS );
	order_total_ed->SetFont( wxFont( 18, 74, 90, 92, false, wxT("Arial Narrow") ) );
	order_total_ed->Enable( false );
	
	gSizer1->Add( order_total_ed, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	change_lb = new wxStaticText( this, wxID_ANY, wxT("Change"), wxDefaultPosition, wxDefaultSize, 0 );
	change_lb->Wrap( -1 );
	change_lb->SetFont( wxFont( 14, 74, 90, 92, false, wxT("Arial Narrow") ) );
	
	gSizer1->Add( change_lb, 0, wxALL, 5 );
	
	change_ed = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 120,-1 ), wxTE_READONLY|wxTE_RIGHT|wxTAB_TRAVERSAL|wxWANTS_CHARS );
	change_ed->SetFont( wxFont( 18, 74, 90, 92, false, wxT("Arial Narrow") ) );
	change_ed->Enable( false );
	
	gSizer1->Add( change_ed, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	bSizer2->Add( gSizer1, 1, wxEXPAND, 0 );
	
	exact_amount_btn = new wxButton( this, wxID_ANY, wxT("Exact amount"), wxDefaultPosition, wxSize( 200,50 ), 0|wxWANTS_CHARS );
	exact_amount_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	
	bSizer2->Add( exact_amount_btn, 0, wxALIGN_CENTER|wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	
	bSizer2->Add( 0, 10, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	process_btn = new wxButton( this, wxID_ANY, wxT("Commit processing"), wxDefaultPosition, wxSize( -1,50 ), 0|wxWANTS_CHARS );
	process_btn->SetFont( wxFont( 13, 74, 90, 92, false, wxT("Arial Narrow") ) );
	process_btn->SetBackgroundColour( wxColour( 204, 255, 204 ) );
	process_btn->Enable( false );
	
	bSizer6->Add( process_btn, 2, wxALIGN_BOTTOM|wxALL, 5 );
	
	
	bSizer6->Add( 0, 0, 0, wxEXPAND, 5 );
	
	cancel_btn = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxSize( -1,50 ), 0|wxWANTS_CHARS );
	cancel_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	cancel_btn->SetBackgroundColour( wxColour( 255, 204, 204 ) );
	
	bSizer6->Add( cancel_btn, 1, wxALIGN_BOTTOM|wxALL, 5 );
	
	bSizer2->Add( bSizer6, 0, 0, 5 );
	
	main_sizer->Add( bSizer2, 0, wxEXPAND, 0 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	wxGridBagSizer* gbSizer1;
	gbSizer1 = new wxGridBagSizer( 0, 0 );
	gbSizer1->SetFlexibleDirection( wxBOTH );
	gbSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	num7_btn = new wxButton( this, wxID_ANY, wxT("7"), wxDefaultPosition, wxSize( 80,50 ), 0|wxWANTS_CHARS );
	num7_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	gbSizer1->Add( num7_btn, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 2 );
	
	num8_btn = new wxButton( this, wxID_ANY, wxT("8"), wxDefaultPosition, wxSize( 80,50 ), 0|wxWANTS_CHARS );
	num8_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	gbSizer1->Add( num8_btn, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 2 );
	
	num9_btn = new wxButton( this, wxID_ANY, wxT("9"), wxDefaultPosition, wxSize( 80,50 ), 0|wxWANTS_CHARS );
	num9_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	gbSizer1->Add( num9_btn, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 2 );
	
	num4_btn = new wxButton( this, wxID_ANY, wxT("4"), wxDefaultPosition, wxSize( 80,50 ), 0|wxWANTS_CHARS );
	num4_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	gbSizer1->Add( num4_btn, wxGBPosition( 1, 0 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 2 );
	
	num5_btn = new wxButton( this, wxID_ANY, wxT("5"), wxDefaultPosition, wxSize( 80,50 ), 0|wxWANTS_CHARS );
	num5_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	gbSizer1->Add( num5_btn, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 2 );
	
	num6_btn = new wxButton( this, wxID_ANY, wxT("6"), wxDefaultPosition, wxSize( 80,50 ), 0|wxWANTS_CHARS );
	num6_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	gbSizer1->Add( num6_btn, wxGBPosition( 1, 2 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 2 );
	
	num1_btn = new wxButton( this, wxID_ANY, wxT("1"), wxDefaultPosition, wxSize( 80,50 ), 0|wxWANTS_CHARS );
	num1_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	gbSizer1->Add( num1_btn, wxGBPosition( 2, 0 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 2 );
	
	num2_btn = new wxButton( this, wxID_ANY, wxT("2"), wxDefaultPosition, wxSize( 80,50 ), 0|wxWANTS_CHARS );
	num2_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	gbSizer1->Add( num2_btn, wxGBPosition( 2, 1 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 2 );
	
	 num3_btn = new wxButton( this, wxID_ANY, wxT("3"), wxDefaultPosition, wxSize( 80,50 ), 0|wxWANTS_CHARS );
	num3_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	gbSizer1->Add(  num3_btn, wxGBPosition( 2, 2 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 2 );
	
	num0_btn = new wxButton( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( -1,50 ), 0|wxWANTS_CHARS );
	num0_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	gbSizer1->Add( num0_btn, wxGBPosition( 3, 0 ), wxGBSpan( 1, 2 ), wxALL|wxEXPAND, 2 );
	
	num00_btn = new wxButton( this, wxID_ANY, wxT("00"), wxDefaultPosition, wxSize( 80,50 ), 0|wxWANTS_CHARS );
	num00_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	gbSizer1->Add( num00_btn, wxGBPosition( 3, 2 ), wxGBSpan( 1, 1 ), wxALL|wxEXPAND, 2 );
	
	clear_btn = new wxButton( this, wxID_ANY, wxT("Clear"), wxDefaultPosition, wxSize( 100,-1 ), 0|wxWANTS_CHARS );
	clear_btn->SetFont( wxFont( 12, 74, 90, 90, false, wxT("Arial Narrow") ) );
	clear_btn->SetMinSize( wxSize( 100,50 ) );
	
	gbSizer1->Add( clear_btn, wxGBPosition( 4, 1 ), wxGBSpan( 1, 1 ), wxALL, 2 );
	
	speed_num100_btn = new wxButton( this, wxID_ANY, wxT("$100"), wxDefaultPosition, wxSize( 80,50 ), 0|wxWANTS_CHARS );
	speed_num100_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	gbSizer1->Add( speed_num100_btn, wxGBPosition( 0, 4 ), wxGBSpan( 1, 1 ), wxALL, 2 );
	
	speed_num50_btn = new wxButton( this, wxID_ANY, wxT("$50"), wxDefaultPosition, wxSize( 80,50 ), 0|wxWANTS_CHARS );
	speed_num50_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	gbSizer1->Add( speed_num50_btn, wxGBPosition( 1, 4 ), wxGBSpan( 1, 1 ), wxALL, 2 );
	
	speed_num20_btn = new wxButton( this, wxID_ANY, wxT("$20"), wxDefaultPosition, wxSize( 80,50 ), 0|wxWANTS_CHARS );
	speed_num20_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	gbSizer1->Add( speed_num20_btn, wxGBPosition( 2, 4 ), wxGBSpan( 1, 1 ), wxALL, 2 );
	
	speed_num10_btn = new wxButton( this, wxID_ANY, wxT("$10"), wxDefaultPosition, wxSize( 80,50 ), 0|wxWANTS_CHARS );
	speed_num10_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	gbSizer1->Add( speed_num10_btn, wxGBPosition( 3, 4 ), wxGBSpan( 1, 1 ), wxALL, 2 );
	
	speed_num5_btn = new wxButton( this, wxID_ANY, wxT("$5"), wxDefaultPosition, wxSize( 80,50 ), 0|wxWANTS_CHARS );
	speed_num5_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	gbSizer1->Add( speed_num5_btn, wxGBPosition( 4, 4 ), wxGBSpan( 1, 1 ), wxALL, 2 );
	
	speed_num1_btn = new wxButton( this, wxID_ANY, wxT("$1"), wxDefaultPosition, wxSize( 80,50 ), 0|wxWANTS_CHARS );
	speed_num1_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	gbSizer1->Add( speed_num1_btn, wxGBPosition( 0, 5 ), wxGBSpan( 1, 1 ), wxALL, 2 );
	
	speed_num05_btn = new wxButton( this, wxID_ANY, wxT("¢50"), wxDefaultPosition, wxSize( 80,50 ), 0|wxWANTS_CHARS );
	speed_num05_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	gbSizer1->Add( speed_num05_btn, wxGBPosition( 1, 5 ), wxGBSpan( 1, 1 ), wxALL, 2 );
	
	speed_num005_btn = new wxButton( this, wxID_ANY, wxT("¢5"), wxDefaultPosition, wxSize( 80,50 ), 0|wxWANTS_CHARS );
	speed_num005_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	gbSizer1->Add( speed_num005_btn, wxGBPosition( 4, 5 ), wxGBSpan( 1, 1 ), wxALL, 2 );
	
	speed_num025_btn = new wxButton( this, wxID_ANY, wxT("¢25"), wxDefaultPosition, wxSize( 80,50 ), 0|wxWANTS_CHARS );
	speed_num025_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	gbSizer1->Add( speed_num025_btn, wxGBPosition( 2, 5 ), wxGBSpan( 1, 1 ), wxALL, 2 );
	
	speed_num01_btn = new wxButton( this, wxID_ANY, wxT("¢10"), wxDefaultPosition, wxSize( 80,50 ), 0|wxWANTS_CHARS );
	speed_num01_btn->SetFont( wxFont( 16, 74, 90, 90, false, wxT("Tahoma") ) );
	
	gbSizer1->Add( speed_num01_btn, wxGBPosition( 3, 5 ), wxGBSpan( 1, 1 ), wxALL, 2 );
	
	bSizer3->Add( gbSizer1, 1, wxEXPAND, 0 );
	
	main_sizer->Add( bSizer3, 0, wxEXPAND, 0 );
	
	this->SetSizer( main_sizer );
	this->Layout();
	main_sizer->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ) );
	cash_tendered_ed->Connect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	cash_tendered_ed->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( base_calc_f::cash_tendered_edChange ), NULL, this );
	exact_amount_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::exact_amount_btnClick ), NULL, this );
	exact_amount_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	process_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::process_btnClick ), NULL, this );
	process_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	cancel_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::cancel_btnClick ), NULL, this );
	cancel_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	num7_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::num_btnClick ), NULL, this );
	num7_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	num8_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::num_btnClick ), NULL, this );
	num8_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	num9_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::num_btnClick ), NULL, this );
	num9_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	num4_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::num_btnClick ), NULL, this );
	num4_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	num5_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::num_btnClick ), NULL, this );
	num5_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	num6_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::num_btnClick ), NULL, this );
	num6_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	num1_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::num_btnClick ), NULL, this );
	num1_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	num2_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::num_btnClick ), NULL, this );
	num2_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	 num3_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::num_btnClick ), NULL, this );
	 num3_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	num0_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::num_btnClick ), NULL, this );
	num0_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	num00_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::num_btnClick ), NULL, this );
	num00_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	clear_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::clear_btnClick ), NULL, this );
	clear_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	speed_num100_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::speed_num_btnClick ), NULL, this );
	speed_num100_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	speed_num50_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::speed_num_btnClick ), NULL, this );
	speed_num50_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	speed_num20_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::speed_num_btnClick ), NULL, this );
	speed_num20_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	speed_num10_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::speed_num_btnClick ), NULL, this );
	speed_num10_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	speed_num5_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::speed_num_btnClick ), NULL, this );
	speed_num5_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	speed_num1_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::speed_num_btnClick ), NULL, this );
	speed_num1_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	speed_num05_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::speed_num_btnClick ), NULL, this );
	speed_num05_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	speed_num005_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::speed_num_btnClick ), NULL, this );
	speed_num005_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	speed_num025_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::speed_num_btnClick ), NULL, this );
	speed_num025_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	speed_num01_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::speed_num_btnClick ), NULL, this );
	speed_num01_btn->Connect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
}

base_calc_f::~base_calc_f()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ) );
	cash_tendered_ed->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	cash_tendered_ed->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( base_calc_f::cash_tendered_edChange ), NULL, this );
	exact_amount_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::exact_amount_btnClick ), NULL, this );
	exact_amount_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	process_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::process_btnClick ), NULL, this );
	process_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	cancel_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::cancel_btnClick ), NULL, this );
	cancel_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	num7_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::num_btnClick ), NULL, this );
	num7_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	num8_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::num_btnClick ), NULL, this );
	num8_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	num9_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::num_btnClick ), NULL, this );
	num9_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	num4_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::num_btnClick ), NULL, this );
	num4_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	num5_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::num_btnClick ), NULL, this );
	num5_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	num6_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::num_btnClick ), NULL, this );
	num6_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	num1_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::num_btnClick ), NULL, this );
	num1_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	num2_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::num_btnClick ), NULL, this );
	num2_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	 num3_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::num_btnClick ), NULL, this );
	 num3_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	num0_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::num_btnClick ), NULL, this );
	num0_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	num00_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::num_btnClick ), NULL, this );
	num00_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	clear_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::clear_btnClick ), NULL, this );
	clear_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	speed_num100_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::speed_num_btnClick ), NULL, this );
	speed_num100_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	speed_num50_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::speed_num_btnClick ), NULL, this );
	speed_num50_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	speed_num20_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::speed_num_btnClick ), NULL, this );
	speed_num20_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	speed_num10_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::speed_num_btnClick ), NULL, this );
	speed_num10_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	speed_num5_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::speed_num_btnClick ), NULL, this );
	speed_num5_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	speed_num1_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::speed_num_btnClick ), NULL, this );
	speed_num1_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	speed_num05_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::speed_num_btnClick ), NULL, this );
	speed_num05_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	speed_num005_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::speed_num_btnClick ), NULL, this );
	speed_num005_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	speed_num025_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::speed_num_btnClick ), NULL, this );
	speed_num025_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
	speed_num01_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( base_calc_f::speed_num_btnClick ), NULL, this );
	speed_num01_btn->Disconnect( wxEVT_CHAR, wxKeyEventHandler( base_calc_f::onChar ), NULL, this );
}

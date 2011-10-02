#include "payment_f.h"
#include <lang/xml_lang.h>
#include <memory>

extern std::auto_ptr<ksi_cafe::xml_lang_t> locale;

payment_f::payment_f( wxWindow* parent, const wxString& title, bool process_, bool cc_processing)
:
wxDialog( parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxCAPTION ), process_(process_), result_(-1) {
	if (process_) {
		this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
		wxBoxSizer* bSizer1;
		bSizer1 = new wxBoxSizer( wxVERTICAL );
	
		wxGridBagSizer* gbSizer2;
		gbSizer2 = new wxGridBagSizer( 0, 0 );
		gbSizer2->SetFlexibleDirection( wxBOTH );
		gbSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
		cancel_btn = new wxButton( this, wxID_ANY, locale->get("global", "CANCEL_BTN", "Cancel"), wxDefaultPosition, wxSize( 100,50 ), 0 );
		cancel_btn->SetBackgroundColour( wxColour( 255, 204, 204 ) );
		gbSizer2->Add( cancel_btn, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
		process_btn = new wxButton( this, wxID_ANY, locale->get("global", "PROCESS_BTN", "Process"), wxDefaultPosition, wxSize( 300,50 ), 0 );
		gbSizer2->Add( process_btn, wxGBPosition( 0, 0 ), wxGBSpan( 1, 3 ), wxALL, 5 );
		process_btn->SetBackgroundColour( wxColour( 204, 255, 204 ) );
	
		bSizer1->Add( gbSizer2, 1, wxEXPAND, 5 );
	
		this->SetSizer( bSizer1 );
		this->Layout();
		bSizer1->Fit( this );
	
		// Connect Events
		cancel_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( payment_f::cancel_btnClick ), NULL, this );
		process_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( payment_f::process_btnClick ), NULL, this );
	}
	else {
		this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
		wxBoxSizer* bSizer1;
		bSizer1 = new wxBoxSizer( wxVERTICAL );
	
		wxGridBagSizer* gbSizer2;
		gbSizer2 = new wxGridBagSizer( 0, 0 );
		gbSizer2->SetFlexibleDirection( wxBOTH );
		gbSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
		cash_btn = new wxButton( this, wxID_ANY, locale->get("payment_f", "cash_btn", "Cash"), wxDefaultPosition, wxSize( 100,50 ), 0 );
		cash_btn->SetBackgroundColour( wxColour( 204, 255, 204 ) );
		gbSizer2->Add( cash_btn, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
		credit_card_btn = new wxButton( this, wxID_ANY, locale->get("payment_f", "credit_card_btn", "Credit card"), wxDefaultPosition, wxSize( 100,50 ), 0 );
		credit_card_btn->SetBackgroundColour( wxColour( 204, 255, 204 ) );
		credit_card_btn->Enable(cc_processing);
		gbSizer2->Add( credit_card_btn, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
		check_btn = new wxButton( this, wxID_ANY, locale->get("payment_f", "check_btn", "Check"), wxDefaultPosition, wxSize( 100,50 ), 0 );
		check_btn->SetBackgroundColour( wxColour( 204, 255, 204 ) );
		check_btn->Enable(false);
		gbSizer2->Add( check_btn, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );
	
		cancel_btn = new wxButton( this, wxID_ANY, locale->get("global", "CANCEL_BTN", "Cancel"), wxDefaultPosition, wxSize( 100,50 ), 0 );
		cancel_btn->SetBackgroundColour( wxColour( 255, 204, 204 ) );
		gbSizer2->Add( cancel_btn, wxGBPosition( 1, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );
		
		
		bSizer1->Add( gbSizer2, 1, wxEXPAND, 5 );
	
		this->SetSizer( bSizer1 );
		this->Layout();
		bSizer1->Fit( this );
	
		
	
		// Connect Events
		cash_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( payment_f::cash_btnClick ), NULL, this );
		credit_card_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( payment_f::credit_card_btnClick ), NULL, this );
		check_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( payment_f::check_btnClick ), NULL, this );
		cancel_btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( payment_f::cancel_btnClick ), NULL, this );
	}
	
	CentreOnParent();
}

payment_f::~payment_f() {
	// Disconnect Events
	if (process_) {
		process_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( payment_f::process_btnClick ), NULL, this );
	}
	else {
		cash_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( payment_f::cash_btnClick ), NULL, this );
		credit_card_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( payment_f::credit_card_btnClick ), NULL, this );
		check_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( payment_f::check_btnClick ), NULL, this );
	}
	cancel_btn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( payment_f::cancel_btnClick ), NULL, this );
}

void payment_f::cash_btnClick( wxCommandEvent& event )
{
	// TODO: Implement cash_btnClick
	result_ = 1;
	EndModal(wxID_OK);
}

void payment_f::credit_card_btnClick( wxCommandEvent& event )
{
	// TODO: Implement credit_card_btnClick
	result_ = 2;
	EndModal(wxID_OK);
}

void payment_f::check_btnClick( wxCommandEvent& event )
{
	// TODO: Implement check_btnClick
	result_ = 3;
	EndModal(wxID_OK);
}

void payment_f::cancel_btnClick( wxCommandEvent& event )
{
	// TODO: Implement cancel_btnClick
	EndModal(wxID_CANCEL);
}

void payment_f::process_btnClick( wxCommandEvent& event )
{
	// TODO: Implement process_btnClick
	result_ = 4;
	EndModal(wxID_OK);
}

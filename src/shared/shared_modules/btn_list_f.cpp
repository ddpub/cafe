#include "btn_list_f.h"
#include <wx/msgdlg.h>
#include <wx/stattext.h>
#include <ksi_include/ksi_exceptions.hpp>
#include <wx_util/wx_util.hpp>


btn_list_f::btn_list_f( wxWindow* parent, const wxString& caption_, const wxString& footer_, std::vector<wxString>& cap_vec_, std::vector<wxColour>& color_vec_, std::vector<bool>& checked_, std::vector<bool>& enabled_, std::vector<bool>& wrap_)
:
base_btn_list_f( parent ), cap_vec_(cap_vec_), color_vec_(color_vec_), checked_(checked_), enabled_(enabled_), wrap_(wrap_), table_show_(0)
{
	SetLabel(caption_);
	
	wxBoxSizer* bSizer;
	bSizer = new wxBoxSizer( wxVERTICAL );
	
	{
		fgSizer = new wxFlexGridSizer ( 6, 2, 1, 1);
		fgSizer->SetFlexibleDirection( wxBOTH );
		fgSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
		
		for (size_t i=0; i<12; ++i)
		{
			wxButton* btn = new wxButton (this, wxID_ANY, _(""), wxDefaultPosition, wxSize( 240, 120 ), 0);
			btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
			fgSizer->Add( btn, 0, wxEXPAND, 0 );
			btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( btn_list_f::OnClick ), NULL, this);
			btn_vec_.push_back(btn);
		}		
		bSizer->Add( fgSizer, 1, wxEXPAND, 5 );
		this->SetSizer ( bSizer );
		this->Layout();
		bSizer->Fit ( this );
	}
	try
	{
		list_.reset(new list_buttons_t(btn_vec_, cap_vec_, color_vec_, checked_, enabled_, wrap_));
		list_->show();
		bSizer->Fit( this );
		Fit();
	}
	catch (exception_t& ex)
	{
		wxMessageBox(to_uc(ex.comp_message()));
	}
	
	wxStaticText* footer_lb = 0;
	if (footer_ != _("")) {
		footer_lb = new wxStaticText( this, wxID_ANY, footer_, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
		footer_lb->Wrap( bSizer->GetSize().GetWidth() );
		bSizer->Add( footer_lb, 0, wxALL|wxEXPAND, 5 );
	}
	
	this->Layout();	
	bSizer->Fit( this );
	
	CentreOnParent();
}

btn_list_f::btn_list_f( wxWindow* parent, const wxString& caption_, const wxString& footer_, table_show_t* table_show_)
: base_btn_list_f( parent ), table_show_(table_show_)
{
	SetLabel(caption_);
	
	wxBoxSizer* bSizer;
	bSizer = new wxBoxSizer( wxVERTICAL );
	
	{
		fgSizer = new wxFlexGridSizer ( 4, 6, 1, 1);
		fgSizer->SetFlexibleDirection( wxBOTH );
		fgSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
		
		for (size_t i=0; i<24; ++i)
		{
			wxButton* btn = new wxButton (this, wxID_ANY, _(""), wxDefaultPosition, wxSize( 120, 120 ), 0);
			btn->SetFont( wxFont( 10, 74, 90, 90, false, wxT("Arial Narrow") ) );
			fgSizer->Add( btn, 0, wxEXPAND, 0 );
			btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( btn_list_f::OnClick ), NULL, this);
			btn_vec_.push_back(btn);
		}		
		bSizer->Add( fgSizer, 1, wxEXPAND, 5 );
		this->SetSizer ( bSizer );
		this->Layout();
		bSizer->Fit ( this );
	}
	try
	{
		table_show_->set_buttons(btn_vec_);
		table_show_->show();
		bSizer->Fit( this );
		Fit();
	}
	catch (exception_t& ex)
	{
		wxMessageBox(to_uc(ex.comp_message()));
	}
	
	wxStaticText* footer_lb = 0;
	if (footer_ != _("")) {
		footer_lb = new wxStaticText( this, wxID_ANY, footer_, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
		footer_lb->Wrap( bSizer->GetSize().GetWidth() );
		bSizer->Add( footer_lb, 0, wxALL|wxEXPAND, 5 );
	}
	
	this->Layout();	
	bSizer->Fit( this );
	
	CentreOnParent();
}

btn_list_f::~btn_list_f()
{
	for (size_t i=0; i<btn_vec_.size(); ++i) 
		btn_vec_[i]->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( btn_list_f::OnClick ), NULL, this);
}

void btn_list_f::OnClick( wxCommandEvent& event )
{
	wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
	try
	{
		if (!btn) return;
		if (table_show_) {
			btn_status::status_t cur_status = table_show_->get_status(btn);
			if (cur_status == btn_status::next) table_show_->right();
			else if (cur_status == btn_status::prev) table_show_->left();
			else if (cur_status == btn_status::close) {
				EndModal(wxID_OK);
				return;
			}
			else if (cur_status == btn_status::table) {
				table_btn_ = btn;
				EndModal(wxID_HIGHEST);
				return;
			}
			else if (cur_status == btn_status::table_group) {
				ksi_cafe::table_t table_group = table_show_->get_table_group(btn);
				table_show_->down(table_group.table_group_id());
			}
			else if (cur_status == btn_status::parent) {
				table_show_->up();
			}
			else {
				wxMessageBox(to_uc(boost::lexical_cast<std::string>(cur_status)));
			}
		}
		else {
			if (list_->get_status(btn) == btn_status::next) list_->show_next_page();
			else if (list_->get_status(btn) == btn_status::prev) list_->show_prev_page();
			else EndModal(list_->get_modal_code(btn));
		}
		fgSizer->Fit( this );
		Fit();
		CentreOnParent();
	}
	catch (exception_t& ex)
	{
		wxMessageBox(to_uc(ex.comp_message()));
	}
}

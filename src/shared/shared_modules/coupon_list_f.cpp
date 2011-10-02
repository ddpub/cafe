#include "coupon_list_f.h"
#include <wx/msgdlg.h>
#include <ksi_include/ksi_exceptions.hpp>
#include <wx_util/wx_util.hpp>
#include <ksi_order/coupon.hpp>
#include <ksi_order/order_interface.hpp>
#include "coupon_calc_f.h"
#include "auth_f.h"
#include <ksi_util/users.hpp>
#include "count_f.h"
#include "wx_authorise.hpp"
#include <wx/stattext.h>
#include <boost/lexical_cast.hpp>
#include <shared_modules/connect.hpp>
#include <lang/xml_lang.h>
#include <ksi_util/log.hpp>

extern std::auto_ptr<ksi_cafe::coupon_in_cafe_t> coupon_in_cafe;
extern std::auto_ptr<ksi_cafe::users_t> users;
extern std::auto_ptr<ksi_cafe::xml_lang_t> locale;
extern std::auto_ptr<file_log_t> flog;

#define can_add wxColour( 0, 128, 0 )
#define cant_add wxColour( 128, 0, 0 )

coupon_list_f::coupon_list_f( wxWindow* parent, const wxString& caption_, const wxString& footer_, ksi_cafe::order_t* order)
:
base_btn_list_f( parent ), order(order)
{
	SetLabel(caption_);

	wxBoxSizer* bSizer;
	bSizer = new wxBoxSizer( wxVERTICAL );
	
	fgSizer = new wxFlexGridSizer ( 6, 2, 1, 1);
	fgSizer->SetFlexibleDirection( wxBOTH );
	fgSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	for (size_t i=0; i<12; ++i)
	{
		wxButton* btn = new wxButton (this, wxID_ANY, _(""), wxDefaultPosition, wxSize( 200, 100 ), 0);
		fgSizer->Add( btn, 0, wxEXPAND, 0 );
		btn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( coupon_list_f::OnClick ), NULL, this);
		btn_vec_.push_back(btn);
	}		
	bSizer->Add( fgSizer, 1, wxEXPAND, 5 );
	this->SetSizer ( bSizer );
	this->Layout();
	bSizer->Fit ( this );

	wxStaticText* footer_lb = 0;
	if (footer_ != _("")) {
		footer_lb = new wxStaticText( this, wxID_ANY, footer_, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
		footer_lb->Wrap( bSizer->GetSize().GetWidth() );
		bSizer->Add( footer_lb, 0, wxALL, 5 );
	}
	
	this->Layout();	
	bSizer->Fit( this );

	try
	{
		std::vector<ksi_cafe::coupon_t*> coupon = coupon_in_cafe->coupon();
		cap_vec_.push_back(locale->get("global", "adjustable_discount_str", "Adjustable discount"));
		color_vec_.push_back(wxColour( 255, 255, 0 ));
		checked_vec_.push_back(order->adjustable_discount_exist());
		enabled_vec_.push_back(true);
		wrap_vec_.push_back(true);
		int not_combinable_exist = coupon_in_cafe->not_combinable_exist();
		for (size_t i=0; i<coupon.size(); ++i)
		{
			bool enable = false;
			if (not_combinable_exist == coupon[i]->coupon_id() && coupon_in_cafe->coupon_count(coupon[i])) enable = true;
			else if (!not_combinable_exist && coupon[i]->combinable()) enable = true;
			else if (coupon_in_cafe->coupon_count(coupon[i]) == 0) enable = true;
			cap_vec_.push_back(to_uc(coupon[i]->title()));
			if (ksi_cafe::can_apply_coupon(*coupon[i], order->all_dish()))
				color_vec_.push_back(can_add);
			else
				color_vec_.push_back(cant_add);
			checked_vec_.push_back(coupon_in_cafe->in_selected(coupon[i]));
			enabled_vec_.push_back(enable);
			wrap_vec_.push_back(true);
		}
		list_.reset(new list_buttons_t(btn_vec_, cap_vec_, color_vec_, checked_vec_, enabled_vec_, wrap_vec_));
		list_->show();
		bSizer->Fit( this );
		Fit();
	}
	catch (exception_t& ex)
	{
		wxMessageBox(to_uc(ex.comp_message()));
	}

	CentreOnParent();
}

coupon_list_f::~coupon_list_f()
{
	for (size_t i=0; i<btn_vec_.size(); ++i)
		btn_vec_[i]->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( coupon_list_f::OnClick ), NULL, this);
}

void coupon_list_f::OnClick( wxCommandEvent& event )
{
	wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
	try {
		if (!btn) return;
		if (list_->get_status(btn) == btn_status::next) list_->show_next_page();
		else if (list_->get_status(btn) == btn_status::prev) list_->show_prev_page();
		else if (list_->get_status(btn) == btn_status::item) {
			std::vector<ksi_cafe::coupon_t*> coupon = coupon_in_cafe->coupon();
			*flog << "coupon_vec.size()=\"" + boost::lexical_cast<std::string>(coupon.size()) + "\"";
			int not_combinable_exist = coupon_in_cafe->not_combinable_exist();
			int index = list_->get_modal_code(btn)-wxID_HIGHEST;
			*flog << "index=\"" + boost::lexical_cast<std::string>(index) + "\"";
			if (index == 0 && order->adjustable_discount_exist()) {
				order->save_adjustable_discount();
				list_->change_checked(index, false);
				EndModal(list_->get_modal_code(btn));
				return;
			}
			else if (index == 0 && !order->adjustable_discount_exist())  {
				int add_coupon = 0;
				add_coupon = wx_mng_auth(this);
				if (add_coupon) {
					wx_window_ptr<coupon_calc_f> form (new coupon_calc_f(this, order->subtotal(), locale->get("global", "discount_in_percent_str", "Discount in percent"), locale->get("global", "discount_in_doll_str", "Discount in $")));
					if (form->ShowModal() != wxID_OK) return;
					double discount = form->discount_amount();
					order->add_fixed_discount(discount, -1, to_mb(locale->get("global", "adjustable_discount_str", "adjustable discount")), 0);
					list_->change_checked(index, true);
					EndModal(list_->get_modal_code(btn));
					return;
				}
			}
			else {
				ksi_cafe::coupon_t* coupon_ptr = coupon[index-1];
				if (coupon_in_cafe->in_selected(coupon_ptr)) {
					coupon_in_cafe->unselect(coupon_ptr);
					list_->change_checked(index, false);
					EndModal(list_->get_modal_code(btn));
					return;
				}
				else {
					int add_coupon = -1;
					int mng_id = 0;
					if (coupon_ptr->auth_req()) {
						add_coupon = wx_mng_auth(this);
						mng_id = add_coupon;
					}
					if (add_coupon > 0 || add_coupon == -1) {
						int size = 0;
						if (!coupon_ptr->auto_scalable()) {
							wx_window_ptr<count_f> count_form (new count_f(this, locale->get("global", "specify_number_of_discount_coupons_str", "specify number of discount coupons"), 1, 100, 1));
							if (count_form->ShowModal() != wxID_OK) return;
							size = count_form->get_count();
						}
						coupon_in_cafe->select(coupon_ptr->coupon_id(), size, mng_id);
						list_->change_checked(index, true);
						EndModal(list_->get_modal_code(btn));
						return;
					}
				}
			}
		}
		else {
			EndModal(list_->get_modal_code(btn));
			return;
		}
		std::vector<ksi_cafe::coupon_t*> coupon = coupon_in_cafe->coupon();
		int not_combinable_exist = coupon_in_cafe->not_combinable_exist();
		for (size_t i=0; i<coupon.size(); ++i) {
			bool enable = false;
			if (not_combinable_exist == coupon[i]->coupon_id() && coupon_in_cafe->coupon_count(coupon[i])) enable = true;
			else if (!not_combinable_exist && coupon[i]->combinable()) enable = true;
			else if (coupon_in_cafe->coupon_count(coupon[i]) == 0) enable = true;
			list_->change_enabled(i+1, enable);
		}
		list_->refresh();
		fgSizer->Fit( this );
		Fit();
	}
	catch (exception_t& ex) {
		wxMessageBox(to_uc(ex.comp_message()));
	}
}

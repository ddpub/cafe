#include "count_f.h"
#include <boost/lexical_cast.hpp>
#include <wx_util/wx_util.hpp>

count_f::count_f( wxWindow* parent, const wxString& caption, int min_, int max_, int cur_val_ )
:
base_count_f( parent ), min_(min_), max_(max_), cur_val_(cur_val_)
{
	SetLabel(caption);
	val_ed->SetValue(to_uc(boost::lexical_cast<std::string>(cur_val_)));
	CentreOnParent();
}

void count_f::spin_btnDown( wxSpinEvent& event )
{
	// TODO: Implement spin_btnDown	
	if (cur_val_ == min_) return;
	val_ed->SetValue(to_uc(boost::lexical_cast<std::string>(--cur_val_)));	
}

void count_f::spin_btnUp( wxSpinEvent& event )
{
	// TODO: Implement spin_btnUp
	if (cur_val_ == max_) return;
	val_ed->SetValue(to_uc(boost::lexical_cast<std::string>(++cur_val_)));	
}

void count_f::ok_btnClick( wxCommandEvent& event )
{
	// TODO: Implement ok_btnClick
	EndModal(wxID_OK);
}

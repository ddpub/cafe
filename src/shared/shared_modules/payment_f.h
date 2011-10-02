#ifndef __payment_f__
#define __payment_f__

#include <wx/string.h>
#include <wx/button.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/gbsizer.h>
#include <wx/sizer.h>
#include <wx/dialog.h>


class payment_f : public wxDialog 
{
protected:
	void cash_btnClick( wxCommandEvent& event );
	void credit_card_btnClick( wxCommandEvent& event );
	void check_btnClick( wxCommandEvent& event );
	void cancel_btnClick( wxCommandEvent& event );
	void process_btnClick( wxCommandEvent& event );
private:
	wxButton* cash_btn;
	wxButton* credit_card_btn;
	wxButton* check_btn;
	wxButton* cancel_btn;
	wxButton* process_btn;
	bool process_;
	int result_;
public:
	/** Members */
	int result() const { return result_; }
public:
	/** Constructor */
	payment_f( wxWindow* parent, const wxString& title, bool process_, bool cc_processing);
	~payment_f();
private:
	payment_f();
	
};

#endif // __payment_f__

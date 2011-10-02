///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __base_card_swipe_f__
#define __base_card_swipe_f__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class base_card_swipe_f
///////////////////////////////////////////////////////////////////////////////
class base_card_swipe_f : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_label;
		wxButton* cancel_btn;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnChar( wxKeyEvent& event ){ event.Skip(); }
		virtual void cancel_btnClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnLeftMouseUp( wxMouseEvent& event ){ event.Skip(); }
		
	
	public:
		base_card_swipe_f( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxTAB_TRAVERSAL|wxWANTS_CHARS );
		~base_card_swipe_f();
	
};

#endif //__base_card_swipe_f__

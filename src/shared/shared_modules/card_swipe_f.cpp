#include "card_swipe_f.h"
#include <wx/msgdlg.h>
#include <boost/lexical_cast.hpp>

card_swipe_f::card_swipe_f( wxWindow* parent, const wxString& title, const wxString& label )
:
base_card_swipe_f( parent ), card_reading_(false), catch_return_(false)
{
	SetLabel(title);
	m_label->SetLabel(label);
	CentreOnParent();
}

card_swipe_f::~card_swipe_f() {
}

void card_swipe_f::cancel_btnClick( wxCommandEvent& event )
{
	// TODO: Implement cancel_btnClick
	//EndModal(wxID_CANCEL);
}

void card_swipe_f::OnLeftMouseUp( wxMouseEvent& event ) {
	wxPoint pos = event.GetPosition();
	if (pos.x > 0 && pos.x < cancel_btn->GetSize().GetWidth() && pos.y > 0 && pos.y < cancel_btn->GetSize().GetHeight())
	EndModal(wxID_CANCEL);
}

void card_swipe_f::OnChar( wxKeyEvent& event ) {
	if (event.GetKeyCode() == WXK_RETURN || event.GetKeyCode() == WXK_NUMPAD_ENTER) {
		if (catch_return_) {
			catch_return_ = false;
			EndModal(wxID_OK);
		}
	}
	else if (card_reading_) {
		if (event.GetUnicodeKey() == wxChar('?')) card_reading_ = false;
		buff_ += event.GetUnicodeKey();
		if (!card_reading_) catch_return_ = true;
	}
	else if (!card_reading_ && event.GetUnicodeKey() == wxChar(';')) {
		card_reading_ = true;
		buff_.clear();
		buff_ += event.GetUnicodeKey();
	}
	else event.Skip();
}

#ifndef DMITRY_DERGACHEV_CUSTOM_TREELISTCTRL_H_21_05_2008
#define DMITRY_DERGACHEV_CUSTOM_TREELISTCTRL_H_21_05_2008

#include <wx_util/treelistctrl/treelistctrl.h>


class wxExpenseListCtrl: public wxTreeListCtrl
{
public:
    wxExpenseListCtrl(wxWindow *parent, wxWindowID id = -1,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = wxTR_DEFAULT_STYLE,
               const wxValidator &validator = wxDefaultValidator,
               const wxString& name = wxTreeListCtrlNameStr )
        : wxTreeListCtrl(parent, id, pos, size, style, validator, name)
    {

    }
	
	virtual ~wxExpenseListCtrl() {}
	
	wxString OnGetItemText(wxTreeItemData* item_data, long column) const;
}; 

class wxRefundTreeListCtrl: public wxTreeListCtrl
{
public:
    wxRefundTreeListCtrl(wxWindow *parent, wxWindowID id = -1,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = wxTR_DEFAULT_STYLE,
               const wxValidator &validator = wxDefaultValidator,
               const wxString& name = wxTreeListCtrlNameStr )
        : wxTreeListCtrl(parent, id, pos, size, style, validator, name)
    {

    }

    virtual ~wxRefundTreeListCtrl() {}

	wxString OnGetItemText (wxTreeItemData* item_data, long column) const;
};

class wxModifierTreeListCtrl: public wxTreeListCtrl
{
public:
    wxModifierTreeListCtrl(wxWindow *parent, wxWindowID id = -1,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = wxTR_DEFAULT_STYLE,
               const wxValidator &validator = wxDefaultValidator,
               const wxString& name = wxTreeListCtrlNameStr )
        : wxTreeListCtrl(parent, id, pos, size, style, validator, name)
    {

    }

    virtual ~wxModifierTreeListCtrl() {}

	wxString OnGetItemText (wxTreeItemData* item_data, long column) const;
};

class wxOrderTreeListCtrl: public wxTreeListCtrl
{
public:
    wxOrderTreeListCtrl(wxWindow *parent, wxWindowID id = -1,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = wxTR_DEFAULT_STYLE,
               const wxValidator &validator = wxDefaultValidator,
               const wxString& name = wxTreeListCtrlNameStr )
        : wxTreeListCtrl(parent, id, pos, size, style, validator, name)
    {

    }

    virtual ~wxOrderTreeListCtrl() {}

	wxString OnGetItemText (wxTreeItemData* item_data, long column) const;
};

class wxSplitOrderTreeListCtrl: public wxTreeListCtrl
{
public:
    wxSplitOrderTreeListCtrl(wxWindow *parent, wxWindowID id = -1,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = wxTR_DEFAULT_STYLE,
               const wxValidator &validator = wxDefaultValidator,
               const wxString& name = wxTreeListCtrlNameStr )
        : wxTreeListCtrl(parent, id, pos, size, style, validator, name)
    {

    }

    virtual ~wxSplitOrderTreeListCtrl() {}

	wxString OnGetItemText (wxTreeItemData* item_data, long column) const;
};


#endif

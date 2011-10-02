#include "advance_f.h"
#include <wx/msgdlg.h>
#include <ksi_include/wx_tree_node.hpp>
#include <ksi_include/custom_treelistctrl.hpp>
#include <ksi_util/users.hpp>
#include <ksi_include/ksi_exceptions.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
//#include <ksi_util/date_time.hpp>
//#include <ksi_util/currency.hpp>
#include <lang/xml_lang.h>
#include <memory>

extern std::auto_ptr<ksi_cafe::expense_list_t> expense_list;
extern std::auto_ptr<ksi_cafe::users_t> users;
//extern std::auto_ptr<ksi_cafe::base_currency_t> currency;
//extern std::auto_ptr<ksi_cafe::format_date_time_t> format_date_time;
extern std::auto_ptr<ksi_cafe::xml_lang_t> locale;

wxString wxExpenseListCtrl::OnGetItemText (wxTreeItemData* item_data, long column) const {
	try {
		expense_node_t* node = dynamic_cast<expense_node_t*>(item_data);
		if (!node) THROW_LOCATION(0, "wxString wxExpenseListCtrl::OnGetItemText (wxTreeItemData*, long) const"); 
		const ksi_cafe::expense_t* expense = reinterpret_cast<const ksi_cafe::expense_t*>(node->col1());
		if (column == 0) {
			return to_uc(boost::lexical_cast<std::string>(expense->expense_id()));
		}
		else if (column == 1) {
			return to_uc(users->user(expense->opener_id()).fio());
		}
		else if (column == 2) {
			return to_uc(expense_list->type_name(expense->type_id()));
		}
		else if (column == 3) {
			return locale->get_date_time(expense->created());//to_uc(format_date_time->date_time(expense->created()));
		}
		else if (column == 4) {
			return locale->get_currency(expense->amount());//to_uc(currency->format(expense->amount()));
		}
	}
	catch (exception_t& ex) {
		wxMessageBox(to_uc(ex.comp_message()));
	}
	return _("");

}

advance_f::advance_f( wxWindow* parent, const wxString& title, int manager_id_)
:
base_advance_f( parent ), kbd_(1), kbd_case_(0), active_(comment_ed), manager_id_(manager_id_)
{
	SetLabel(title);
	
	notebook->SetPageText(0, locale->get("advance_f", "page1", "Create new expense"));
	notebook->SetPageText(1, locale->get("advance_f", "page2", "Open expenses"));
	expense_type_sizer->GetStaticBox()->SetLabel(locale->get("advance_f", "expense_type_sizer", "Expense type"));
	amount_lb->SetLabel(locale->get("advance_f", "amount_lb", "Amount:"));
	comment_sizer->GetStaticBox()->SetLabel(locale->get("advance_f", "comment_sizer", "Comment"));
	ok_btn->SetLabel(locale->get("advance_f", "ok_btn", "Ok"));
	cancel_btn->SetLabel(locale->get("advance_f", "cancel_btn", "Cancel"));
	comment2_sizer->GetStaticBox()->SetLabel(locale->get("advance_f", "comment2_sizer", "Comment"));
	change_lb->SetLabel(locale->get("advance_f", "change_lb", "Change:"));
	
	open_expenses_list->GetColumn(0).SetText(locale->get("advance_f", "sg_expense_id", "ID"));
	open_expenses_list->GetColumn(1).SetText(locale->get("advance_f", "sg_opener", "Opener"));
	open_expenses_list->GetColumn(2).SetText(locale->get("advance_f", "sg_expense_type", "Expense type"));
	open_expenses_list->GetColumn(3).SetText(locale->get("advance_f", "sg_date", "Date"));
	open_expenses_list->GetColumn(4).SetText(locale->get("advance_f", "sg_amount", "Amount"));
	
	open_expenses_sizer->GetStaticBox()->SetLabel(locale->get("advance_f", "open_expenses_sizer", "Open expenses"));

	us_low_[m_button1] = _("1");
	us_up_[m_button1] = _("!");
	us_low_[m_button2] = _("2");
	us_up_[m_button2] = _("@");
	us_low_[m_button3] = _("3");
	us_up_[m_button3] = _("#");
	us_low_[m_button4] = _("4");
	us_up_[m_button4] = _("$");
	us_low_[m_button5] = _("5");
	us_up_[m_button5] = _("%");
	us_low_[m_button6] = _("6");
	us_up_[m_button6] = _("^");
	us_low_[m_button7] = _("7");
	us_up_[m_button7] = _("&");
	us_low_[m_button8] = _("8");
	us_up_[m_button8] = _("*");
	us_low_[m_button9] = _("9");
	us_up_[m_button9] = _("(");
	us_low_[m_button0] = _("0");
	us_up_[m_button0] = _(")");
	us_low_[m_button10] = _("-");
	us_up_[m_button10] = _("_");
	us_low_[m_button11] = _("=");
	us_up_[m_button11] = _("+");
	us_low_[m_buttonq] = _("q");
	us_up_[m_buttonq] = _("Q");
	us_low_[m_buttonw] = _("w");
	us_up_[m_buttonw] = _("W");
	us_low_[m_buttone] = _("e");
	us_up_[m_buttone] = _("E");
	us_low_[m_buttonr] = _("r");
	us_up_[m_buttonr] = _("R");
	us_low_[m_buttont] = _("t");
	us_up_[m_buttont] = _("T");
	us_low_[m_buttony] = _("y");
	us_up_[m_buttony] = _("Y");
	us_low_[m_buttonu] = _("u");
	us_up_[m_buttonu] = _("U");
	us_low_[m_buttoni] = _("i");
	us_up_[m_buttoni] = _("I");
	us_low_[m_buttono] = _("o");
	us_up_[m_buttono] = _("O");
	us_low_[m_buttonp] = _("p");
	us_up_[m_buttonp] = _("P");
	us_low_[m_buttona] = _("a");
	us_up_[m_buttona] = _("A");
	us_low_[m_buttons] = _("s");
	us_up_[m_buttons] = _("S");
	us_low_[m_buttond] = _("d");
	us_up_[m_buttond] = _("D");
	us_low_[m_buttonf] = _("f");
	us_up_[m_buttonf] = _("F");
	us_low_[m_buttong] = _("g");
	us_up_[m_buttong] = _("G");
	us_low_[m_buttonh] = _("h");
	us_up_[m_buttonh] = _("H");
	us_low_[m_buttonj] = _("j");
	us_up_[m_buttonj] = _("J");
	us_low_[m_buttonk] = _("k");
	us_up_[m_buttonk] = _("K");
	us_low_[m_buttonl] = _("l");
	us_up_[m_buttonl] = _("L");
	us_low_[m_buttonz] = _("z");
	us_up_[m_buttonz] = _("Z");
	us_low_[m_buttonx] = _("x");
	us_up_[m_buttonx] = _("X");
	us_low_[m_buttonc] = _("c");
	us_up_[m_buttonc] = _("C");
	us_low_[m_buttonv] = _("v");
	us_up_[m_buttonv] = _("V");
	us_low_[m_buttonb] = _("b");
	us_up_[m_buttonb] = _("B");
	us_low_[m_buttonn] = _("n");
	us_up_[m_buttonn] = _("N");
	us_low_[m_buttonm] = _("m");
	us_up_[m_buttonm] = _("M");
	us_low_[m_button12] = _("[");
	us_up_[m_button12] = _("{");
	us_low_[m_button13] = _("]");
	us_up_[m_button13] = _("}");
	us_low_[m_button14] = _(";");
	us_up_[m_button14] = _(":");
	us_low_[m_button15] = _("'");
	us_up_[m_button15] = _("\"");
	us_low_[m_button16] = _("\\");
	us_up_[m_button16] = _("|");
	us_low_[m_button17] = _(",");
	us_up_[m_button17] = _("<");
	us_low_[m_button18] = _(".");
	us_up_[m_button18] = _(">");
	us_low_[m_button19] = _("/");
	us_up_[m_button19] = _("?");
	
	ru_low_[m_button1] = _("1");
	ru_up_[m_button1] = _("!");
	ru_low_[m_button2] = _("2");
	ru_up_[m_button2] = _("\"");
	ru_low_[m_button3] = _("3");
	ru_up_[m_button3] = _("№");
	ru_low_[m_button4] = _("4");
	ru_up_[m_button4] = _(";");
	ru_low_[m_button5] = _("5");
	ru_up_[m_button5] = _("%");
	ru_low_[m_button6] = _("6");
	ru_up_[m_button6] = _(":");
	ru_low_[m_button7] = _("7");
	ru_up_[m_button7] = _("?");
	ru_low_[m_button8] = _("8");
	ru_up_[m_button8] = _("*");
	ru_low_[m_button9] = _("9");
	ru_up_[m_button9] = _("(");
	ru_low_[m_button0] = _("0");
	ru_up_[m_button0] = _(")");
	ru_low_[m_button10] = _("-");
	ru_up_[m_button10] = _("_");
	ru_low_[m_button11] = _("=");
	ru_up_[m_button11] = _("+");
	ru_low_[m_buttonq] = _("й");
	ru_up_[m_buttonq] = _("Й");
	ru_low_[m_buttonw] = _("ц");
	ru_up_[m_buttonw] = _("Ц");
	ru_low_[m_buttone] = _("у");
	ru_up_[m_buttone] = _("У");
	ru_low_[m_buttonr] = _("к");
	ru_up_[m_buttonr] = _("К");
	ru_low_[m_buttont] = _("е");
	ru_up_[m_buttont] = _("Е");
	ru_low_[m_buttony] = _("н");
	ru_up_[m_buttony] = _("Н");
	ru_low_[m_buttonu] = _("г");
	ru_up_[m_buttonu] = _("Г");
	ru_low_[m_buttoni] = _("ш");
	ru_up_[m_buttoni] = _("Ш");
	ru_low_[m_buttono] = _("щ");
	ru_up_[m_buttono] = _("Щ");
	ru_low_[m_buttonp] = _("з");
	ru_up_[m_buttonp] = _("З");
	ru_low_[m_buttona] = _("ф");
	ru_up_[m_buttona] = _("Ф");
	ru_low_[m_buttons] = _("ы");
	ru_up_[m_buttons] = _("Ы");
	ru_low_[m_buttond] = _("в");
	ru_up_[m_buttond] = _("В");
	ru_low_[m_buttonf] = _("а");
	ru_up_[m_buttonf] = _("А");
	ru_low_[m_buttong] = _("п");
	ru_up_[m_buttong] = _("П");
	ru_low_[m_buttonh] = _("р");
	ru_up_[m_buttonh] = _("Р");
	ru_low_[m_buttonj] = _("о");
	ru_up_[m_buttonj] = _("О");
	ru_low_[m_buttonk] = _("л");
	ru_up_[m_buttonk] = _("Л");
	ru_low_[m_buttonl] = _("д");
	ru_up_[m_buttonl] = _("Д");
	ru_low_[m_buttonz] = _("я");
	ru_up_[m_buttonz] = _("Я");
	ru_low_[m_buttonx] = _("ч");
	ru_up_[m_buttonx] = _("Ч");
	ru_low_[m_buttonc] = _("с");
	ru_up_[m_buttonc] = _("С");
	ru_low_[m_buttonv] = _("м");
	ru_up_[m_buttonv] = _("М");
	ru_low_[m_buttonb] = _("и");
	ru_up_[m_buttonb] = _("И");
	ru_low_[m_buttonn] = _("т");
	ru_up_[m_buttonn] = _("Т");
	ru_low_[m_buttonm] = _("ь");
	ru_up_[m_buttonm] = _("Ь");
	ru_low_[m_button12] = _("х");
	ru_up_[m_button12] = _("Х");
	ru_low_[m_button13] = _("ъ");
	ru_up_[m_button13] = _("Ъ");
	ru_low_[m_button14] = _("ж");
	ru_up_[m_button14] = _("Ж");
	ru_low_[m_button15] = _("э");
	ru_up_[m_button15] = _("Э");
	ru_low_[m_button16] = _("\\");
	ru_up_[m_button16] = _("/");
	ru_low_[m_button17] = _("б");
	ru_up_[m_button17] = _("Б");
	ru_low_[m_button18] = _("ю");
	ru_up_[m_button18] = _("Ю");
	ru_low_[m_button19] = _(".");
	ru_up_[m_button19] = _(",");
	
	apply_kbd();
	
	amount_ed->SetValue(locale->get_currency(0));
	change_ed->SetValue(locale->get_currency(0));
	
	type_vec_ = expense_list->type_vec();
	
	for (size_t i=0; i<type_vec_.size(); ++i) type_str_vec.push_back(to_uc(type_vec_[i].name()));
	expense_type_list->Set(type_str_vec.size(), &type_str_vec[0]);
	expense_type_list->Select(0);
	
	open_expenses_list->AddRoot(_("root"));
	std::vector<const ksi_cafe::expense_t*> expense_vec = expense_list->open_expense_vec();
	for (size_t i=0; i<expense_vec.size(); ++i) {
		wxTreeItemId curr = open_expenses_list->AppendItem(open_expenses_list->GetRootItem(), _(""), -1, -1, new expense_node_t(node_type::expense, expense_vec[i])); 
	}
	// Connect Events
	open_expenses_list->Connect ( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( advance_f::sgSelChanged ), NULL, this );
	open_expenses_list->Connect ( wxEVT_COMMAND_TREE_SEL_CHANGING, wxTreeEventHandler( advance_f::sgSelChanging ), NULL, this );

	CentreOnParent();
}

advance_f::~advance_f() {
	// Disconnect Events
	open_expenses_list->Disconnect ( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( advance_f::sgSelChanged ), NULL, this );
	open_expenses_list->Disconnect ( wxEVT_COMMAND_TREE_SEL_CHANGING, wxTreeEventHandler( advance_f::sgSelChanging ), NULL, this );

}

void advance_f::sgSelChanged( wxTreeEvent& event )
{		
	wxTreeItemId id = event.GetItem();
	if (!event.GetItem().IsOk()) return;
	if (event.GetItem() == open_expenses_list->GetRootItem()) return;
	wxTreeItemData* data = open_expenses_list->GetItemData(id);
	expense_node_t* node = dynamic_cast<expense_node_t*>(data);
	if (!node) THROW_LOCATION(0, "void advance_f::sgSelChanged( wxTreeEvent& )"); 
	if (node->type() != node_type::expense) THROW_LOCATION(1, "void advance_f::sgSelChanged( wxTreeEvent& )"); 
	const ksi_cafe::expense_t* expense = reinterpret_cast<const ksi_cafe::expense_t*>(node->col1());
	m_textCtrl7->SetValue(to_uc(expense->description()));
	ok_btn->Enable(valid());
}

void advance_f::sgSelChanging( wxTreeEvent& event )
{
	ok_btn->Enable(false);
}


bool advance_f::valid() {
	if (notebook->GetCurrentPage() == m_panel1) {
		if (comment_ed->GetValue() != _("") && int(locale->reverse_currency(amount_ed->GetValue())*100) != 0)  return true;
		else return false;
	}
	else if (notebook->GetCurrentPage() == m_panel2) {
		return open_expenses_list->GetSelection().IsOk();
	}
	return false;
}

void advance_f::notebookPageChanging( wxNotebookEvent& event ) {
	event.Skip();
}

void advance_f::notebookPageChanged( wxNotebookEvent& event ) {
	if (notebook->GetCurrentPage() == m_panel1) {
		active_ = change_ed;
		ok_btn->Enable(open_expenses_list->GetSelection().IsOk());
	}
	else if (notebook->GetCurrentPage() == m_panel2) {
		active_ = comment_ed;
		if (comment_ed->GetValue() != _("") && int(locale->reverse_currency(amount_ed->GetValue()))*100 != 0) ok_btn->Enable(true);
		else ok_btn->Enable(false);
	}
	event.Skip();
}

void advance_f::apply_kbd() {
	if (kbd_ == 1 && kbd_case_ == 0) {
		for (std::map<wxButton*, wxString>::const_iterator i=us_low_.begin(); i != us_low_.end(); ++i) i->first->SetLabel(i->second);
		kbd_btn->SetLabel(_("us"));
	}
	else if (kbd_ == 1 && kbd_case_ == 1) {
		for (std::map<wxButton*, wxString>::const_iterator i=us_up_.begin(); i != us_up_.end(); ++i) i->first->SetLabel(i->second);
		kbd_btn->SetLabel(_("US"));
	}
	else if (kbd_ == 2 && kbd_case_ == 0) {
		for (std::map<wxButton*, wxString>::const_iterator i=ru_low_.begin(); i != ru_low_.end(); ++i) i->first->SetLabel(i->second);
		kbd_btn->SetLabel(_("ru"));
	}
	else if (kbd_ == 2 && kbd_case_ == 1) {
		for (std::map<wxButton*, wxString>::const_iterator i=ru_up_.begin(); i != ru_up_.end(); ++i) i->first->SetLabel(i->second);
		kbd_btn->SetLabel(_("RU"));
	}
}



void advance_f::amount_edFocus( wxFocusEvent& event )
{
	// TODO: Implement amount_edFocus
	active_ = amount_ed;
	ok_btn->Enable(valid());
}

void advance_f::comment_edFocus( wxFocusEvent& event )
{
	// TODO: Implement comment_edFocus
	active_ = comment_ed;
	ok_btn->Enable(valid());
}

void advance_f::change_edFocus( wxFocusEvent& event ) {
	active_ = change_ed;
	ok_btn->Enable(valid());
}

void advance_f::open_expenses_listSelected( wxListEvent& event )
{
	// TODO: Implement open_expenses_listSelected
	ok_btn->Enable(valid());
}

void advance_f::OnExpenseTypeSelected( wxCommandEvent& event ) {
	event.Skip();
	ok_btn->Enable(valid());
}

void advance_f::btnClick( wxCommandEvent& event )
{
	// TODO: Implement btnClick
		wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
	if (btn == 0) return;
	if (active_ == comment_ed) {
		comment_ed->WriteText(btn->GetLabel());
	}
	else if ((active_ == amount_ed || active_ == change_ed)) {
		wxTextCtrl* ctrl = dynamic_cast<wxTextCtrl*>(active_);
		if (!ctrl) return;
		wxString sval = btn->GetLabel();
		ctrl->SetInsertionPoint(ctrl->GetLastPosition());
		int val = -1;
		if (sval == _("1")) val = 1;
		else if (sval == _("2")) val = 2;
		else if (sval == _("3")) val = 3;
		else if (sval == _("4")) val = 4;
		else if (sval == _("5")) val = 5;
		else if (sval == _("6")) val = 6;
		else if (sval == _("7")) val = 7;
		else if (sval == _("8")) val = 8;
		else if (sval == _("9")) val = 9;
		else if (sval == _("0")) val = 0;
		
		if (val != -1) {
			double amount = locale->reverse_currency(ctrl->GetValue());
			if (amount > 999999) return;
			amount = amount*10+double(val)/100;
			ctrl->SetValue(locale->get_currency(amount));
		}
	}
	ok_btn->Enable(valid());
}

void advance_f::bs_btnClick( wxCommandEvent& event )
{
	// TODO: Implement bs_btnClick
	if (active_ == comment_ed) {
		long from = comment_ed->GetInsertionPoint() - 1;
		long to = comment_ed->GetInsertionPoint();
		comment_ed->Remove(from, to);
	}
	else if ((active_ ==  amount_ed || active_ == change_ed)) {
		wxTextCtrl* ctrl = dynamic_cast<wxTextCtrl*>(active_);
		if (!ctrl) return;
		ctrl->SetValue(locale->get_currency(0));
	}
	ok_btn->Enable(valid());
}

void advance_f::enter_btnClick( wxCommandEvent& event )
{
	// TODO: Implement enter_btnClick
	wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
	if (btn == 0) return;
	if (active_ == comment_ed) {
		comment_ed->WriteText(_("\n"));
	}
	ok_btn->Enable(valid());
}

void advance_f::shift_btnClick( wxCommandEvent& event )
{
	// TODO: Implement shift_btnClick
	wxToggleButton* btn = dynamic_cast<wxToggleButton*>(event.GetEventObject());
	if (btn == 0) return;
	if (btn->GetValue()) {
		shift1_btn->SetValue(true);
		shift2_btn->SetValue(true);
		kbd_case_ = 1;
	}
	else {
		shift1_btn->SetValue(false);
		shift2_btn->SetValue(false);
		kbd_case_ = 0;
	}
	apply_kbd();
}

void advance_f::kbd_btnClick( wxCommandEvent& event )
{
	// TODO: Implement kbd_btnClick
	if (kbd_ == 1) {
		kbd_ = 2;
	}
	else {
		kbd_ = 1;
	}
	apply_kbd();
}

void advance_f::space_btnClick( wxCommandEvent& event )
{
	// TODO: Implement space_btnClick
	wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
	if (btn == 0) return;
	if (active_ == comment_ed) {
		comment_ed->WriteText(_(" "));
	}
	ok_btn->Enable(valid());
}

void advance_f::ok_btnClick( wxCommandEvent& event )
{
	// TODO: Implement ok_btnClick

	if (notebook->GetCurrentPage() == m_panel1) {
		int type_id = type_vec_[expense_type_list->GetSelection()].type_id();
		expense_list->open_expense(users->user().user_id(), type_id, locale->reverse_currency(amount_ed->GetValue()), to_mb(comment_ed->GetValue()), manager_id_); 
	}
	else if (notebook->GetCurrentPage() == m_panel2) {
		wxTreeItemId id = open_expenses_list->GetSelection();
		wxTreeItemData* data = open_expenses_list->GetItemData(id);
		expense_node_t* node = dynamic_cast<expense_node_t*>(data);
		if (!node) THROW_LOCATION(0, "void advance_f::sgSelChanged( wxTreeEvent& )"); 
		if (node->type() != node_type::expense) THROW_LOCATION(1, "void advance_f::sgSelChanged( wxTreeEvent& )"); 
		const ksi_cafe::expense_t* expense = reinterpret_cast<const ksi_cafe::expense_t*>(node->col1());

		expense_list->close_expense(expense->expense_id(), locale->reverse_currency(change_ed->GetValue()));
	}	
	
	EndModal(wxID_OK);
}

void advance_f::cancel_btnClick( wxCommandEvent& event )
{
	// TODO: Implement cancel_btnClick
	EndModal(wxID_CANCEL);
}

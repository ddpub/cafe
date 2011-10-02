
#include <ksi_include/ksi_exceptions.hpp>

#ifdef _UNIX_BUILD_

#include <wx/utils.h>
#include <wx_util/wx_util.hpp>
#include <wx/msgdlg.h>

std::vector<std::string> get_mac() {
	std::vector<std::string> mac_vec;
	wxArrayString arr;
	wxExecute(_("sh ifconfig | grep -o --basic-regexp 'HWaddr \\w\\w:\\w\\w:\\w\\w:\\w\\w:\\w\\w:\\w\\w' | sed \"s/:/-/g\" | sed \"s/HWaddr //g\""), arr);
	for (size_t i=0; i<arr.Count(); ++i) {
		mac_vec.push_back(to_mb(arr.Item(i)));
		wxMessageBox(arr.Item(i));
	}
	return mac_vec;
}

#endif //_UNIX_BUILD_

#ifdef _WIN32_BUILD_

#include <wx/utils.h>
#include <wx_util/wx_util.hpp>
#include <boost/regex.hpp>

std::vector<std::string> get_mac() {
	std::vector<std::string> mac_vec;
	wxArrayString arr;
	wxExecute(_("ipconfig /all"), arr);
	boost::regex fetch_mac_expression("\\s+((?:[A-F0-9]{2}-){5}(?:[A-F0-9]{2}){1})\\s+");
	std::string src = "";
	{
		wxString tmp = _("");
		for (size_t i=0; i<arr.Count(); ++i) { tmp = tmp + _(" ") + arr.Item(i); }
		wxString tmp2 = _("");
		for (size_t i=0; i<tmp.Len(); ++i) {
			if (tmp[i] >= 'a' && tmp[i] <= 'f') tmp2.Append(tmp[i]);
			else if (tmp[i] >= 'A' && tmp[i] <= 'F') tmp2.Append(tmp[i]);
			else if (tmp[i] == '-') tmp2.Append(tmp[i]);
			else if (tmp[i] >= '0' && tmp[i] <= '9') tmp2.Append(tmp[i]);
			else tmp2.Append(' ');
		}
		tmp = tmp2;
		src = to_mb(tmp);
	}
	std::vector<std::string> result;

	if (boost::regex_split(std::back_inserter(result), src, fetch_mac_expression)) {
		for (size_t j=0; j<result.size(); ++j) {
			mac_vec.push_back(result[j]);
		}
	}
	return mac_vec;
}

#endif // _WIN32_BUILD_

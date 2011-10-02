
#include "format.hpp"
#include <vector>
#include <map>
#include <iterator>
#include <boost/lexical_cast.hpp>

namespace ksi_cafe {

std::string en_phone_format_t::format(const std::string& source) const {
	std::string result = "";
	int group = 1;
	if (source != "")
		for (std::string::const_reverse_iterator i=source.rbegin(); i!=source.rend(); ++i) {
			if (std::distance(source.rbegin(),i) == 4) result = '-' + result;
			else if (std::distance(source.rbegin(), i) == 7) result = '-' + result;
			result = *i + result;
		}
	return result;
}

std::string en_phone_format_t::reverse(const std::string& source) const {
	std::string result = "";
	if (source != "")
		for (std::string::const_reverse_iterator i=source.rbegin(); i!=source.rend(); ++i) {
			if (*i != '-') result = *i + result;
		}
	return result;
}



};

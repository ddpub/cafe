
#include "reg.hpp"

#include <boost/regex.hpp>
#include <fstream>
#include <ksi_include/ksi_exceptions.hpp>
#include <boost/lexical_cast.hpp>


std::pair<std::string, std::string> param_t::fetch() const
{
	std::vector<std::string> result;
	std::string src = src_;
	boost::regex fetch_param_expression("\\s*([\\w]+)\\s*=\\s*([\\w\\s.:/$]*)\\s*#*");
	if (boost::regex_split(std::back_inserter(result), src, fetch_param_expression))
	{
		if (result.size() != 2) throw exception_t(1, "param_t::fetch()", "src_ = \"" + src_ + "\"");
	}
	else throw exception_t(2, "param_t::fetch()", "src_ = \"" + src_ + "\"");

	std::string key = result[0];
	std::string val = result[1];
	result.clear();
	boost::regex more_fetch_param_expression("(?:\\s*)([\\w.:/$]*)(?:\\s*)");
	if (boost::regex_split(std::back_inserter(result), val, more_fetch_param_expression))
	{
	}
	else throw exception_t(3, "param_t::fetch()", "src_ = \"" + src_ + "\"");
	for (size_t i=0; i<result.size(); ++i) { val += result[i]; if (i+1 != result.size()) val += " "; }
	return std::make_pair(key, val);
}

void file_reg_t::load_form_file()
{
	std::fstream f (file_.c_str(), std::ios_base::in|std::ios_base::out);
	if (!f) return;
	param_t param;
	while (!f.eof())
	{
		std::string src;
		std::getline(f, src);
		try
		{
			std::pair<std::string, std::string> p = param.fetch(src);
			set(p.first, p.second);
		}
		catch(exception_t&)
		{
		}
	}
}

void file_reg_t::save_to_file()
{
	std::fstream f (file_.c_str(), std::ios_base::out);
	if (!f) return;
	for (kv_map_t::iterator i=kv_.begin(); i!= kv_.end(); ++i)
	{
		f << i->first << " = " << i->second << std::endl;
		f.flush();
	}
}


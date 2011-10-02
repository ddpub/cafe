
#include "convert.hpp"
#include <sstream>
#include <boost/lexical_cast.hpp>

std::string convert_t::alf_ = "";//"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZàáâãäå¸æçèéêëìíîïğñòóôõö÷øùúûüışÿÀÁÂÃÄÅ¨ÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞß";
std::string convert_t::alf10_ = "0123456789";
std::string convert_t::alf15_ = "0123456789:;<=>";
std::string convert_t::alf44_ = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_-+~=()|";
std::string convert_t::alf95_ = "";
const std::string ro = "1995565086972449";

convert_t::convert_t(int in_base_, int out_base_)
: in_base_(in_base_), out_base_(out_base_)
{
	in_set_ = alf_.substr(0, in_base_);
	out_set_ = alf_.substr(0, out_base_);
}

void convert_t::add(size_t n)
{
	size_t pos = 0;
	size_t buf = n;
	do
	{
		if (pos < buff_.size())
		{
			buf += buff_[pos];
			buff_[pos] = buf % out_base_;
		}
		else
			buff_.push_back(buf % out_base_);

		buf = buf/out_base_;
		++pos;
	} while (buf > 0);
}

void convert_t::mul(size_t n)
{
	std::vector<size_t> tmp_buff;
	size_t res_len = buff_.size();
	if (res_len == 0) return;
	size_t cur_pos = 0;
	size_t buf = 0;
	do
	{
		size_t a = 0;
		if (cur_pos < res_len)
		{
			a = buff_[cur_pos]*n;
		}
		a+=buf;

		tmp_buff.push_back(a % out_base_);
		buf = a/out_base_;
		++cur_pos;
	} while ((res_len>cur_pos) || (buf!=0));
	buff_ = tmp_buff;
}

std::string convert_t::go(int in_base, int out_base, const std::string& input)
{
	in_base_ = in_base;
	out_base_ = out_base;
	in_set_ = alf_.substr(0, in_base_);
	out_set_ = alf_.substr(0, out_base_);
	buff_.clear();
	size_t count_in = input.length();
	for (size_t i=0; i<count_in; ++i)
	{
		char a = input[i];
		if (i != 0) mul(in_base_);

		add(in_set_.find(a, 0));

	}
	std::string result = "";
	size_t count_out = buff_.size();
	for (size_t i=0; i<count_out; ++i)
		result = out_set_[buff_[i]] + result;

	return result;
}


std::string change_alf(const std::string& input, const std::string& in_alf, const std::string& out_alf)
{
	std::string result = "";

	for (size_t i=0; i<input.length(); ++i)
	{
		result = result + out_alf[in_alf.find(input[i])];
	}
	return result;
}

std::string to_10(const std::string& input) { return change_alf(input, convert_t::alf(), convert_t::alf10()); }
std::string to_15(const std::string& input) { return change_alf(input, convert_t::alf(), convert_t::alf15()); }
std::string to_44(const std::string& input) { return change_alf(input, convert_t::alf(), convert_t::alf44()); }
std::string to_95(const std::string& input) { return change_alf(input, convert_t::alf(), convert_t::alf95()); }
std::string from_10(const std::string& input) { return change_alf(input, convert_t::alf10(), convert_t::alf()); }
std::string from_15(const std::string& input) { return change_alf(input, convert_t::alf15(), convert_t::alf()); }
std::string from_44(const std::string& input) { return change_alf(input, convert_t::alf44(), convert_t::alf()); }
std::string from_95(const std::string& input) { return change_alf(input, convert_t::alf95(), convert_t::alf()); }

std::string summ_str(const std::string& v1, const std::string& v2)
{
	std::stringstream ss;
	std::string result;

	int val1;
	int val2;
	int mem = 0;
	char ch;

	for (size_t i=0; i<((v1.length() > v2.length()) ? v1.length() : v2.length()); ++i)
	{
		if (v1.length() >= 1+i)
		{
			ss << v1[v1.length()-i-1]; ss >> val1;
			ss.clear();
		}
		else val1 = 0;
		if (v2.length() >= 1+i)
		{
			ss << v2[v2.length()-i-1]; ss >> val2;
			ss.clear();
		}
		else val2 = 0;
		int sum = val1 + val2 + mem;
		if (sum >= 10) { sum = sum % 10; mem = 1; }
		else mem = 0;
		ss << sum; ss >> ch; ss.clear();

		result = ch + result;
	}
	if (mem) result = '1' + result;
	return result;
}

std::string diff_str(const std::string& v1, const std::string& v2)
{
	std::stringstream ss;
	std::string result;

	int val1;
	int val2;
	int mem = 0;
	char ch;

	for (size_t i=0; i<((v1.length() > v2.length()) ? v1.length() : v2.length()); ++i)
	{
		if (v1.length() >= 1+i)
		{
			ss << v1[v1.length()-i-1]; ss >> val1;
			ss.clear();
		}
		else val1 = 0;
		if (v2.length() >= 1+i)
		{
			ss << v2[v2.length()-i-1]; ss >> val2;
			ss.clear();
		}
		else val2 = 0;

		int minus;
		if (val1 >= val2 + mem)
		{
			minus = val1 - mem - val2;
			mem = 0;
		}
		else
		{
			minus = 10 + val1 - mem - val2;
			mem = 1;
		}
		ss << minus; ss >> ch; ss.clear();
		result = ch + result;
	}
	return result;
}

std::string conv_password(const std::string& str)
{
	convert_t conv;
	std::string pass = str;
	pass = from_95(pass);
	pass = conv.go(95, 15, pass);
	pass = to_15(pass);
	return pass;
}

std::string conv_login(const std::string& str)
{
	convert_t conv;
	std::string login = str;
	while (login.length() < 10) login = login = login + "|";
	login = from_44(login);
	login = conv.go(44, 10, login);
	login = summ_str(login, ro);
	login = conv.go(10, 15, login);
	login = to_15(login);
	return login;
}

std::string unconv_password(const std::string& str)
{
	convert_t conv;
	std::string pass = str;
	pass = from_15(pass);
	pass = conv.go(15, 95, pass);
	pass = to_95(pass);
	return pass;
}

std::string unconv_login(const std::string& str)
{
	convert_t conv;
	std::string login = str;
	login = from_15(login);
	login = conv.go(15, 10, login);
	login = diff_str(login, ro);
	login = conv.go(10, 44, login);
	login = to_44(login);
	login = login.substr(0, login.find('|', 0));
	return login;
}

bool correct_ksi_record(const std::string& str)
{
	if (str.length() < 3) return false;
	if (str[0] == ';' && str[str.length()-1] == '?') {
		if (str[1] == '=' && str[2] == '=') return true;
	}
	return false;
}

size_t ksi_version(const std::string& str) {
	if (str.length() < 4) return 0;
	try {
		size_t res = boost::lexical_cast<size_t>(str[3]);
		return res;
	}
	catch (...) {
	}
	return 0;
}

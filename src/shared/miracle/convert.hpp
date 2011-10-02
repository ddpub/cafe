#ifndef DMITRY_DERGACHEV_18_02_2008_CONVERT_H
#define DMITRY_DERGACHEV_18_02_2008_CONVERT_H

#include <string>
#include <vector>

class convert_t
{
public:
	convert_t(int in_base_=10, int out_base_=10);
	std::string go(int in_base, int out_base, const std::string& input);
public:
	static const std::string& alf()
	{
		alf_ = "";
		for (size_t i=48; i<58; ++i)
			alf_ = alf_ + char(i);
		for (size_t i=1; i<48; ++i)
			alf_ = alf_ + char(i);
		for (size_t i=58; i<127; ++i)
			alf_ = alf_ + char(i);
		return alf_;
	}
	static const std::string& alf10() { return alf10_; }
	static const std::string& alf15() { return alf15_; }
	static const std::string& alf44() { return alf44_; }
	static const std::string& alf95()
	{
		alf95_ = "";
		for (size_t i=33; i<128; ++i)
			alf95_ = alf95_ + char(i);
		return alf95_;
	}

private:
	void add(size_t n);
	void mul(size_t n);
private:
	static std::string alf_;
	static std::string alf10_;
	static std::string alf15_;
	static std::string alf44_;
	static std::string alf95_;
	int in_base_;
	int out_base_;
	std::vector<size_t> buff_;
	std::string in_set_;
	std::string out_set_;
private:
	convert_t(const convert_t&);
	convert_t& operator=(const convert_t&);
};


std::string change_alf(const std::string& input, const std::string& in_alf, const std::string& out_alf);
std::string to_10(const std::string& input);
std::string to_15(const std::string& input);
std::string to_44(const std::string& input);
std::string to_95(const std::string& input);
std::string from_10(const std::string& input);
std::string from_15(const std::string& input);
std::string from_44(const std::string& input);
std::string from_95(const std::string& input);
std::string summ_str(const std::string& v1, const std::string& v2);
std::string diff_str(const std::string& v1, const std::string& v2);

std::string conv_password(const std::string& str);
std::string conv_login(const std::string& str);
std::string unconv_password(const std::string& str);
std::string unconv_login(const std::string& str);

bool correct_ksi_record(const std::string& str);
size_t ksi_version(const std::string& str);


#endif

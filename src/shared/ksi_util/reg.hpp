#ifndef DMITRY_DERGACHEV_27_03_2008_REG_H
#define DMITRY_DERGACHEV_27_03_2008_REG_H

#include <string>
#include <vector>
#include <map>

class param_t
{
public:
	param_t(): src_("") {}
	explicit param_t(const std::string& src_): src_(src_) {}
	void assign(const std::string source) { src_ = source; }
	std::pair<std::string, std::string> fetch(const std::string source) { src_ = source; return fetch(); }
	std::pair<std::string, std::string> fetch() const;
private:
	std::string src_;
private:
	param_t& operator=(const param_t&);
	param_t(const param_t&);
};

template <typename X, typename Y>
class base_reg_t
{
protected:
	typedef X key_t;
	typedef Y val_t;
	typedef std::map<key_t, val_t> kv_map_t;
public:
	base_reg_t() {}
	virtual ~base_reg_t() {}
	const val_t& get(const key_t& key) { return kv_[key]; }
	void set(const key_t& key, const val_t& val) { kv_[key] = val; }
	virtual void sync() = 0;
protected:
	kv_map_t kv_;
private:
	base_reg_t(const base_reg_t&);
	base_reg_t& operator=(const base_reg_t&);
};


class file_reg_t: public base_reg_t<std::string, std::string>
{
public:
	explicit file_reg_t(const std::string& file_):file_(file_) { load_form_file(); }
	~file_reg_t() {}
	virtual void sync() { save_to_file(); }
private:
	void load_form_file();
	void save_to_file();
private:
	std::string file_;
};


#endif

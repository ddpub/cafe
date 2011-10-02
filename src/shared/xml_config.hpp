#ifndef DMITRY_DERGACHEV_03_07_2008_XML_CONFIG_H
#define DMITRY_DERGACHEV_03_07_2008_XML_CONFIG_H

#include <string>
#include <memory>
#include <map>

class wxXmlDocument;

class xml_config_t
{
public:
	explicit xml_config_t(const std::string& file_);
	~xml_config_t();
	void set(const std::string& part, const std::string& key, const std::string& val);
	//void set(const std::string& key, const std::string& val, const std::map<std::string, std::string>& attr);
	std::string get(const std::string& part, const std::string& key);
	//std::map<std::string, std::string> get_attr(const std::string& key);
	void sync();
private:
	std::auto_ptr<wxXmlDocument> xml_doc_;
	std::string file_;
private:
	xml_config_t(const xml_config_t&);
	xml_config_t& operator=(const xml_config_t&);
};

#endif

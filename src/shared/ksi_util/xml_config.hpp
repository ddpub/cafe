#ifndef DMITRY_DERGACHEV_09_12_2008_XML_CONFIG_H
#define DMITRY_DERGACHEV_09_12_2008_XML_CONFIG_H

#include <string>
#include <map>
#include <memory>

class TiXmlDocument;

class xml_config_t {
public:
	xml_config_t(const std::string& file_);
	~xml_config_t();
	std::string get_attr(const std::string& part, const std::string& key);
	std::string get_text(const std::string& part);
	void sync();
private:
	std::auto_ptr<TiXmlDocument> xml_doc_;
	std::string file_;
private:
	xml_config_t();
	xml_config_t(const xml_config_t&);
	xml_config_t& operator=(const xml_config_t&);
};

#endif


#include "xml_config.hpp"
#include <ksi_include/ksi_exceptions.hpp>

#define TIXML_USE_STL
#include <tinyxml/tinyxml.h>



xml_config_t::xml_config_t(const std::string& file_): file_(file_) {
	xml_doc_.reset(new TiXmlDocument(file_));
	if (!xml_doc_->LoadFile(file_))
		THROW_MESSAGE(0, file_ + " " + boost::lexical_cast<std::string>(xml_doc_->ErrorDesc()), "xml_config_t::xml_config_t(const std::string&)"); 
}

std::string xml_config_t::get_text(const std::string& part) {
	for ( TiXmlNode* node = xml_doc_->FirstChild(); node != 0; node = node->NextSibling()) {
		if (node->Type() == TiXmlNode::ELEMENT) {
			if (node->ValueStr() == part) {
				const char* c_text = node->ToElement()->GetText();
				std::string str_val = "";
				if (c_text) str_val = std::string(c_text);
				return str_val;
			}
		}
	}
	return "";
}

std::string xml_config_t::get_attr(const std::string& part, const std::string& key) {
	for ( TiXmlNode* node = xml_doc_->FirstChild(); node != 0; node = node->NextSibling()) {
		if (node->Type() == TiXmlNode::ELEMENT) {
			if (node->ValueStr() == part) {
				std::map<std::string, std::string> attr_map;
				TiXmlAttribute* attr = node->ToElement()->FirstAttribute();
				while (attr) {
					attr_map[attr->NameTStr()] = attr->ValueStr();
					attr = attr->Next();
				}
				return attr_map[key];
			}
		}
	}
	return "";
}

void xml_config_t::sync() {
	xml_doc_->SaveFile(file_);
}

xml_config_t::~xml_config_t() {
	//sync();
}

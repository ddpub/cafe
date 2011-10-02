
#include "xml_config.hpp"
#include <wx/xml/xml.h>
#include <wx_util/wx_util.hpp>
#include <wx/file.h>
#include <ksi_include/ksi_exceptions.hpp>
#include <fstream>

xml_config_t::xml_config_t(const std::string& file_): file_(file_)
{
	xml_doc_.reset(new wxXmlDocument());
	{		
		if (!wxFile::Exists(to_uc(file_).c_str()))
		{
			//std::ofstream of (file_.c_str());
			//of << "<config></config>";
			//of.close();
			xml_doc_->SetRoot(new wxXmlNode(wxXML_ELEMENT_NODE, _("config")));    
		}
		else if (!xml_doc_->Load(to_uc(file_))) THROW_MESSAGE(0, "can't open file: " + file_, "xml_config_t::xml_config_t(const std::string&)")
	}
}

xml_config_t::~xml_config_t()
{
	sync();
}

void xml_config_t::sync()
{
	if (!xml_doc_->Save(to_uc(file_))) THROW_MESSAGE(0, "can't save file: " + file_, "void xml_config_t::sync()")
}

std::string xml_config_t::get(const std::string& part, const std::string& key)
{
	wxXmlNode *child = xml_doc_->GetRoot()->GetChildren();
	while (child)
	{
		if (child->GetName() == to_uc(part))
		{
			wxString res;
			if (child->GetPropVal(to_uc(key), &res)) return to_mb(res);
			return "";
		}
		child = child->GetNext();
	}
	return "";
}

void xml_config_t::set(const std::string& part, const std::string& key, const std::string& val)
{	
	wxXmlNode *child = xml_doc_->GetRoot()->GetChildren();	
	while (child)
	{
		if (child->GetName() == to_uc(part))
		{
			if (!child->HasProp(to_uc(key))) child->AddProperty(to_uc(key), to_uc(val));
			else
			{
				wxXmlProperty* prop = child->GetProperties();
				while (prop)
				{
					if (prop->GetName() == to_uc(key)) 
					{
						prop->SetValue(to_uc(val));
						return;
					}
					prop = prop->GetNext();
				}
			}
			return;
		}
		child = child->GetNext();
	}
	wxXmlNode* node = new wxXmlNode(wxXML_ELEMENT_NODE, to_uc(part));
	node->AddProperty(to_uc(key), to_uc(val));
	xml_doc_->GetRoot()->AddChild(node);	
}

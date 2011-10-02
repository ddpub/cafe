
#include "xml_check_printer.hpp"
#include <ksi_include/ksi_exceptions.hpp>
#define TIXML_USE_STL
#include <tinyxml/tinyxml.h>


//---------------------------------------------------------------------------

size_t width(const xml_attr::font_t& font)
{
	if (font == xml_attr::normal) return 12;
	else if (font == xml_attr::hx2) return 12;
	else if (font == xml_attr::hx2wx2) return 24;
	THROW_LOCATION(0, "size_t width(const xml_attr::font_t&)");
}

std::string align_str(const xml_attr::align_t& align)
{
	if (align == xml_attr::left) return "left";
	else if (align == xml_attr::center) return "center";
	else if (align == xml_attr::right) return "right";
	THROW_LOCATION(0, "std::string align_str(const xml_attr::align_t&)");
}

std::string font_str(const xml_attr::font_t& font)
{
	if (font == xml_attr::normal) return "normal";
	else if (font == xml_attr::hx2) return "hx2";
	else if (font == xml_attr::hx2wx2) return "hx2wx2";
	THROW_LOCATION(0, "std::string font_str(const xml_attr::font_t&)");
}

std::string column_t::xml_str() const
{
	return std::string("<col multiline=\"")
		    + (multiline_ ? "1" : "")
			+ "\" stretch = \""
			+ (stretch_ ? "1" : "")
			+ "\" align=\""
			+ align_str(align_)
			+ "\" lspace=\""
			+ boost::lexical_cast<std::string>(lspace_)
			+ "\" rspace=\""
			+ boost::lexical_cast<std::string>(rspace_)
			+ "\" lf=\""
			+ boost::lexical_cast<std::string>(lf_)
			+ "\" font=\""
			+ font_str(font_)
			+ "\">"
			+ text_
			+ "</col>\n";
}

std::string xml_table_t::xml_str() const
{
	std::string res = "<table>\n";
	for (size_t i=0; i<rows_.size(); ++i)
	{
		res += "<row>\n";
		for (size_t j=0; j<rows_[i].size(); ++j)
			res += rows_[i][j].xml_str();
		res += "</row>\n";
	}
	res += "</table>\n";
	return res;
}

std::vector<std::string> split(std::string text, size_t max_length)
{
	std::vector<std::string> words;
	std::string word;
	for (size_t i=0; i<text.length(); ++i)
	{
		if (text[i] == '\n') text[i] = ' ';
		if (text[i] != ' ') word = word + text[i];
		if (text[i] == ' ' && word != "")
		{
			words.push_back(word);
			word = "";
		}
	}
	if (word != "") words.push_back(word);
	std::vector<std::string> lines;
	std::string line;
  for (size_t i=0; i<words.size(); i++)
  {
    if (words[i].length() == 0) continue;
    if (line.length() == 0)
    {
      if (words[i].length() <= max_length)
      {
        line = words[i];
      }
      else
      {
        line = words[i].substr(0, max_length);
        lines.push_back(line);
        line = "";
        words[i] = words[i].substr(max_length, words[i].length()-max_length);
        i--;
      }
    }
    else
    {
      if (max_length - line.length() - 1 >= words[i].length())
      {
        line = line + " " + words[i];
      }
      else
      {
        lines.push_back(line);
        line = "";
        i--;
      }
    }
  }
  if (line != "") lines.push_back(line);

	return lines;
}

void check_printer_t::set_font(const xml_attr::font_t& font) const
{
	if (font == xml_attr::normal) printer_->set_font_size(0);
	else if (font == xml_attr::hx2) printer_->set_font_size(1);
	else if (font == xml_attr::hx2wx2) printer_->set_font_size(2);
	else THROW_LOCATION(0, "void check_printer_t::font(font_t&)");
}

void check_printer_t::fl() const
{
	for (size_t i=0; i< size_t(printer_->ccount_size()*12/printer_->wide_size()); ++i)
		printer_->print("-");
}


void check_printer_t::table(const xml_table_t& table) const
{
//	size_t char_size = size_t(printer_->wide_size());
	size_t char_count = size_t(printer_->ccount_size());
	size_t col_count = table.row(0).size();
	std::vector<size_t> col_len_vec(col_count);
	std::vector<size_t> col_lspace_vec(col_count);
	std::vector<size_t> col_rspace_vec(col_count);
	std::vector<bool> col_stretch_vec(col_count);

	for(size_t j=0; j<col_count; ++j) { col_len_vec[j] = 0; col_lspace_vec[j] = 0; col_rspace_vec[j] = 0; col_stretch_vec[j]=false; }


	for (size_t i=0; i<table.size(); ++i) {
		for (size_t j=0; j<table.row(i).size(); ++j) {
			size_t col_lspace = table.row(i)[j].lspace();
			size_t col_rspace = table.row(i)[j].rspace();
			if (col_lspace > col_lspace_vec[j]) col_lspace_vec[j] = col_lspace;
			if (col_rspace > col_rspace_vec[j]) col_rspace_vec[j] = col_rspace;

			if (table.row(i)[j].stretch()) col_stretch_vec[j] = true;
			size_t col_width = table.row(i)[j].text().length()*width(table.row(i)[j].font());
			if (col_width > col_len_vec[j]) col_len_vec[j] = col_width;
		}
	}

	size_t accumulate_len = 0;
	for (size_t j=0; j<table.row(0).size(); ++j) {
		if (col_stretch_vec[j]) {
			for (size_t i=0; i<col_len_vec.size(); ++i) {
				if (i != j) accumulate_len += col_len_vec[i] + col_lspace_vec[i] + col_rspace_vec[i];
			}
			col_len_vec[j] = char_count*12 - accumulate_len -col_lspace_vec[j]-col_rspace_vec[j];
			break;
		}
	}

	for (size_t i=0; i<table.size(); ++i) {
		for (size_t j=0; j<table.row(i).size(); ++j) {
			size_t curr_pos = 0;
			for (size_t k=0; k<j; ++k) curr_pos += col_len_vec[k]+col_lspace_vec[k] + col_rspace_vec[k];
			size_t char_width = width(table.row(i)[j].font());
			if (table.row(i)[j].lf() != 0) {
				printer_->absolute_move(curr_pos);
				for (size_t k=0; k<(col_lspace_vec[j]+col_rspace_vec[j]+col_len_vec[j])/char_width; ++k) {
					std::string text = "-";
					printer_->print(text);
				}
			}
			else if (table.row(i)[j].multiline()) {
				std::string text = table.row(i)[j].text();

				std::vector<std::string> lines = split(text, col_len_vec[j]/char_width);

				for (size_t k=0; k<lines.size(); ++k) {
					if (table.row(i)[j].align() == xml_attr::center) {
						printer_->absolute_move(curr_pos+table.row(i)[j].lspace()+(col_len_vec[j]-lines[k].length()*char_width)/2);
					}
					else if(table.row(i)[j].align() == xml_attr::right) {
						printer_->absolute_move(curr_pos+table.row(i)[j].lspace()+col_len_vec[j]-lines[k].length()*char_width);
					}
					else {
						printer_->absolute_move(curr_pos+table.row(i)[j].lspace());
					}
					set_font(table.row(i)[j].font());
					printer_->print(lines[k]);
					if (k<lines.size()-1) printer_->lf();
				}
			}
			else {
				std::string text = table.row(i)[j].text().substr(0, col_len_vec[j]);
				if (table.row(i)[j].align() == xml_attr::center) {
					printer_->absolute_move(curr_pos+table.row(i)[j].lspace()+(col_len_vec[j]-text.length()*char_width)/2);
				}
				else if (table.row(i)[j].align() == xml_attr::right) {
					printer_->absolute_move(curr_pos+table.row(i)[j].lspace()+col_len_vec[j]-text.length()*char_width);
				}
				else {
					printer_->absolute_move(curr_pos+table.row(i)[j].lspace());
				}
				set_font(table.row(i)[j].font());
				printer_->print(text);
			}
		}
		printer_->lf();
	}
}

void print_xml(base_printer_t* printer, const std::string& message) {
	if (!printer) THROW_LOCATION(0, "void print_xml(base_printer_t*, const std::string&)");
	check_printer_t p(printer);
	
	std::string xml_ = message;
	if (message.find("<?xml") == std::string::npos)
		xml_ = std::string("<?xml version=\"1.0\" encoding=\"windows-1251\"?><receipt>")+message+std::string("</receipt>");
	TiXmlDocument xml("receipt.xml");
	xml.Parse(xml_.c_str());

	for ( TiXmlNode* node = xml.FirstChild("receipt")->FirstChild(); node != 0; node = node->NextSibling())
	{
		if (node->Type() != TiXmlNode::ELEMENT) continue;
		if (node->ValueStr() == "bell") p.bell();			
		else if (node->ValueStr() == "open_drawer") p.open_drawer();
		else if (node->ValueStr() == "test") p.test();
		else if (node->ValueStr() == "lf") p.lf();
		else if (node->ValueStr() == "cut") p.cut();
		else if (node->ValueStr() == "logo") p.logo();
		else if (node->ValueStr() == "fl") {
			TiXmlAttribute* attr = node->ToElement()->FirstAttribute();
			std::map<std::string, std::string> attr_map;
			while (attr) {
				attr_map[attr->NameTStr()] = attr->ValueStr();
				attr = attr->Next();
			}
			if (attr_map["font"] == "normal") p.set_font(xml_attr::normal);
			else if (attr_map["font"] == "hx2") p.set_font(xml_attr::hx2);
			else if (attr_map["font"] == "hx2wx2") p.set_font(xml_attr::hx2wx2);
			p.fl();
		}
		else if (node->ValueStr() == "barcode") {
			std::string text = node->ToElement()->GetText();
			if (text != "") {
				p.barcode(text);
			}
		}
		else if (node->ValueStr() == "table")
		{
			std::vector<row_t> rows;
			for (TiXmlNode* row_node = node->FirstChild(); row_node != 0; row_node = row_node->NextSibling())
			{
				row_t cols;
				for (TiXmlNode* col_node = row_node->FirstChild(); col_node != 0; col_node = col_node->NextSibling())
				{
					std::map<std::string, std::string> attr_map;
					{
						TiXmlAttribute* attr = col_node->ToElement()->FirstAttribute();
						while (attr)
						{
							attr_map[attr->NameTStr()] = attr->ValueStr();
							attr = attr->Next();
						}
					}
					bool multiline = true;
					if (attr_map["multiline"] == "") multiline = false;
					bool stretch = true;
					if (attr_map["stretch"] == "") stretch = false;
					xml_attr::align_t align = xml_attr::left;
					if (attr_map["align"] == "right") align = xml_attr::right;
					else if (attr_map["align"] == "center") align = xml_attr::center;
					size_t lspace = 0;
					if (attr_map["lspace"] != "") lspace = boost::lexical_cast<size_t>(attr_map["lspace"]);
					size_t rspace = 0;
					if (attr_map["rspace"] != "") rspace = boost::lexical_cast<size_t>(attr_map["rspace"]);
					xml_attr::font_t font = xml_attr::normal;
					if (attr_map["font"] == "hx2") font = xml_attr::hx2;
					else if (attr_map["font"] == "hx2wx2") font = xml_attr::hx2wx2;
					size_t lf = 0;
					if (attr_map["lf"] != "") lf = boost::lexical_cast<size_t>(attr_map["lf"]);

					std::string text;
					for (TiXmlNode* find_text_node = col_node->FirstChild(); find_text_node != 0; find_text_node = find_text_node->NextSibling())
					{
						if (find_text_node->Type() != TiXmlNode::TEXT) continue;
						text = find_text_node->ToText()->ValueStr();
						break;
					}
					cols.push_back(column_t(text, multiline, stretch, align, lspace, rspace, font, lf));
				}
				rows.push_back(cols);
			}
			if (rows.size()) p.table(xml_table_t(rows));
		}
	}
	printer->done(xml_);
}

/*
 * =====================================================================================
 *
 *       Filename:  pos.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  24.12.2007 13:55:08 MSK
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pavel G. Koukoushkin (), koukoushkinpg@ksi.ru
 *        Company:  Phlint and K, Moscow, Russia
 *
 * =====================================================================================
 */

#include "pos.hpp"
#include <oracle_client/ksi_exception.hpp>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <map>
#include <tinyxml/tinyxml.h>
#include <boost/regex.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <ostream>
#include <istream>
#include <ksi_util/log.hpp>



using namespace std;
using namespace boost;

const char* request_template = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
  "<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\""
  " xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\""
  " xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">"
  "<soap:Body>"
  "<PosRequest clientType=\"PosGatewayClient\" clientVer=\"1.0.0.0\""
  " xmlns=\"http://Hps.Exchange.PosGateway\"><Ver1.0>"
  "</Ver1.0></PosRequest></soap:Body></soap:Envelope>";

string explain_hps_error_code(int gateway_rsp_code)
{
  switch (gateway_rsp_code)
  {
  case -2:
    return "Invalid license";
  case -3:
    return "Inactive license";
  case -4:
    return "Expired license";
  case -5:
    return "Invalid site";
  case -6:
    return "Inactive site";
  case -7:
    return "Expired site";
  case -8:
    return "Invalid device";
  case -9:
    return "Inactive device";
  case -10:
    return "Expired device";
  case -11:
    return "Invalid user";
  case -12:
    return "Invalid user password";
  case -13:
    return "Inactive user";
  case -14:
    return "Expired user";
  case -15:
    return "Invalid service";
  case -16:
    return "Inactive service";
  case -17:
    return "Expired service";
  case -18:
    return "Invalid device service";
  case -19:
    return "Inactive device service";
  case -20:
    return "Expired device service";
  case -21:
    return "Unauthorized";
  case +1:
    return "Gateway system error";
  case +2:
    return "Duplicate transactions";
  case +3:
    return "Invalid original transaction";
  case +4:
    return "Transaction already associated with batch";
  case +5:
    return "No current batch";
  case +6:
    return "Transaction not associated with batch";
  case +7:
    return "Invalid report parameters";
  case +8:
    return "Bad track data";
  case +9:
    return "No transaction associated with batch";
  case +10:
    return "Empty report";
  case +11:
    return "Original transaction not CPC";
  case +12:
    return "Invalid CPC data";
  case +13:
    return "Invalid edit data";
  case +14:
    return "Invalid card number";
  case +15:
    return "Batch close in progress";
  default: return "Heartland payment system returned unexpected error";
  }
}

void iterate_children(const TiXmlNode* node)
{
  const TiXmlNode* elt = node->FirstChild();
  if (!elt)
    return;
  do
  { 
    if (elt->Type() == TiXmlNode::ELEMENT)
      cout << dynamic_cast<const TiXmlElement*>(elt)->ValueStr() << endl;
    else
      cout << "Not an element" << endl;
  }
  while (elt = elt->NextSibling());
}

template<class T>
void append_node(TiXmlNode* parent, const std::string& name, T value)
{
  TiXmlElement* elt = new TiXmlElement(name);
  string str_val = boost::lexical_cast<std::string>(value);
  TiXmlText* text = new TiXmlText(str_val);
  elt->LinkEndChild(text);
  parent->LinkEndChild(elt);
}

struct __getter
{
  const TiXmlNode* __node;
  auto_ptr<__getter> __child;
  __getter(const TiXmlNode* parent) : __node(parent) 
    {   }

  __getter& get(const std::string& name)
    {
      const TiXmlNode* curr = __node->FirstChild(name.c_str());
      while(curr != NULL && curr->Type() != TiXmlNode::ELEMENT)
      {
        curr = curr->NextSibling(name.c_str());
      }
      if (!curr)
      {
        //     iterate_children(__node);
        throw ksi_client::fatal_pos_exception(0, string("XML format error tag not found: ") + name, 0);
      }
      __child.reset(new __getter(curr));
      return *__child;
    }

  template<class T>
  void get_value(const std::string& name, T& value)
    {
      const TiXmlNode* curr = __node->FirstChild(name);
      while(curr != NULL && curr->Type() != TiXmlNode::ELEMENT)
      {
        curr = curr->NextSibling(name.c_str());
      }
      if (!curr || curr->Type() != TiXmlNode::ELEMENT)
      {
        //       iterate_children(__node);
        throw ksi_client::fatal_pos_exception(0, string("XML format error tag not found: ") + name, 0);
      }
      const char* c_text = dynamic_cast<const TiXmlElement*>(curr)->GetText();
      string str_val = "";
      if (c_text)
        str_val = string(c_text);
      try
      {
        value = lexical_cast<T>(str_val);
      }
      catch (bad_cast& e)
      {
        stringstream errss;
        errss << "Bad cast problem for parameter \"" << name << "\" " << "with "
          "string value \"" << str_val << '\"';
        throw ksi_client::fatal_pos_exception(0, errss.str(), 0);
      }
    }
};


struct __setter
{
  TiXmlNode* __parent;
  std::auto_ptr<__setter> __child_obj;
  __setter(TiXmlNode* node)
    : __parent(node)
    {
    }
  
  __setter& add_node(const std::string& node_name)
    {
      TiXmlElement* elt = new TiXmlElement(node_name);
      TiXmlNode* elt_node = dynamic_cast<TiXmlNode*>(elt);
      __parent->LinkEndChild(elt_node);
      __child_obj.reset(new __setter(elt));
      return *__child_obj;
    }

  __setter& select_node(const std::string& node_name)
    {
      TiXmlNode* found = __parent->FirstChild(node_name);
      if (!found)
        return add_node(node_name);
      __child_obj.reset(new __setter(found));
      return *__child_obj;
    }

  template <class T>
  void add_node_value(const string& node_name, const T& value)
    {
      string text_value = lexical_cast<std::string>(value);
      TiXmlElement* elt = new TiXmlElement(node_name);
      TiXmlText* ti_text = new TiXmlText(text_value);
      elt->LinkEndChild(ti_text);
      TiXmlNode* elt_node = dynamic_cast<TiXmlNode*>(elt);
      __parent->LinkEndChild(elt_node);
    }
};

namespace ksi_client
{

  std::string amount_cast(double amt)
  {
    std::stringstream retval;
    int fl = int((amt - int(amt)) * 100 + .5);
    retval << ((amt < 0) ? "-" : "") << int(amt) << "." << (fl < 10 ? "0" : "") << fl;
    return retval.str();
  }
  class user_pos_impl : public ksi_client::user_pos_i
  {
  private:
    std::auto_ptr<TiXmlElement> __xml_header;
    bool __do_report;
    map<int, string> __err_list;
    map<int, string> __auth_code_cache;
    struct __txn_detail_t
    { 
      std::string card_holder_first_name;
      std::string card_holder_last_name;
      std::string card_type;
    };
    std::map<int, __txn_detail_t> __txn_detail_cache;
    void __init_err_list()
      {
        typedef pair<int, string> __pair_t;
        __err_list.insert(__pair_t(0, ""));
        __err_list.insert(__pair_t(-2, "Invalid license"));
        __err_list.insert(__pair_t(-3, "Inactive license"));
        __err_list.insert(__pair_t(-4, "Expired license"));
        __err_list.insert(__pair_t(-5, "Invalid site"));
        __err_list.insert(__pair_t(-6, "Inactive site"));
        __err_list.insert(__pair_t(-7, "Expired site"));
        __err_list.insert(__pair_t(-8, "Invalid device"));
        __err_list.insert(__pair_t(-9, "Inactive device"));
        __err_list.insert(__pair_t(-10, "Expired device"));
        __err_list.insert(__pair_t(-11, "Invalid user"));
        __err_list.insert(__pair_t(-12, "Invalid user password"));
        __err_list.insert(__pair_t(-13, "Inactive user"));
        __err_list.insert(__pair_t(-14, "Expired user"));
        __err_list.insert(__pair_t(-15, "Invalid service"));
        __err_list.insert(__pair_t(-16, "Inactive service"));
        __err_list.insert(__pair_t(-17, "Expired service"));
        __err_list.insert(__pair_t(-18, "Invalid device service"));
        __err_list.insert(__pair_t(-19, "Inactive device service"));
        __err_list.insert(__pair_t(-20, "Expired device service"));
        __err_list.insert(__pair_t(1, "Gateway system error"));
        __err_list.insert(__pair_t(2, "Duplicate transactions"));
        __err_list.insert(__pair_t(3, "Invalid original transaction"));
        __err_list.insert(__pair_t(4, "Transaction already associated with batch"));
        __err_list.insert(__pair_t(5, "No current batch"));
        __err_list.insert(__pair_t(6, "Transaction not associated with batch"));
        __err_list.insert(__pair_t(7, "Invalid report parameters"));
        __err_list.insert(__pair_t(8, "Bad track data"));
        __err_list.insert(__pair_t(9, "No transaction associated with batch"));
        __err_list.insert(__pair_t(10, "Empty report"));
      }
    void __do_report_txn_detail(int txn_id);
    static void __check_header(const TiXmlNode* node);
  public:
    user_pos_impl() : __do_report(false) { __init_err_list(); };
  
    user_pos_impl(
      const int license_id,
      const int site_id,
      const int device_id,
      const std::string& username,
      const std::string& password
      )
      : __xml_header(new TiXmlElement("Header"))
      {
        append_node(__xml_header.get(),  "LicenseId", license_id);
        append_node(__xml_header.get(), "SiteId", site_id);
        append_node(__xml_header.get(), "DeviceId", device_id);
        append_node(__xml_header.get(), "UserName", username);
        append_node(__xml_header.get(), "Password", password);
        __init_err_list();
      };

    virtual int credit_auth(
      double amt,
      double gratuity_amount,
      const std::string& card_nbr,
      const int exp_month,
      const int exp_year,
      const std::string& cvv2 = "",
      const bool card_present = false,
      const bool reader_present = false);
    virtual int credit_auth(
      double amt,
      double gratuity_amount,
      const std::string& track_data);
    virtual int test_credentials();
    virtual int credit_add_to_batch(const int txn_id, double amt = 0, double amt_info = 0);
    virtual int credit_void(const int tnx_id);
    virtual int credit_return(double total, const std::string& track_data);
    virtual int credit_return(double total, const std::string& cc_num, int exp_month, int exp_year, const std::string& cvv2, bool card_present = true, bool reader_present = true);
    virtual string get_card_holder_first_name(int txn_id);
    virtual string get_card_holder_last_name(int txn_id);
    virtual string get_card_type(int txn_id);
    virtual const string& get_auth_code(int txn_id);
    virtual int get_current_batch_id();
    virtual bool get_do_report() { return __do_report; };
    virtual void set_do_report(bool do_report) { __do_report = do_report; };
  };
};


std::auto_ptr<ksi_client::user_pos_i> ksi_client::create_user_pos(const int license_id,
                                                                  const int site_id, 
                                                                  const int device_id,
                                                                  const std::string& username,
                                                                  const std::string& password)
{ 
  return auto_ptr<user_pos_i>(new user_pos_impl(
                                license_id,
                                site_id, 
                                device_id,
                                username,
                                password)
    );
}

#define TRANSACTION(NAME, VARIABLE, HPS_VARIABLE)                       \
  ver_1_0_req_t req;                                                    \
  transaction_req_t transaction_req;                                    \
  req.set_Header(__header);                                             \
  transaction_req.set_ ##NAME(VARIABLE);                                \
  req.set_Transaction(transaction_req);                                 \
  hps_type_t ht = do_request(req.data());                               \
  ver_1_0_rsp_t rsp(ht);                                                \
  int code = rsp.get_Header().get_GatewayRspCode();                     \
  if (code)                                                             \
  {                                                                     \
    stringstream err;                                                   \
    string msg = rsp.get_Header(0).get_GatewayRspMsg();                 \
    err << "Error with code: " << code << "; ";                         \
    if (msg.size()) err << "And message: " << msg;                      \
    else if (__err_list.find(code) != __err_list.end()) err << "And message: " << __err_list[code]; \
    msg = "";                                                           \
    msg = string(err.str().c_str());                                    \
    throw logical_pos_exception(0, msg, 0);                             \
  }                                                                     \
  HPS_VARIABLE = hps_type_t(rsp.get_Transaction().get_ ##NAME().data());

void ksi_client::user_pos_impl::__check_header(const TiXmlNode* node)
{}

TiXmlNode* find_node_by_name(TiXmlNode* parent, const std::string& text)
{
  if (parent->NoChildren()) return NULL;
  TiXmlNode *target = parent->FirstChild(text);
  if (target)
    return target;
  target = parent->FirstChild();
  do
  {
    TiXmlNode* ret = find_node_by_name(target, text);
    if (ret)
      return ret;
  }
  while (target = target->NextSibling());
  return NULL;
}

TiXmlNode* prepare_document(TiXmlDocument * doc, const TiXmlNode* header)
{
  doc->Parse(request_template);
  TiXmlNode* root_node =
    find_node_by_name(dynamic_cast<TiXmlNode*>(doc->RootElement()),
                      string("Ver1.0"));
  root_node->InsertEndChild(*(header));
  return __setter(root_node).add_node("Transaction").__parent;
}


const TiXmlNode* preparse_response(const TiXmlDocument* doc, int* txn_id_p =
                                   NULL)
{
  const TiXmlNode* root_node = dynamic_cast<const TiXmlNode*>(doc->RootElement());
  int rsp_code = 0;
  const TiXmlNode* ver_10 =
    find_node_by_name(const_cast<TiXmlNode*>(root_node), "Ver1.0");
  if (!ver_10)
  {
    const TiXmlNode* soap_error_text =
      find_node_by_name(const_cast<TiXmlNode*>(root_node), "soap:Reason");
    if (!soap_error_text)
      throw ksi_client::fatal_pos_exception(0, string("XML format error"), 0);
    string err_text;
    __getter(soap_error_text).get_value("soap:Text", err_text);
    throw ksi_client::fatal_pos_exception(0, string("XML format error: ") +
                                          err_text, 0);
  }
  __getter(ver_10).get("Header").get_value("GatewayRspCode", rsp_code);
  if (rsp_code)
  {
    string rsp_text;
    try
    {
      __getter(ver_10).get("Header").get_value("GatewayRspMsg", rsp_text);
      rsp_text = string("Hearland payment system returned error: ")  + rsp_text;
    }
    catch (ksi_client::pos_exception& e)
    {
      rsp_text = explain_hps_error_code(rsp_code);
    }
    catch (...)
    {
      rsp_text = "Unexpected error while get response message.";
    }
    throw ksi_client::logical_pos_exception(0, string("Transaction error: ")
                                            + rsp_text, rsp_code);
  }
  if (txn_id_p)
    __getter(ver_10).get("Header").get_value("GatewayTxnId", *txn_id_p);\
  try
  {
    const TiXmlNode* retval = __getter(ver_10).get("Transaction").__node;
    return retval;
  }
  catch (ksi_client::pos_exception&){ }
  return NULL;
}

void process(const string& req, string& resp)
{
  int count = 0;
  for (;; ++count)
  {
    std::auto_ptr<file_log_t> flog;
    try {
      flog.reset(new file_log_t("xml_trace.log", 10));
    }
    catch (...) {
      throw ksi_client::fatal_pos_exception(1, "can't create file_log_t", 1);
    }
    boost::asio::streambuf response;
    try
    {
      *flog << "---REQUEST--TRY-" << count << "--";
      *flog << req;
      *flog << "---END---REQUEST-TRY-" << count << "--";

      boost::asio::io_service io_service;
      boost::asio::ip::tcp::resolver resolver(io_service);
      boost::asio::ip::tcp::resolver::query query("posgateway.secureexchange.net", "https");
      boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
      boost::asio::ip::tcp::resolver::iterator endpoint_end;
    
      boost::asio::ssl::context ctx(io_service, boost::asio::ssl::context::sslv23);

      boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket (io_service,
                                                                     ctx);
      boost::system::error_code error = boost::asio::error::host_not_found;
      while (error && endpoint_iterator != endpoint_end)
      {
        socket.lowest_layer().close();
        socket.lowest_layer().connect(*endpoint_iterator++, error);
      }
      if (error)
        throw boost::system::system_error(error);

      socket.handshake(boost::asio::ssl::stream_base::client);

      boost::asio::streambuf request;
      std::ostream request_stream(&request);

      request_stream << "POST /Hps.Exchange.PosGateway/PosGatewayService.asmx HTTP/1.1\n";
      request_stream << "Host: posgateway.secureexchange.net\n";
      request_stream << "Content-Type: text/xml\n";
      request_stream << "Content-Length:" << req.size();
      request_stream << "\r\n\r\n";
      request_stream << req;
    
      boost::asio::write(socket, request);

      istream response_stream(&response);
      boost::asio::read_until(socket, response, "\r\n");

      std::string http_version;
      response_stream >> http_version;
      unsigned int status_code;
      response_stream >> status_code;
      std::string status_message;
      std::getline(response_stream, status_message);
      if (!response_stream || http_version.substr(0, 5) != "HTTP/")
      {
        std::cout << "Invalid response\n";
      }
      if (status_code != 200)
      {
        std::cout << "Response returned with status code " << status_code << "\n";
      }


      boost::asio::read_until(socket, response, "\r\n\r\n"); 
    
      string header_item;
      int pos;
      int len = 0;
      while ((pos = header_item.find("Content-Length")) == string::npos)
        getline(response_stream, header_item);
   
      string str_len = header_item.substr(16, header_item.size() - 17); 
      len = boost::lexical_cast<int>(str_len);

      boost::asio::read_until(socket, response, "</soap:Envelope>", error);
      if (error != 0)
        throw boost::system::system_error(error);
      stringstream ss;
      ss << &response;

      resp = ss.str();

      *flog << "---RESPONSE---";
      *flog << resp;
      *flog << "---END---RESPONSE---";
      return;
    }
    catch (boost::system::system_error& e)
    {
      stringstream ss;
      ss << &response;
      *flog << "---CORRUPTED-RESPONSE---";
      *flog << ss.str();
      *flog << "---END-CORRUPTED-RESPONSE---";
      string s = "Communication error: Network problem: ";
      if (count == 2)
        throw ksi_client::fatal_pos_exception(0, s + e.what(), 0);
    }
    catch (std::exception& e)
    {
      stringstream ss;
      ss << &response;
      *flog << "---CORRUPTED-RESPONSE---";
      *flog << ss.str();
      *flog << "---END-CORRUPTED-RESPONSE---";
      string s = "Communication error: Problem: ";
      throw ksi_client::fatal_pos_exception(0, s + e.what(), 0);
    }
  }
}

void ksi_client::user_pos_impl::__do_report_txn_detail(int txn_id)
{
  auto_ptr<TiXmlDocument> doc  (new TiXmlDocument());
  TiXmlNode* root_node = prepare_document(doc.get(), __xml_header.get());
  __setter(root_node).add_node("ReportTxnDetail").add_node_value("TxnId", txn_id);
  TiXmlPrinter prntr;
  doc->Accept(&prntr);
  string resp;
  process(prntr.Str(), resp);
  doc.reset(new TiXmlDocument());
  doc->Parse(resp.c_str());
  const TiXmlNode* resp_root_node = preparse_response(doc.get());
  __getter data
    (__getter(resp_root_node).get("ReportTxnDetail").get("Data").__node);
  __txn_detail_t tdc;
  data.get_value("CardHolderLastName", tdc.card_holder_last_name);
  data.get_value("CardHolderFirstName", tdc.card_holder_first_name);
  data.get_value("CardType", tdc.card_type);
  __txn_detail_cache[txn_id] = tdc;
}

const string& ksi_client::user_pos_impl::get_auth_code(int txn_id)
{
  if (__auth_code_cache.end() == __auth_code_cache.find(txn_id))
    throw fatal_pos_exception(0, "There are no CreditAuth transaction with such TxnId");
  return __auth_code_cache[txn_id];
}

int ksi_client::user_pos_impl::credit_auth(
  double amt,
  double gratuity_amount,
  const std::string& card_nbr,
  const int exp_month,
  const int exp_year,
  const std::string& cvv2,
  const bool card_present,
  const bool reader_present)
{ 
  auto_ptr<TiXmlDocument> doc  (new TiXmlDocument());
  TiXmlNode* root_node = prepare_document(doc.get(), __xml_header.get());
  __setter block_1_setter
    (__setter(root_node).add_node("CreditAuth").add_node("Block1").__parent);

  __setter manual_entry
    (block_1_setter.add_node("CardData").add_node("ManualEntry").__parent);
  manual_entry.add_node_value("CardNbr", card_nbr);
  manual_entry.add_node_value("ExpMonth", exp_month);
  manual_entry.add_node_value("ExpYear", exp_year);
  manual_entry.add_node_value("CardPresent", card_present ? "Y" : "N");
  manual_entry.add_node_value("ReaderPresent", reader_present ? "Y" : "N");
  if (cvv2.size())
    manual_entry.add_node_value("CVV2", cvv2);
  
  block_1_setter.add_node_value("Amt", amount_cast(amt));
  block_1_setter.add_node_value("GratuityAmtInfo",
                                amount_cast(gratuity_amount));
  block_1_setter.add_node_value("AllowDup", "N");

  TiXmlPrinter prntr;
  doc->Accept(&prntr);
  string resp;
  process(prntr.Str(), resp);
  doc.reset(new TiXmlDocument());
  doc->Parse(resp.c_str());
  
  int txn_id;
  const TiXmlNode* resp_root_node = preparse_response(doc.get(), &txn_id);
  string auth_code;
  string rsp_code;
  __getter(resp_root_node).get("CreditAuth").get_value("RspCode", rsp_code);
  __getter(resp_root_node).get("CreditAuth").get_value("AuthCode", auth_code);
  __auth_code_cache[txn_id] = auth_code;

  if (rsp_code != "00")
  { 
    stringstream ss;
    ss << "Transaction rejected with error: ";
    ss << rsp_code << ";";
    string msg;
    __getter(resp_root_node).get("CreditAuth").get_value("RspText", msg);
    
    if (msg.size())
      ss << " Error text: " << msg;
    msg = string(ss.str().c_str());
    throw logical_pos_exception(0, msg, 0);
  }
  
  if (__do_report)
  {
    try
    {
      __do_report_txn_detail(txn_id);
    }
    catch (pos_exception&)
    {
    }
  }


  return txn_id;
}


int ksi_client::user_pos_impl::credit_auth(
  double amt,
  double gratuity_amount,
  const std::string& track_data)
{
  auto_ptr<TiXmlDocument> doc  (new TiXmlDocument());
  TiXmlNode* root_node = prepare_document(doc.get(), __xml_header.get());
  __setter block_1
    (__setter(root_node).add_node("CreditAuth").add_node("Block1").__parent);
  block_1.add_node("CardData").add_node_value("TrackData", track_data);
  block_1.add_node_value("Amt", amount_cast(amt));
  block_1.add_node_value("GratuityAmtInfo", amount_cast(gratuity_amount));
  block_1.add_node_value("AllowDup", "N");

  TiXmlPrinter prntr;
  doc->Accept(&prntr);
  string resp;
  process(prntr.Str(), resp);
  doc.reset(new TiXmlDocument());
  doc->Parse(resp.c_str());
  int txn_id;
  const TiXmlNode* resp_root_node = preparse_response(doc.get(), &txn_id);
  string auth_code;
  string rsp_code;
  __getter(resp_root_node).get("CreditAuth").get_value("RspCode", rsp_code);
  __getter(resp_root_node).get("CreditAuth").get_value("AuthCode", auth_code);
  __auth_code_cache[txn_id] = auth_code;

  if (rsp_code != "00")
  {
    stringstream ss;
    ss << "Transaction rejected with error: ";
    ss << rsp_code << ";";
    string msg;
    __getter(resp_root_node).get("CreditAuth").get_value("RspText", auth_code);
    
    if (msg.size())
      ss << " Error text: " << msg;
    msg = string(ss.str().c_str());
    throw logical_pos_exception(0, msg, 0);
  }
 

  if (__do_report)
  {
    try
    {
      __do_report_txn_detail(txn_id);
    }
    catch(pos_exception& )
    {
    }
  }

  return txn_id;
}


int ksi_client::user_pos_impl::test_credentials()
{
  auto_ptr<TiXmlDocument> doc  (new TiXmlDocument());
  TiXmlNode* root_node = prepare_document(doc.get(), __xml_header.get());
  __setter(root_node).add_node("TestCredentials");
  TiXmlPrinter prntr;
  doc->Accept(&prntr);
  string resp;
  process(prntr.Str(), resp);
  doc.reset(new TiXmlDocument());
  doc->Parse(resp.c_str());
  int txn_id;
  const TiXmlNode* resp_root_node = preparse_response(doc.get(), &txn_id);
  return txn_id;
}

int ksi_client::user_pos_impl::credit_add_to_batch(const int txn_id, double amt, double amt_info)
{
  auto_ptr<TiXmlDocument> doc  (new TiXmlDocument());
  TiXmlNode* root_node = prepare_document(doc.get(), __xml_header.get());
  __setter credit_add_to_batch
    (__setter(root_node).add_node("CreditAddToBatch").__parent);
  credit_add_to_batch.add_node_value("GatewayTxnId", txn_id);
  if (amt)
  {
    credit_add_to_batch.add_node_value("Amt", amount_cast(amt));
    if (amt_info)
      credit_add_to_batch.add_node_value("GratuityAmtInfo", amount_cast(amt_info));
  }
  TiXmlPrinter prntr;
  doc->Accept(&prntr);
  string resp;
  process(prntr.Str(), resp);
  doc.reset(new TiXmlDocument());
  doc->Parse(resp.c_str());
  int resp_txn_id;
  const TiXmlNode* resp_root_node = preparse_response(doc.get(), &resp_txn_id);
  return resp_txn_id;
}


int ksi_client::user_pos_impl::credit_void(const int txn_id)
{
  auto_ptr<TiXmlDocument> doc  (new TiXmlDocument());
  TiXmlNode* root_node = prepare_document(doc.get(), __xml_header.get());
  __setter(root_node).add_node("CreditVoid").add_node_value("GatewayTxnId", txn_id);
  TiXmlPrinter prntr;
  doc->Accept(&prntr);
  string resp;
  process(prntr.Str(), resp);
  doc.reset(new TiXmlDocument());
  doc->Parse(resp.c_str());
  int resp_txn_id;
  const TiXmlNode* resp_root_node = preparse_response(doc.get(), &resp_txn_id);
  return resp_txn_id;
}

int ksi_client::user_pos_impl::credit_return(double total, const std::string& track_data)
{
  auto_ptr<TiXmlDocument> doc  (new TiXmlDocument());
  TiXmlNode* root_node = prepare_document(doc.get(), __xml_header.get());
  __setter block_1
    (__setter(root_node).add_node("CreditReturn").add_node("Block1").__parent);
  block_1.add_node("CardData").add_node_value("TrackData", track_data);
  block_1.add_node_value("Amt", amount_cast(total));

  TiXmlPrinter prntr;
  doc->Accept(&prntr);
  string resp;
  process(prntr.Str(), resp);
  doc.reset(new TiXmlDocument());
  doc->Parse(resp.c_str());
  int txn_id;
  const TiXmlNode* resp_root_node = preparse_response(doc.get(), &txn_id);
  return txn_id;
}

int ksi_client::user_pos_impl::credit_return(double total, const std::string& cc_num, int exp_month, int exp_year, const std::string& cvv2, bool card_present, bool reader_present)
{
  auto_ptr<TiXmlDocument> doc  (new TiXmlDocument());
  TiXmlNode* root_node = prepare_document(doc.get(), __xml_header.get());
  __setter block_1
    (__setter(root_node).add_node("CreditReturn").add_node("Block1").__parent);
  __setter manual_entry
    (block_1.add_node("CardData").add_node("ManualEntry").__parent);
  manual_entry.add_node_value("CardNbr", cc_num);
  manual_entry.add_node_value("ExpMonth", exp_month);
  manual_entry.add_node_value("ExpYear", exp_year);
  manual_entry.add_node_value("CardPresent", card_present ? "Y" : "N");
  manual_entry.add_node_value("ReaderPresent", reader_present ? "Y" : "N");
  if (cvv2.size())
    manual_entry.add_node_value("CVV2", cvv2);
  block_1.add_node_value("Amt", amount_cast(total));

  TiXmlPrinter prntr;
  doc->Accept(&prntr);
  string resp;
  process(prntr.Str(), resp);
  doc.reset(new TiXmlDocument());
  doc->Parse(resp.c_str());
  int txn_id;
  const TiXmlNode* resp_root_node = preparse_response(doc.get(), &txn_id);
  return txn_id;
}


string ksi_client::user_pos_impl::get_card_holder_first_name(int txn_id)
{
  if (__txn_detail_cache.find(txn_id) == __txn_detail_cache.end())
    __do_report_txn_detail(txn_id);
  return __txn_detail_cache[txn_id].card_holder_first_name;
}

string ksi_client::user_pos_impl::get_card_holder_last_name(int txn_id)
{
  if (__txn_detail_cache.find(txn_id) == __txn_detail_cache.end())
    __do_report_txn_detail(txn_id);
  return __txn_detail_cache[txn_id].card_holder_last_name;
}

string ksi_client::user_pos_impl::get_card_type(int txn_id)
{
  if (__txn_detail_cache.find(txn_id) == __txn_detail_cache.end())
    __do_report_txn_detail(txn_id);
  return __txn_detail_cache[txn_id].card_type;
}

int ksi_client::user_pos_impl::get_current_batch_id()
{
  auto_ptr<TiXmlDocument> doc  (new TiXmlDocument());
  TiXmlNode* root_node = prepare_document(doc.get(), __xml_header.get());
  __setter(root_node).add_node("ReportBatchDetail");
  TiXmlPrinter prntr;
  doc->Accept(&prntr);
  string resp;
  process(prntr.Str(), resp);
  doc.reset(new TiXmlDocument());
  doc->Parse(resp.c_str());
  int txn_id = 0;
  const TiXmlNode* resp_root_node;
  try
  {
    resp_root_node = preparse_response(doc.get(), &txn_id);
  }
  catch (pos_exception& e)
  {
    if (10 != e.code())
      throw fatal_pos_exception(e.msg_id(), e.what(), e.code());
    return 0;
  }
  int batch_id;
  __getter(resp_root_node).get("ReportBatchDetail").get("Header").get_value("BatchId",
                                                                            batch_id);
  return batch_id; 
}

void cc_processing(int* txn_id, ksi_client::pos_exception** except, boost::function<int ()> func) {
	try {
		*txn_id = func();
	}
	/*
	catch (ksi_client::logical_pos_exception& ex) {
		*except = new ksi_client::logical_pos_exception(ex);
	}
	catch (ksi_client::fatal_pos_exception& ex) {
		*except = new ksi_client::fatal_pos_exception(ex);
	}
	*/
	catch (ksi_client::pos_exception& ex) {
		*except = ex.clone();//new ksi_client::pos_exception(ex);
	}
	catch (std::exception& ex) {
		*except = new ksi_client::pos_exception(-1, ex.what(), -1);
	}
	catch (...) {
		*except = new ksi_client::pos_exception(-2, "...", -2);
	}
}


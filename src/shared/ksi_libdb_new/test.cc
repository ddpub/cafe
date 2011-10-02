#include "oracle.hh"
#include <iostream>
#include "database_exception.hpp"
#include <locale>
#include <fstream>
//#include "ibpp/core/ibpp.h"

using namespace database::oracle;
using namespace std;
using namespace boost::posix_time;

int main(int argc, char** argv)
{
  try
  {
    //image id 25235
    session sess;
    sess.connect("pashac", "111", "172.16.67.3", "cafe", 1521);
/*    try
    {
      sess.execute("DROP TABLE test_tab");
    }
    catch (std::exception& e)
    {
      cerr << e.what() << endl;
    }
    sess.execute("CREATE TABLE test_tab (char_data varchar(4000), lob_data blob, clob_data clob)");*/
/*    stmt.prepare("insert into test_tab (char_data, lob_data, clob_data) values (:char_data, :lob_data, :clob_data)");
    ifstream fs ("/home/pavel/DSC00438.JPG");
    fs.seekg(0, ios::end);
    size_t size = fs.tellg();
    fs.seekg(0, ios::beg);
    std::vector<unsigned char> img_data;
    std::vector<char> char_data;
    char_data.resize(20, 'a');
    img_data.resize(size);
    cout << img_data.size() << endl;
    fs.read(reinterpret_cast<char*>(img_data.data()), size);
    fs.close();

    stmt.set(img_data, "lob_data");
    stmt.set(char_data, "clob_data");
    stmt.set(std::string("wfefwefw"), "char_data");
    stmt.execute();
    sess.commit();*/

 /*   for (int i = 0; i < 10; i++)
      sess.execute("insert into test_tab (char_data, dt_data) values ('wefwefwefwe', sysdate)");
    sess.commit(); */

    statement stmt = statement(sess.adapter());
    stmt.prepare("insert into test_tab (char_data, dt_data) values (:char_data, sysdate)");
    stmt.set(string("Wefwegerg"), ":char_data");
    stmt.execute();
    sess.commit();

/*    stmt.prepare("SELECT char_data, lob_data, clob_data FROM test_tab");
    stmt.execute();
    stmt.next();
    string str_data = stmt.get(0);
    cout << str_data << endl;
    boost::shared_ptr<std::vector<unsigned char> > ret_ptr = stmt.get(1);
    boost::shared_ptr<std::vector<char> >  char_ret_ptr = stmt.get(2);
    size = ret_ptr.get()->size();
    cout << size << endl;
    ofstream ofs("/tmp/img.jpg");
    ofs.write(reinterpret_cast<char*>(ret_ptr.get()->data()), size);
    ofs.close();*/
  }
  catch (database::exception& e)
  {
    cerr << e.what() << endl;
  }
}


// This is the main project file for VC++ application project 
// generated using an Application Wizard.

#include <string>
#include <vector>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <algorithm>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include "sync.h"

#ifndef __WIN32__
#include <signal.h>
#include <unistd.h>


#endif // __WIN32__

bool stop(false);

memo_out_put_t mm;

bool valid_local(false);
bool valid_server(false);
bool connert_error(false);
//std::string ver("1.2.0.128");
namespace version{
    extern std::string version;
	extern std::string compamny;
	extern std::string product;
	extern std::string copyright;
}

// This is the entry point for this application

void term_handler(int a)
{ 
	stop = true;
	//std::cout << boost::lexical_cast<std::string>( boost::posix_time::second_clock::local_time()) + " application: terminated" << std::endl;
	std::cout << mm.put("application: terminated", 3);
}

int main(int argc, char* argv[])
{	
		//std::cout << boost::lexical_cast<std::string>( boost::posix_time::second_clock::local_time()) + " application: started" << std::endl;
		std::cout << mm.put("application: started", 3);
		std::string ksi_pwd = "";
		std::string config_str;
		
		std::string from;
		std::string to;
		std::string format = "";

		bool next(false);

		config_t new_conf;
		new_conf.refresh = 0; new_conf.main = ""; new_conf.local = ""; new_conf.db = false; new_conf.db_upd = 0; 

#ifndef __WIN32__

		struct sigaction sa;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = 0;
		sa.sa_handler = term_handler;
		sigaction(SIGTERM, &sa, 0);
#endif //__WIN32__

		std::vector<char> action(0);
		bool correct(true);

		for (int i=1; i<argc; i++)
		{
			bool tmp = next;
			next = false;
			if (tmp) continue;
			//std::count << (argv[i]);
			switch (argv[i][1])
			{
			case 's':			//central server host
				{
					  if (argv[i][2] == 'e')
					  {
						  if (i+1 < argc)
						  {
							new_conf.main = argv[i+1];
							next = true;
							break;
						  }
						  else 
						  {
							std::cout << "Hostname is not correct";
							return 3;
						  }
						  
					  }
					  else if (argv[i][2] == 'y')  //sync
					  {
						  action.push_back('s');
						  break;
					  }
				  }
			case 'l':		//cafe(local) host
				  {
					if (i+1 < argc)
					{
						new_conf.local = argv[i+1];
						next = true;
						break;
					}
					 else 
					{
						std::cout << "Hostname is not correct";
						return 3;
					}

				  }
			 case 'r':			//refresh interval
				  {
					  try  
					  {
						  new_conf.refresh = boost::lexical_cast<int>(argv[++i]);
						  if (new_conf.refresh <= 0) 
						  {
							  std::cerr << boost::lexical_cast<std::string>( boost::posix_time::second_clock::local_time()) + " refresh interval could be more than 0" << std::endl;
							  std::cout << boost::lexical_cast<std::string>( boost::posix_time::second_clock::local_time()) + " application: terminated with 3" << std::endl;
							  std::cout.flush(); 
							  std::cerr.flush(); 
							  return 3;
						  }
					  }
					  catch(...) 
					  {
						  new_conf.refresh = 10;
						  std::cerr << boost::lexical_cast<std::string>( boost::posix_time::second_clock::local_time()) + " refresh interval is not valid" << std::endl;
						  std::cout << boost::lexical_cast<std::string>( boost::posix_time::second_clock::local_time()) + " application: terminated with 2" << std::endl;
						  std::cout.flush(); 
						  std::cerr.flush(); 
						  return 2;
					  }
					  break;
				  }  

			  case 'd':			//database update   db_notupd new_conf.db
				  {
					  
					  if (argv[i][2] == 'b')
					  {
						if (i+1 < argc) 
						{
							if (argv[i+1][0] == '1') 
							{
								new_conf.db_upd = 1; 
								action.push_back('b');
							}
							else if (argv[i+1][0] == '0') new_conf.db_upd = 2; 
						  //if (argv[i][2] == 'b')
						  //{
						  new_conf.db = true;
						  //new_conf.db_upd = 1; 
						  //action.push_back('b');
						  break;
						}
						else 
						{
							std::cout << "dbupdate parameter is not correct";
							return 3;
						}

					  }
					  else if (argv[i][2] == 'e')       //delete
					  {
						  try
						  {
								if (!correct) 
								{
									std::cout << "initial_fill and delete is incompatible operations";
									return 3;
								}
								else correct = false;
						
							if (i+1 < argc) 
							{
								if (argv[i+1][0]!='-' && argv[i+1][0]!= '/') 
								{ 
									ksi_pwd = argv[i+1];
									next = true;
									action.push_back('C');
								}
								else action.push_back('c');
							}
							else action.push_back('c');
						  }
						  catch(...) {  std::cerr << boost::lexical_cast<std::string>( boost::posix_time::second_clock::local_time()) + " uncorrect config filename" << std::endl;  }
					  break;
						  action.push_back('c');
						  break;
					  }
					  else if (argv[i][2] == 'o')       //download
					  {
						  action.push_back('d');
						  break;
					  }
					  break;
				  }
				  
			  case 'i':			//initial fill
				  {
					  try
					  {
						if (!correct) 
						{
							std::cout << "initial_fill and delete is incompatible operations";
							return 3;
						}
						else correct = false;
						if (i+1 < argc) 
						{
							ksi_pwd = argv[i+1];
							next = true;
							action.push_back('i');
						}
						else 
						{
							std::cout << "Password for initial fill is not correct";
							return 3;
						}
					  }
					  catch(...) {  std::cerr << boost::lexical_cast<std::string>( boost::posix_time::second_clock::local_time()) + " uncorrect config filename" << std::endl;  }
					  break;
				  }
				  
			  case 'u':			//upload
				  {
					  action.push_back('u');
					  break;
				  }
			  case 'v':			//version
				  {
					  std::cout << mm.put(version::product+" v"+version::version+" "+version::copyright, 3);
					  break;
				  }
			  case 'f':			//config file
				  {
					  
					  if (argv[i][2] == 'i')
					  {
						try
						{
							if (i+1 < argc) 
							{
								config_str = argv[i+1];
							}
							else std::cout << "Config filename is not correct";

						}
						catch(...) 
						{  
							std::cerr << boost::lexical_cast<std::string>( boost::posix_time::second_clock::local_time()) + " uncorrect config filename"  << std::endl; 
							config_str = " ";

						}
						next = true;
						break;
					  }
					  else if (argv[i][2] == 'r') //from delete date  - crear database history
					  {
						if (i+1 < argc) 
						{
							from = argv[i+1];
							next = true;
						}
						else 
						{
							std::cout << "From_time parameter is not correct";
							return 3;
						}
					  }
					  else if (argv[i][2] == 'o')  //format date  - crear database history
					  {
						if (i+1 < argc) 
						{
							format = argv[i+1];
							next = true;
						}
						else 
						{
							std::cout << "Format_time parameter is not correct";
							return 3;
						}
					  }
				break;
				  }
			  case 't':			//to delete date  - crear database history
				  {
						if (i+1 < argc) 
						{
							to = argv[i+1];
							next = true;
						}
						else 
						{
							std::cout << "To_time parameter is not correct";
							return 3;
						}
					  
				  }

			}
		}	

		{
			bool save(false);
			config_t conf;
			if (conf.load_config(config_str))
			{
				if (new_conf.main == "" || new_conf.main == conf.main) new_conf.main = conf.main; else save = true;
				if (new_conf.local == "" || new_conf.local == conf.local) new_conf.local = conf.local; else  save = true; 
				if ((new_conf.db_upd != conf.db_upd) && (new_conf.db_upd != 0))	{ conf.db_upd = new_conf.db_upd; save = true; }
					//if (conf.db) { new_conf.db = false; conf.db = false; save = true; }
					//else { conf.db = new_conf.db; save = true; 	}
				//else new_conf.db = conf.db;
				if (new_conf.refresh <= 0 || new_conf.refresh == conf.refresh) new_conf.refresh = conf.refresh; else  save = true; 
			}
			else { save = true; }
			if (save) new_conf.save_config(config_str);
		}		

		sync_t Sync(new_conf.local, new_conf.main);

		for (size_t i=0; i<action.size(); i++)
		{

			switch (action[i])
			{
			case 'i':
				{
					Sync.InitFill(ksi_pwd, from, to, format);
					break;
				}
			case 'c':
				{
					std::string ppwd = "";
					Sync.DBClear(from, to, format, ppwd);
					break;
				}
			case 'C':
				{
					Sync.DBClear(from, to, format, ksi_pwd);
					break;
				}
			case 'b':
				{ 
					Sync.DBAction();
					break;
				}
			case 'd':
				{
					Sync.Download();
					break;
				}
			case 'u':
				{
					Sync.Upload();
					break;
				}
			case 's':
				{
					if (new_conf.refresh <= 0) new_conf.refresh = 10;
					boost::posix_time::ptime start = boost::posix_time::second_clock::local_time() + boost::posix_time::seconds(new_conf.refresh);
					while(!stop)
					{	
						boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
						if (now > start)
						{
							bool ddb(false); if (new_conf.db_upd == 1) ddb = true;
							Sync.Sync(ddb);   				 
							start = boost::posix_time::second_clock::local_time() + boost::posix_time::seconds(new_conf.refresh);
						} 
						boost::this_thread::sleep(start - now); //new_conf.refresh*500
					} 
					break;
				}
			}  
		}
		//std::cout << mm.put(" application: terminated", 3);
		//std::cout << boost::lexical_cast<std::string>( boost::posix_time::second_clock::local_time()) + " application: terminated" << std::endl;
		std::cout.flush(); 
		std::cerr.flush(); 
		return 0;
}

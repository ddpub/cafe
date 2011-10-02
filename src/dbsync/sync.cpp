#include "sync.h"

#include <memory>
#include <boost/lexical_cast.hpp>
#include <locale>

//#include <iostream>
//#include <stdio.h>
//#include <sys\stat.h>
#include <fstream>
//#include <sstream>

#include <boost/date_time/posix_time/posix_time.hpp>
//#include <boost/thread/thread.hpp>

//#include <ksi_util/bag_spooler.hpp>

extern memo_out_put_t mm;

extern bool stop;
namespace version {
	extern std::string version;
};
//extern std::auto_ptr<bag_spooler_t> spooler;
//int app_id = 14;
//db
connect_t::connect_t(std::string& Host, int Type) : host(Host), type(Type)
{
	sess = create_session();
	refresh();
/*	try
	{
		//std::auto_ptr<oracle_session_i> session;
		//if (type) sess = create_session(1); else 
		sess = create_session();
		
		std::string pwd = "ksi";
		sess->connect("sync", pwd, get_host(), get_sid());
            
		std::auto_ptr<oracle_query_i> query = sess->create_query();
		query->create_statement("select coalesce(cast(value as int), 0) from ksi.settings where key = 'CAFE_ID'");
		query->execute_statement();
            
		if (!query->next())	id = int(boost::get<double>(query->get(0)));
		valid = true;
	}
	catch(connection_problem_exception &e)
	{
		std::cerr << mm.put(host + " is not valid - " + e.what(), 3);  // << std::endl;
		valid = false;
	}
	catch(ksi_exception &e)
	{
		//spooler->send_bag(bag_record_t(1, app_id, version::version, std::string("error: boost bad get - ") + e.what()));
		std::cerr << mm.put(Host + " is not valid - " + e.what(), 3); // << std::endl;
		//spooler->send_bag(mm.put(Host + " is not valid - " + e.what(), 3));
		valid = false;
		//stop = true;
		
	}
	catch(std::exception &e)
	{
		std::cerr << mm.put(Host + " is not valid - " + e.what(), 3); // << std::endl;
		//spooler->send_bag(mm.put(Host + " is not valid - " + e.what(), 3));
		valid = false;
		//stop = true;
	}
	catch(...)
	{
		std::cerr << mm.put(Host + " is not valid - unexpected error", 3); //<< std::endl;
		//spooler->send_bag(mm.put(Host + " is not valid - unexpected error", 3));
		valid = false;
		//stop = true;
	}*/
}

bool connect_t::refresh()
{
	try
	{
		//std::auto_ptr<oracle_session_i> session;
		//if (type) sess.reset(create_session(1).get()); else 
		
		
		std::string pwd = "ksi";
		sess->connect("sync", pwd, get_host(), get_sid());
            
		std::auto_ptr<oracle_query_i> query = sess->create_query();
		query->create_statement("select coalesce(cast(value as int), 0) from ksi.settings where key = 'CAFE_ID'");
		query->execute_statement();

		if (!query->next())	id = int(boost::get<double>(query->get(0)));
		
		valid = true;
	}
	catch(connection_problem_exception &e)
	{
		std::cerr << mm.put(host + " is not valid - " + e.what(), 3);  // << std::endl;
		valid = false;
	}
	catch(ksi_exception &e)
	{
		std::cerr << mm.put(host + " is not valid - " + e.what(), 3);  // << std::endl;
		valid = false;
		//stop = true;
		
	}
	catch(std::exception &e)
	{
		std::cerr << mm.put(host + " is not valid - " + e.what(), 3); // << std::endl;
		valid = false;
		//stop = true;
	}
	catch(...)
	{
		std::cerr << mm.put(host + " is not valid - unexpected error", 3); // << std::endl;
		valid = false;
		//stop = true;
	}
	return valid;
}

/*************************************************************************************/
/*   DBAction (database update)                                                      */
/*************************************************************************************/
void sync_t::DBAction()
{
	// function for update db version
	//extern bool stop;
	//if (local.get_type() != main.get_type()) return;
	
	try
	{
        if (!main.is_valid()) if (!main.refresh()) return;
		if (!local.is_valid()) if (!local.refresh()) return;

		
		//std::auto_ptr<oracle_session_i> sess_local;
		//if (local.get_type()) sess_local = create_session(1); else sess_local = create_session();
		//sess_local->connect("sync", "ksi", local.get_tns());
		int vvv(0);
        {
			std::auto_ptr<oracle_query_i> query = local.create_query();//sess_local->create_query();
			query->create_statement("select coalesce(cast(value as int), 0) as value from ksi.settings where key = 'VERSION'");
			query->execute_statement();
		
			if (!query->next()) vvv = int(boost::get<double>(query->get(0)));
		}
        
		//std::auto_ptr<oracle_session_i> sess_main;
		//if (main.get_type()) sess_main = create_session(1); else sess_main = create_session();
		//sess_main->connect("sync", "ksi", main.get_tns());
        
		std::auto_ptr<oracle_query_i> select = main.create_query(); //sess_main->create_query();
		select->create_statement("select version_id, history from ksi.db_version where version_id > :vers");
		select->set("vers", vvv);
		select->execute_statement(); 
		
		bool tmp(true);
		while (!select->next())
		{
			// apply sql
			if (stop) break;
			if (tmp) { tmp = false; std::cout << mm.put("started ", 6); } 
			std::cout.flush(); 
			std::vector<char> clob_vector = boost::get<std::vector<char> >(select->get(1));
			for (int i=0; i<clob_vector.size(); i++)  
			  char ccc = clob_vector[i];
			std::string str_sql = std::string(&(clob_vector[0]), clob_vector.size());	
			str_sql.erase(std::remove(str_sql.begin(), str_sql.end(), '\r'), str_sql.end());
			local.direct_execute(str_sql); //sess_local
			local.direct_execute("begin ksi.compile_all(); end;"); //sess_local
			
            std::auto_ptr<oracle_query_i> query = local.create_query();
			query->create_statement("update ksi.settings set value = :num_ver where key = 'VERSION'");
			query->set("num_ver", select->get(0));
            
			query->execute_statement();
			vvv = (int) boost::get<double> (select->get(0));
			std::cout << mm.put(("new db version " + boost::lexical_cast<std::string>(vvv)), 6);
			std::cout.flush(); 
			
			local.commit();
		}
		std::cout << mm.put("current db version " + boost::lexical_cast<std::string>(vvv), 6); // << std::endl;
		std::cout.flush(); 
  }

  catch(connection_problem_exception &e)
  {
  
		std::cerr << mm.put(std::string("connection problem: ") + e.what(), 6);
		main.error();
		local.error();
		//main.refresh();
		//local.refresh();
		//stop = true;
  }
  catch(boost::bad_get &e)
  {
	  std::cerr << mm.put(std::string("error: boost bad get") , 6); // << std::endl;
	  //stop = true;
	  //spooler->send_bag(bag_record_t(1, app_id, version::version, std::string("error: boost bad get - ") + e.what()));
  }
  catch(ksi_exception &e)
  {
	  std::cerr << mm.put(std::string("error: ") + e.what() , 6); // << std::endl;
	  //stop = true;
  }
  catch (...)
  {
	  std::cerr << mm.put("error: unexpected error", 6); // << std::endl;
	  //stop = true;
  }
  std::cerr.flush(); 
}

/*************************************************************************************/
/*  Clear datatbase history                                                    */
/*************************************************************************************/
void sync_t::DBClear(std::string& from, std::string& to, std::string& format, std::string& pwd)
{
	try
	{
	// function for update db version
	//extern bool stop;
	
		if (!main.is_valid()) if (!main.refresh()) return;
	
		std::cout << "start clear...";
		if (format == "") format = std::string("mm/dd/yyyy_hh24:mi:ss");
		if (from != "") from = "to_date('"+from+"', '"+format +"')"; else from = "null";
		if (to != "") to = "to_date('"+to+"', '"+format+"')"; else to = "null";
	
		std::auto_ptr<oracle_session_i> sess_local;
		//if (main.get_type()) sess_local = create_session(1); else 
		sess_local = create_session();
    
		if (pwd == "")	sess_local->connect("sync", "ksi", local.get_host());
		else sess_local->connect("ksi", pwd, local.get_host());
	
		std::auto_ptr<oracle_query_i> query = sess_local->create_query();//sess_local->create_query();
		query->create_statement("begin ksi.clear_old_history("+from+", "+to+"); end;");
		query->execute_statement();
	
		sess_local->commit();
		std::cout << "ok"<< std::endl;
	}

	catch(connection_problem_exception &e)
	{
		std::cerr << mm.put(std::string("connection problem: ") + e.what(), 6) << std::endl;
		main.error();
		local.error();
		//stop = true;
	}
	catch(boost::bad_get &e)
	{
		std::cerr << mm.put(std::string("error: boost bad get") , 6); // << std::endl;
	  //stop = true;
	  //spooler->send_bag(bag_record_t(1, app_id, version::version, std::string("error: boost bad get - ") + e.what()));
	}
	catch(ksi_exception &e)
	{
		std::cerr << mm.put(std::string("error: ") + e.what() , 6); // << std::endl;
	  //stop = true;
	}
	catch (...)
	{
		std::cerr << mm.put("error: unexpected error", 6); // << std::endl;
	  //stop = true;
	}
	std::cerr.flush(); 
}
/*************************************************************************************/
/*   Action (download & upload)                                                      */
/*************************************************************************************/
void sync_t::Action(int action_t)
{
// replication logical transaction
std::string direct;
//extern bool stop;
try
{
	
	if (action_t == 1) direct = "Download";
	else if (action_t == 2) direct = "Upload";

	if (!main.is_valid()) if (!main.refresh()) return;
	if (!local.is_valid()) if (!local.refresh()) return;
    bool ora_from = true; //(direct == "Download" ? main: local);
    bool ora_to = true; //(direct == "Download" ? local: main);

	//std::string pwd = "ksi";
	//std::auto_ptr<oracle_session_i> sess_main;
	//if (main.get_type()) sess_main = create_session(1); else sess_main = create_session();
	//sess_main->connect("sync", pwd, main.get_tns());

    //std::auto_ptr<oracle_session_i> sess_local;
	//if (local.get_type()) sess_local = create_session(1); else sess_local = create_session();
	//sess_local->connect("sync", pwd, local.get_tns());

/* set parametrs depend from directions */

/*** update field last connected on central server***/
    {
	  //!!extern std::string local.get_id();
	  //!!extern std::string main.get_id();
		std::auto_ptr<oracle_query_i> cafe_query = main.create_query(); //sess_main->create_query();
		cafe_query->create_statement("select last_connected, dbsync_ver from ksi.cafe where cafe_id = :cf_id");  // for update of last_connected, dbsync_ver nowait
		cafe_query->set("cf_id", local.get_id());
		cafe_query->execute_statement();

		cafe_query->create_statement("update ksi.cafe set last_connected = sysdate, dbsync_ver = :ver where cafe_id = :cf_id");
		cafe_query->set("cf_id", local.get_id());
		cafe_query->set("ver", version::version);
		cafe_query->execute_statement();
		main.commit();
	  
		std::auto_ptr<oracle_query_i> cafe_query1 = local.create_query(); //sess_main->create_query();
		cafe_query1->create_statement("select last_connected, dbsync_ver from ksi.cafe where cafe_id = :cf_id for update of last_connected, dbsync_ver nowait");
		cafe_query1->set("cf_id", local.get_id());
		cafe_query1->execute_statement();
	  
		cafe_query1->create_statement("update ksi.cafe set last_connected = sysdate, dbsync_ver = :ver where cafe_id = :cf_id");
		cafe_query1->set("cf_id", local.get_id());
		cafe_query1->set("ver", version::version);
		cafe_query1->execute_statement();
		local.commit();
    }

	//std::auto_ptr<oracle_session_i>
	connect_t& Server (direct == "Download" ? main : local); //sess_main : sess_local
    //std::auto_ptr<oracle_session_i>
	connect_t& Client (direct == "Upload" ? main : local); //sess_main : sess_local   .session()

	int main_cafe = int(direct == "Download" ? main.get_id() : local.get_id());
	int local_cafe = int(direct == "Upload" ? main.get_id() : local.get_id());


/*** select new transaction for upload/download***/
    std::auto_ptr<oracle_query_i> ServerTrans = Server.create_query();
    ServerTrans->create_statement("delete from ksi.transaction where ready is not null and cafe_id = :cf_id1 and transaction_id not in (select distinct transaction_id from ksi.transaction_action where cafe_id = :cf_id)"); //ServerTrans->create_statement 
    ServerTrans->set("cf_id", local_cafe);
    ServerTrans->set("cf_id1", local_cafe);
    ServerTrans->execute_statement();
	Server.commit();
    
    ServerTrans->create_statement("select t.transaction_id, t.ready , t.type_id, (case when t.cafe_id = ksi.util.get_main_server_id() then 1 else 0 end), sum(case a.type_id when 4 then 1 else 0 end) as fff \
      from ksi.transaction t \
        inner join ksi.transaction_action a on a.transaction_id = t.transaction_id and a.cafe_id = t.cafe_id \
        where t.cafe_id = :cf_id1 and t.ready is not null and ( \
          t.transaction_id < (select min(transaction_id) from ksi.transaction where cafe_id = :cf_id2 and ready is null) \
          or not exists(select transaction_id from ksi.transaction where cafe_id = :cf_id3 and ready is null)) \
        group by t.transaction_id, t.ready ,  t.type_id, t.cafe_id \
        order by t.transaction_id ");
    //!!extern std::string local.get_id();
	ServerTrans->set("cf_id1", local_cafe);
	ServerTrans->set("cf_id2", local_cafe);
	ServerTrans->set("cf_id3", local_cafe);

    ///------------------- count sum instructions & others --------------------------------
    std::auto_ptr<oracle_query_i> query = Server.create_query();
    query->create_statement("select count(t.transaction_id) as t_all \
							, coalesce(sum(case when t.ready is null then 0 else 1 end),0) as t_ready  \
							, coalesce(sum(case when t.ready is null then 0 else (select count(num) from ksi.transaction_action where transaction_id = t.transaction_id) end),0) as t_sql \
        from ksi.transaction t \
        where cafe_id = :cf_id");
    query->set("cf_id", local_cafe);
    query->execute_statement();
	int vsego(0);  int all(0);
	if (!query->next())
	{
		vsego = int(boost::get<double>(query->get(1)));      // сколько закрытых транзакций в очереди
		//int pr_max_ta = boost::get<double>(query->get(2));  // сколько sql-запросов в очереди
		all = int(boost::get<double>(query->get(0)));        // сколько транзакций в очереди всего (включая незакрытые)
	}

    std::auto_ptr<oracle_query_i> query_client = Client.create_query();
    query_client->create_statement("select queue_id from ksi.cafe where cafe_id = :cf_id");
    query_client->set("cf_id", local.get_id());
    query_client->execute_statement();
	int queue(0);
	if (!query_client->next()) queue = int(boost::get<double>(query_client->get(0)));
	//VARIANT queue = query_client->get(0);
	//int ss = boost::get<int>(queue);
    int row_count = 0;
    bool err_flag(false);
    //bool stop(false);
    //int row_skiped = 0;

/*** change current position on start position in progress bar & set max position for progress bar***/
    int pr_pos = 1;
///-------------------------------------------------------------------------///

    int num = 0;

    ServerTrans->execute_statement();
	bool tmp(true);
	boost::posix_time::ptime start_log = boost::posix_time::second_clock::local_time() + boost::posix_time::seconds(60);
    while (!ServerTrans->next() && (!stop))
    {
		
		if (tmp && (direct == "Download")) {  tmp = false;  std::cout << mm.put( "started " + boost::lexical_cast<std::string>(vsego) + " trns" ,0);  }
		if (tmp && (direct == "Upload")) {  tmp = false;  std::cout << mm.put( "started " + boost::lexical_cast<std::string>(vsego) + " trns",1);  }
		std::cout.flush(); 
		{
			boost::posix_time::ptime now_log = boost::posix_time::second_clock::local_time();
			if (now_log > start_log)
			{
				start_log += boost::posix_time::seconds(60);
				int persent = (row_count*100)/vsego;
				if (direct == "Download") std::cout << mm.put( boost::lexical_cast<std::string>(persent) + "% completed..." ,0);
				if (direct == "Upload") std::cout << mm.put( boost::lexical_cast<std::string>(persent) + "% completed..." ,1);
				std::cout.flush(); 
			}
		}
		
		bool link_flag = false;
		int trans_id = int(boost::get<double>(ServerTrans->get(0)));//transaction_id
        //std::cout << "select: trans_id = " << trans_id << "   queue = " << queue << std::endl;
		if ( trans_id != queue )
        {           
            try
            {
                
                /* Get transaction from central server */

                /* get transaction */
                //!!extern std::string local.get_id(); 
                
				std::auto_ptr<oracle_query_i> query_action = Server.create_query();
                query_action->create_statement("select sql, type_id, num, table_name from ksi.transaction_action where transaction_id = :transact_id and num is not null and cafe_id = :cf_id5 order by transaction_id, num");
                query_action->set("transact_id", trans_id);
                query_action->set("cf_id5", local_cafe);
                query_action->execute_statement();

                while (!query_action->next() && (!stop))
                {                    

					num = int(boost::get<double>(query_action->get(2))); 
                    /* image field transfer */

					if (boost::get<double>(query_action->get(1)) == 4)   //type_id
					{
						try
						{
							std::vector<char> img;
							std::string img_id_str = boost::get<std::string>(query_action->get(0));
							int img_id = boost::lexical_cast<int>(img_id_str.substr(23, img_id_str.length() - 23));
							
							std::auto_ptr<oracle_query_i> query_img_sel = Server.create_query();
							query_img_sel->create_statement("select data from ksi.image where image_id = :img");
							query_img_sel->set("img", img_id);
							query_img_sel->execute_statement();
	  
							if (!query_img_sel->next()) 
								img = boost::get<std::vector<char, std::allocator<char> > >(query_img_sel->get(0));  //, std::allocator<char> 
							//{
								//std::vector<char> blob_vector = boost::get<std::vector<char> >(select->get(1));
								//for (int i=0; i<clob_vector.size(); i++) char ccc = clob_vector[i];
								//std::string str_sql = std::string(&(clob_vector[0]), clob_vector.size());	
								//str_sql.erase(std::remove(str_sql.begin(), str_sql.end(), '\r'), str_sql.end());
								//local.session()->direct_execute(str_sql); //sess_local
							
							//else throw exception_t(10055, "Image doesn't load", "HRusers_t::load_role()");
							
							std::auto_ptr<oracle_query_i> query_img_ins = Client.create_query();
							query_img_ins->create_statement("begin ksi.pp_shared_pkg.save_img(:img_id, :img); end;");
							query_img_ins->set("img_id", img_id);
							query_img_ins->set("img", img);
							
							query_img_ins->execute_statement();
						}
						catch(...) {  }
					}
					else //if (boost::get<double>(query_action->get(1)) != 4)   //type_id
                    {
                        /* sql transfer */

                        std::auto_ptr<oracle_query_i> query_trans = Client.create_query();
                        //std::string sql = query_action->get(1);
                        //convert(ora_from, ora_to, sql)
						Client.direct_execute(boost::get<std::string>(query_action->get(0))); //query_trans->create_statement
                          //query_trans->execute_statement();
                    }
                    
					
                }

                //!!extern std::string main.get_id();
                std::auto_ptr<oracle_query_i> qquery = Client.create_query();
				
				qquery->create_statement("select last_transferred, queue_id from ksi.cafe where cafe_id = :cf_id for update of last_transferred, queue_id nowait");
				qquery->set("cf_id", local.get_id());
				qquery->execute_statement();
                //std::string sql = "update ksi.cafe set last_transferred = sysdate where cafe_id = :cf_id";
                //convert(ora_from, ora_to, sql);
                qquery->create_statement("update ksi.cafe set last_transferred = sysdate, queue_id = :trans_id where cafe_id = :cf_id");
                qquery->set("cf_id", local.get_id());
                qquery->set("trans_id", trans_id);
                qquery->execute_statement();

				//std::cout << "update: cafe_id = "<< local_cafe << "  trans_id = " << trans_id << std::endl;
				
				if (((int) boost::get<double>(ServerTrans->get(2)) == 168) && (direct == "Download")/*boost::get<int>(ServerTrans->get(4)) != 1*/)
                {
                  std::auto_ptr<oracle_query_i> tmp  = Client.create_query();
				  Client.direct_execute("begin ksi.order_copy; end;");//  tmp->create_statement("begin ksi.order_copy; end;");
                  //tmp->execute_statement();
                }

                Client.commit();
                queue = trans_id;
				row_count++;
                err_flag = false;
            }
			
			catch(connection_problem_exception &e)
			{
				//stop = true;
				//Client.rollback();
				main.error();
				local.error();
				//err_flag = true;
				if (direct == "Download")
				{
					if (row_count > 0) 
						std::cout << mm.put( std::string("finished ") + boost::lexical_cast<std::string>(row_count) + " trns", 0);// << std::endl;
					std::cerr << mm.put( std::string("error on trns ") + boost::lexical_cast<std::string>(trans_id) + "/" + boost::lexical_cast<std::string>(main_cafe) + "-" + boost::lexical_cast<std::string>(num) + " - " + e.what(), 4); // << std::endl;
				}
				else if (direct == "Upload")
				{
					if (row_count > 0) 
						std::cout << mm.put( std::string("finished ") + boost::lexical_cast<std::string>(row_count) + " trns", 1);// << std::endl;
					std::cerr << mm.put( std::string("error on trns ") + boost::lexical_cast<std::string>(trans_id) + "/" + boost::lexical_cast<std::string>(main_cafe) + "-" + boost::lexical_cast<std::string>(num)  + " - " + e.what(), 5); // << std::endl;
				}
				
				if (direct == "Download") std::cerr << mm.put(std::string("connection problem: ") + e.what(), 0);
				else std::cerr << mm.put(std::string("connection problem: ") + e.what(), 1);
				std::cout.flush(); 
				std::cerr.flush(); 
				return;
			}
			catch(ksi_exception &E)
            {
				//Client.rollback();
				//err_flag = true;
				//stop = true;
				if (direct == "Download")
				{
					if (row_count > 0) 
						std::cout << mm.put( std::string("finished ") + boost::lexical_cast<std::string>(row_count) + " trns", 0);// << std::endl;
					std::cerr << mm.put( std::string("error on trns ") + boost::lexical_cast<std::string>(trans_id) + "/" + boost::lexical_cast<std::string>(main_cafe) + "-" + boost::lexical_cast<std::string>(num) + " - " + E.what(),4); // << std::endl;
				}
				else if (direct == "Upload")
				{
					if (row_count > 0) 
						std::cout << mm.put( std::string("finished ") + boost::lexical_cast<std::string>(row_count) + " trns", 1);// << std::endl;
					std::cerr << mm.put( std::string("error on trns ") + boost::lexical_cast<std::string>(trans_id) + "/" + boost::lexical_cast<std::string>(main_cafe) + "-" + boost::lexical_cast<std::string>(num)  + " - " + E.what(),5); // << std::endl;
				}
				std::cout.flush(); 
				std::cerr.flush(); 
				return;
			}
            catch  (...)   /* displaying errors */
            {                
				//stop=true;
				//Client.rollback();
				//err_flag = true;
                if (direct == "Download")
                {
					if (row_count > 0) 
						std::cout << mm.put( std::string("finished ") + boost::lexical_cast<std::string>(row_count) + " trns", 0);// << std::endl;
					std::cerr << mm.put( std::string("error on trns ") + boost::lexical_cast<std::string>(trans_id) + "/" + boost::lexical_cast<std::string>(main_cafe) + "-" + boost::lexical_cast<std::string>(num) + " - unexpected error", 4);// << std::endl;
                }
                else if (direct == "Upload")
                {
					if (row_count > 0) 
						std::cout << mm.put( std::string("finished ") + boost::lexical_cast<std::string>(row_count) + " trns", 1);// << std::endl;
					std::cerr << mm.put( std::string("error on trns") + boost::lexical_cast<std::string>(trans_id) + "/" + boost::lexical_cast<std::string>(main_cafe) + "-" + boost::lexical_cast<std::string>(num) + " - unexpected error",5);// << std::endl;
                }
				std::cout.flush(); 
				std::cerr.flush(); 
                return;
            }
        }
        try
        {
                // delete apllying on client transaction from server
			if (!err_flag)
			{
				std::auto_ptr<oracle_query_i> ServerDel = Server.create_query();
                ServerDel->create_statement("delete from ksi.transaction where transaction_id = :tr_id and cafe_id = :cf_id");
                ServerDel->set("tr_id", trans_id);
                ServerDel->set("cf_id", local_cafe);
                ServerDel->execute_statement();
                Server.commit();
			}
				//std::cout << "delete: trans_id = "<< trans_id << "    cafe = " << local_cafe << std::endl;
        }
		catch(connection_problem_exception &e)
		{
			std::cerr << std::string("connection problem: ") + e.what() << std::endl;
			//stop = true;
			std::cout.flush(); 
			std::cerr.flush(); 
			return;
		}
        catch(ksi_exception &E)
        {
          if (direct == "Download")
			  std::cerr << mm.put(std::string("error deleting on trns ") + boost::lexical_cast<std::string>(trans_id) + "/" + boost::lexical_cast<std::string>(main_cafe) + " - "  + E.what(), 0);// << std::endl;
          else if (direct == "Upload")
			  std::cerr << mm.put(std::string("error deleting on trns ") + boost::lexical_cast<std::string>(trans_id) + "/" + boost::lexical_cast<std::string>(main_cafe) + " - " + E.what(), 1);// << std::endl;
		  std::cout.flush(); 
		  std::cerr.flush(); 
		  //return;
        }
        catch(...)
        {
			if (direct == "Download")
				std::cerr << mm.put(std::string("error deleting on trns ") + boost::lexical_cast<std::string>(trans_id) + "/" + boost::lexical_cast<std::string>(main_cafe) +" - unexpected error", 0);// << std::endl;
			else if (direct == "Upload")
				std::cerr << mm.put(std::string("error deleting on trns ") + boost::lexical_cast<std::string>(trans_id) + "/" + boost::lexical_cast<std::string>(main_cafe) + " - unexpected error", 1);// << std::endl;
			//stop=true;
			std::cout.flush(); 
			std::cerr.flush(); 
			//return;
		}
		//}
        /*if (link_flag)
        {
            std::auto_ptr<TOraQuery> q_link (new TOraQuery(0));
            q_link->Session = Client;
            q_link->create_statement("drop database link img.sync" ;
            q_link->execute_statement();
        }*/
    }
    /* ------------------------------------------------ message on end cycle download/upload ------------------------------------------------- */
	if (row_count > 0 && !err_flag)
    {
            //!!extern std::string main.get_id();
		    if (direct == "Download")
            {
				if ( all != vsego)
					std::cout << mm.put( std::string("finished ") + boost::lexical_cast<std::string>(row_count) + " trns", 0, true);// << std::endl;
                else
					std::cout << mm.put( std::string("finished ") + boost::lexical_cast<std::string>(row_count) + " trns", 0);// << std::endl;
            }
            else if (direct == "Upload")
            {
				if (all != vsego)
					std::cout << mm.put( std::string("finished ") + boost::lexical_cast<std::string>(row_count) + " trns" ,1, true);// << std::endl;
                else
					std::cout << mm.put( std::string("finished ") + boost::lexical_cast<std::string>(row_count) + " trns",1);// << std::endl;
            }
            else throw std::string("unexpected error");
            //prbrAction->Position = 0;
    }
    else if(!err_flag)
    {
        if (direct == "Download")
        {
            if (all != vsego)
				std::cout << mm.put(std::string("no transaction(s) for Download"), 0, true);// << std::endl;
			else std::cout << mm.put(std::string("no transaction(s) for Download"), 0);// << std::endl;
        }
        else if (direct == "Upload")
        {
            if (all != vsego)
				std::cout << mm.put(std::string("no transaction(s) for Upload"), 1, true);// << std::endl;
			else std::cout << mm.put(std::string("no transaction(s) for Upload"), 1);// << std::endl;
        }
    }
	std::cout.flush(); 
	std::cerr.flush(); 

}
    //*
catch(connection_problem_exception &e)
{
	//stop = true;
	//main.rollback();
	//local.rollback();
	main.error();
	local.error();
	if (direct == "Download") std::cerr << mm.put(std::string("connection problem: ") + e.what(), 0);
	else if (direct == "Upload") std::cerr << mm.put(std::string("connection problem: ") + e.what(), 1);
	std::cerr.flush(); 
	return;
}
catch(ksi_exception &E)
{
	//stop = true;
	//main.rollback();
	//local.rollback();
	if (direct == "Download")
		std::cerr << mm.put(std::string("error - ") + E.what(), 0);// << std::endl;
	else if (direct == "Upload")
		std::cerr << mm.put(std::string("error - ") + E.what(), 1);// << std::endl;
	std::cerr.flush(); 
	return;
}

catch (...)
{
	//stop = true;
	//main.rollback();
	//local.rollback();
	if (direct == "Download") std::cerr << mm.put("error - unexpected error", 0);
	else if (direct == "Upload") std::cerr << mm.put("error - unexpected error", 1);
	std::cerr.flush(); 
    return;
}

} 


/*************************************************************************************/
/*   Copy orders                                                                     */
/*************************************************************************************/
void sync_t::copy_orders()
{
	try
	{
	//std::string pwd = "ksi";
	//std::auto_ptr<oracle_session_i> sess_main;
	//if (main.get_type()) sess_main = create_session(1); else sess_main = create_session();
    //std::auto_ptr<oracle_session_i> sess_main = create_session();
	//sess_main->connect("sync", pwd, main.get_tns());

	//!!extern std::string local.get_id();

		std::auto_ptr<oracle_query_i> query = main.create_query();//sess_main->create_query();
		query->create_statement("begin ksi.order_sent(:cf_id); end;");
		query->set("cf_id", local.get_id());
		query->execute_statement();
		main.commit(); //sess_main->commit();
    
		query->create_statement("begin ksi.order_addtrans(:cf_id); end;");
		query->set("cf_id", local.get_id());
		query->execute_statement();
		main.commit(); //sess_main->commit();
	}
	catch(connection_problem_exception &e)
	{
		main.error();
		local.error();
		std::cerr << mm.put(std::string("copy orders: connection problem: ") + e.what(), 0) << std::endl;
		std::cerr.flush(); 
		//stop = true;
		return;
	}
	catch(ksi_exception &e)
	{
		std::cerr << mm.put(std::string("copy orders: error - ") + e.what(),0);// << std::endl;
		std::cerr.flush(); 
	  	//stop = true;
		return;
	}
  catch(...) 
  {   
		std::cerr << mm.put("copy orders: error - unexpected error", 0) << std::endl; 
		std::cerr.flush(); 		
		//stop = true;
		return;  
	}
}

/*************************************************************************************/
/*   Initial fill                                                                    */
/*************************************************************************************/
void sync_t::InitFill(std::string ksi_pwd, std::string from = "", std::string to = "", std::string format = "")
{

  std::vector<std::string> sequence_list(8);
  sequence_list[0] = "KSI.INVOICE_SEQ";
  sequence_list[1] = "KSI.ORDER_DISH_CONS_SEQ";
  sequence_list[2] = "KSI.ORDER_DISH_SEQ";
  sequence_list[3] = "KSI.PRODUCT_CONSIGNMENT_SEQ";
  sequence_list[4] = "KSI.PRODUCT_INVOICE_SEQ";
  sequence_list[5] = "KSI.OPRDER_SEQ";
  sequence_list[6] = "KSI.SHIFT_SEQ";
  sequence_list[7] = "KSI.SEANCE_SEQ";
  sequence_list[8] = "KSI.CCTRANS_SEQ";
  //* добавить seance и  shift sequence

  std::vector<std::string> table_list(76);
  table_list[0] = "CAFE_TYPE";
  table_list[1] = "TRANSFER_TYPE";
  table_list[2] = "ACTION_TYPE";
  table_list[3] = "TRANSACTION_TYPE";
  table_list[4] = "CASHIER_HOTKEY";
  table_list[5] = "ORDER_STATUS";
  table_list[6] = "ORDER_TYPE";
  table_list[7] = "PAYMENT_TYPE";
  table_list[8] = "EXPENSE_TYPE";
  table_list[9] = "ROLE";
  table_list[10] = "APPLICATION";
  table_list[11] = "APPLICATION_ROLE";
  table_list[12] = "STATE";
  table_list[13] = "CCTRAN_TYPE";
  table_list[14] = "POSITION";
  table_list[15] = "POSITION_ROLE";
  table_list[16] = "PERSON";
  table_list[17] = "PERSON_ROLE";
  table_list[18] = "CAFE";
  table_list[19] = "CAFE_PERSON";
  table_list[20] = "MENU";
  table_list[21] = "IMAGE";
  table_list[22] = "MENU_ITEM";
  table_list[23] = "DISH_GROUP";
  table_list[24] = "KITCHEN_TYPE_GROUP";
  table_list[25] = "KITCHEN_TYPE";
  table_list[26] = "DISH";
  table_list[27] = "SUBDISH";
  table_list[28] = "MOD_GROUP";
  table_list[29] = "DISH_MOD_GROUP";
  table_list[30] = "MODIFIER";
  table_list[31] = "MENU_DISH";
  table_list[32] = "PRODUCT_GROUP";
  table_list[33] = "MEASURE_TYPE";
  table_list[34] = "MEASURE";
  table_list[35] = "PRODUCT";
  table_list[36] = "INGREDIENT_GROUP";
  table_list[37] = "INGREDIENT";
  table_list[38] = "INGREDIENTS";
  table_list[39] = "INGREDIENT_PRODUCT";
  table_list[40] = "DISH_INGREDIENT";
  table_list[41] = "MENU_DISH_MODIFIER";
  table_list[42] = "DISH_PRODUCT";
  table_list[43] = "DELIVERY_SCHEDULE";
  table_list[44] = "WAREHOUSE";
  table_list[45] = "WAREHOUSER";
  table_list[46] = "SUPPLIER";
  table_list[47] = "WAREHOUSE_SUPPLIER";
  table_list[48] = "SUPPLIER_PRODUCT";
  table_list[49] = "COUPON";
  table_list[50] = "COUPON_CAFE";
  table_list[51] = "COUPON_CONDITION";
  table_list[52] = "COUPON_APPLICATION";
  table_list[53] = "ROOM_IMAGE";
  table_list[54] = "ROOM_TABLE";
  table_list[55] = "TERMINAL";
  table_list[56] = "TOKEN";
  table_list[57] = "KITCHEN";
  table_list[58] = "TABLE_GROUP";
  table_list[59] = "KITCHEN_WAREHOUSE";
  table_list[60] = "TABLES";
  table_list[61] = "TABLE_GROUP_KITCHEN";

  table_list[62] = "SHIFT";
  table_list[63] = "MENU_SHIFT";
  table_list[64] = "SHIFT_PERSON";
  table_list[65] = "INVOICE";
  table_list[66] = "PRODUCT_INVOICE";
  table_list[67] = "PRODUCT_CONSIGNMENT";     
  table_list[68] = "SEANCE";
  table_list[69] = "ORDER_DELIVERY";
  table_list[70] = "ORDERS";
  table_list[71] = "ORDER_DISH";
  table_list[72] = "ORDER_DISH_MODIFIER";
  table_list[73] = "ORDER_DISH_CONSIGNMENT";
  table_list[74] = "ORDER_CCTRANS";
  table_list[75] = "BATCH";

  
	  //std::vector<std::string> t_table_list(4);
  //t_table_list[0] = "TERMINAL";
  //t_table_list[1] = "TOKEN";
  //t_table_list[2] = "KITCHEN";
  //t_table_list[3] = "TABLE_GROUP";


  //std::vector<std::string> w_table_list(6);
  //w_table_list[0] = "INVOICE";
  //w_table_list[1] = "PRODUCT_INVOICE";
  //w_table_list[2] = "PRODUCT_CONSINMENT";
  //w_table_list[3] = "KITCHEN_WAREHOUSE";
  //w_table_list[4] = "TABLES";
  //w_table_list[5] = "TABLE_GROUP_KITCHEN"; //

  std::vector<std::string> d_table_list(84);
  d_table_list[0] = "TRANSACTION";
  d_table_list[1] = "PERSON_REGISTER";
  d_table_list[2] = "CAFE_PERSON";
  d_table_list[3] = "SHIFT_PERSON";
  d_table_list[4] = "EXPENSE";
  d_table_list[5] = "MENU_SHIFT";
  d_table_list[6] = "COUPON_CAFE";
  d_table_list[7] = "ROOM_IMAGE";
  d_table_list[8] = "ROOM_TABLE";
  d_table_list[9] = "COUPON_APPLICATION";
  d_table_list[10] = "COUPON_CONDITION";
  d_table_list[11] = "ORDER_CHECK";
  d_table_list[12] = "ORDER_DISH_MODIFIER";
  d_table_list[13] = "ORDER_DISH_CONSIGNMENT";
  d_table_list[14] = "ORDER_DISH";
  d_table_list[15] = "BATCH";
  d_table_list[16] = "ORDER_CCTRANS";
  d_table_list[17] = "BONUS_HISTORY";
  d_table_list[18] = "INGREDIENT_PRODUCTION";
  d_table_list[19] = "ORDERS";
  d_table_list[20] = "COUPON";
  d_table_list[21] = "ORDER_DELIVERY";
  d_table_list[22] = "SEANCE";
  d_table_list[23] = "STOCKTAKING";
  d_table_list[24] = "PRODUCT_CONSIGNMENT";
  d_table_list[25] = "PRODUCT_INVOICE";
  d_table_list[26] = "INVOICE";
  d_table_list[27] = "SHIFT";
  d_table_list[28] = "TERMINAL";
  d_table_list[29] = "TOKEN";
  d_table_list[30] = "CLIENT";
  d_table_list[31] = "SUPPLIER_PRODUCT";
  d_table_list[32] = "WAREHOUSE_SUPPLIER";
  d_table_list[33] = "SUPPLIER";

  d_table_list[34] = "KITCHEN_WAREHOUSE";
  d_table_list[35] = "TABLE_GROUP_KITCHEN";
  d_table_list[36] = "KITCHEN";

  d_table_list[37] = "WAREHOUSER";
  d_table_list[38] = "WAREHOUSE";
  d_table_list[39] = "DELIVERY_SCHEDULE";
  d_table_list[40] = "DISH_PRODUCT";
  d_table_list[41] = "MENU_DISH_MODIFIER";
  d_table_list[42] = "DISH_INGREDIENT";
  d_table_list[43] = "INGREDIENT_PRODUCT";
  d_table_list[44] = "INGREDIENTS";
  d_table_list[45] = "INGREDIENT";
  d_table_list[46] = "INGREDIENT_GROUP";
  d_table_list[47] = "PRODUCT";
  d_table_list[48] = "PRODUCT_GROUP";
  d_table_list[49] = "MENU_DISH";
  d_table_list[50] = "MODIFIER";
  d_table_list[51] = "DISH_MOD_GROUP";
  d_table_list[52] = "MOD_GROUP";
  d_table_list[53] = "SUBDISH";
  d_table_list[54] = "DISH";
  d_table_list[55] = "KITCHEN_TYPE";
  d_table_list[56] = "DISH_GROUP";
  d_table_list[57] = "MENU_ITEM";
  d_table_list[58] = "IMAGE";
  d_table_list[59] = "MENU";
  d_table_list[60] = "TABLES";
  d_table_list[61] = "TABLE_GROUP";
  d_table_list[62] = "CAFE";
  d_table_list[63] = "PERSON_ROLE";
  d_table_list[64] = "KITCHEN_TYPE_GROUP";
//  d_table_list[63] = "LOG";
  d_table_list[65] = "TRANSACTION_TYPE";
  d_table_list[66] = "PERSON";
  d_table_list[67] = "POSITION_ROLE";
  d_table_list[68] = "POSITION";
  d_table_list[69] = "CCTRAN_TYPE";
  d_table_list[70] = "STATE";
  d_table_list[71] = "APPLICATION_ROLE";
  d_table_list[72] = "APPLICATION";
  d_table_list[73] = "ROLE";
  d_table_list[74] = "EXPENSE_TYPE";
  d_table_list[75] = "PAYMENT_TYPE";
  d_table_list[76] = "ORDER_TYPE";
  d_table_list[77] = "ORDER_STATUS";
  d_table_list[78] = "CASHIER_HOTKEY";
  d_table_list[79] = "CAFE_TYPE";
  d_table_list[80] = "ACTION_TYPE";
  d_table_list[81] = "TRANSFER_TYPE";
  d_table_list[82] = "MEASURE";
  d_table_list[83] = "MEASURE_TYPE";
  
  //d_table_list[66] = "ACTION_TYPE";
  //d_table_list[71] = "TRANSFER_TYPE";
  //d_table_list[72] = "CAFE_TYPE";
	
  std::cout << "This will destroy all the data from the cafe server" << std::endl; //.\nAre you sure? (Y/N)
  std::cout.flush(); 
    try
	{
		std::auto_ptr<oracle_session_i> sess_try;
		if (main.get_type()) sess_try = create_session(1); else sess_try = create_session();
		sess_try->connect("ksi", ksi_pwd, main.get_host());
	}
	catch(ksi_exception &E)
	{
		std::cerr << mm.put(std::string("ksi connection error - ") + E.what(),7) << std::endl;
		std::cerr.flush(); 
		//stop = true;
		return;
	}
	catch(...) 
	{   
		std::cerr << mm.put("ksi connection error - unexpected error", 7) << std::endl;  
		std::cerr.flush();   
		//stop = true; 
		return;
	}


	std::auto_ptr<oracle_session_i> sess_main;
	if (main.get_type()) sess_main = create_session(1); else sess_main = create_session();
    sess_main->connect("ksi", ksi_pwd, main.get_host());


	std::auto_ptr<oracle_session_i> sess_local;
	if (local.get_type()) sess_local = create_session(1); else sess_local = create_session();
    sess_local->connect("sync", "ksi", local.get_host());

	std::cout << mm.put("started",7) << std::endl;
	std::cout.flush(); 
    
	try 
	{
/*********  delete data ***********/
/*		for (size_t i = 0; i< d_table_list.size(); i++)
		{
			if (d_table_list[i] == "") continue;
			std::cout << mm.put("cleaning table " + d_table_list[i] + " ...", 7); // << std::endl;
			std::cout.flush(); 
			std::string sql;
			if  (d_table_list[i] == "PERSON")
			{
				sess_local->direct_execute("delete from ksi." + d_table_list[i] + "  where login <> 'sync' and login <> 'ksi'");
				//query->create_statement(sql);
				//query->execute_statement();
				sql = "update ksi.person set state_id = null, position_id = null";
			}
			else if (d_table_list[i] == "PRODUCT_INVOICE")
			{
				sql = "delete from ksi.product_invoice";
			}
			else if (d_table_list[i] == "PRODUCT_CONSIGNMENT")
			{
				sess_local->direct_execute("update ksi.product_invoice set consignment_id = null");
				sql = "delete from ksi.product_consignment";
			}
			else
			{
				sql = "delete from ksi." + d_table_list[i];
			}
			sess_local->direct_execute(sql);

			std::cout << mm.put(" comlpleted", 2) << std::endl;
			std::cout.flush(); 
		}
		
		
/*********  insert data ***********/
		int cb_data_actyvity(1);
		bool ddates(false);
		if (from != "" || to != "")
		{
			if (format == "") format = std::string("mm/dd/yyyy_hh24:mi:ss");
			if (from != "") from = "to_date('"+from+"', '"+format +"')"; else from = "to_date('01/01/1890', 'mm/dd/yyyy')";
			if (to != "") to = "to_date('"+to+"', '"+format+"')"; else to = "sysdate";
			ddates=true;
		}
		
		for (size_t i = 1; i < table_list.size(); i++ ) 
		{
			if (table_list[i] == "") continue;
			if (i == 62 && cb_data_actyvity == 0) break;

			
			std::cout << mm.put("filling table " + table_list[i] + " ...", 7);
			std::cout.flush(); 
			
			std::string whr("");
			bool cafe_col(false);	

			if (table_list[i] == "IMAGE") whr = " ";
			else if (table_list[i] == "MENU_ITEM") { whr = " where menu_id in (select menu_id from ksi.menu where cafe_id = :cf_id) order by coalesce(parent_id, 0), menu_item_id"; cafe_col = true; }
			//else if (table_list[i] == "DISH_GROUP") { whr = " order by coalesce(parent_id, 0)"; }
			else if (table_list[i] == "MENU_DISH" || table_list[i] == "MENU_DISH_MODIFIER") { whr = " where menu_item_id in (select menu_item_id from ksi.menu_item inner join ksi.menu on menu.menu_id = menu_item.menu_id and menu.cafe_id = :cf_id)"; cafe_col = true;}
			else if (table_list[i] == "WAREHOUSE") whr = " ";
			else if (table_list[i] == "CAFE") whr = " "; 
			else if (table_list[i] == "COUPON" || table_list[i] == "COUPON_APPLICATION" || table_list[i] == "COUPON_CONDITION") { whr = " where coupon_id in (select coupon_id from ksi.coupon_cafe where cafe_id = :cf_id) or coupon_id = -1"; cafe_col = true; }
			else if (table_list[i] == "KITCHEN_WAREHOUSE") { whr = " where kitchen_id in (select kitchen_id from ksi.kitchen where cafe_id = :cf_id)"; cafe_col = true; }
			else if (table_list[i] == "PRODUCT_INVOICE") { whr = " where invoice_id in (select invoice_id from ksi.invoice where cafe_id = :cf_id)"; cafe_col = true; }
			else if (table_list[i] == "TABLES") { whr = " where table_group_id in (select table_group_id from ksi.table_group where cafe_id = :cf_id)"; cafe_col = true; }
			else if (table_list[i] == "PRODUCT_CONSIGNMENT") { whr = " where consignment_id in (select consignment_id from ksi.product_invoice inner join ksi.invoice on invoice.invoice_id = product_invoice.invoice_id and invoice.cafe_id = :cf_id)"; cafe_col = true; }
			else if (table_list[i] == "DISH_GROUP" || table_list[i] == "PRODUCT_GROUP" || table_list[i] == "INGREDIENT_GROUP") { whr = " order by coalesce(parent_id,0), group_id"; cafe_col = false; }
			else if (table_list[i] == "TABLE_GROUP_KITCHEN") { whr = " where kitchen_id in (select kitchen_id from ksi.kitchen where cafe_id = :cf_id)"; cafe_col = true; }

			else if (table_list[i] == "PERSON") { whr = " where login <> 'ksi' and login <> 'sync'"; }
			
			if (ddates) 
			{
				if (table_list[i] == "ORDERS") 
				{ 
					whr = " where cafe_id = :cf_id and created between "+from+" and "+to+" "; 
					cafe_col = true;
				}
				else if (table_list[i] == "ORDER_DISH" || table_list[i] == "ORDER_DISH_CONSIGNMENT")
				{
					whr = " where cafe_id = :cf_id and time between "+from+" and "+to+" "; 
					cafe_col = true;
				}
				else if (table_list[i] == "ORDER_DISH_MODIFIER")
				{
					whr = " where cafe_id = :cf_id and order_dish_id in(select order_dish_id from ksi.order_dish where cafe_id = order_dish_modifier.cafe_id and time between "+from+" and "+to+") "; 
					cafe_col = true;
				}
				else if (table_list[i] == "ORDER_CCTRANS")
				{
					whr = " where cafe_id = :cf_id and tran_date between "+from+" and "+to+" "; 
					cafe_col = true;
				}
				else if (table_list[i] == "ORDER_DELIVERY")
				{
				
				}
				else if (table_list[i] == "BATCH")
				{ 
					whr = " where cafe_id = :cf_id and batch_id in (select batch_id from ksi.order_cctrans where cafe_id = :cf_id and tran_date between "+from+" and "+to+") "; 
					cafe_col = true;
				}
			}
		
			{
				std::auto_ptr<oracle_query_i> query = sess_main->create_query();
				query->create_statement("select column_name from dba_tab_columns where owner = 'KSI' and table_name = :tn");                        
				query->set("tn", table_list[i]);
				query->execute_statement();
				std::string sql_col = "";
				std::string sql_col1 = "";
				std::string sql_var = "";
				std::vector<std::string> vec_param;
				int cons_col = 0; int prodinv_col = 0; int whs_col = 0; //int par_col(-1);
				bool is_blob(false);
				bool is_clob(false);
				
                while (!query->next()) 
				{
					std::string col = boost::get<std::string>(query->get(0));
					if (col == "ORDER_DEBUG" && table_list[i] == "ORDERS")   //  if orders.order_bedug  --  clob
					{ 
						is_clob = true;  
						sql_var += std::string("empty_clob(), "); 
					}
					else if (col == "DATA" && table_list[i] == "IMAGE")  // if image.data  --  blob
					{ 
						is_clob = true; 
						sql_var += std::string("empty_blob(), "); 
					}
					else 
					{
						vec_param.push_back(col);
						sql_var += std::string(":") + col + ", ";
						sql_col += col + ", ";
					}	
					sql_col1 += col + ", ";
					
					if (col == "CAFE_ID" && whr == "") cafe_col = true;
					if (col == "CONSIGNMENT_ID") cons_col = vec_param.size()-1;
					if (col == "WAREHOUSE_ID") whs_col = vec_param.size()-1;
					if (col == "PRODUCT_INVOICE_ID") prodinv_col = vec_param.size()-1;
					
					//if (col == "PARENT_ID") par_col = vec_param.size()-1;
				}
				sql_col = sql_col.substr(0, sql_col.length() -2);
				sql_col1 = sql_col1.substr(0, sql_col1.length() -2);
				sql_var = sql_var.substr(0, sql_var.length() -2);

				if (cafe_col && whr == "") { whr = " where cafe_id = :cf_id";  cafe_col = true; }
	
				std::auto_ptr<oracle_query_i> select_query = sess_main->create_query();
				
				select_query->create_statement("select " + sql_col + " from ksi." + table_list[i] + whr);
				if (cafe_col) select_query->set("cf_id", local.get_id()); 
				select_query->execute_statement();

				std::auto_ptr<oracle_query_i> insert_query = sess_local->create_query();

				while (!select_query->next())
				{
					/*if (table_list[i] == "IMAGE") 
					{
						insert_query->create_statement(std::string("insert into ksi.IMAGE(image_id, data) values(:image_id, empty_blob())"));
						insert_query->set("image_id", select_query->get(0));
						insert_query->execute_statement();
						sess_local->commit();
						continue;
					}*/
					insert_query->create_statement(std::string("insert into ksi.") + table_list[i] + "(" + sql_col1 + ") values(" + sql_var + ")");
					for (int j=0; j < vec_param.size(); j++) 
					{
						//if (vec_param[j] == "ORDER_DEBUG") continue;
						if (select_query->out_is_null(j) /*|| i == par_col*/)
						{
							insert_query->set(vec_param[j], null_t());
							continue;
						}
						insert_query->set(vec_param[j], select_query->get(j));
					}
					insert_query->execute_statement();	
					
					/*if (table_list[i] == "PRODUCT_CONSIGNMENT") 
					{
						insert_query->create_statement("update ksi.product_invoice set consignment_id = :cons_id where product_invoice_id = :prod_inv and warehouse_id = :whs");
						insert_query->set("cons_id", select_query->get(cons_col));
						insert_query->set("prod_inv", select_query->get(prodinv_col));
						insert_query->set("whs", select_query->get(whs_col));
						insert_query->execute_statement();
					}*/
				}
				if (is_clob)
				{
					if (table_list[i] == "ORDERS")
					{
						select_query->create_statement("select cafe_id, order_id, order_debug from ksi.orders where cafe_id = :cf_id");
						select_query->execute_statement();
						while (!select_query->next())
						{
							insert_query->create_statement("begin ksi.pp_shared_pkg.save_order_debug(:cf_id, :ord_id, :debug_val); end;");
							insert_query->set("cf_id", select_query->get(0));
							insert_query->set("ord_id", select_query->get(1));
							insert_query->set("debug_val", select_query->get(2));
							insert_query->execute_statement();
						}
					}
				}
					
				if (is_blob)
				{
					if (table_list[i] == "IMAGE")
					{
						select_query->create_statement("select image_id, data from ksi.image");
						select_query->execute_statement();
						while (!select_query->next())
						{
							insert_query->create_statement("begin ksi.pp_shared_pkg.save_img(:img_id, :img); end;");
							insert_query->set("img_id", select_query->get(0));
							insert_query->set("img", select_query->get(1));
							insert_query->execute_statement();
						}
						
					}
				}
				/*if (par_col != -1)
				{//if (table_list[i] == "DISH_GROUP")
					if (table_list[i] == "MENU_ITEM") select_query->create_statement("select parent_id, menu_item_id from ksi." + table_list[i] + whr);
					else  select_query->create_statement("select parent_id, group_id from ksi." + table_list[i] + whr);
					//if (cafe_col && whr == "") { whr = " where cafe_id = :cf_id";  cafe_col = true; }
					if (cafe_col) select_query->set("cf_id", local.get_id()); 
					select_query->execute_statement();
					while(!select_query->next())
					{
						if (table_list[i] == "MENU_ITEM") insert_query->create_statement("update ksi." + table_list[i] + " set parent_id = :val1 where menu_item_id = :val2" );
					    else insert_query->create_statement("update ksi." + table_list[i] + " set parent_id = :val1 where group_id = :val2" );
						if (select_query->out_is_null(0)) insert_query->set("val1", null_t());
						else insert_query->set("val1", select_query->get(0));
						if (select_query->out_is_null(1)) insert_query->set("val2", null_t());
						else insert_query->set("val2", select_query->get(1));
					}
				}*/
			}
			sess_local->commit();
			std::cout << mm.put("ok", 2) << std::endl;
			std::cout.flush(); 
		}	
		//sess_local->commit();
		return;

//****************************** update seqvenses *****************************************
		//*
		for (int i = 0; i < int(sequence_list.size()); i++)
		{
			if (sequence_list[i] == "") continue;
			int num;  std::string seq_name;
			std::auto_ptr<oracle_query_i> query = sess_main->create_query();
			switch (i)
			{
			case 0:
				query->create_statement("select max(invoice_id) as num from ksi.invoice where cafe_id = :cf_id");
				seq_name = "ksi.invoice_seq";
				break;
			case 1:
				query->create_statement("select max(order_dish_cons_id) as num from ksi.order_dish_consignment where cafe_id = :cf_id");
				seq_name = "ksi.order_dish_cons_seq";
				break;
			case 2:
				query->create_statement("select max(order_dish_id) as num from ksi.order_dish where cafe_id = :cf_id");
				seq_name = "ksi.order_dish_seq";
				break;
			case 3:
				query->create_statement("select max(consignment_id) as num from ksi.product_consignment inner join ksi.warehouse on warehouse.warehouse_id = product_consignment.warehouse_id and warehouse.cafe_id = :cf_id");
				seq_name = "ksi.product_consignment_seq";
				break;
			case 4:
				query->create_statement("select max(product_invoice_id) as num from ksi.product_invoice inner join ksi.warehouse on warehouse.warehouse_id = product_invoice.warehouse_id and warehouse.cafe_id = :cf_id");
				seq_name = "ksi.product_invoice_seq";
				break;
			case 5 :
				query->create_statement("select max(order_id) as num from ksi.orders where cafe_id = :cf_id");
				seq_name = "ksi.order_seq";
				break;
			case 6 :                                     
				query->create_statement("select max(shift_id) as num from ksi.shift where cafe_id = :cf_id");
				seq_name = "ksi.shift_seq";
				break;
			case 7 :
				query->create_statement("select max(seance_id) as num from ksi.seance where cafe_id = :cf_id");
				seq_name = "ksi.seance_seq";
				break; 
			case 8 :
				query->create_statement("select max(trans_id) as num from ksi.order_cctrans where cafe_id = :cf_id");
				seq_name = "ksi.cctrans_seq";
				break; 
			} 
			query->set("cf_id", local.get_id());
			query->execute_statement();
			if (!query->next()) num = int(boost::get<double>(query->get(0)) + 10);
			try {  sess_local->direct_execute(std::string("drop sequence ") + seq_name); } catch(...) { std::cout << mm.put(std::string("warning: sequence not droped"),7); std::cout << mm.put(" ", 2); } 
			try {  sess_local->direct_execute(std::string("create sequence ") + seq_name + " minvalue 0 maxvalue 9000000000000000000 start with " + boost::lexical_cast<std::string>(num) + " increment by 1 nocache order"); } catch(...) { std::cout << mm.put(std::string("warning: sequence not exist created"),7); std::cout << mm.put(" ", 2); } 
			std::cout.flush(); 
				
	  }
//***************************** end update seqvenses **************************************
      
// *************************** COPYING ORACLE USERS **************************************** 
	  std::auto_ptr<oracle_query_i> main_query = sess_main->create_query();
	  if (main.get_type() == 1) main_query->create_statement("select cast(usename as varchar) from pg_shadow where usename <> 'sync' and usename <> 'ksi' and usename in (select login from ksi.person)");
	  else main_query->create_statement("select name from sys.user$ where name <> 'sync' and name <> 'ksi' and name in (select login from ksi.person)");
	  main_query->execute_statement();
          sess_local->commit();

	  std::vector<std::string> is_user;
	  {
		  std::auto_ptr<oracle_query_i> local_query = sess_local->create_query();
		  if (local.get_type() == 1) local_query->create_statement("select cast(usename as varchar) from pg_shadow where usename <> 'sync' and usename <> 'ksi' and usename in (select login from ksi.person)");
		  else local_query->create_statement("select name, password from sys.user$ where name <> 'sync' and name <> 'ksi' and name in (select login from ksi.person)");	  
		  local_query->execute_statement();
		  while(!local_query->next()) is_user.push_back(boost::get<std::string>(local_query->get(0)));
	  }

	  while (!main_query->next())
	  {
		  std::string us_name = boost::get<std::string>(main_query->get(0)); 
		  std::string us_password = boost::get<std::string>(main_query->get(1)); 
		  bool find(false);
		  for (int i=0; i<is_user.size(); i++) if (is_user[i] == us_name) find = true;
		  if (!find) 
		  {
			  for (int i = 0; i < us_name.length(); i++) us_name[i] = std::tolower(us_name[i], std::locale());
			  std::cout << mm.put(std::string("processing user ") + us_name + " ... ", 7);
			  std::cout.flush(); 
			  try    //* -=try create new user=-         //*
			  {
				  sess_local->direct_execute(std::string("create user ") + us_name + " identified by \""+us_password+"\"");
                                  sess_local->commit();
			  }
			  catch(ksi_exception &e)   {	std::cerr << mm.put(std::string("error - ") + e.what(), 7);  std::cerr << mm.put(std::string(" User ") + us_name + " don't created ", 2);  }//* -=if user already exist go to next user=- *         //*
			  std::cerr.flush(); 
		  }
		  std::auto_ptr<oracle_query_i> role_query = sess_main->create_query();
		  role_query->create_statement("select granted_role from sys.dba_role_privs where grantee = '" + us_name + "'");
		  role_query->execute_statement();
		  //* -=grant privileges to new user=-   //*
		  while (!role_query->next())
		  {
			  try
			  {
				  std::string str_role = "grant " + boost::get<std::string>(role_query->get(0)) + " to \"" + us_name + "\"";
				  sess_local->direct_execute(str_role);
                                  sess_local->commit();
			  }
			  catch(ksi_exception &e)   {	std::cerr << mm.put(std::string("error - ") + e.what(), 7);  std::cerr << mm.put(std::string(" User ") + us_name + " don't granted", 2); }
			  std::cerr.flush(); 
		  }
		  std::cout << mm.put(" added", 2) << std::endl;
		  std::cout.flush(); 
	  }
// ******************************************************************************** //
	  //*
	  std::cout << mm.put("cleaning download queue ... ", 7) << std::endl;
	  std::cout.flush(); 
	  sess_main->direct_execute(std::string("delete from ksi.transaction where cafe_id = ") + boost::lexical_cast<std::string>(local.get_id()) );
	  //query->execute_statement();
	  std::cout << mm.put(" comlpleted", 2) << std::endl;
	  std::cout.flush(); 
	  sess_local->commit();
	  sess_main->commit();
  }
  catch (ksi_exception &e)
  {
	  std::cerr << mm.put(std::string("error - ") + e.what(), 7) << std::endl;
	  std::cerr << mm.put("   Rolled back", 7) << std::endl;
  }
  catch (std::exception &e)
  {  
	  std::cerr << mm.put(std::string("error - ") + e.what(), 7) << std::endl;
	  std::cerr << mm.put("   Rolled back", 7) << std::endl;

  }
  catch (...)
  {  
	  std::cerr << mm.put("error - unexpected error", 7) << std::endl;
	  std::cerr << mm.put("   Rolled back", 7) << std::endl;
  }
  std::cout << mm.put("done",7) << std::endl;
  std::cout.flush(); 
  std::cerr.flush(); 
}

/*************************************************************************************/
/*   Sync                                                                            */
/*************************************************************************************/
void sync_t::Sync(bool db_upd)
{
	if (db_upd) DBAction();
	Download();
	
	Upload();
}


/*************************************************************************************/
/*   config_t                                                                        */
/*************************************************************************************/
bool config_t::load_config(std::string file_name)
{
	try
	{
		if (file_name == "") file_name = "dbsync_config.conf";
		MType = 0;  LType = 0;
		{
			FILE *stream;
			if ((stream = fopen(file_name.c_str(), "r+")) == NULL)  return false;
		}

		std::ifstream file_op(file_name.c_str(), std::ios_base::out);
		if (file_op.fail()) return false;
		std::string sss;
		while (!file_op.eof())
		{
			//if (file_op.bad()) throw;
			file_op >> sss;
			switch(sss[0])
			{
			case 'm': 
				main = sss.substr(5, sss.size()-5);
				break;
			case 'l':
				local = sss.substr(6, sss.size()-6);
				break;
			case 'd':
				try
				{
					//db = (sss.substr(3, sss.size()-3) == "1" ? true : false);
					db_upd = (sss.substr(3, sss.size()-3) == "1" ? 1 : 2);
				}
				catch(...) {  }
				break;
			case 't':
				try
				{
					MType = 0;  LType = 0;//if (sss[5] == 's'){	if (sss[10] == 'p') MType = 1  }
					//if (sss[5] == 'l') { if (sss[11] == 'p') LType = 1 }
				}
				catch(...) { MType = 0;  LType = 0; }
				break;
			case 'r':
				try
				{
					refresh = boost::lexical_cast<int>(sss.substr(8, sss.size()-8));
				}
				catch(...) {  }
				break;                
			}
		}
		file_op.close();

	}
	catch(...) { return false; }
	return true;
}

bool config_t::save_config(std::string file_name)
{
	try
	{
		if (file_name == "") file_name = "dbsync_config.conf";
		{
			FILE *stream;
			stream = fopen(file_name.c_str(), "w+");
			fclose(stream);
		}
		std::ofstream file_op(file_name.c_str(), std::ios_base::in);
		std::string ss;

		file_op << "main=" << main << std::endl;
		file_op << "type_main=" << (MType?"postgres":"oracle") << std::endl;
		file_op << "local=" << local << std::endl;
		file_op << "type_local=" << (LType?"postgres":"oracle") << std::endl;
		file_op << "db=" << ((db_upd == 1) ? 1 : 0) << std::endl;
		file_op << "refresh=" << refresh << std::endl;

		file_op.close();
		
	}
	catch(...) { return false; }
	return true;
}




#include "wilton/wilton.h"
#include "wilton/wiltoncall.h"

#include "wilton/wilton_server.h"
#include "wilton/wilton_logging.h"

#include <unistd.h>
#include <string>

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include <iostream>


std::string index_page("error");

void check_err(char* err) {
    if (NULL != err) {
        puts(err);
        wilton_free(err);
        exit(1);
    }
}

void hello(void* ctx, wilton_Request* req) {
    (void) ctx;
    wilton_Request_send_response(req, "hello\n", 6);
}

void index_handler(void* ctx, wilton_Request* req) {
	(void) ctx;
#ifdef VERBAL
	std::cout << "get_handler called" << std::endl;

	char* data_out_storage[1024];
	int data_len = 0;

	char* meatadata_out_storage[1024];
	int meatadata_len = 0;

	// Обращение к свойствам запроса
	char* req_err = wilton_Request_get_request_data(req, data_out_storage, &data_len);
	check_err(req_err);
	if (NULL == req_err) {
		std::cout << "req data len : " << data_len << std::endl;
	}

	req_err = wilton_Request_get_request_metadata(req, meatadata_out_storage, &meatadata_len);
	check_err(req_err);
	if (NULL == req_err) {
		std::cout << "req metadata len : " << meatadata_len << std::endl;
		if (0 != meatadata_len) {
			std::cout << "req metadata: " << *meatadata_out_storage << std::endl;
		}
	}
#endif

	wilton_Request_send_response(req, index_page.c_str(), index_page.size());	
}

void post_handler(void* ctx, wilton_Request* req) {
	(void) ctx;

#ifdef VERBAL	
	std::cout << "post_handler called" << std::endl;
#endif

	wilton_Request_send_response(req, index_page.c_str(), index_page.size());	
}

void init_logging() {
    const char* lconf = "{ \
					        \"appenders\": [{  \
					            \"appenderType\": \"CONSOLE\",  \
					            \"filePath\": \"./log/log.txt\",  \
					            \"layout\": \"%d{%Y-%m-%d %H:%M:%S,%q} [%-5p %-5.5t %-20.20c] %m%n\",  \
					            \"thresholdLevel\": \"TRACE \"  \
					        }],  \
					        \"loggers\": [{  \
					            \"name\": \"my.logger.name\",  \
					            \"level\": \"TRACE \"  \
					        }]  \
					    }";


    char* lerr = wilton_logger_initialize(lconf, (int) strlen(lconf));
    check_err(lerr);
}

int main(int argc, char const *argv[])
{
	index_page = std::string("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\"> \
			<html> \
			 <head> \
			  <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"> \
			  <title>Пример веб-страницы</title> \
			 </head> \
			 <body> \
			  <h1>Заголовок</h1> \
			  <!-- Комментарий --> \
			  <p>Первый абзац.</p> \
			  <p>Второй абзац.</p> \
			 </body> \
			</html> \
			");


	std::string str_work_ticks_count("40");
	std::string str_threads_count("1");

	if (1 < argc) {
		str_threads_count.assign(argv[1]);
	}

	if (2 < argc) {
		str_work_ticks_count.assign(argv[2]);
	}

	// Сделаем валидацию позже

	std::uint32_t max_ticks = std::stoi(str_work_ticks_count);

 
	init_logging();

	char* err;
    wilton_Server* server;
    std::string server_conf = std::string("{ \
    							\"numberOfThreads\": ") + str_threads_count + std::string(", \
    							\"tcpPort\": 8080, \
								\"ipAddress\": \"127.0.0.1\"}");


    wilton_HttpPath* path_to_hello;
    err = wilton_HttpPath_create(&path_to_hello, "GET", (int) strlen("GET"), "/hello", (int) strlen("/hello"), NULL, hello);
    check_err(err);

    wilton_HttpPath* path_to_index;
    err = wilton_HttpPath_create(&path_to_index, "GET", (int) strlen("GET"), "/index.html", (int) strlen("/index.html"), NULL, index_handler);
    check_err(err);

    wilton_HttpPath* path_to_POST_test;
    err = wilton_HttpPath_create(&path_to_POST_test, "POST", (int) strlen("POST"), "/index.html", (int) strlen("/index.html"), NULL, post_handler);
    check_err(err);

    const int paths_size = 3;
    int paths_counter = 0;
	wilton_HttpPath* paths[paths_size];
    paths[paths_counter++] = path_to_hello;
    paths[paths_counter++] = path_to_index;
    paths[paths_counter++] = path_to_POST_test;

    err = wilton_Server_create(&server, server_conf.c_str(), (int) strlen(server_conf.c_str()), paths, paths_size);
    check_err(err);

    sleep(max_ticks);

    // std::uint32_t counter = 0;

    // std::cout << "after wilton_Server_create" << std::endl;
    // while (true) {

    // 	//std::cout << counter << "\n";

    // 	usleep(100000);
    // 	++counter;
    // 	if (max_ticks == counter) {
    // 		break;
    // 	}
    // }

    // std::cout << "after wait time ended" << std::endl;


    wilton_HttpPath_destroy(path_to_hello);
    wilton_HttpPath_destroy(path_to_index);

    err = wilton_Server_stop(server);
    check_err(err);


	return 0;
}
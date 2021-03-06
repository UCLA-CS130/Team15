CC=g++
GTEST_DIR=nginx-configparser/googletest/googletest

CFLAGS=-g -std=c++11 -static-libgcc -static-libstdc++ -pthread -Wl,-Bstatic -lboost_log -lboost_thread -lboost_filesystem -lboost_regex -lboost_system -lboost_iostreams -lm -lz -Wall -Werror
HEADERS=Response.h Request.h HttpMessage.h server.h connection.h RequestMgr.h RequestHandler.h EchoHandler.h StaticHandler.h nginx-configparser/config_parser.h requestconfig.h mime_types.hpp NotFoundHandler.h StatusHandler.h ServerStatus.h markdown.h markdown-tokens.h CompressionHandler.h
CLASS_SOURCES=server.cpp connection.cpp RequestMgr.cpp RequestHandler.cpp EchoHandler.cpp StaticHandler.cpp ProxyHandler.cpp nginx-configparser/config_parser.cc mime_types.cpp NotFoundHandler.cpp StatusHandler.cpp ServerStatus.cpp markdown.cpp markdown-tokens.cpp CompressionHandler.cpp

SOURCES=main.cpp $(CLASS_SOURCES)
TESTFILES=server_test.cc connection_test.cc Http_test.cc RequestMgr_test.cc RequestHandler_test.cc EchoHandler_test.cc StaticHandler_test.cc NotFoundHandler_test.cc StatusHandler_test.cc ServerStatus_test.cc ProxyHandler_test.cc CompressionHandler_test.cc
all: webserver

webserver : $(SOURCES) $(HEADERS)
	$(CC) $(SOURCES) $(CFLAGS) -o $@

clean :
	rm -f webserver server_test *~ *.o *.a *.gcda *.gcno *.gcov coverage_results
	rm -rf deploy


test : $(SOURCES) $(HEADERS) $(TESTFILES)
	$(CC) -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
	ar -rv libgtest.a gtest-all.o
	$(CC) -isystem $(GTEST_DIR)/include $(TESTFILES) $(CLASS_SOURCES) ${GTEST_DIR}/src/gtest_main.cc libgtest.a $(CFLAGS) -fprofile-arcs -ftest-coverage -o server_test
	./server_test
	cd nginx-configparser && chmod a+x build_tests.sh && ./build_tests.sh && ./config_parser_test

check :
	./test_script.sh
	python parse_coverage.py

deploy :
	mkdir deploy
	cp -r static deploy
	cp config_file deploy
	tar -xf binary.tar -C deploy/
	cp Dockerfile.run deploy
	docker build -f ./deploy/Dockerfile.run -t webserver ./deploy
	docker save -o webserver_img.tar webserver

integration :
	./integration_test.sh

proxy_302_test :
	chmod +x proxy_302_local_test.sh && ./proxy_302_local_test.sh


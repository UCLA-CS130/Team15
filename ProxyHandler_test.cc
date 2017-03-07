#include <string>
#include <fstream>
#include <boost/log/trivial.hpp>
#include "ProxyHandler.h"
#include "gtest/gtest.h"

namespace Team15 {
namespace server {

  TEST(ProxyHandlerInit, SimpleHost) {
    ProxyHandler handler;
    std::string uri_prefix = "/2020";
    NginxConfig config;
    config.statements_.emplace_back(new NginxConfigStatement);
    config.statements_.back().get()->tokens_.push_back("server_host");
    config.statements_.back().get()->tokens_.push_back("localhost");
    EXPECT_EQ(handler.Init(uri_prefix, config), RequestHandler::OK);

  }

  TEST(ProxyHandlerInit, SimplePort) {
    ProxyHandler handler;
    std::string uri_prefix = "/2020";
    NginxConfig config;
    config.statements_.emplace_back(new NginxConfigStatement);
    config.statements_.back().get()->tokens_.push_back("server_port");
    config.statements_.back().get()->tokens_.push_back("2020");
    EXPECT_EQ(handler.Init(uri_prefix, config), RequestHandler::OK);
  }

  TEST(ProxyHandlerInit, WrongNumArgsToHost) {
    ProxyHandler handler;
    std::string uri_prefix = "/2020";
    NginxConfig config;
    config.statements_.emplace_back(new NginxConfigStatement);
    config.statements_.back().get()->tokens_.push_back("server_port");
    config.statements_.back().get()->tokens_.push_back("2020");
    config.statements_.back().get()->tokens_.push_back("test");
    EXPECT_EQ(handler.Init(uri_prefix, config),RequestHandler::INVALID_INPUT);
  }

  TEST(ProxyHandlerCreateProxyRequest, SimpleRequest) {
    ProxyHandler handler;
    std::string uri_prefix = "/2020";
    NginxConfig config;
    config.statements_.emplace_back(new NginxConfigStatement);
    config.statements_.back().get()->tokens_.push_back("server_port");
    config.statements_.back().get()->tokens_.push_back("2020");
    handler.Init(uri_prefix, config);
    Request test_request;
    test_request.Seturi("/2020/index.html");
    test_request.SetMethod("GET");
    test_request.SetBody("test body");
    
    EXPECT_EQ(handler.CreateProxyRequest(test_request)->uri(), "/index.html");
  }

  TEST(ProxyHandlerCreateProxyRequest, NoUri) {
    ProxyHandler handler;
    std::string uri_prefix = "/2020";
    NginxConfig config;
    config.statements_.emplace_back(new NginxConfigStatement);
    config.statements_.back().get()->tokens_.push_back("server_port");
    config.statements_.back().get()->tokens_.push_back("2020");
    handler.Init(uri_prefix, config);
    Request test_request;
    test_request.Seturi("/2020");
    test_request.SetMethod("GET");
    test_request.SetBody("test body");

    EXPECT_EQ(handler.CreateProxyRequest(test_request)->uri(), "/");
  }

  TEST(ProxyHandlerError, SimpleError) {
    ProxyHandler handler;
    boost::system::error_code test_error;
    EXPECT_EQ(handler.HandleError("Test Error", test_error), RequestHandler::OK);
  }

  TEST(ProxyHandlerParseAndQuery, QuestionMarkInString) {
    ProxyHandler handler;
    std::string relative_url = "test?url";
    std::string path = "test_path/";
    std::string query = "test_query/";
    handler.ParsePathAndQuery(relative_url, path, query);
    EXPECT_EQ(path, "test_path/test");
    EXPECT_EQ(query, "test_query/url");
  }

  TEST(ProxyHandlerParseAndQuery, NoQuestionMark) {
    ProxyHandler handler;
    std::string relative_url = "test";
    std::string path = "test_path/";
    std::string query = "test_query/";
    handler.ParsePathAndQuery(relative_url, path, query);
    EXPECT_EQ(path, "test_path/test");
    EXPECT_EQ(query, "test_query/");
  }

}
}

#ifndef STATICHANDLER_H
#define STATICHANDLER_H

#include "RequestHandler.h"
#include <boost/filesystem/path.hpp>
#include "CompressionHandler.h"
namespace Team15 {
namespace server {

class StaticHandler : public RequestHandler {
 public:
  virtual RequestHandler::Status Init(const std::string& uri_prefix,
        NginxConfig config);
  virtual RequestHandler::Status HandleRequest(const Request& request, 
        Response* response);
 private:
  std::string uri_prefix_;
  boost::filesystem::path rootPath_;
  CompressionHandler compressionHandler_;
};

//REGISTER_REQUEST_HANDLER(StaticHandler);

}
}

#endif // STATICHANDLER_H

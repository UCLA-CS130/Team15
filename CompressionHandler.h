#ifndef COMPRESSIONHANDLER_H
#define COMPRESSIONHANDLER_H

#include "RequestHandler.h"
namespace Team15 {
namespace server {

class CompressionHandler : public RequestHandler {
 public:
  virtual RequestHandler::Status Init(const std::string& uri_prefix,
        NginxConfig config);
  virtual RequestHandler::Status HandleRequest(const Request& request, 
        Response* response);
};

}
}

#endif // COMPRESSIONHANDLER_H

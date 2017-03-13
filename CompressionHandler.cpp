#include "CompressionHandler.h"
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <sstream>

namespace Team15 {
namespace server {

  RequestHandler::Status CompressionHandler::Init(const std::string& uri_preix,NginxConfig config) {
    // don't need to do much
    return RequestHandler::Status::OK;
  }

  RequestHandler::Status CompressionHandler::HandleRequest(const Request& request, Response* response) {
    std::stringstream ss, comp;
    ss << response->body();
    boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
    in.push(boost::iostreams::gzip_compressor());
    in.push(ss);
    boost::iostreams::copy(in,comp);
    response->SetBody(comp.str());
    response->AddHeader("Content-Encoding","gzip");
    response->AddHeader("Content-Length",std::to_string(comp.str().size()));
    return RequestHandler::Status::OK;
  }
    
    
}
}

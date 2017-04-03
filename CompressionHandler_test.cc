#include <string>
#include <fstream>
#include <boost/log/trivial.hpp>

#include "gtest/gtest.h"
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <sstream>
#include "CompressionHandler.h"
TEST(CompressionHandlerTest, BasicGzipTest) {
  std::string basic_string = "basic_string";
Team15::server::CompressionHandler ch;
  ch.Init("",NginxConfig());
Request r;
Response response;
response.SetBody(basic_string);
  ch.HandleRequest(r,&response);
  boost::iostreams::filtering_streambuf<boost::iostreams::input> inbuf;
  inbuf.push(boost::iostreams::gzip_decompressor());
std::stringstream compressed;
compressed << response.body();
  inbuf.push(compressed);
  std::stringstream out;
  boost::iostreams::copy(inbuf,out);
EXPECT_STREQ(out.str().c_str(),basic_string.c_str());
}


TEST(CompressionHandlerTest, LongStringGzip) {
  std::string basic_string = "asdfjklqewryio12347890zxcvnm,adfsjkl;oiu143\r\nasdfklasdf010238474327878cxcjxlzcjxzKLCJkxzLCJZXKppqowueriwqeorunmmdfsfsdkfsdi23782318!#$@%^&*^%$#@!%^&GFDKLVCJXKLVCZXVZVXCM<NVCZXM<NVZCX";
Team15::server::CompressionHandler ch;
  ch.Init("",NginxConfig());
Request r;
Response response;
response.SetBody(basic_string);
  ch.HandleRequest(r,&response);
  boost::iostreams::filtering_streambuf<boost::iostreams::input> inbuf;
  inbuf.push(boost::iostreams::gzip_decompressor());
std::stringstream compressed;
compressed << response.body();
  inbuf.push(compressed);
  std::stringstream out;
  boost::iostreams::copy(inbuf,out);
EXPECT_STREQ(out.str().c_str(),basic_string.c_str());
}



TEST(CompressionHandlerTest, BinaryGzipTest) {
std::ifstream is("libgtest.a",std::ios::in | std::ios::binary);
ASSERT_EQ(!!is,true); // check that is is valid
std::string basic_string;

char c;
while (is.get(c)) {
basic_string +=c;
}
is.close();

Team15::server::CompressionHandler ch;
  ch.Init("",NginxConfig());
Request r;
Response response;
response.SetBody(basic_string);
  ch.HandleRequest(r,&response);
  boost::iostreams::filtering_streambuf<boost::iostreams::input> inbuf;
  inbuf.push(boost::iostreams::gzip_decompressor());
std::stringstream compressed;
compressed << response.body();
  inbuf.push(compressed);
  std::stringstream out;
  boost::iostreams::copy(inbuf,out);
EXPECT_STREQ(out.str().c_str(),basic_string.c_str());
}

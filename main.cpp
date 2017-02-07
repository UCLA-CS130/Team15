#include "server.h"
#include <iostream>
#include <boost/version.hpp>
#include "nginx-configparser/config_parser.h"
#include <vector>
#include <unordered_map>
using namespace std;

void parseValuePairs(unordered_map<string, string> &valuePairs, NginxConfig config) {

  for (int k = 0; k < (int) config.statements_.size();k++) { 

    if (config.statements_[k]->child_block_ != NULL) {
      //config.statements_[k]->tokens_.size() == 1) {
      parseValuePairs(valuePairs, *(config.statements_[k]->child_block_));
      //continue;
    }

    if (config.statements_[k]->tokens_.size() == 1)
      continue;

    string token1, token2;
    if (config.statements_[k]->tokens_[0] == "path") {
      token1 = config.statements_[k]->tokens_[1];
      token2 = config.statements_[k]->tokens_[2];
    } else {
      token1 = config.statements_[k]->tokens_[0];
      token2 = config.statements_[k]->tokens_[1];
    }

    printf("%s, %s\n", token1.c_str(), token2.c_str());
    valuePairs.insert({token1, token2});
  }
}

int main(int argc, char** argv) {

  NginxConfigParser config_parser;
  NginxConfig config;
  bool validParse = config_parser.Parse(argv[1], &config);
  if (!validParse) return -1;

  unordered_map<string, string> valuePairs;
  parseValuePairs(valuePairs, config);

  // printf("%s\n", valuePairs["listen"].c_str());
  // printf("%s\n", valuePairs["server_name"].c_str());

  Team15::server::server s(valuePairs["server_name"],valuePairs["listen"]);   
  //s.run();
  
  return 0;
}

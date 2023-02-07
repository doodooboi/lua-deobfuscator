#include <fstream>
#include <string>
#include "boost/regex.hpp"

#include <iostream>

bool debug {false};

std::string convertHex(const boost::smatch &match) {
  if (debug) {std::cout << match[0].str() << " => " << std::to_string(std::stoull(match[0].str(),nullptr,16)) << std::endl;}

  return std::to_string(std::stoull(match[0].str(),nullptr,16));
}

std::string applyMath(const boost::smatch &match) {
  
}

int main() {
  std::ofstream output("Output.txt");
  std::ifstream input;

  std::string curline;

  boost::regex removeComments {"-{2}\\[\\[(.+?(?=\\]\\]))\\]\\]"};
  boost::regex getAllHexadecimal {"(0x).+?(?=\\W)"};
  boost::regex getNumbers {"(\\-|)[0-9]+(\\s+)[\\+|\\-|\\*|\\/](\\s+)[0-9]*"}; // use this after hex -> int

  input.open("stupid.txt");

  while (std::getline(input, curline)) {

    curline = boost::regex_replace(curline, removeComments, "");
    curline = boost::regex_replace(curline,getAllHexadecimal, convertHex);

    output << curline << "\n";
  }

  output.close();
  input.close();

  return 0;
}
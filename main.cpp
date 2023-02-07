#include <fstream>
#include <string>
#include "boost/regex.hpp"

#include <iostream>
#include <vector>

bool debug {false};
int hex {0};
int comments {0};

std::vector<std::string> split(std::string const &str, const char delim) {
    std::vector<std::string> out {};
    size_t start;
    size_t end = 0;
 
    while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
    {
        end = str.find(delim, start);
        out.push_back(str.substr(start, end - start));
    }

  return out;
}

std::string convertHex(const boost::smatch &match) {
  if (debug) {std::cout << match[0].str() << " => " << std::to_string(std::stoull(match[0].str(),nullptr,16)) << std::endl;}

  hex++;

  return std::to_string(std::stoull(match[0].str(),nullptr,16));
}

std::string removeComment(const boost::smatch &match) {
  comments++;

  return "";
}

std::string doMath(const boost::smatch &match) {
  std::string s = match[0].str();
  std::size_t divider = s.find("/");

  unsigned long long n1 = std::stoull(s.substr(0, divider));
  unsigned long long n2 = std::stoull(s.substr(divider + 1, s.length()));

  return std::to_string(n1 / n2);
}

std::string removeParenthesis(const boost::smatch &match) {
  std::string s = match[0].str();

  return s.substr(1, s.length() - 2);
}

std::string fixUniStrings(const boost::smatch &match) {
  std::string s = match[0].str();


  return s.substr(1, s.length() - 2);
}

int main() {
  std::ofstream output("Output-2.txt");
  std::ifstream input;

  std::string curline;

  boost::regex removeComments {"-{2}\\[\\[(.+?(?=\\]\\]))\\]\\]"};
  boost::regex getAllHexadecimal {"(0x).+?(?=\\W)"};
  boost::regex getNumbers {"(\\-|)[0-9]+(\\s+)[\\+|\\-|\\*|\\/](\\s+)[0-9]*"}; // use this after hex -> int
  boost::regex removeParenthesisReg {"\\([0-9]{1,}\\)"};
  boost::regex getUniStrings {"\\[\".+?(?=\")\"\\]"}

  input.open("stupid.txt");

  while (std::getline(input, curline)) {

    curline = boost::regex_replace(curline, removeComments, removeComment);
    curline = boost::regex_replace(curline,getAllHexadecimal, convertHex);
    
    for (int i = 0; i < 2; i++) { // :numbers
      curline = boost::regex_replace(curline, getNumbers, doMath);
      curline = boost::regex_replace(curline, removeParenthesisReg, removeParenthesis);
    }
    curline = boost::regex_replace(curline, getNumbers, doMath); // :numbers



    output << curline << "\n";
  }

  std::cout << "Cleaned " << hex << " hex characters and " << comments << " comments" << std::endl;

  output.close();
  input.close();

  return 0;
}
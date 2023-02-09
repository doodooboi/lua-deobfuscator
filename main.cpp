#include <fstream>
#include <string>
//#include <filesystem>
#include "boost/regex.hpp"

#include <iostream>
#include <vector>

bool debug {false};
int hex {0};
int comments {0};

boost::regex getDoubleDot {"\\.\\."};

std::vector<std::string> split(std::string const &str, const char delim) {
    std::vector<std::string> out {};
    std::size_t start;
    std::size_t end = 0;
 
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
  std::string output;
  std::vector<std::string> splitted {split(s, '\\')};

  for (unsigned short int i = 0; i < splitted.size(); i++) {
    output += static_cast<char>(std::stoi(splitted[i]));
  }

  return output;
}

std::string fixStringChar(const boost::smatch &match) {
  std::string s = match[0].str();
  std::size_t start {s.find('(') + 1};
  std::size_t end {s.find(')')};

  std::string output {""};
  std::vector<std::string> splitted {split(s.substr(start, end-start), ',')};

  for (unsigned short int i = 0; i < splitted.size(); i++) {
    output += static_cast<char>(std::stoi(splitted[i]));
  }

  //std::cout << s << " :-> " << output << std::endl;

  return "\"" + output + "\"";
}

std::string fixConcatChar(const boost::smatch &match) {
  std::string s = match[0].str();
  std::string output {""};

  s = boost::regex_replace(s, getDoubleDot, ".");

  std::vector<std::string> splitted {split(s, '.')};

  std::cout << s << "-:>" << splitted.size() << std::endl;

  for (unsigned short int i = 0; i < splitted.size(); i++) {
    std::cout << splitted[i].substr(splitted[i].find('"') + 1,1) << "!" << std::endl;
    output += splitted[i].substr(splitted[i].find('"') + 1,1);//static_cast<char>(std::stoi(splitted[i]));
  }

  //std::cout << s << " :-> " << output << std::endl;

  return "\"" + output + "\"";
}

int main() {
  std::ofstream output("Output-4.txt");
  std::ifstream input;

  std::string curline;

  boost::regex removeComments {"-{2}\\[\\[(.+?(?=\\]\\]))\\]\\]"};
  boost::regex getAllHexadecimal {"(0x).+?(?=\\W)"};
  boost::regex getNumbers {"(\\-|)[0-9]+(\\s+)[\\+|\\-|\\*|\\/](\\s+)[0-9]*"}; // use this after hex -> int
  boost::regex removeParenthesisReg {"\\([0-9]{1,}\\)"};
  boost::regex getUniStrings {"\\\\(\\d)+"};
  boost::regex stringCharRegex {"((_ENV(\\[(\"|)string(\"|)(\\]|))\\[\"char\"\\]|string.char)\\((.+?(?=\\)))\\))"};
  boost::regex getConcatChar {"\"c\"(\\s{0,}\\.{0,}\\s{0,})\"h\"(\\s{0,}\\.{0,}\\s{0,})\"a\"(\\s{0,}\\.{0,}\\s{0,})\"r\""};

  input.open("stupid.txt");

  while (std::getline(input, curline)) {

    curline = boost::regex_replace(curline, removeComments, removeComment);
    curline = boost::regex_replace(curline,getAllHexadecimal, convertHex);
    
    for (int i = 0; i < 2; i++) { // :numbers
      curline = boost::regex_replace(curline, getNumbers, doMath);
      curline = boost::regex_replace(curline, removeParenthesisReg, removeParenthesis);
    }
    curline = boost::regex_replace(curline, getNumbers, doMath); // :numbers

    curline = boost::regex_replace(curline, getUniStrings, fixUniStrings);
    curline = boost::regex_replace(curline, getConcatChar, fixConcatChar);  
    curline = boost::regex_replace(curline, stringCharRegex, fixStringChar);
    curline = boost::regex_replace(curline, getConcatChar, fixConcatChar);  
    curline = boost::regex_replace(curline, stringCharRegex, fixStringChar);

    curline = boost::regex_replace(curline, getConcatChar, fixConcatChar);  

    output << curline << "\n";
  }

  std::cout << "Cleaned " << hex << " hex characters and " << comments << " comments" << std::endl;

  output.close();
  input.close();

  // std::cout << "Input: " << std::filesystem::file_size("stupid.txt") << std::endl;
  // std::cout << "Output: " << std::filesystem::file_size("Output-4.txt") << std::endl;
  
  return 0;
}
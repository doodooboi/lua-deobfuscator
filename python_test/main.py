import re

# boost::regex removeComments {"-{2}\\[\\[(.+?(?=\\]\\]))\\]\\]"};
  # boost::regex getAllHexadecimal {"(0x).+?(?=\\W)"};
  # boost::regex getNumbers {"(\\-|)[0-9]+(\\s+)[\\+|\\-|\\*|\\/](\\s+)[0-9]*"}; // use this after hex -> int
  # boost::regex removeParenthesisReg {"\\([0-9]{1,}\\)"};
  # boost::regex getUniStrings {"\\\\(\\d)+"};
  # boost::regex stringCharRegex {"((_ENV\\[\"string\"\\]\\[\"char\"\\]|string.char)\\((.+?(?=\\)))\\))"};

# while (std::getline(input, curline)) {

#     curline = boost::regex_replace(curline, removeComments, removeComment);
#     curline = boost::regex_replace(curline,getAllHexadecimal, convertHex);
    
#     for (int i = 0; i < 2; i++) { // :numbers
#       curline = boost::regex_replace(curline, getNumbers, doMath);
#       curline = boost::regex_replace(curline, removeParenthesisReg, removeParenthesis);
#     }
#     curline = boost::regex_replace(curline, getNumbers, doMath); // :numbers

#     curline = boost::regex_replace(curline, getUniStrings, fixUniStrings);
#     curline = boost::regex_replace(curline, stringCharRegex, fixStringChar);
#     curline = boost::regex_replace(curline, stringCharRegex, fixStringChar);

#     output << curline << "\n";
#   }

#   std::cout << "Cleaned " << hex << " hex characters and " << comments << " comments" << std::endl;

#   output.close();
#   input.close();

def fixHexadecimal(match):
  return str(int(match.group(), 16));

def fixParenthesis(match):
  print(match.group(), " -> ", match.group()[1:-1])

  return match.group()[1:-1];

def fixMath(match):
  match = match.group();
  divider = match.find("/");

  n1 = int(match[0:divider]);
  n2 = int(match[divider + 1:len(match)]);

  return str(n1 / n2);

patterns = {
  "comments": "-{2}\\[\\[(.+?(?=\\]\\]))\\]\\]",
  "hexadecimal": "(0x).+?(?=\\W)",
  "numbers":"(\\-|)[0-9]+(\\s+)[\\+|\\-|\\*|\\/](\\s+)[0-9]*",
  "parenthesis": "\\([0-9]{1,}\\)",
  "unicode": "\\\\(\\d)+",
  "string": "((_ENV\\[\"string\"\\]\\[\"char\"\\]|string.char)\\((.+?(?=\\)))\\))"
}

def main():
  output = open("./py-output.txt", "w");

  with open("../stupid.txt", 'r') as f:
    for line in f:
        line = re.sub(patterns["comments"], "", line);
        line = re.sub(patterns["hexadecimal"], fixHexadecimal, line);

        for i in range(0, 2):
          line = re.sub(patterns["numbers"], fixMath, line);
          line = re.sub(patterns["parenthesis"], fixParenthesis, line);

        line = re.sub(patterns["numbers"], fixMath, line);
        output.write(line);

main()
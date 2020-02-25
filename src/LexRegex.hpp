#ifndef REGEX_HPP
#define REGEX_HPP

#include <unordered_set>
#include <vector>

#include "LexNfa.hpp"

namespace Lex {

  extern std::unordered_set<char> regexOperators;
  extern std::unordered_set<char> brackets;
  extern std::unordered_set<char> nongraphicalElements;
  extern std::unordered_set<char> miscellaneousElements;

  struct RegexElement {
    char symbol;
    bool isOperator;

    RegexElement();
    RegexElement(char symbol);
    RegexElement(char symbol, bool isOperator);
  };

  int operatorPrecedence(char operatorSymbol);
  std::vector<RegexElement> parseRegex(std::string regexPattern);
  std::vector<RegexElement> insertExplicitConcatenationOperator(std::string regexPattern);
  std::vector<RegexElement> convertRegexToPostFix(std::string regexPattern);
  Nfa convertRegexToNfa(std::string regexPattern, std::string tokenType, int tokenPriority);
}

#endif
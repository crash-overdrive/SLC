#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <deque>
#include <set>

#include "LexDfa.hpp"
#include "LexNfa.hpp"
#include "LexRegex.hpp"
#include "LexToken.hpp"

namespace Lex {

  struct Scanner {
    Dfa finalDfa;
    std::vector<Token> matchedTokens;
    const std::set<std::string> stripTokens = {"LINE_TERMINATOR", "WHITESPACE", "SINGLE_LINE_COMMENT", "MULTI_LINE_COMMENT"};

    friend std::istream &operator>>(std::istream& stream, Scanner& scanner);
    void initializeDfaForRegexPatterns(std::string regexTokenMappingFilePath);
    bool maximalMunch(std::deque<char> input);
    bool scan(std::istream &inputStream);
    std::vector<Token> getTokens() const;
  };
  std::istream& operator>>(std::istream& stream, Scanner& scanner);

}

#endif

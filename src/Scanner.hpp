#ifndef LEXER_HPP
#define LEXER_HPP

#include <iostream>

namespace Lex {

class Scanner {
public:
  void read(const char c);
  bool error() const;
  // vector<Token> buildTokens();

private:
  friend std::istream &operator<<(std::istream &IStream, Scanner &Scanner);
  // DFA
  // NFA
};

std::istream &operator>>(std::istream &IStream, Scanner &Scanner);

} // namespace Lex

#endif // LEXER_HPP

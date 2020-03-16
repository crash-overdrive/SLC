#ifndef LEXTOKEN_HPP
#define LEXTOKEN_HPP

#include <iostream>
#include <string>

namespace Lex {

struct Token {
  Token();
  Token(const std::string &type, const std::string &lexeme);
  std::string type;
  std::string lexeme;
};

std::istream &operator>>(std::istream &stream, Token &tok);
std::ostream &operator<<(std::ostream &stream, const Token &tok);

} // namespace Lex

#endif // LEXTOKEN_HPP

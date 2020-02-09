#ifndef LEXTOKEN_HPP
#define LEXTOKEN_HPP

#include <iostream>
#include <string>

namespace Lex {

struct Token {
  Token();
  Token(const std::string &Type, const std::string &Lexeme);
  std::string Type;
  std::string Lexeme;
};

std::istream &operator>>(std::istream &Stream, Token &Tok);
std::ostream &operator<<(std::ostream &Stream, const Token &Tok);

} // namespace Lex

#endif // LEXTOKEN_HPP

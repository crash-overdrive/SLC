#include "LexToken.hpp"

Lex::Token::Token(std::string type, std::string lexeme)
    : type(std::move(type)), lexeme(std::move(lexeme)) {}

std::istream &Lex::operator>>(std::istream &stream, Token &tok) {
  return stream >> tok.type >> tok.lexeme;
}

std::ostream &Lex::operator<<(std::ostream &stream, const Token &tok) {
  return stream << tok.type << ' ' << tok.lexeme;
}

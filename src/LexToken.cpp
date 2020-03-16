#include "LexToken.hpp"

Lex::Token::Token() : type(), lexeme() {}

Lex::Token::Token(const std::string &type, const std::string &lexeme)
    : type(type), lexeme(lexeme) {}

std::istream &Lex::operator>>(std::istream &stream, Token &tok) {
  return stream >> tok.type >> tok.lexeme;
}

std::ostream &Lex::operator<<(std::ostream &stream, const Token &tok) {
  return stream << tok.type << ' ' << tok.lexeme;
}

#include "LexToken.hpp"

Lex::Token::Token() : Type(), Lexeme() {}

Lex::Token::Token(const std::string &Type, const std::string &Lexeme)
    : Type(Type), Lexeme(Lexeme) {}

std::istream &Lex::operator>>(std::istream &Stream, Token &Tok) {
  return Stream >> Tok.Type >> Tok.Lexeme;
}

std::ostream &Lex::operator<<(std::ostream &Stream, const Token &Tok) {
  return Stream << Tok.Type << ' ' << Tok.Lexeme;
}

#ifndef PARSEDFA_HPP
#define PARSEDFA_HPP

#include "LexToken.hpp"
#include "ParseTree.hpp"
#include <string>
#include <unordered_map>

namespace Parse {

class DFA {
public:
  DFA();
  void read(const Lex::Token &Tok);
  void clear();
  bool error() const;
  bool accept() const;
  Tree buildTree();
  bool parse(std::istream &IStream);
  bool parse(std::istream &IStream, std::ostream &OStream);

private:
  struct Production {
    std::string LHS;
    std::vector<std::string> RHS;
  };

  struct State {
    std::unordered_map<std::string, const State &> Shifts;
    std::unordered_map<std::string, const Production &> Reduces;
  };

  friend std::istream &operator>>(std::istream &Stream, DFA &DFA);
  std::unordered_map<unsigned int, Production> Productions;
  std::unordered_map<unsigned int, State> States;
  std::vector<const Production *> FinalProductions;
  std::vector<const State *> StateStack;
  std::vector<std::unique_ptr<Node>> NodeStack;
  std::string StartSymbol;
  bool errorState;
  const State *Current;
  void shift(const State &State);
  void reduce(const Production &Production);
};

std::istream &operator>>(std::istream &Stream, Parse::DFA &DFA);

} // namespace Parse

#endif // PARSERDFA_HPP

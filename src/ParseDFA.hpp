#ifndef PARSEDFA_HPP
#define PARSEDFA_HPP

#include "LexToken.hpp"
#include "ParseTree.hpp"

namespace Parse {

class DFA {
public:
  DFA();
  void read(const Lex::Token &tok);
  void clear();
  bool error() const;
  bool accept() const;
  Tree buildTree();
  bool parse(std::istream &iStream);
  bool parse(const std::vector<Lex::Token> &tokens);

private:
  struct Production {
    std::string lhs;
    std::vector<std::string> rhs;
  };

  struct State {
    std::unordered_map<std::string, const State &> shifts;
    std::unordered_map<std::string, const Production &> reduces;
  };

  friend std::istream &operator>>(std::istream &stream, DFA &dfa);
  std::unordered_map<unsigned int, Production> productions;
  std::unordered_map<unsigned int, State> states;
  std::vector<const Production *> finalProductions;
  std::vector<const State *> stateStack;
  std::vector<std::unique_ptr<Node>> nodeStack;
  std::string startSymbol;
  bool errorState;
  const State *current;
  void shift(const State &state);
  void reduce(const Production &production);
};

std::istream &operator>>(std::istream &stream, Parse::DFA &dfa);

} // namespace Parse

#endif // PARSERDFA_HPP

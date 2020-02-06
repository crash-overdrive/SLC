#ifndef PARSEDFA_HPP
#define PARSEDFA_HPP

#include "ParseTree.hpp"
#include <string>
#include <unordered_map>

namespace Parse {

class DFA {
public:
  DFA();
  void configure(std::istream &Stream);
  void read(const std::string &Token);
  bool error() const;
  bool accept() const;
  Tree buildTree();

private:
  struct Production {
    std::string LHS;
    std::vector<std::string> RHS;
  };

  struct State {
    std::unordered_map<std::string, const State &> Shifts;
    std::unordered_map<std::string, const Production &> Reduces;
  };

  std::unordered_map<unsigned int, Production> Productions;
  std::unordered_map<unsigned int, State> States;
  std::vector<std::reference_wrapper<const Production>> FinalProductions;
  std::vector<std::reference_wrapper<const State>> StateStack;
  std::vector<std::unique_ptr<Node>> NodeStack;
  std::string StartSymbol;
  bool errorState;
  State &Current;
  void shift(const State &State);
  void reduce(const Production &Production);
};

} // namespace Parse

#endif // PARSER_HPP

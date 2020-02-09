#ifndef LEXER_HPP
#define LEXER_HPP

#include <iostream>
#include <vector>

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

struct Definition {
  Definition(std::string type, std::string regex) : type(type), regex(regex){};
  std::string type;
  std::string regex;
};

struct DfaTransition {
  std::vector<int> previousStates;
  std::vector<int> nextStates;
  char transitionSymbol;

  DfaTransition(std::vector<int> previousStates, std::vector<int> nextStates,
                char transitionSymbol)
      : previousStates(previousStates), nextStates(nextStates),
        transitionSymbol(transitionSymbol){};
};

struct Dfa {
  std::vector<std::vector<int>> states;
  std::vector<char> alphabets;
  std::vector<DfaTransition> transitions;
  std::vector<int> startStates;
  std::vector<int> acceptingStates;
};

struct NfaTransition {
  int previousState;
  int nextState;
  char transitionSymbol;

  NfaTransition(int previousState, int nextState, char transitionSymbol)
      : previousState(previousState), nextState(nextState),
        transitionSymbol(transitionSymbol){};
};

class Nfa {
public:
  std::vector<int> states;
  std::vector<char> alphabets;
  std::vector<NfaTransition> transitions;
  int startState;
  std::vector<int> acceptingStates;

  int getNumberOfStates();
  void initialiseStates(int numberOfStates);
  void setAlphabets(std::vector<char> givenAlphabets);
  void addTransition(int previousState, int nextState, char transitionSymbol);
  void setAcceptingStates(std::vector<int> newAcceptingStates);
  void setStartState(int state);
  std::vector<int> epsilonClosure(std::vector<int> givenStates);
  std::vector<int> getDfaStartStates();
  void
  computeDfaStatesAndTransitions(std::vector<std::vector<int>> &dfaStates,
                                 std::vector<DfaTransition> &dfaTransitions);
  std::vector<std::vector<int>> getDfaStates();
  Dfa convertToDfa();

  int getStartState();
  void shiftStates(int shiftValue);

  std::vector<int> getAcceptingStates();
};

Nfa concatenate(Nfa firstNfa, Nfa secondNfa);
Nfa orSelection(Nfa firstNfa, Nfa secondNfa);
Nfa kleeneStar(Nfa nfa);
Nfa plus(Nfa nfa);

std::istream &operator>>(std::istream &IStream, Scanner &Scanner);

} // namespace Lex

#endif // LEXER_HPP

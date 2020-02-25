#ifndef LEXER_HPP
#define LEXER_HPP

#include <iostream>
#include <vector>

namespace Lex {

constexpr char EPSILON = -1;

class Scanner {
public:
  void read(const char c);
  bool error() const;
  bool scan(std::istream &IStream);

private:
  friend std::istream &operator<<(std::istream &IStream, Scanner &Scanner);
  // DFA
  // NFA
};

struct RegexDefinition {
  std::string type;
  std::string regex;

  RegexDefinition(std::string type, std::string regex) : type(type), regex(regex){};
};

struct Token {
  std::string kind;
  std::string lexeme;

  Token() : kind("UNINITIALISED_TOKEN"), lexeme("") {};
  Token(std::string kind, std::string lexeme) : kind(kind), lexeme(lexeme) {};
};

struct DfaAcceptingState {
  std::vector<int> dfaStates;
  std::string tokenKind;
  int priority;

  DfaAcceptingState(): dfaStates({-1}), tokenKind("UNINITIALISED"), priority(INT32_MAX) {};
  DfaAcceptingState(std::vector<int> dfaStates, std::string tokenKind, int priority) :
  dfaStates(dfaStates), tokenKind(tokenKind), priority(priority) {};
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
  std::vector<DfaAcceptingState> acceptingStates;

  std::vector<int> currentStates;
  std::vector<char> processedTransitionSymbols;
  Token lastKnownAcceptingToken;

  // returns false if we ever get into a stuck state, true otherwise
  // updates processedTransitionSymbols
  // updates lastKnownAcceptingState, lastKnownAcceptingLexeme and
  // processedTransitionSymbols if we ever get into an accepting state
  bool transitionForward(char symbol);

  // returns last known accepting token, resets value of lastKnownAcceptingToken
  Token getLastKnownAcceptingToken();

  // resets the current state of Dfa to the start state
  void resetDfa();

  std::vector<Token> maximalMunch();
};

struct NfaAcceptingState {
  int nfaState;
  std::string tokenKind;
  int priority;

  NfaAcceptingState(int nfaState) :
  nfaState(nfaState), tokenKind("UNINITIALISED"), priority(INT32_MAX) {};

  NfaAcceptingState(int nfaState, std::string tokenKind, int priority) :
  nfaState(nfaState), tokenKind(tokenKind), priority(priority) {};
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
  std::vector<NfaAcceptingState> acceptingStates;

  int getNumberOfStates();
  void initialiseStates(int numberOfStates);
  void setAlphabets(std::vector<char> givenAlphabets);
  void setAcceptingStates(std::vector<NfaAcceptingState> newAcceptingStates);
  std::vector<NfaAcceptingState> getAcceptingStates();
  void setAcceptingStatesTokenAndPriority(std::string token, int priority);
  void setStartState(int state);
  int getStartState();
  void shiftStates(int shiftValue);
  void addTransition(int previousState, int nextState, char transitionSymbol);

  std::vector<int> epsilonClosure(std::vector<int> givenStates);
  std::vector<int> getDfaStartStates();
  void
  computeDfaStatesAndTransitions(std::vector<std::vector<int>> &dfaStates,
                                 std::vector<DfaTransition> &dfaTransitions);
  std::vector<DfaAcceptingState> getDfaAcceptingStates(std::vector<std::vector<int>> dfaStates);
  Dfa convertToDfa();
};

Nfa concatenate(Nfa& firstNfa, Nfa& secondNfa);
Nfa orSelection(Nfa& firstNfa, Nfa& secondNfa);
Nfa kleeneStar(Nfa& nfa);
Nfa plus(Nfa& nfa);

std::istream &operator>>(std::istream &IStream, Scanner &Scanner);

} // namespace Lex

#endif // LEXER_HPP

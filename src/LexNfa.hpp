#ifndef NFA_HPP
#define NFA_HPP

#include <set>
#include <unordered_map>

#include "LexDfa.hpp"

namespace Lex {

  struct MyHashFunctions {
    std::hash<int> intHash;

    size_t operator()(const std::set<int>& states) const;
  };

  struct Nfa {
    int startState;
    std::set<int> states;
    std::unordered_map<int, std::unordered_map<char, std::set<int>>> transitions;
    std::unordered_map<int, AcceptingStateInfo> acceptingStates;

    void setStartState(int state);
    int getStartState();
    int getNumberOfStates();
    void initialiseStates(int numberOfStates);
    void shiftStates(int shiftValue);
    void addTransition(int currentState, char transitionSymbol, int nextState);
    void addAcceptingState(int state, AcceptingStateInfo newAcceptingStateInfo);
    void printInfo();

    std::set<int> epsilonClosure(std::set<int> givenStates);
  };

  Nfa createSimpleNfa(char transitionSymbol, std::string tokenKind, int tokenPriority);
  Nfa concatenateOperator(Nfa& firstNfa, Nfa& secondNfa);
  Nfa orOperator(Nfa& firstNfa, Nfa& secondNfa);
  Nfa rangeOperator(Nfa& firstNfa, Nfa& secondNfa);
  Nfa starOperator(Nfa& nfa);
  Nfa notOperator(Nfa& nfa);
  Nfa questionOperator(Nfa& nfa);

  Dfa convertToDfa(Nfa& nfa);
}

#endif

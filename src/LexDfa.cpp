#include <cassert>
#include <iostream>

#include "LexDfa.hpp"

std::set<int> Lex::trapState = {-1};
std::set<char> Lex::alphabets;

void Lex::setAlphabets() {
  Lex::alphabets = {' ', '!', '\"', '#', '$', '%', '&', '\'', '(', ')',
                    '*', '+', ',', '-', '.', '/', ':', ';', '<', '=', '>',
                    '?', '@', '[', '\\', ']', '^',  '_', '{', '|', '}', '~',
                    '\b', '\f', '\n', '\t', '\r'};
  for (char c = '0'; c <= '9'; ++c) {
    Lex::alphabets.insert(c);
  }
  for (char c = 'A'; c <= 'Z'; ++c) {
    Lex::alphabets.insert(c);
  }
  for (char c = 'a'; c <= 'z'; ++c) {
    Lex::alphabets.insert(c);
  }
}

Lex::AcceptingStateInfo::AcceptingStateInfo()
    : tokenKind(""), tokenPriority(INT32_MAX) {}

Lex::AcceptingStateInfo::AcceptingStateInfo(std::string tokenKind,
                                            int tokenPriority)
    : tokenKind(tokenKind), tokenPriority(tokenPriority) {}

// bool Lex::AcceptingStateInfo::operator==(const AcceptingStateInfo&
// acceptingState) const {
//   return this->tokenPriority == acceptingState.tokenPriority &&
//   this->tokenKind == acceptingState.tokenKind;
// }

void Lex::Dfa::setStartState(int state) {
  assert(states.find(state) != states.end());

  startState = state;
}

void Lex::Dfa::setTrapState(int state) {
  assert(states.find(state) != states.end());

  trapState = state;
}

void Lex::Dfa::initialiseStates(int numberOfStates) {
  assert(states.empty());

  for (int count = 0; count < numberOfStates; ++count) {
    states.insert(count);
  }
}

void Lex::Dfa::addTransition(int previousState, char transitionSymbol,
                             int nextState) {
  assert(states.find(previousState) != states.end());
  assert(states.find(nextState) != states.end());
  assert(Lex::alphabets.find(transitionSymbol) != Lex::alphabets.end());
  assert(transitions.find(previousState) == transitions.end() ||
         transitions.find(previousState)->second.find(transitionSymbol) ==
             transitions.find(previousState)->second.end());

  transitions[previousState][transitionSymbol] = nextState;
}

void Lex::Dfa::addAcceptingState(
    int state, Lex::AcceptingStateInfo newAcceptingStateInfo) {
  assert(states.find(state) != states.end());
  assert(acceptingStates.find(state) == acceptingStates.end());

  acceptingStates[state] = newAcceptingStateInfo;
}

void Lex::Dfa::printInfo() {
  std::cout << "Printing DFA info!!!!" << std::endl;

  std::cout << "Dfa start state: " << startState << std::endl;
  std::cout << "Dfa trap state: " << trapState << std::endl;

  std::cout << "Dfa states" << std::endl;
  for (auto const &state : states) {
    std::cout << state << " ";
  }
  std::cout << std::endl;

  std::cout << "Dfa Accepting states" << std::endl;
  for (auto const &acceptingState : acceptingStates) {
    std::cout << "State: " << acceptingState.first
              << " Token: " << acceptingState.second.tokenKind
              << " Priority: " << acceptingState.second.tokenPriority
              << std::endl;
  }

  std::cout << "Dfa transitions" << std::endl;
  for (auto const &transition : transitions) {
    for (auto const &transitionInfo : transition.second) {
      std::cout << "Previous State: " << transition.first
                << " Transition character: " << transitionInfo.first
                << " Next State: " << transitionInfo.second << std::endl;
    }
  }
}

int Lex::Dfa::getNextState(int currentState, char transitionSymbol) {
  assert(states.find(currentState) != states.end());
  assert(Lex::alphabets.find(transitionSymbol) != Lex::alphabets.end());

  if (transitions.find(currentState) == transitions.end() ||
      transitions.find(currentState)->second.find(transitionSymbol) ==
          transitions.find(currentState)->second.end()) {
    // assert(("Dfa has no transition for character", false));
  }

  return transitions[currentState][transitionSymbol];
}

Lex::AcceptingStateInfo Lex::Dfa::getAcceptingState(int currentState) {
  if (acceptingStates.find(currentState) != acceptingStates.end()) {
    return acceptingStates[currentState];
  } else {
    return Lex::AcceptingStateInfo();
  }
}

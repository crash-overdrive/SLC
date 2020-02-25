#include <cassert>
#include <iostream>
#include <set>
#include <unordered_map>

#include "LexNfa.hpp"

size_t Lex::MyHashFunctions::operator()(const std::set<int>& states) const {
  size_t hashValue = 0;
  for (auto const& state : states) {
    hashValue += intHash(state);
  }
  return hashValue;
}


void Lex::Nfa::setStartState(int state) {
  assert(states.find(state) != states.end());

  startState = state;
}


int Lex::Nfa::getStartState() {
	return startState;
}


int Lex::Nfa::getNumberOfStates() {
	return states.size();
}


void Lex::Nfa::initialiseStates(int numberOfStates) {
	assert(states.empty());

  for (int count = 0; count < numberOfStates; ++count) {
    states.insert(count);
  }
}


void Lex::Nfa::shiftStates(int shiftValue) {
  std::set<int> newStates;
  std::unordered_map<int, std::unordered_map<char, std::set<int>>> newTransitions;
  std::unordered_map<int, AcceptingStateInfo> newAcceptingStates;


  while (!states.empty()) {
    auto stateIterator = states.begin();

    newStates.insert(*stateIterator + shiftValue);
    states.erase(stateIterator);
  }

  for (auto const& transition : transitions) {
    int previousState = transition.first;
    for (auto const& transitionInfo : transition.second) {
      char symbol = transitionInfo.first;
      for (auto const& nextState : transitionInfo.second) {
        newTransitions[previousState + shiftValue][symbol].insert(nextState + shiftValue);
      }
    }
  }

  for (auto const& acceptingState : acceptingStates) {
    newAcceptingStates[acceptingState.first + shiftValue] = acceptingState.second;
  }

  states = newStates;
  transitions = newTransitions;
  acceptingStates = newAcceptingStates;
  startState += shiftValue;
}


void Lex::Nfa::addTransition(int previousState, char transitionSymbol, int nextState) {
  assert(states.find(previousState) != states.end());
  assert(states.find(nextState) != states.end());
  if (transitionSymbol != Lex::EPSILON) {
    assert(Lex::alphabets.find(transitionSymbol) != Lex::alphabets.end());
  }
  assert(transitions.find(previousState) == transitions.end() || transitions.find(previousState)->second.find(transitionSymbol) == transitions.find(previousState)->second.end()
  || transitions.find(previousState)->second.find(transitionSymbol)->second.find(nextState) == transitions.find(previousState)->second.find(transitionSymbol)->second.end());

  transitions[previousState][transitionSymbol].insert(nextState);
}


void Lex::Nfa::addAcceptingState(int state, Lex::AcceptingStateInfo newAcceptingStateInfo) {
  assert(states.find(state) != states.end());
  assert(acceptingStates.find(state) == acceptingStates.end());

  acceptingStates[state] = newAcceptingStateInfo;
}


void Lex::Nfa::printInfo() {
  std::cout << "Printing NFA info!!!!" << std::endl;

  std::cout << "Nfa start state: " <<  startState << std::endl;

  std::cout << "Nfa states" << std::endl;
  for (auto const& state: states) {
    std::cout << state << " ";
  }
  std::cout << std::endl;

  std::cout << "Nfa Accepting states" << std::endl;
  for (auto const& acceptingState: acceptingStates) {
    std::cout << "State: " << acceptingState.first << " Token: " <<
    acceptingState.second.tokenKind << " Priority: " <<
    acceptingState.second.tokenPriority << std::endl;
  }

  std::cout << "Nfa transitions" << std::endl;
  for (auto const& transition: transitions) {
    std::cout << "Previous State: " << transition.first;
    for (auto const& transitionInfo : transition.second) {
      std::cout << " Transition symbol: " << transitionInfo.first << std::endl;
      std::cout << "New States - " << std::endl;
      for (auto const& nextState : transitionInfo.second) {
        std::cout << nextState << " ";
      }
      std::cout << std::endl;
    }
  }
}


std::set<int> Lex::Nfa::epsilonClosure(std::set<int> givenStates) {
  for (auto const &state : givenStates) {
    assert(states.find(state) != states.end());
    (void)state;
  }

  std::set<int> epsilonClosureList = givenStates;
  std::set<int> workList = givenStates;

  while (!workList.empty()) {
    auto currentState = workList.begin();

    if (transitions.find(*currentState) != transitions.end() && transitions.find(*currentState)->second.find(Lex::EPSILON) != transitions.find(*currentState)->second.end()) {
      std::set<int> newStates = transitions[*currentState][Lex::EPSILON];

      for (auto const& newState : newStates) {
        if (epsilonClosureList.find(newState) == epsilonClosureList.end()) {
          epsilonClosureList.insert(newState);
          workList.insert(newState);
        }
      }
    }
    workList.erase(currentState);
  }

  return epsilonClosureList;
}


Lex::Nfa Lex::createSimpleNfa(char transitionSymbol, std::string tokenKind, int tokenPriority) {
  assert(Lex::alphabets.find(transitionSymbol) != Lex::alphabets.end());

  Nfa finalNfa;

  finalNfa.initialiseStates(2);
  finalNfa.addTransition(0, transitionSymbol, 1);
  finalNfa.addAcceptingState(1, Lex::AcceptingStateInfo(tokenKind, tokenPriority));
  finalNfa.setStartState(0);

  return finalNfa;
}


Lex::Nfa Lex::concatenateOperator(Nfa& firstNfa, Nfa& secondNfa) {
  Nfa finalNfa;

  // shift second Nfa states so that states dont clash
  secondNfa.shiftStates(firstNfa.getNumberOfStates());
  // initialise finalNfa states
  finalNfa.initialiseStates(firstNfa.getNumberOfStates() + secondNfa.getNumberOfStates());
  // add first Nfa transitions
  for (auto const& transition : firstNfa.transitions) {
    int previousState = transition.first;
    for (auto const& transitionInfo : transition.second) {
      char transitionSymbol = transitionInfo.first;
      for (auto const& nextState : transitionInfo.second) {
        finalNfa.addTransition(previousState, transitionSymbol, nextState);
      }
    }
  }
  // add second Nfa transitions
  for (auto const &transition : secondNfa.transitions) {
    int previousState = transition.first;
    for (auto const& transitionInfo : transition.second) {
      char transitionSymbol = transitionInfo.first;
      for (auto const& nextState : transitionInfo.second) {
        finalNfa.addTransition(previousState, transitionSymbol, nextState);
      }
    }
  }
  // add epsilon transitions from first Nfa accepting state to second Nfa start state
  for (auto const &firstNfaAcceptingState : firstNfa.acceptingStates) {
    finalNfa.addTransition(firstNfaAcceptingState.first, EPSILON, secondNfa.getStartState());
  }
  // set final Nfa accepting state as second Nfa accepting state
  for (auto const &secondNfaAcceptingState : secondNfa.acceptingStates) {
  	finalNfa.addAcceptingState(secondNfaAcceptingState.first, secondNfaAcceptingState.second);
  }
  // set final Nfa start state as first Nfa's start state
  finalNfa.setStartState(firstNfa.getStartState());

  return finalNfa;
}


Lex::Nfa Lex::orOperator(Nfa& firstNfa, Nfa& secondNfa) {
  Nfa finalNfa;

  // Auxillary state introduced: stateSize1 + stateSize2
  int auxillaryState = firstNfa.getNumberOfStates() + secondNfa.getNumberOfStates();
  // shiftsecond Nfa states so that states dont clash
  secondNfa.shiftStates(firstNfa.getNumberOfStates());
  // initialise finalNfa states
  finalNfa.initialiseStates(firstNfa.getNumberOfStates() + secondNfa.getNumberOfStates() + 1);
  // add first Nfa transitions
  for (auto const& transition : firstNfa.transitions) {
    int previousState = transition.first;
    for (auto const& transitionInfo : transition.second) {
      char transitionSymbol = transitionInfo.first;
      for (auto const& nextState : transitionInfo.second) {
        finalNfa.addTransition(previousState, transitionSymbol, nextState);
      }
    }
  }
  // add second Nfa transitions
  for (auto const &transition : secondNfa.transitions) {
    int previousState = transition.first;
    for (auto const& transitionInfo : transition.second) {
      char transitionSymbol = transitionInfo.first;
      for (auto const& nextState : transitionInfo.second) {
        finalNfa.addTransition(previousState, transitionSymbol, nextState);
      }
    }
  }
  // add epsilon transition from auxillary state : stateSize1 + stateSize2 to starting state of first and second Nfa
  finalNfa.addTransition(auxillaryState, EPSILON, firstNfa.getStartState());
  finalNfa.addTransition(auxillaryState, EPSILON, secondNfa.getStartState());
  // set accepting state of final Nfa to accepting state of first and second Nfa
  for (const auto& firstNfaAcceptingState : firstNfa.acceptingStates) {
    finalNfa.addAcceptingState(firstNfaAcceptingState.first, firstNfaAcceptingState.second);
  }
  for (const auto& secondNfaAcceptingState : secondNfa.acceptingStates) {
    finalNfa.addAcceptingState(secondNfaAcceptingState.first, secondNfaAcceptingState.second);
  }
  // set start state of final Nfa to auxillary state
  finalNfa.setStartState(auxillaryState);

  return finalNfa;
}


Lex::Nfa Lex::starOperator(Nfa& nfa) {
  Nfa finalNfa;

  // Auxillary state introduced: stateSize
  int auxillaryState = nfa.getNumberOfStates();
  // initialise final Nfa states
  finalNfa.initialiseStates(nfa.getNumberOfStates() + 1);
  // copy given Nfa transitions into the final Nfa
  for (auto const &transition : nfa.transitions) {
    int previousState = transition.first;
    for (auto const& transitionInfo : transition.second) {
      char transitionSymbol = transitionInfo.first;
      for (auto const& nextState : transitionInfo.second) {
        finalNfa.addTransition(previousState, transitionSymbol, nextState);
      }
    }
  }
  // add epsilon transition from new auxillary state to start state of given Nfa
  finalNfa.addTransition(auxillaryState, EPSILON, nfa.getStartState());
  // add epsilon transition from accepting states of given Nfa to new auxillary state
  for (auto const &acceptingState : nfa.acceptingStates) {
    finalNfa.addTransition(acceptingState.first, EPSILON, auxillaryState);
  }
  // set accepting states of final Nfa to new auxillary state
  finalNfa.addAcceptingState(auxillaryState, nfa.acceptingStates.begin()->second);
  // set start state of Final Nfa to new auxillary state
  finalNfa.setStartState(auxillaryState);

  return finalNfa;
}


Lex::Nfa Lex::rangeOperator(Nfa& firstNfa, Nfa& secondNfa)  {
  assert(firstNfa.transitions.size() == 1 && secondNfa.transitions.size() == 1);
  assert(firstNfa.transitions.begin()->second.size() == 1 && secondNfa.transitions.begin()->second.size() == 1);
  assert(firstNfa.transitions.begin()->second.begin()->second.size() == secondNfa.transitions.begin()->second.begin()->second.size());
  assert(firstNfa.transitions.begin()->second.begin()->first < secondNfa.transitions.begin()->second.begin()->first);

  Nfa finalNfa;
  char startToken = firstNfa.transitions.begin()->second.begin()->first;
  char endToken = secondNfa.transitions.begin()->second.begin()->first;

  // initialise final Nfa states
  finalNfa.initialiseStates(2);
  // add transitions from state 0 to state 1 from all tokens in the range of startToken to endToken
  for (char token = startToken; token <= endToken; ++token) {
    finalNfa.addTransition(0, token, 1);
  }
  // add state 1 as an accepting state
  finalNfa.addAcceptingState(1, firstNfa.acceptingStates.begin()->second);
  // set state 0 as the starting state
  finalNfa.setStartState(0);

  return finalNfa;
}


Lex::Nfa Lex::notOperator(Nfa& nfa) {
	assert(!Lex::alphabets.empty());

  std::set<char> bannedTokens;
  Nfa finalNfa;

  for (auto const& transition : nfa.transitions) {
    for (auto const& transitionInfo : transition.second) {
      bannedTokens.insert(transitionInfo.first);
    }
  }

  // initialise final Nfa states
  finalNfa.initialiseStates(2);
   // add transitions from state 0 to state 1 for all tokens in alphabets except the banned tokens
  for (auto& token : Lex::alphabets) {
    if (bannedTokens.find(token) == bannedTokens.end()) {
      finalNfa.addTransition(0, token, 1);
    }
  }
  // add state 1 as an accepting state
  finalNfa.addAcceptingState(1, nfa.acceptingStates.begin()->second);
  // set state 0 as the starting state
  finalNfa.setStartState(0);

  return finalNfa;
}

Lex::Nfa Lex::questionOperator(Nfa& nfa) {
  Nfa finalNfa;

  // Auxillary state introduced: stateSize
  int auxillaryState = nfa.getNumberOfStates();
  // initialise final Nfa states
  finalNfa.initialiseStates(nfa.getNumberOfStates() + 1);
  // copy given Nfa transitions into the final Nfa
  for (auto const &transition : nfa.transitions) {
    int previousState = transition.first;
    for (auto const& transitionInfo : transition.second) {
      char transitionSymbol = transitionInfo.first;
      for (auto const& nextState : transitionInfo.second) {
        finalNfa.addTransition(previousState, transitionSymbol, nextState);
      }
    }
  }
  // add epsilon transition from new auxillary state to start state of given Nfa
  finalNfa.addTransition(auxillaryState, EPSILON, nfa.getStartState());
  // add accepting state of given nfa to finalNfa
  for (auto const& acceptingState : nfa.acceptingStates) {
    finalNfa.addAcceptingState(acceptingState.first, acceptingState.second);
  }
  // add new auxillary state as accepting states of final Nfa
  finalNfa.addAcceptingState(auxillaryState, nfa.acceptingStates.begin()->second);
  // set start state of Final Nfa to new auxillary state
  finalNfa.setStartState(auxillaryState);

  return finalNfa;
}


Lex::Dfa Lex::convertToDfa(Nfa& nfa) {
  // computed Nfa to Dfa Multi states
  std::set<std::set<int>> computedDfaMultiStates = {Lex::trapState};
  // workList of multistates to work through
  std::set<std::set<int>> workList;
  // dfa multi state to single state mapping
  std::unordered_map<std::set<int>, int, Lex::MyHashFunctions> multiStateMappings;
  // final Dfa transitions
  std::unordered_map<int, std::unordered_map<char, int>> dfaTransitions;
  // final Dfa accepting states
  std::unordered_map<int, AcceptingStateInfo> dfaAcceptingStates;
  // final Dfa start state
  int dfaStartState;
  // final Dfa trap state
  int dfaTrapState;

  std::set<int> startStateClosure = nfa.epsilonClosure({nfa.startState});
  int newSingleState = 0;

  computedDfaMultiStates.insert(startStateClosure);
  workList.insert(startStateClosure);

  multiStateMappings[trapState] = newSingleState++;
  multiStateMappings[startStateClosure] = newSingleState++;

  for (auto const& alphabet : Lex::alphabets) {
    dfaTransitions[multiStateMappings[trapState]][alphabet] = multiStateMappings[trapState];
  }

  while (!workList.empty()) {
    auto currentStates = workList.begin();

    for (auto const &alphabet : Lex::alphabets) {
      std::set<int> statesReachable;

      for (auto const &state : *currentStates) {
        if(nfa.transitions.find(state) != nfa.transitions.end() &&
        nfa.transitions.find(state)->second.find(alphabet) != nfa.transitions.find(state)->second.end()) {
          for (auto const& nextState : nfa.transitions.find(state)->second.find(alphabet)->second) {
            statesReachable.insert(nextState);
          }
        }
      }

      statesReachable = nfa.epsilonClosure(statesReachable);

      if (!statesReachable.empty()) {
        // if statesReachable is not empty and doesnt have a single state mapping, give it a single state mapping
        if (multiStateMappings.find(statesReachable) == multiStateMappings.end()) {
          multiStateMappings[statesReachable] = newSingleState++;
        }
        assert(multiStateMappings.find(*currentStates) != multiStateMappings.end());
        assert(multiStateMappings.find(statesReachable) != multiStateMappings.end());
        assert(dfaTransitions.find(multiStateMappings[*currentStates]) == dfaTransitions.end() ||
        dfaTransitions.find(multiStateMappings[*currentStates])->second.find(alphabet) == dfaTransitions.find(multiStateMappings[*currentStates])->second.end());

        dfaTransitions[multiStateMappings[*currentStates]][alphabet] = multiStateMappings[statesReachable];
        // TODO: check for correctness of this statement
        if (computedDfaMultiStates.find(statesReachable) == computedDfaMultiStates.end() && workList.find(statesReachable) == workList.end()) {
          computedDfaMultiStates.insert(statesReachable);
          workList.insert(statesReachable);
        }
      } else {
        assert(multiStateMappings.find(*currentStates) != multiStateMappings.end());
        assert(dfaTransitions.find(multiStateMappings[*currentStates]) == dfaTransitions.end() ||
        dfaTransitions.find(multiStateMappings[*currentStates])->second.find(alphabet) == dfaTransitions.find(multiStateMappings[*currentStates])->second.end());

        dfaTransitions[multiStateMappings[*currentStates]][alphabet] = multiStateMappings[trapState];
      }
    }
    workList.erase(currentStates);
  }

  for (auto const& multiStateMapping : multiStateMappings) {
    std::set<int> multiStates = multiStateMapping.first;
    int singleState = multiStateMapping.second;
    bool isAccepting = false;
    std::string tokenKind;
    int tokenPriority;

    for (auto const& state : multiStates) {
      if (nfa.acceptingStates.find(state) != nfa.acceptingStates.end()) {
        if (!isAccepting) {
          isAccepting = true;
          tokenKind = nfa.acceptingStates[state].tokenKind;
          tokenPriority = nfa.acceptingStates[state].tokenPriority;
        } else if(nfa.acceptingStates[state].tokenPriority < tokenPriority) {
          tokenKind = nfa.acceptingStates[state].tokenKind;
          tokenPriority = nfa.acceptingStates[state].tokenPriority;
        }
      }
    }
    if (isAccepting) {
      assert(dfaAcceptingStates.find(singleState) == dfaAcceptingStates.end());

      dfaAcceptingStates[singleState] = AcceptingStateInfo(tokenKind, tokenPriority);
    }
  }

  assert(multiStateMappings.find(startStateClosure) != multiStateMappings.end());
  assert(multiStateMappings.find(trapState) != multiStateMappings.end());

  dfaStartState = multiStateMappings[startStateClosure];
  dfaTrapState = multiStateMappings[trapState];

  Lex::Dfa dfa;

  dfa.initialiseStates(multiStateMappings.size());

  for (auto const &transition : dfaTransitions) {
    int previousState = transition.first;
    for (auto const& transitionInfo : transition.second) {
      char transitionSymbol = transitionInfo.first;
      int nextState = transitionInfo.second;
      dfa.addTransition(previousState, transitionSymbol, nextState);
    }
  }

  for (auto& acceptingState : dfaAcceptingStates) {
    dfa.addAcceptingState(acceptingState.first, acceptingState.second);
  }

  dfa.setStartState(dfaStartState);
  dfa.setTrapState(dfaTrapState);

  return dfa;
}

#include "Scanner.hpp"
#include <algorithm>
#include <fstream>
#include <set>
#include <vector>

namespace Lex {
  constexpr char EPSILON = -1;

  int Nfa::getNumberOfStates() {
    return states.size();
  }

  void Nfa::initialiseStates(int numberOfStates) {
    for (int count = 1; count <= numberOfStates; ++count) {
      states.push_back(count);
    }
  }

  void Nfa::setAlphabets(std::vector<char> givenAlphabets) {
    alphabets = givenAlphabets;
  }

  void Nfa::addTransition(int previousState, int nextState, char transitionSymbol) {
    transitions.push_back(NfaTransition(previousState, nextState, transitionSymbol));
  }

  void Nfa::setAcceptingStates(std::vector<int> newAcceptingStates) {
    acceptingStates = newAcceptingStates;
  }

  std::vector<int> Nfa::getAcceptingStates() {
    return acceptingStates;
  }

  void Nfa::setStartState(int state) {
    startState = state;
  }

  int Nfa::getStartState() {
    return startState;
  }

  void Nfa::shiftStates(int shiftValue) {
    for (size_t i = 0; i < states.size(); ++i) {
      states[i] += shiftValue;
    }

      for (size_t i = 0; i < transitions.size(); ++i) {
        transitions[i].previousState += shiftValue;
        transitions[i].nextState += shiftValue;
      }

      for(size_t i = 0; i < acceptingStates.size(); ++i) {
        acceptingStates[i] += shiftValue;
      }

      startState += shiftValue;
    }

  std::vector<int> Nfa::epsilonClosure(std::vector<int> givenStates) {
    for (auto const& state: givenStates) {
      if (std::find(states.begin(), states.end(), state) == states.end()) {
        std::cout << "Error computing Epsilon closure for state: " << state << " .. State doesnt exist in Nfa" << std::endl;
        return {};
      }
    }

    std::vector<int> workList = givenStates;
    std::vector<int> epsilonClosureList = givenStates;

    while (!workList.empty()) {
      int currentState = workList.back();

      workList.pop_back();

      for(auto const& transition: transitions) {
        if (transition.transitionSymbol == EPSILON && transition.previousState == currentState) {
          int newState = transition.nextState;
          if (std::find(epsilonClosureList.begin(), epsilonClosureList.end(), newState) == epsilonClosureList.end()) {
            epsilonClosureList.push_back(newState);
            workList.push_back(newState);
          }
        }
      }
    }

    std::sort(epsilonClosureList.begin(), epsilonClosureList.end());
    return epsilonClosureList;
  }

  std::vector<int> Nfa::getDfaStartStates() {
    return epsilonClosure({startState});
  }

    void Nfa::computeDfaStatesAndTransitions(std::vector<std::vector<int>>& dfaStates, std::vector<DfaTransition>& dfaTransitions) {
      dfaStates = {getDfaStartStates()};
      std::vector<std::vector<int>> workList = {getDfaStartStates()};

      while (!workList.empty()) {
        std::vector<int> currentStates = workList.back();

        workList.pop_back();

        // std::cout << "Popped from Worklist: ";
        // for (auto const& num: currentStates) {
        //   std::cout << num << " ";
        // }
        // std::cout << std::endl;

        for (auto const& alphabet: alphabets) {
          std::set<int> statesReachable;

          for (auto const& state: currentStates) {
            for (auto const& transition: transitions) {
              if (transition.previousState == state && transition.transitionSymbol == alphabet) {
                // std::cout << "State: " << state << " Alphabet: " << int{alphabet} << " State added: " << transition.nextState << std::endl;
                statesReachable.insert(transition.nextState);
              }
            }
          }
          std::vector<int> states(statesReachable.begin(), statesReachable.end());

          states = epsilonClosure(states);
          dfaTransitions.push_back(DfaTransition(currentStates, states, alphabet));

          // std::cout << "Transition: " << int {alphabet} << std::endl << "Result: ";
          // for (auto const& state: states) {
          //   std::cout << state << " ";
          // }
          // std::cout << std::endl;

          if (!states.empty() && (std::find(workList.begin(), workList.end(), states) == workList.end())
          && (std::find(dfaStates.begin(), dfaStates.end(), states) == dfaStates.end())) {
            dfaStates.push_back(states);
            workList.push_back(states);
          }
        }
      }
    }

    Dfa Nfa::convertToDfa() {
      Dfa convertedDfa;
      convertedDfa.alphabets = alphabets;
      convertedDfa.startStates = getDfaStartStates();
      computeDfaStatesAndTransitions(convertedDfa.states, convertedDfa.transitions);
      // convertedDfa.acceptingStates = getDfaAcceptingTransitions();
      return convertedDfa;
    }

  Nfa concatenate(Nfa firstNfa, Nfa secondNfa) {
    Nfa finalNfa;

    // shift second Nfa states so that states dont clash
    secondNfa.shiftStates(firstNfa.getNumberOfStates());

    // initialise finalNfa states
    finalNfa.initialiseStates(firstNfa.getNumberOfStates() + secondNfa.getNumberOfStates());

    // add first Nfa transitions
    for(auto const& firstNfaTransition: firstNfa.transitions) {
      finalNfa.addTransition(firstNfaTransition.previousState, firstNfaTransition.nextState, firstNfaTransition.transitionSymbol);
    }

    // add second Nfa transitions
    for(auto const& secondNfaTransition: secondNfa.transitions) {
      finalNfa.addTransition(secondNfaTransition.previousState, secondNfaTransition.nextState, secondNfaTransition.transitionSymbol);
    }

    // add epsilon transitions from first Nfa accepting state to second Nfa start state
    for(auto const& firstNfaAcceptingState: firstNfa.acceptingStates) {
      finalNfa.addTransition(firstNfaAcceptingState, secondNfa.getStartState(), EPSILON);
    }

    // set final Nfa accepting state as second Nfa accepting state
    finalNfa.setAcceptingStates(secondNfa.getAcceptingStates());

    // set final Nfa start state as first Nfa's start state
    finalNfa.setStartState(firstNfa.getStartState());
    return finalNfa;
  }

  Nfa orSelection(Nfa firstNfa, Nfa secondNfa) {
    Nfa finalNfa;

    // Auxillary state introduced: stateSize1 + stateSize2 + 1
    int auxillaryState = firstNfa.getNumberOfStates() + secondNfa.getNumberOfStates() + 1;

    // shiftsecond Nfa states so that states dont clash
    secondNfa.shiftStates(firstNfa.getNumberOfStates());

    // initialise finalNfa states
    finalNfa.initialiseStates(firstNfa.getNumberOfStates() + secondNfa.getNumberOfStates() + 1);

    // add first Nfa transitions
    for(auto const& firstNfaTransition: firstNfa.transitions) {
      finalNfa.addTransition(firstNfaTransition.previousState, firstNfaTransition.nextState, firstNfaTransition.transitionSymbol);
    }

    // add second Nfa transitions
    for(auto const& secondNfaTransition: secondNfa.transitions) {
      finalNfa.addTransition(secondNfaTransition.previousState, secondNfaTransition.nextState, secondNfaTransition.transitionSymbol);
    }

    // add epsilon transition from auxillary state : stateSize1 + stateSize2 + 1 to starting state of first and second Nfa
    finalNfa.addTransition(auxillaryState, firstNfa.getStartState(), EPSILON);
    finalNfa.addTransition(auxillaryState, secondNfa.getStartState(), EPSILON);

    // set accepting state of final Nfa to accepting state of first and second Nfa
    std::vector<int> acceptingStates(firstNfa.getAcceptingStates());
    acceptingStates.insert(acceptingStates.end(), secondNfa.getAcceptingStates().begin(), secondNfa.getAcceptingStates().end());
    finalNfa.setAcceptingStates(acceptingStates);

    // set start state of final Nfa to auxillary state
    finalNfa.setStartState(auxillaryState);

    return finalNfa;
  }

  Nfa kleeneStar(Nfa nfa) {
    Nfa finalNfa;

    // Auxillary state introduced: stateSize+1
    int auxillaryState = nfa.getNumberOfStates() + 1;

    // initialise final Nfa states
    finalNfa.initialiseStates(nfa.getNumberOfStates() + 1);

    // copy given Nfa transitions into the final Nfa
    for(auto const& nfaTransition: nfa.transitions) {
      finalNfa.addTransition(nfaTransition.previousState, nfaTransition.nextState, nfaTransition.transitionSymbol);
    }

    // add epsilon transition from new auxillary state to start state of given Nfa
    finalNfa.addTransition(auxillaryState, nfa.startState, EPSILON);

    // add epsilon transition from accepting state of given Nfa to new auxillary state
    for(auto const& nfaAcceptingState: nfa.acceptingStates) {
      finalNfa.addTransition(nfaAcceptingState, auxillaryState, EPSILON);
    }

    // set accepting states of final Nfa to new auxillary state
    finalNfa.setAcceptingStates({auxillaryState});

    // set start state of Final Nfa to new auxillary state
    finalNfa.setStartState(auxillaryState);

    return finalNfa;
  }

  Nfa plus(Nfa nfa) {
    Nfa finalNfa;

    // Auxillary state introduced: stateSize+1
    int auxillaryState = nfa.getNumberOfStates() + 1;

    // initialise final Nfa states
    finalNfa.initialiseStates(nfa.getNumberOfStates() + 1);

    // copy given Nfa transitions into the final Nfa
    for(auto const& nfaTransition: nfa.transitions) {
      finalNfa.addTransition(nfaTransition.previousState, nfaTransition.nextState, nfaTransition.transitionSymbol);
    }

    // add epsilon transition from new auxiallry state to start state of given Nfa
    finalNfa.addTransition(auxillaryState, nfa.startState, EPSILON);

    // add epsilon transition from accepting state of given Nfa to new auxillary state
    for(auto const& nfaAcceptingState: nfa.acceptingStates) {
      finalNfa.addTransition(nfaAcceptingState, auxillaryState, EPSILON);
    }

    // set accepting states of final Nfa to new auxillary state
    finalNfa.setAcceptingStates({auxillaryState});

    // set start state of Final Nfa to start state of given Nfa
    finalNfa.setStartState(nfa.getStartState());

    return finalNfa;
  }

} // namespace Lex

  int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    //if (argc != 2) {
      //std::cout << "Invalid number of arguments passed, required: 2, got: " << argc << std::endl;
      //return 42;
    //}

    //std::ifstream file;
    //file.open(argv[1]);

    //std::vector<Lex::Definition> definitions;
    //std::string delimiter = " -=- ";

    //if (file.is_open()) {
        //while (!file.eof()) {
          //std::string line;

          //std::getline(file, line);
          //// std::cout << "Read: " << line << std::endl;
          //int position = line.find(delimiter);
          //definitions.push_back(Lex::Definition(line.substr(0, position), line.substr(position + delimiter.length())));
        //}
        //file.close();
    //} else {
      //std::cout << "File: " << argv[1] << " not found.. quitting" << std::endl;
      //return 42;
    //}

    //std::cout << "Definitions generated: " << std::endl;
    //for(auto const& definition: definitions) {
    //std::cout << "[" << definition.type << "] {" << definition.regex << "}" << std::endl;
    //}

    Lex::Nfa nfa;
    nfa.initialiseStates(4);

    nfa.setAlphabets({0, 1});

    nfa.addTransition(1, 2, 0);
    nfa.addTransition(1, 3, Lex::EPSILON);
    nfa.addTransition(2, 2, 1);
    nfa.addTransition(2, 4, 1);
    nfa.addTransition(3, 2, Lex::EPSILON);
    nfa.addTransition(3, 4, 0);
    nfa.addTransition(4, 3, 0);

    nfa.setStartState(1);

    nfa.setAcceptingStates({3,4});

    std::cout << "Epsilon clsoure is for {1} is: ";
    std::vector<int> test1 = nfa.epsilonClosure({1});
    for (auto const& num: test1) {
      std::cout << num << " ";
    }
    std::cout << std::endl;

    std::cout << "Epsilon clsoure for {4} is: ";
    std::vector<int> test2 = nfa.epsilonClosure({4});
    for (auto const& num: test2) {
      std::cout << num << " ";
    }
    std::cout << std::endl;

    Lex::Dfa dfa;
    nfa.computeDfaStatesAndTransitions(dfa.states, dfa.transitions);

    std::cout << "Dfa states" << std::endl;
    for (auto const& dfaState: dfa.states) {
      for (auto const& state: dfaState) {
        std::cout << state << " ";
      }
      std::cout << std::endl;
    }

    std::cout << "Dfa transitions" << std::endl;
    for (auto const& dfaTransition: dfa.transitions) {
      std::cout << "Transition character: " << int {dfaTransition.transitionSymbol} << std::endl;
      std::cout << "Previous State: ";
      for (auto const& state: dfaTransition.previousStates) {
        std::cout << state << " ";
      }
      std::cout << std::endl;
      std::cout << "Next State: ";
      for (auto const& state: dfaTransition.nextStates) {
        std::cout << state << " ";
      }
      std::cout << std::endl;
    }
    return 0;
  }

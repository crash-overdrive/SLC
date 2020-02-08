#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

constexpr int EPSILON = -1;

namespace Lex {

class NfaTransition {
  public:
  int previousState;
  int nextState;
  char transitionSymbol;

  NfaTransition(int previousState, int nextState, char transitionSymbol):
  previousState(previousState), nextState(nextState), transitionSymbol(transitionSymbol) {};
};

class Nfa {
  public:
  std::vector<int> states;
  // std::vector<char> alphabets;
  std::vector<NfaTransition> transitions;
  int startState;
  std::vector<int> acceptingStates;

  int getNumberOfStates() {
    return states.size();
  }

  void initialiseStates(int numberOfStates) {
    for (int count = 1; count <= numberOfStates; ++count) {
      states.push_back(count);
    }
  }

  void addTransition(int previousState, int nextState, char transitionSymbol) {
    transitions.push_back(NfaTransition(previousState, nextState, transitionSymbol));
  }

  void setAcceptingStates(std::vector<int> newAcceptingStates) {
    acceptingStates = newAcceptingStates;
  }

  std::vector<int> getAcceptingStates() {
    return acceptingStates;
  }

  void setStartState(int state) {
    startState = state;
  }

  int getStartState() {
    return startState;
  }

  void shiftStates(int shiftValue) {
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

  std::vector<int> epsilonClosure(std::vector<int> givenStates) {
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
};

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

class DfaTransition {
  public:
  std::vector<int> previousState;
  std::vector<int> nextState;
  char transitionSymbol;

  DfaTransition(std::vector<int> previousState, std::vector<int> nextState, char transitionSymbol):
  previousState(previousState), nextState(nextState), transitionSymbol(transitionSymbol) {};
};

class DFA {
  public:
  std::vector<std::vector<int>> states;
  // std::vector<char> alphabets;
  std::vector<DfaTransition> transitions;
  int startState;
  std::vector<int> acceptingStates;
};


class Definition {
  public:
  std::string type;
  std::string regex;
  Definition(std::string type, std::string regex): type(type), regex(regex) {};
};

} // Lex

//int main(int argc, char *argv[]) {
  //if (argc != 2) {
    //std::cout << "Invalid number of arguments passed, required: 2, got: " << argc << std::endl;
    //return 42;
  //}

  //std::ifstream file;
  //file.open(argv[1]);

  //std::vector<Definition> definitions;
  //std::string delimiter = " -=- ";

  //if (file.is_open()) {
      //while (!file.eof()) {
        //std::string line;

        //std::getline(file, line);
        //// std::cout << "Read: " << line << std::endl;
        //int position = line.find(delimiter);
        //definitions.push_back(Definition(line.substr(0, position), line.substr(position + delimiter.length())));
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

  //Nfa nfa;
  //nfa.initialiseStates(4);
  //nfa.addTransition(1, 2, 0);
  //nfa.addTransition(1, 3, EPSILON);
  //nfa.addTransition(2, 2, 1);
  //nfa.addTransition(2, 4, 1);
  //nfa.addTransition(3, 2, EPSILON);
  //nfa.addTransition(3, 4, 0);
  //nfa.addTransition(4, 3, 0);
  //nfa.setStartState(1);
  //nfa.setAcceptingStates({3,4});
  //std::vector<int> test1 = nfa.epsilonClosure({1});
  //for (auto const& num: test1) {
    //std::cout << num << " ";
  //}

  //std::cout << std::endl;

  //std::vector<int> test2 = nfa.epsilonClosure({2,3});
  //for (auto const& num: test2) {
    //std::cout << num << " ";
  //}
  //return 0;
//}

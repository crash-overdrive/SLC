#ifndef LEXDFA_HPP
#define LEXDFA_HPP

#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace Lex {

	extern std::set<int> trapState;
  extern std::set<char> alphabets;
  constexpr char EPSILON = -1;

  void setAlphabets();

	struct AcceptingStateInfo {
    std::string tokenKind;
    int tokenPriority;

    AcceptingStateInfo();
    AcceptingStateInfo(std::string tokenKind, int tokenPriority);
    // bool operator==(const AcceptingStateInfo& acceptingState) const;
  };

	struct Dfa {
    int startState;
    int trapState;
    std::unordered_set<int> states;
	  std::unordered_map<int, std::unordered_map<char, int>> transitions;
	  std::unordered_map<int, AcceptingStateInfo> acceptingStates;

    void setStartState(int state);
    void setTrapState(int state);
    void initialiseStates(int numberOfStates);
    void addTransition(int currentState, char transitionSymbol, int nextState);
    void addAcceptingState(int state, AcceptingStateInfo newAcceptingState);
    void printInfo();

    int getNextState(int currentState, char transitionSymbol);
    AcceptingStateInfo getAcceptingState(int currentState);
	};
}

#endif

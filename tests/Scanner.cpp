#include "Scanner.hpp"
#include "catch.hpp"

TEST_CASE("scanner extracts tokens properly", "[scanner-dfa]") {
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
  nfa.setAcceptingStates({3, 4});

  SECTION("epsilon closure test") {
    std::vector<int> test1{1, 2, 3};
    REQUIRE(test1 == nfa.epsilonClosure({1}));
    std::vector<int> test2{4};
    REQUIRE(test2 == nfa.epsilonClosure({4}));
  }
}

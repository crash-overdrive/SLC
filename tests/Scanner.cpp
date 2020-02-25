#include "Config.hpp"
#include "LexScanner.hpp"
#include "TestConfig.hpp"
#include "catch.hpp"
#include <fstream>
#include <sstream>

TEST_CASE("scanner detects Java", "[scanner-java]") {
  Lex::Scanner Scanner;
  std::ifstream ScannerStream;
  ScannerStream.open(TokensLexFile);
  ScannerStream >> Scanner;
  std::ifstream JavaStream;

  SECTION("scanner output") {
    JavaStream.open(TestDataDir + "/java/a1/J1_publicclasses.java");
    CHECK(Scanner.scan(JavaStream));
    std::vector<Lex::Token> Tokens = Scanner.getTokens();
    std::ostringstream ScanOstream;
    for (const auto &Token : Scanner.getTokens()) {
      ScanOstream << Token << '\n';
    }

    std::ifstream TokensStream;
    TokensStream.open(TestDataDir + "/tokens/a1/J1_publicclasses.tokens");
    std::string TokenStr(std::istreambuf_iterator<char>{TokensStream}, {});
    REQUIRE(ScanOstream.str() == TokenStr);
  }
}

// TEST_CASE("scanner extracts tokens properly", "[scanner-dfa]") {
//   Lex::Nfa nfa;
//   nfa.initialiseStates(4);
//   nfa.setAlphabets({0, 1});
//   nfa.addTransition(1, 2, 0);
//   nfa.addTransition(1, 3, Lex::EPSILON);
//   nfa.addTransition(2, 2, 1);
//   nfa.addTransition(2, 4, 1);
//   nfa.addTransition(3, 2, Lex::EPSILON);
//   nfa.addTransition(3, 4, 0);
//   nfa.addTransition(4, 3, 0);
//   nfa.setStartState(1);

//   SECTION("epsilon closure test") {
//     std::vector<int> test1{1, 2, 3};
//     REQUIRE(test1 == nfa.epsilonClosure({1}));
//     std::vector<int> test2{4};
//     REQUIRE(test2 == nfa.epsilonClosure({4}));
//   }
// }

// TEST_CASE("scanner converts nfa to dfa", "[nfa-dfa]") {
//   Lex::Nfa nfa;
//   nfa.initialiseStates(13);

//   nfa.addTransition(1, 2, Lex::EPSILON);
//   nfa.addTransition(1, 3, Lex::EPSILON);
//   nfa.addTransition(1, 4, Lex::EPSILON);
//   nfa.addTransition(3, 12, Lex::EPSILON);
//   nfa.addTransition(7, 8, Lex::EPSILON);
//   nfa.addTransition(8, 7, Lex::EPSILON);

//   nfa.addTransition(2, 9, 'c');
//   nfa.addTransition(3, 5, 'a');
//   nfa.addTransition(5, 9, 'c');
//   nfa.addTransition(3, 6, 'b');
//   nfa.addTransition(6, 10, 'c');
//   nfa.addTransition(4, 7, 'a');
//   nfa.addTransition(4, 8, 'b');
//   nfa.addTransition(7, 10, 'c');
//   nfa.addTransition(7, 11, 'a');
//   nfa.addTransition(8, 11, 'a');
//   nfa.addTransition(11, 12, 'd');
//   nfa.addTransition(9, 1, 'd');
//   nfa.addTransition(10, 1, 'e');

//   nfa.setStartState(1);
//   // Lex::Dfa dfa = nfa.convertToDfa();

//   SECTION("Transitions are valid in NFA terms") {
//     std::vector<int> prior{5, 7, 8};
//     std::vector<int> next{9, 10};
//     bool exist = false;
//     // for (unsigned int i = 0; i < dfa.transitions.size(); i++) {
//     //   if (dfa.transitions[i].previousStates == prior &&
//     //       dfa.transitions[i].nextStates == next &&
//     //       dfa.transitions[i].transitionSymbol == 'c') {
//     //     exist = true;
//     //     break;
//     //   }
//     // }
//     REQUIRE(exist);
//   }
// }

#include <cassert>
#include <deque>
#include <fstream>
#include <iostream>
#include <vector>

#include "LexScanner.hpp"

std::istream &Lex::operator>>(std::istream &stream, Lex::Scanner &scanner) {
  std::vector<Lex::Nfa> nfas;
  std::string delimiter = " @@@ ";
  int priority = 1;
  int shiftValue = 1;
  std::string line;

  Lex::setAlphabets();

  while (std::getline(stream, line)) {
    size_t position = line.find(delimiter);
    std::string tokenType = line.substr(0, position);
    std::string regexPattern = line.substr(position + delimiter.length());
    Lex::Nfa nfa = Lex::convertRegexToNfa(regexPattern, tokenType, ++priority);

    nfa.shiftStates(shiftValue);
    shiftValue += nfa.getNumberOfStates();
    nfas.push_back(nfa);
  }

  Lex::Nfa finalNfa;
  int numberOfStates = shiftValue;

  finalNfa.initialiseStates(numberOfStates);
  for (auto &nfa : nfas) {
    finalNfa.addTransition(0, Lex::EPSILON, nfa.getStartState());
    for (auto &transition : nfa.transitions) {
      int previousState = transition.first;
      for (auto const &transitionInfo : transition.second) {
        char transitionSymbol = transitionInfo.first;
        for (auto const &nextState : transitionInfo.second) {
          finalNfa.addTransition(previousState, transitionSymbol, nextState);
        }
      }
    }
    for (auto &acceptingState : nfa.acceptingStates) {
      finalNfa.addAcceptingState(acceptingState.first, acceptingState.second);
    }
  }
  finalNfa.setStartState(0);

  scanner.finalDfa = Lex::convertToDfa(finalNfa);

  // scanner.finalDfa.printInfo();
  return stream;
}

bool Lex::Scanner::maximalMunch(std::deque<char> input) {
  std::vector<char> processedTransitionSymbols;
  std::vector<Lex::Token> tokensGenerated;
  Lex::Token lastToken;
  int currentState = finalDfa.startState;
  int position = -1;

  matchedTokens.clear();
  matchedTokens = {Lex::Token("BOF", "BOF")};

  while (!input.empty()) {
    char ch = input.front();

    input.pop_front();
    ++position;
    processedTransitionSymbols.push_back(ch);
    currentState = finalDfa.getNextState(currentState, ch);
    Lex::AcceptingStateInfo acceptingStateInfo =
        finalDfa.getAcceptingState(currentState);

    if (acceptingStateInfo.tokenKind != "") {
      std::string newAcceptedLexeme(processedTransitionSymbols.begin(),
                                    processedTransitionSymbols.end());

      processedTransitionSymbols.clear();
      lastToken.lexeme += newAcceptedLexeme;
      lastToken.type = acceptingStateInfo.tokenKind;
    }

    if (currentState == finalDfa.trapState) {
      if (lastToken.type == "") {
        for (auto const &token : tokensGenerated) {
          std::cout << token << std::endl;
        }
        std::string garbage(processedTransitionSymbols.begin(),
                            processedTransitionSymbols.end());

        std::cout << "Scanning failed at position " << position
                  << " didnt expect symbol: " << ch << " parsed: " << garbage
                  << std::endl;
        return false;
      }

      if (stripTokens.find(lastToken.type) == stripTokens.end()) {
        matchedTokens.push_back(lastToken);
      }
      lastToken = Lex::Token();
      currentState = finalDfa.startState;
      while (!processedTransitionSymbols.empty()) {
        input.push_front(processedTransitionSymbols.back());
        --position;
        processedTransitionSymbols.pop_back();
      }
    }
  }
  if (!processedTransitionSymbols.empty()) {
    std::string garbage(processedTransitionSymbols.begin(),
                        processedTransitionSymbols.end());
    std::cout << "Scanning failed at end of file, no matched pattern for "
              << garbage << std::endl;
    return false;
  }
  matchedTokens.push_back(Lex::Token("EOF", "EOF"));
  return true;
}

bool Lex::Scanner::scan(std::istream &inputStream) {
  std::deque<char> input;
  char ch;

  inputStream.unsetf(std::ios_base::skipws);

  while (inputStream >> ch) {
    input.push_back(ch);
  }

  return maximalMunch(input);
}

std::vector<Lex::Token> Lex::Scanner::getTokens() const {
  return matchedTokens;
}

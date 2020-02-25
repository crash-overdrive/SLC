#include <cassert>
#include <iostream>
#include <stack>
#include <unordered_set>
#include <vector>

#include "LexRegex.hpp"

std::unordered_set<char> Lex::regexOperators = {'-', '*', '|', '^', '`', '?'};
std::unordered_set<char> Lex::brackets = {'(', ')'};
std::unordered_set<char> Lex::nongraphicalElements = {'b', 'B', 'f', 'F' ,'n', 'N', 'r', 'R', 't', 'T'};
std::unordered_set<char> Lex::miscellaneousElements = {'\\'};

Lex::RegexElement::RegexElement():
  symbol(0), isOperator(false) {};


Lex::RegexElement::RegexElement(char symbol):
  symbol(symbol), isOperator(false) {};


Lex::RegexElement::RegexElement(char symbol, bool isOperator):
  symbol(symbol), isOperator(isOperator) {};


int Lex::operatorPrecedence(char operatorSymbol) {
  assert(regexOperators.find(operatorSymbol) != regexOperators.end());
  switch (operatorSymbol) {
    case '^': return 0;
    case '|': return 1;
    case '`': return 2;
    case '*': return 3;
    case '?': return 4;
    case '-': return 5;
  }
  return INT32_MAX;
}

std::vector<Lex::RegexElement> Lex::parseRegex(std::string regexPattern) {
  std::vector<Lex::RegexElement> parsedRegex;

  for (unsigned int index = 0; index < regexPattern.length(); ++index) {
    char token = regexPattern[index];

    if (token == '\\') { // check if token is escaped
      index = index + 1;
      token = regexPattern[index];

       if (regexOperators.find(token) != regexOperators.end() ||
          brackets.find(token) != brackets.end()) { // check if operator or bracket is escaped
          parsedRegex.push_back(RegexElement(token));
       } else if (nongraphicalElements.find(token) != nongraphicalElements.end()) { // check if non graphical character is escaped
          switch (token) {
            case 'b': parsedRegex.push_back(RegexElement('\b'));
                      break;
            case 'B': parsedRegex.push_back(RegexElement('\b'));
                      break;
            case 'f': parsedRegex.push_back(RegexElement('\f'));
                      break;
            case 'F': parsedRegex.push_back(RegexElement('\f'));
                      break;
            case 'n': parsedRegex.push_back(RegexElement('\n'));
                      break;
            case 'N': parsedRegex.push_back(RegexElement('\n'));
                      break;
            case 'r': parsedRegex.push_back(RegexElement('\r'));
                      break;
            case 'R': parsedRegex.push_back(RegexElement('\r'));
                      break;
            case 't': parsedRegex.push_back(RegexElement('\t'));
                      break;
            case 'T': parsedRegex.push_back(RegexElement('\t'));
                      break;
          }
       } else if (miscellaneousElements.find(token) != miscellaneousElements.end()) { // check if miscellaneous element is escaped
          parsedRegex.push_back(RegexElement('\\'));
       } else { // ERROR
          assert(("Unexpected element found after \\ while parsing regex", false));
       }
    } else if (regexOperators.find(token) != regexOperators.end() ||
              brackets.find(token) != brackets.end()) { // check if token is an operator or bracket
      parsedRegex.push_back(RegexElement(token, true));
    } else { // for every other case just add to parsedRegex
      parsedRegex.push_back(RegexElement(token));
    }
  }

  // std::cout << "PARSED PATTERN: ";
  // for (auto& regexElement : parsedRegex) {
  //   std::cout << regexElement.symbol;
  // }
  // std::cout << std::endl;

  return parsedRegex;
}


std::vector<Lex::RegexElement> Lex::insertExplicitConcatenationOperator(std::string regexPattern) {
  std::vector<Lex::RegexElement> parsedRegex = Lex::parseRegex(regexPattern);
  std::vector<Lex::RegexElement> concatenatedRegexPattern;

  for (unsigned int index = 0; index < parsedRegex.size(); ++index) {
    char token = parsedRegex[index].symbol;
    bool isOperator = parsedRegex[index].isOperator;

    concatenatedRegexPattern.push_back(RegexElement(token, isOperator));

    if (isOperator && (token == '|' || token == '-' || token == '(' || token == '^')) {
      continue;
    }

    if (index < parsedRegex.size() - 1) {
      char lookAheadToken = parsedRegex[index + 1].symbol;
      bool isOperatorAhead = parsedRegex[index + 1].isOperator;

      if (isOperatorAhead && (lookAheadToken == '*' || lookAheadToken == '+' || lookAheadToken == '?' ||
          lookAheadToken == '|' || lookAheadToken == ')' || lookAheadToken == '-')) {
        continue;
      }
      concatenatedRegexPattern.push_back(RegexElement('`', true));
    }
  }

  // std::cout << "CONCATENATED PATTERN: ";
  // for (auto& regexElement : concatenatedRegexPattern) {
  //   std::cout << regexElement.symbol;
  // }
  // std::cout << std::endl;

  // for (auto& regexElement : concatenatedRegexPattern) {
  //   if (regexElement.isOperator) {
  //     std::cout << regexElement.symbol << " is an operator!" << std::endl;
  //   }
  // }

  return concatenatedRegexPattern;
}


std::vector<Lex::RegexElement> Lex::convertRegexToPostFix(std::string regexPattern) {
  std::vector<Lex::RegexElement> concatenatedRegexPattern = Lex::insertExplicitConcatenationOperator(regexPattern);
  std::vector<Lex::RegexElement> postFixRegexPattern;
  std::stack<char> operatorStack;

  for (auto& element : concatenatedRegexPattern) {
    char token = element.symbol;
    bool isOperator = element.isOperator;

    if (isOperator) {
      if (token == '`' || token == '*' || token == '^' || token == '-' || token == '|' || token == '?') {
        while (operatorStack.size() != 0 && operatorStack.top() != '(' &&
          (Lex::operatorPrecedence(operatorStack.top()) >= Lex::operatorPrecedence(token))) {
          postFixRegexPattern.push_back(Lex::RegexElement(operatorStack.top(), true));
          operatorStack.pop();
        }
        operatorStack.push(token);
      } else if (token == '(') {
        operatorStack.push(token);
      } else if (token == ')') {
        while (operatorStack.top() != '(') {
          postFixRegexPattern.push_back(Lex::RegexElement(operatorStack.top(), true));
          operatorStack.pop();
        }
        operatorStack.pop();
      } else {
        assert(("Unexpected character has an attribute of operator in concatenatedRegexPattern", false));
      }
    } else {
      postFixRegexPattern.push_back(Lex::RegexElement(token, false));
    }
  }
  while(operatorStack.size() != 0) {
    postFixRegexPattern.push_back(Lex::RegexElement(operatorStack.top(), true));
    operatorStack.pop();
  }

  // std::cout << "POSTFIX PATTERN: ";
  // for (auto& regexElement : postFixRegexPattern) {
  //   std::cout << regexElement.symbol;
  // }
  // std::cout << std::endl;

  // for (auto& regexElement : postFixRegexPattern) {
  //   if (regexElement.isOperator) {
  //     std::cout << regexElement.symbol << " is an operator!" << std::endl;
  //   }
  // }

  return postFixRegexPattern;
}


Lex::Nfa Lex::convertRegexToNfa(std::string regexPattern, std::string tokenType, int tokenPriority) {
  std::vector<Lex::Nfa> nfaStack;
  Lex::Nfa finalNfa;
  std::vector<Lex::RegexElement> postFixRegexPattern = Lex::convertRegexToPostFix(regexPattern);

  for (auto& element : postFixRegexPattern) {
    char token = element.symbol;
    bool isOperator = element.isOperator;

    if (!isOperator) {

      // create normal NFA
      Lex::Nfa nfa = Lex::createSimpleNfa(token, tokenType, tokenPriority);
      nfaStack.push_back(nfa);
    } else {
      assert(Lex::regexOperators.find(token) != Lex::regexOperators.end());

      if (token == '|') {
        assert(nfaStack.size() >= 2);

        Lex::Nfa secondNfa = nfaStack.back();
        nfaStack.pop_back();
        Lex::Nfa firstNfa = nfaStack.back();
        nfaStack.pop_back();
        nfaStack.push_back(orOperator(firstNfa, secondNfa));
      } else if (token == '`') {
        assert(nfaStack.size() >= 2);

        Lex::Nfa secondNfa = nfaStack.back();
        nfaStack.pop_back();
        Lex::Nfa firstNfa = nfaStack.back();
        nfaStack.pop_back();
        nfaStack.push_back(concatenateOperator(firstNfa, secondNfa));
      } else if (token == '-') {
        assert(nfaStack.size() >= 2);

        Lex::Nfa secondNfa = nfaStack.back();
        nfaStack.pop_back();
        Lex::Nfa firstNfa = nfaStack.back();
        nfaStack.pop_back();
        nfaStack.push_back(rangeOperator(firstNfa, secondNfa));
      } else if (token == '*') {
        assert(nfaStack.size() >= 1);

        Lex::Nfa nfa = nfaStack.back();
        nfaStack.pop_back();
        nfaStack.push_back(starOperator(nfa));
      } else if (token == '?') {
        assert(nfaStack.size() >= 1);

        Lex::Nfa nfa = nfaStack.back();
        nfaStack.pop_back();
        nfaStack.push_back(questionOperator(nfa));
      } else if (token == '^') {
        assert(nfaStack.size() >= 1);

        Lex::Nfa nfa = nfaStack.back();
        nfaStack.pop_back();
        nfaStack.push_back(notOperator(nfa));
      }
    }
  }

  assert(nfaStack.size() == 1);

  finalNfa = nfaStack.back();
  // finalNfa.printInfo();

  return finalNfa;
}

#include "ParseDFA.hpp"
#include "Config.hpp"
#include <sstream>

Parse::DFA::DFA()
    : productions(), states(), finalProductions(), stateStack(), nodeStack(),
      errorState(false), current(&states[0]) {}

void Parse::DFA::read(const Lex::Token &tok) {
  if (errorState) {
    return;
  }
  auto reducesIt = current->reduces.find(tok.type);
  while (reducesIt != current->reduces.end()) {
    const Production &production = reducesIt->second;
    reduce(production);
    shift(current->shifts.at(production.lhs));
    reducesIt = current->reduces.find(tok.type);
  }
  auto shiftsIt = current->shifts.find(tok.type);
  if (shiftsIt != current->shifts.end()) {
    shift(shiftsIt->second);
    nodeStack.emplace_back(std::make_unique<Node>(tok.type, tok.lexeme));
    return;
  }
  errorState = true;
}

void Parse::DFA::clear() {
  stateStack.clear();
  nodeStack.clear();
  errorState = false;
  current = &states[0];
}

bool Parse::DFA::error() const { return errorState; }

bool Parse::DFA::accept() const {
  for (const auto &production : finalProductions) {
    const std::vector<std::string> &rhs = production->rhs;
    if (rhs.size() != nodeStack.size()) {
      continue;
    }
    auto rhsIt = rhs.begin();
    auto nodeIt = nodeStack.begin();
    for (; rhsIt != rhs.end(); ++rhsIt, ++nodeIt) {
      if (*rhsIt != (*nodeIt)->getName()) {
        break;
      }
    }
    if (rhsIt == rhs.end()) {
      return true;
    };
  }
  return false;
}

bool Parse::DFA::parse(std::istream &iStream) {
  Lex::Token tok;
  while (iStream >> tok) {
    read(tok);
    if (errorState) {
      return false;
    }
    iStream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  return accept();
}

bool Parse::DFA::parse(const std::vector<Lex::Token> &tokens) {
  for (const auto &token : tokens) {
    read(token);
    if (errorState) {
      return false;
    }
  }
  return accept();
}

Parse::Tree Parse::DFA::buildTree() {
  std::unique_ptr<Node> root = std::make_unique<Node>(startSymbol);
  for (auto &child : nodeStack) {
    root->addChild(std::move(child));
  }
  clear();
  return Tree(std::move(root));
}

void Parse::DFA::shift(const State &state) {
  stateStack.emplace_back(&state);
  current = &state;
}

void Parse::DFA::reduce(const Production &production) {
  size_t size = production.rhs.size();
  std::unique_ptr<Node> parent = std::make_unique<Node>(production.lhs);
  for (auto it = nodeStack.end() - size; it != nodeStack.end(); ++it) {
    parent->addChild(std::move(*it));
  }
  for (size_t i = 0; i < size; ++i) {
    stateStack.pop_back();
    nodeStack.pop_back();
  }
  nodeStack.emplace_back(std::move(parent));
  current = stateStack.back();
}

std::istream &Parse::operator>>(std::istream &stream, DFA &dfa) {
  unsigned int numLines;
  stream >> numLines;
  for (unsigned int i = 0; i < numLines + 1; i++) {
    stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  stream >> numLines;
  for (unsigned int i = 0; i < numLines + 1; i++) {
    stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  stream >> dfa.startSymbol >> numLines;
  stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  for (unsigned int i = 0; i < numLines; i++) {
    std::string line;
    std::string symbol;
    std::getline(stream, line);
    std::istringstream iss(line);
    iss >> symbol;
    dfa.productions[i].lhs = symbol;
    if (symbol == dfa.startSymbol) {
      dfa.finalProductions.emplace_back(&dfa.productions[i]);
    }
    while (iss >> symbol) {
      dfa.productions[i].rhs.emplace_back(symbol);
    }
  }
  stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  stream >> numLines;
  stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  for (unsigned int i = 0; i < numLines; i++) {
    std::string line;
    std::getline(stream, line);
    std::istringstream iss(line);

    unsigned int stateNum;
    std::string lookAhead;
    std::string actionName;
    unsigned int lastNum;
    iss >> stateNum >> lookAhead >> actionName >> lastNum;
    DFA::State &state = dfa.states[stateNum];
    if (actionName == "reduce") {
      state.reduces.emplace(lookAhead, dfa.productions[lastNum]);
    } else if (actionName == "shift") {
      state.shifts.emplace(lookAhead, dfa.states[lastNum]);
    }
  }
  return stream;
}

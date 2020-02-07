#include "ParseDFA.hpp"
#include "Config.hpp"
#include <sstream>

Parse::DFA::DFA()
    : Productions(), States(), FinalProductions(), StateStack(), NodeStack(),
      errorState(false), Current(States[0]) {}

void Parse::DFA::read(const std::string &Token) {
  if (errorState)
    return;
  auto ReducesIt = Current.Reduces.find(Token);
  while (ReducesIt != Current.Reduces.end()) {
    const Production &Production = ReducesIt->second;
    reduce(Production);
    shift(Current.Shifts.at(Production.LHS));
    ReducesIt = Current.Reduces.find(Token);
  }
  auto ShiftsIt = Current.Shifts.find(Token);
  if (ShiftsIt != Current.Shifts.end()) {
    shift(ShiftsIt->second);
    NodeStack.emplace_back(std::make_unique<Node>(Token));
    return;
  }
  errorState = true;
}

bool Parse::DFA::error() const { return errorState; }

bool Parse::DFA::accept() const {
  for (const auto &Production : FinalProductions) {
    const std::vector<std::string> &RHS = Production.get().RHS;
    if (RHS.size() != NodeStack.size()) {
      continue;
    }
    auto RHSIt = RHS.begin();
    auto NodeIt = NodeStack.begin();
    for (; RHSIt != RHS.end(); ++RHSIt, ++NodeIt) {
      if (*RHSIt != (*NodeIt)->getName()) {
        break;
      }
    }
    if (RHSIt == RHS.end()) {
      return true;
    };
  }
  return false;
}

Parse::Tree Parse::DFA::buildTree() {
  std::unique_ptr<Node> Root = std::make_unique<Node>(StartSymbol);
  for (auto &Child : NodeStack) {
    Root->addChild(std::move(Child));
  }
  NodeStack.clear();
  return Tree(std::move(Root));
}

void Parse::DFA::shift(const State &State) {
  StateStack.emplace_back(State);
  Current = State;
}

void Parse::DFA::reduce(const Production &Production) {
  size_t Size = Production.RHS.size();
  std::unique_ptr<Node> Parent = std::make_unique<Node>(Production.LHS);
  for (auto It = NodeStack.end() - Size; It != NodeStack.end(); ++It) {
    Parent->addChild(std::move(*It));
  }
  for (size_t i = 0; i < Size; ++i) {
    StateStack.pop_back();
    NodeStack.pop_back();
  }
  NodeStack.emplace_back(std::move(Parent));
  Current = StateStack.back();
}

std::istream &Parse::operator>>(std::istream &Stream, DFA &DFA) {
  unsigned int NumLines;
  Stream >> NumLines;
  for (unsigned int i = 0; i < NumLines + 1; i++) {
    Stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  Stream >> NumLines;
  for (unsigned int i = 0; i < NumLines + 1; i++) {
    Stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  Stream >> DFA.StartSymbol >> NumLines;
  Stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  for (unsigned int i = 0; i < NumLines; i++) {
    std::string Line;
    std::string Symbol;
    std::getline(Stream, Line);
    std::istringstream Iss(Line);
    Iss >> Symbol;
    DFA.Productions[i].LHS = Symbol;
    if (Symbol == DFA.StartSymbol) {
      DFA.FinalProductions.emplace_back(DFA.Productions[i]);
    }
    while (Iss >> Symbol) {
      DFA.Productions[i].RHS.emplace_back(Symbol);
    }
  }
  Stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  Stream >> NumLines;
  Stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  for (unsigned int i = 0; i < NumLines; i++) {
    std::string Line;
    std::getline(Stream, Line);
    std::istringstream Iss(Line);

    unsigned int StateNum;
    std::string LookAhead;
    std::string ActionName;
    unsigned int LastNum;
    Iss >> StateNum >> LookAhead >> ActionName >> LastNum;
    DFA::State &State = DFA.States[StateNum];
    if (ActionName == "reduce") {
      State.Reduces.emplace(LookAhead, DFA.Productions[LastNum]);
    } else if (ActionName == "shift") {
      State.Shifts.emplace(LookAhead, DFA.States[LastNum]);
    }
  }
  return Stream;
}

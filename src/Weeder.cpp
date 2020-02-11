#include "Weeder.hpp"
#include <set>

static const Parse::Node *getOtherChild(const Parse::Node *Node) {
  for (const auto &Child : Node->getChildren()) {
    if (Child->getName() != Node->getName()) {
      return Child.get();
    }
  }
  return nullptr;
}

static const Parse::Node *getOnlyChild(const Parse::Node *Node) {
  auto it = Node->getChildren().begin();
  return (*it).get();
}

static std::set<std::string> flatten(const Parse::Node *Node,
                                     const std::string &Name) {
  std::set<std::string> S;
  while (Node->find(Node->getName())) {
    const Parse::Node *Target = getOnlyChild(getOtherChild(Node));
    S.emplace(Target->getName());
    Node = Node->find(Node->getName());
  }
  S.emplace(Node->find(Name)->getName());
  return S;
}

bool Weed::AbstractFinalClass(const Parse::Tree &T) {
  auto Range = T.equalRange("ClassDeclaration");
  for (auto i = Range.first; i != Range.second; ++i) {
    const Parse::Node *ModifierNode = i->second.find("ModifiersList");
    std::set<std::string> S(flatten(ModifierNode, "Modifier"));
    if (S.find("ABSTRACT") != S.end() && S.find("FINAL") != S.end()) {
      return false;
    }
  }
  return true;
}

const std::vector<Weed::Check> Weed::JoosChecks{
    AbstractFinalClass,
};

#include "ParseTree.hpp"

Parse::Node::Node(const std::string &Name) : Name(Name), Tag(), Level(0) {}

Parse::Node::Node(const std::string &Name, const std::string &Tag)
    : Name(Name), Tag(Tag), Level(0) {}

void Parse::Node::addChild(std::unique_ptr<Node> Child) {
  Children.emplace_back(std::move(Child));
}

std::string Parse::Node::getName() const { return Name; }

std::string Parse::Node::getTag() const { return Tag; }

size_t Parse::Node::getLevel() const { return Level; }

const Parse::Node *
Parse::Node::getSuccessor(std::initializer_list<std::string> Names) const {
  const Node *Node = this;
  for (const auto &Name : Names) {
    Node = Node->getChild(Name);
    if (!Node) {
      return nullptr;
    }
  }
  return Node;
}

const std::vector<std::unique_ptr<Parse::Node>> &
Parse::Node::getChildren() const {
  return Children;
}

const Parse::Node *Parse::Node::getChild(const std::string &String) const {
  for (const auto &Child : Children) {
    if (Child->Name == String) {
      return Child.get();
    }
  }
  return nullptr;
}

const Parse::Node *Parse::Node::getFirstChild() const {
  return Children.size() > 0 ? Children[0].get() : nullptr;
}

Parse::Tree::Tree(std::unique_ptr<Node> Root) : Root(std::move(Root)) {
  this->Root->Level = 0;
  for (auto &Parent : *this) {
    std::pair<std::string, Node &> Pair(Parent.Name, Parent);
    for (auto &Child : Parent.Children) {
      Child->Level = Parent.Level + 1;
    }
  }
}

Parse::Tree::Iterator::Iterator(Node *Ptr) : Ptr(Ptr) {}

Parse::Tree::Iterator &Parse::Tree::Iterator::operator++() {
  for (auto It = Ptr->Children.rbegin(); It != Ptr->Children.rend(); ++It) {
    Vector.emplace_back(It->get());
  }
  if (Vector.empty()) {
    Ptr = nullptr;
  } else {
    Ptr = Vector.back();
    Vector.pop_back();
  }
  return *this;
}

Parse::Node &Parse::Tree::Iterator::operator*() const { return *Ptr; }

Parse::Node *Parse::Tree::Iterator::operator->() { return Ptr; }

bool Parse::Tree::Iterator::operator!=(const Iterator &Iter) const {
  return Ptr != Iter.Ptr;
}

bool Parse::Tree::Iterator::operator==(const Iterator &Iter) const {
  return Ptr == Iter.Ptr;
}

Parse::Tree::Iterator Parse::Tree::begin() const {
  return Parse::Tree::Iterator(Root.get());
}

Parse::Tree::Iterator Parse::Tree::end() const {
  return Parse::Tree::Iterator(nullptr);
}

const Parse::Node &Parse::Tree::getRoot() const { return *Root; }

std::ostream &Parse::operator<<(std::ostream &Stream, const Parse::Tree &T) {
  for (const auto &Node : T) {
    Stream << std::string(Node.getLevel(), ' ') << Node.getName() << '\n';
  }
  return Stream;
}

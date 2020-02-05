#include "ParseTree.hpp"

Parse::Node::Node(std::string Name) : Name(Name), Level(0), Children() {}

void Parse::Node::addChild(std::unique_ptr<Parse::Node> Child) {
  Children.emplace_back(std::move(Child));
}

std::string Parse::Node::getName() const { return Name; }

size_t Parse::Node::getLevel() const { return Level; }

Parse::Tree::Tree(std::unique_ptr<Node> Head) : Head(std::move(Head)) {
  this->Head->Level = 0;
  for (const auto &Parent : *this) {
    for (auto &Child : Parent.Children) {
      Child->Level = Parent.Level + 1;
    }
  }
}

Parse::Tree::Iterator::Iterator(Node *Ptr) : Ptr(Ptr), Vector() {}

Parse::Tree::Iterator &Parse::Tree::Iterator::operator++() {
  for (auto It = Ptr->Children.rbegin(); It != Ptr->Children.rend(); ++It) {
    Vector.push_back(It->get());
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

bool Parse::Tree::Iterator::operator!=(const Iterator &Iter) const {
  return Ptr != Iter.Ptr;
}

bool Parse::Tree::Iterator::operator==(const Iterator &Iter) const {
  return Ptr == Iter.Ptr;
}

Parse::Tree::Iterator Parse::Tree::begin() const {
  return Parse::Tree::Iterator(Head.get());
}

Parse::Tree::Iterator Parse::Tree::end() const {
  return Parse::Tree::Iterator(nullptr);
}

std::ostream &Parse::operator<<(std::ostream &Stream, const Parse::Tree &T) {
  for (const auto &Node : T) {
    Stream << std::string(Node.getLevel(), ' ') << Node.getName() << '\n';
  }
  return Stream;
}

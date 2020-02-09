#include "ParseTree.hpp"

Parse::Node::Node(const std::string &Name) : Name(Name), Tag(), Level(0) {}

Parse::Node::Node(const std::string &Name, const std::string &Tag)
    : Name(Name), Tag(Tag), Level(0) {}

void Parse::Node::addChild(std::unique_ptr<Node> Child) {
  Children.emplace_back(std::move(Child));
  std::unique_ptr<Node> &ChildRef = Children.back();
  ChildrenCache.emplace(ChildRef->Name, *ChildRef);
}

std::string Parse::Node::getName() const { return Name; }

size_t Parse::Node::getLevel() const { return Level; }

const std::vector<std::unique_ptr<Parse::Node>> &Parse::Node::getChildren() const {
  return Children;
}

const Parse::Node *Parse::Node::find(const std::string &String) const {
  auto it = ChildrenCache.find(String);
  return (it != ChildrenCache.end()) ? &(it->second) : nullptr;
}

Parse::Tree::Tree(std::unique_ptr<Node> Head) : Head(std::move(Head)) {
  this->Head->Level = 0;
  for (auto &Parent : *this) {
    std::pair<std::string, Node &> Pair(Parent.Name, Parent);
    TreeCache.insert(std::move(Pair));
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

std::pair<Parse::Tree::MMapIt, Parse::Tree::MMapIt>
Parse::Tree::equalRange(const std::string &String) {
  return TreeCache.equal_range(String);
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

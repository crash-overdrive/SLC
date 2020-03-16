#include "ParseTree.hpp"

Parse::Node::Node(const std::string &name) : name(name), tag(), level(0) {}

Parse::Node::Node(const std::string &name, const std::string &tag)
    : name(name), tag(tag), level(0) {}

void Parse::Node::addChild(std::unique_ptr<Node> child) {
  children.emplace_back(std::move(child));
}

std::string Parse::Node::getName() const { return name; }

std::string Parse::Node::getTag() const { return tag; }

size_t Parse::Node::getLevel() const { return level; }

const Parse::Node *
Parse::Node::getSuccessor(std::initializer_list<std::string> names) const {
  const Node *node = this;
  for (const auto &name : names) {
    node = node->getChild(name);
    if (!node) {
      return nullptr;
    }
  }
  return node;
}

const std::vector<std::unique_ptr<Parse::Node>> &
Parse::Node::getChildren() const {
  return children;
}

const Parse::Node *Parse::Node::getChild(const std::string &string) const {
  for (const auto &child : children) {
    if (child->name == string) {
      return child.get();
    }
  }
  return nullptr;
}

const Parse::Node *Parse::Node::getFirstChild() const {
  return children.size() > 0 ? children[0].get() : nullptr;
}

Parse::Tree::Tree() {}

Parse::Tree::Tree(std::unique_ptr<Node> root) : root(std::move(root)) {
  this->root->level = 0;
  for (auto &parent : *this) {
    std::pair<std::string, Node &> pair(parent.name, parent);
    for (auto &child : parent.children) {
      child->level = parent.level + 1;
    }
  }
}

Parse::Tree::Iterator::Iterator(Node *ptr) : ptr(ptr) {}

Parse::Tree::Iterator &Parse::Tree::Iterator::operator++() {
  for (auto it = ptr->children.rbegin(); it != ptr->children.rend(); ++it) {
    vector.emplace_back(it->get());
  }
  if (vector.empty()) {
    ptr = nullptr;
  } else {
    ptr = vector.back();
    vector.pop_back();
  }
  return *this;
}

Parse::Node &Parse::Tree::Iterator::operator*() const { return *ptr; }

Parse::Node *Parse::Tree::Iterator::operator->() { return ptr; }

bool Parse::Tree::Iterator::operator!=(const Iterator &iter) const {
  return ptr != iter.ptr;
}

bool Parse::Tree::Iterator::operator==(const Iterator &iter) const {
  return ptr == iter.ptr;
}

Parse::Tree::Iterator Parse::Tree::begin() const {
  return Parse::Tree::Iterator(root.get());
}

Parse::Tree::Iterator Parse::Tree::end() const {
  return Parse::Tree::Iterator(nullptr);
}

const Parse::Node &Parse::Tree::getRoot() const { return *root; }

std::ostream &Parse::operator<<(std::ostream &stream, const Parse::Tree &t) {
  for (const auto &node : t) {
    stream << std::string(node.getLevel(), ' ') << node.getName() << '\n';
  }
  return stream;
}

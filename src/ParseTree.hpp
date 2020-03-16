#ifndef PARSETREE_HPP
#define PARSETREE_HPP

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Parse {

class Node {
public:
  Node(const std::string &name);
  Node(const std::string &name, const std::string &tag);
  void addChild(std::unique_ptr<Node> child);
  std::string getName() const;
  size_t getLevel() const;
  std::string getTag() const;
  const Node *getSuccessor(std::initializer_list<std::string> names) const;
  const Node *getChild(const std::string &string) const;
  const std::vector<std::unique_ptr<Node>> &getChildren() const;
  const Node *getFirstChild() const;

private:
  friend class Tree;
  std::string name;
  std::string tag;
  size_t level;
  std::vector<std::unique_ptr<Node>> children;
  std::unordered_map<std::string, Node &> childrenCache;
};

class Tree {
public:
  Tree();
  Tree(std::unique_ptr<Node> head);

  class Iterator {
  public:
    Iterator(Node *ptr);
    Iterator &operator++();
    Node &operator*() const;
    Node *operator->();
    bool operator!=(const Iterator &iter) const;
    bool operator==(const Iterator &iter) const;

  private:
    friend std::ostream &operator<<(std::ostream &stream, const Tree &t);
    Node *ptr;
    std::vector<Node *> vector;
  };

  Iterator begin() const;
  Iterator end() const;
  const Node &getRoot() const;

private:
  std::unique_ptr<Node> root;
};

std::ostream &operator<<(std::ostream &stream, const Tree &t);

} // namespace Parse

#endif // PARSETREE_HPP

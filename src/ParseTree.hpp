#ifndef PARSETREE_HPP
#define PARSETREE_HPP

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace Parse {

class Node {
public:
  Node(std::string Name);
  void addChild(std::unique_ptr<Node> child);
  std::string getName() const;
  size_t getLevel() const;

private:
  friend class Tree;
  std::string Name;
  size_t Level;
  std::vector<std::unique_ptr<Node>> Children;
};

class Tree {
public:
  Tree(std::unique_ptr<Node> Head);

  class Iterator {
  public:
    Iterator(Node *Ptr);
    Iterator &operator++();
    Node *get();
    Node &operator*() const;
    Node *operator->() const;
    bool operator!=(const Iterator &Iter) const;
    bool operator==(const Iterator &Iter) const;

  private:
    Node *Ptr;
    std::vector<Node *> Vector;
  };

  Iterator begin() const;
  Iterator end() const;

private:
  friend std::ostream &operator<<(std::ostream &Stream, const Tree &T);
  std::unique_ptr<Node> Head;
};

std::ostream &operator<<(std::ostream &Stream, const Tree &T);

} // namespace Parse

#endif // PARSETREE_HPP

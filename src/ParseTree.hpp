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
  Node(const std::string &Name);
  Node(const std::string &Name, const std::string &Tag);
  void addChild(std::unique_ptr<Node> child);
  std::string getName() const;
  size_t getLevel() const;
  Node *find(const std::string &String) const;

private:
  friend class Tree;
  std::string Name;
  std::string Tag;
  size_t Level;
  std::vector<std::unique_ptr<Node>> Children;
  std::unordered_map<std::string, Node &> ChildrenCache;
};

class Tree {
public:
  using MMapIt = std::multimap<std::string, Node &>::iterator;
  Tree(std::unique_ptr<Node> Head);

  class Iterator {
  public:
    Iterator(Node *Ptr);
    Iterator &operator++();
    Node &operator*() const;
    Node *operator->();
    bool operator!=(const Iterator &Iter) const;
    bool operator==(const Iterator &Iter) const;

  private:
    Node *Ptr;
    std::vector<Node *> Vector;
  };

  std::pair<MMapIt, MMapIt> equalRange(const std::string &String);
  Iterator begin() const;
  Iterator end() const;

private:
  std::unique_ptr<Node> Head;
  std::multimap<std::string, Node &> TreeCache;
};

std::ostream &operator<<(std::ostream &Stream, const Tree &T);

} // namespace Parse

#endif // PARSETREE_HPP

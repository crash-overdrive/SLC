#ifndef CODEGENVISITOR_HPP
#define CODEGENVISITOR_HPP

#include "EnvLocal.hpp"
#include "NameResolver.hpp"

namespace CodeGen {

class Listener : public Name::ResolverListener {
public:
  Listener(std::ostream &ostream);
  void listenLocal(off_t offset) override;

private:
  std::ostream &ostream;
};

class Visitor : public Env::LocalTrackVisitor {
  using LocalTrackVisitor::visit;

public:
  Visitor(std::ostream &ostream, const Env::TypeLink &typeLink);
  void visit(const AST::BinaryExpression &node) override;
  void visit(const AST::Operator &node) override;
  void visit(const AST::UnaryExpression &node) override;
  void visit(const AST::SingleVariableDeclaration &node) override;
  void visit(const AST::VariableDeclaration &node) override;
  void visit(const AST::AssignmentExpression &node) override;
  void visit(const AST::ReturnStatement &node) override;
  void visit(const AST::Name &node) override;
  void visit(const AST::DecIntLiteral &node) override;

private:
  std::ostream &ostream;
  Listener listener;
  Name::ResolverFactory resolverFactory;
  bool address = false;
  std::string operatorInstruction;
};

} // namespace CodeGen

#endif // CODEGENVISITOR_HPP

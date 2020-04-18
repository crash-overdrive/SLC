#ifndef CODEGENVISITOR_HPP
#define CODEGENVISITOR_HPP

#include "EnvLocal.hpp"
#include "NameResolver.hpp"
#include <stack>

namespace CodeGen {

class Listener : public Name::ResolverListener {
public:
  Listener(std::ostream &ostream);
  void listenLocal(off_t offset) override;
  // void listenImplicit() override;
  // void listenField(const Env::Field &) override;
  // void listenStaticField(const Env::Field &) override;
  void listenMethod(const Env::Method &) override;
  void listenStaticMethod(const Env::Method &) override;
  // void listenConstructor(const Env::Constructor &) override;
  // void listenArrayLength() override;
  void generateMethod();

private:
  std::ostream &ostream;
  const Env::Method *staticMethod = nullptr;
  const Env::Method *method = nullptr;
};

class Visitor : public Env::LocalTrackVisitor {
  using LocalTrackVisitor::visit;

public:
  Visitor(std::ostream &ostream, const Env::TypeLink &typeLink);
  void visit(const AST::BinaryExpression &node) override;
  void visit(const AST::UnaryExpression &node) override;
  void visit(const AST::SingleVariableDeclaration &node) override;
  void visit(const AST::VariableDeclaration &node) override;
  void visit(const AST::AssignmentExpression &node) override;
  void visit(const AST::ReturnStatement &node) override;
  void visit(const AST::MethodNameInvocation &node) override;
  void visit(const AST::Name &node) override;
  void visit(const AST::Argument &node) override;
  void visit(const AST::DecIntLiteral &node) override;

private:
  std::ostream &ostream;
  Listener listener;
  Name::ResolverFactory resolverFactory;
  const Env::TypeLink &typeLink;

  std::unique_ptr<AST::Visitor> nameVisitor;
};

class MethodNameVisitor : public AST::Visitor {
public:
  MethodNameVisitor(std::ostream &ostream, Name::MethodResolver resolver);
  void visit(const AST::Name &node) override;

private:
  std::ostream &ostream;
  Name::MethodResolver methodResolver;
};

class FieldNameVisitor : public AST::Visitor {
public:
  FieldNameVisitor(std::ostream &ostream, Name::FieldResolver resolverField);
  void visit(const AST::Name &node) override;

private:
  std::ostream &ostream;
  Name::FieldResolver fieldResolver;
};

class FieldAddressVisitor : public AST::Visitor {
public:
  FieldAddressVisitor(std::ostream &ostream, Name::FieldResolver fieldResolver);
  void visit(const AST::Name &node) override;

private:
  std::ostream &ostream;
  Name::FieldResolver fieldResolver;
};

} // namespace CodeGen

#endif // CODEGENVISITOR_HPP

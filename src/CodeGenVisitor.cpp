#include "CodeGenVisitor.hpp"
#include "ASMStructuralLib.hpp"
#include "ASTVisitorUtil.hpp"
#include "TypeVisitorUtil.hpp"

namespace CodeGen {

Listener::Listener(std::ostream &ostream) : ostream(ostream) {}

void Listener::listenLocal(off_t offset) {
  ASM::printLocalVariable(ostream, offset);
}

void Listener::listenMethod(const Env::Method &method) {
  this->method = &method;
}

void Listener::listenStaticMethod(const Env::Method &method) {
  this->staticMethod = &method;
}

void Listener::generateMethod() {
  if (staticMethod) {
    ASM::printCall(ostream, staticMethod->label);
    ostream << "add esp, " << staticMethod->args.size() * 4 << '\n';
    staticMethod = nullptr;
  } else if (method) {
    ostream << "mov eax, [esp + " << method->args.size() * 4 << "]\n";
    ostream << "mov eax, [eax]\n";
    ostream << "mov eax, [eax + " << method->offset << "]\n";
    ostream << "add esp, " << (method->args.size() + 1) * 4 << '\n';
    method = nullptr;
  } else {
    std::cerr << "Error";
  }
}

Visitor::Visitor(std::ostream &ostream, const Env::TypeLink &typeLink)
    : LocalTrackVisitor(typeLink), ostream(ostream), listener(ostream),
      resolverFactory(getLocal(), typeLink, listener), typeLink(typeLink),
      nameVisitor(std::make_unique<FieldNameVisitor>(
          ostream, resolverFactory.getField())) {}

void Visitor::visit(const AST::BinaryExpression &node) {
  Type::BinaryOperatorVisitor binaryVisitor;

  node.getChild(0).accept(*this);

  node.getChild(1).accept(binaryVisitor);
  Type::BinaryOperator binaryOperator = binaryVisitor.getBinaryOperator();
  if (binaryOperator != Type::BinaryOperator::And ||
      binaryOperator != Type::BinaryOperator::Or) {
    ostream << "push eax\n";
  }
  node.getChild(2).accept(*this);
  ostream << "pop ebx\n";
  ASM::printBinaryOperator(ostream, binaryOperator);
}

void Visitor::visit(const AST::UnaryExpression &node) {
  node.getChild(1).accept(*this);
  Type::UnaryOperatorVisitor unaryVisitor;
  node.getChild(0).accept(*this);
  ASM::printUnaryOperator(ostream, unaryVisitor.getUnaryOperator());
}

void Visitor::visit(const AST::SingleVariableDeclaration &node) {
  Env::LocalTrackVisitor::visit(node);
  ASM::printLocalVariable(ostream, getLocal().getLastVariable().offset);
  ostream << "push eax\n";
}

void Visitor::visit(const AST::VariableDeclaration &node) {
  dispatchChildren(node);
  ASM::printAssignment(ostream);
}

void Visitor::visit(const AST::ReturnStatement &node) {
  dispatchChildren(node);
  ASM::printEpilogue(ostream);
}

void Visitor::visit(const AST::AssignmentExpression &node) {
  nameVisitor = std::make_unique<FieldAddressVisitor>(
      ostream, resolverFactory.getField());
  dispatchChildren(node);
  ASM::printAssignment(ostream);
}

void Visitor::visit(const AST::MethodNameInvocation &node) {
  Type::ArgumentsVisitor argumentsVisitor(typeLink.getTree(), resolverFactory,
                                          typeLink);
  argumentsVisitor.dispatchChildren(node);

  Listener listener(ostream);
  Name::MethodResolver methodResolver = resolverFactory.getMethod(listener);
  methodResolver.setArgs(argumentsVisitor.getArgs());
  nameVisitor = std::make_unique<MethodNameVisitor>(ostream, methodResolver);
  dispatchChildren(node);
  listener.generateMethod();
}

void Visitor::visit(const AST::Name &node) {
  node.accept(*nameVisitor);
  nameVisitor =
      std::make_unique<FieldNameVisitor>(ostream, resolverFactory.getField());
}

void Visitor::visit(const AST::Argument &node) {
  dispatchChildren(node);
  ostream << "push eax\n";
}

void Visitor::visit(const AST::DecIntLiteral &node) {
  ostream << "mov eax, " << node.getLiteral() << '\n';
}

MethodNameVisitor::MethodNameVisitor(std::ostream &ostream,
                                     Name::MethodResolver methodResolver)
    : ostream(ostream), methodResolver(std::move(methodResolver)) {}

void MethodNameVisitor::visit(const AST::Name &node) {
  AST::NameVisitor visitor;
  node.accept(visitor);
  methodResolver.match(visitor.getName());
}

FieldNameVisitor::FieldNameVisitor(std::ostream &ostream,
                                   Name::FieldResolver fieldResolver)
    : ostream(ostream), fieldResolver(std::move(fieldResolver)) {}

void FieldNameVisitor::visit(const AST::Name &node) {
  AST::NameVisitor visitor;
  node.accept(visitor);
  fieldResolver.match(visitor.getName());
  ostream << "mov eax, [eax]\n";
}

FieldAddressVisitor::FieldAddressVisitor(std::ostream &ostream,
                                         Name::FieldResolver fieldResolver)
    : ostream(ostream), fieldResolver(std::move(fieldResolver)) {}

void FieldAddressVisitor::visit(const AST::Name &node) {
  AST::NameVisitor visitor;
  node.accept(visitor);
  fieldResolver.match(visitor.getName());
  ostream << "push eax\n";
}

} // namespace CodeGen

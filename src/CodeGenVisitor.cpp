#include "CodeGenVisitor.hpp"
#include "ASMStructuralLib.hpp"
#include "ASTVisitorUtil.hpp"
#include "TypeVisitorUtil.hpp"

namespace CodeGen {

Listener::Listener(std::ostream &ostream) : ostream(ostream) {}

void Listener::listenLocal(off_t offset) {
  ASM::printLocalVariable(ostream, offset);
}

Visitor::Visitor(std::ostream &ostream, const Env::TypeLink &typeLink)
    : LocalTrackVisitor(typeLink), ostream(ostream), listener(ostream),
      resolverFactory(getLocal(), typeLink, listener) {}

void Visitor::visit(const AST::BinaryExpression &node) {
  Type::BinaryOperatorVisitor binaryVisitor;
  binaryVisitor.dispatchChildren(node);
  Type::BinaryOperator binaryOperator = binaryVisitor.getBinaryOperator();
  if (binaryOperator == Type::BinaryOperator::Or ||
      binaryOperator == Type::BinaryOperator::And) {
  } else {
    operatorInstruction = "push eax\n";
  }
  dispatchChildren(node);
  ostream << "pop ebx\n";
  ASM::printBinaryOperator(ostream, binaryOperator);
}

void Visitor::visit(const AST::Operator &) {
  ostream << operatorInstruction;
  operatorInstruction.clear();
}

void Visitor::visit(const AST::UnaryExpression &node) {
  dispatchChildren(node);
  Type::UnaryOperatorVisitor unaryVisitor;
  unaryVisitor.dispatchChildren(node);
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
  address = true;
  dispatchChildren(node);
  ASM::printAssignment(ostream);
}

void Visitor::visit(const AST::Name &node) {
  AST::NameVisitor visitor;
  node.accept(visitor);
  resolverFactory.getField().match(visitor.getName());
  if (address) {
    ostream << "push eax\n";
    address = false;
  } else {
    ostream << "mov eax, [eax]\n";
  }
}

void Visitor::visit(const AST::DecIntLiteral &node) {
  ostream << "mov eax, " << node.getLiteral() << '\n';
}

} // namespace CodeGen

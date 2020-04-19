#include "CodeGenVisitor.hpp"
#include "ASMStructuralLib.hpp"
#include "ASTVisitorUtil.hpp"
#include "TypeCheckerVisitor.hpp"

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
    ostream << "call eax\n";
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
  node.getChild(0).accept(unaryVisitor);
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

void Visitor::visit(const AST::MethodPrimaryInvocation &node) {
  node.getChild(0).accept(*this);
  ostream << "push eax\n";
  Type::ExpressionVisitor expressionVisitor(typeLink.getTree(), resolverFactory,
                                            typeLink);
  expressionVisitor.dispatchChildren(node);
  AST::PropertiesVisitor propertiesVisitor;
  propertiesVisitor.dispatchChildren(node);
  Type::ArgumentsVisitor argumentsVisitor(typeLink.getTree(), resolverFactory,
                                          typeLink);
  argumentsVisitor.dispatchChildren(node);

  Listener listener(ostream);
  Name::MethodResolver methodResolver = resolverFactory.getMethod(listener);
  std::vector<Env::Type> args = argumentsVisitor.getArgs();
  methodResolver.setArgs(args);
  methodResolver.match(expressionVisitor.getType(),
                       propertiesVisitor.getIdentifier());
  for (size_t i = 0; i < args.size(); ++i) {
    node.getChild(2 + i).accept(*this);
  }
  listener.generateMethod();
}

void Visitor::visit(const AST::IfThenStatement &node) {
  std::string end = labelService.getUniqueLabel();
  node.getChild(0).accept(*this);
  ostream << "cmp eax, 0\n";
  ostream << "je " << end + "\n";
  node.getChild(1).accept(*this);
  ASM::printLabel(ostream, end);
}

void Visitor::visit(const AST::IfThenElseStatement &node) {
  std::string elseLabel = labelService.getUniqueLabel();
  std::string end = labelService.getUniqueLabel();

  node.getChild(0).accept(*this);
  ostream << "cmp eax, 0\n";
  ostream << "je " << elseLabel << '\n';
  node.getChild(1).accept(*this);
  ostream << "jmp " << end << '\n';
  ASM::printLabel(ostream, elseLabel);
  node.getChild(2).accept(*this);
  ASM::printLabel(ostream, end);
}

void Visitor::visit(const AST::WhileStatement &node) {
  std::string begin = labelService.getUniqueLabel();
  std::string end = labelService.getUniqueLabel();

  ASM::printLabel(ostream, begin);
  node.getChild(0).accept(*this);
  ostream << "cmp eax, 0\n";
  ostream << "je " + end + "\n";
  node.getChild(1).accept(*this);
  ostream << "jmp " << begin << '\n';
  ASM::printLabel(ostream, end);
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

void Visitor::visit(const AST::BooleanLiteral &node) {
  if (node.getLiteral() == "true") {
    ostream << "mov eax, 1\n";
  } else {
    ostream << "mov eax, 0\n";
  }
}

void Visitor::visit(const AST::ClassInstanceCreation &node) {
  AST::TypeVisitor typeVisitor(typeLink);
  node.getChild(0).accept(typeVisitor);
  Env::Type type = typeVisitor.getType();
  const Env::TypeContain &contain = type.declare->contain;
  ostream << "mov eax, " << (contain.getFields().size() + 1) * 4 << '\n';
  ASM::printCall(ostream, "__malloc");
  ostream << "mov ebx, " << contain.vtablelabel << '\n';
  ostream << "mov [eax], ebx" << '\n';
}

MethodNameVisitor::MethodNameVisitor(std::ostream &ostream,
                                     Name::MethodResolver methodResolver)
    : ostream(ostream), methodResolver(std::move(methodResolver)) {}

void MethodNameVisitor::visit(const AST::Name &node) {
  AST::NameVisitor visitor;
  node.accept(visitor);
  methodResolver.match(visitor.getName());
  ostream << "mov eax, [eax]\n";
  ostream << "push eax\n";
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

void FrameStackVisitor::visit(const AST::SingleVariableDeclaration &) {
  ++declaration;
}

unsigned int FrameStackVisitor::getDeclaration() const { return declaration; }

} // namespace CodeGen

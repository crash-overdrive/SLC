#include "CodeGenVisitor.hpp"
#include "ASMStructuralLib.hpp"
#include "ASTVisitorUtil.hpp"
#include "TypeCheckerVisitor.hpp"

namespace CodeGen {

Listener::Listener(std::ostream &ostream) : ostream(ostream) {}

void Listener::listenLocal(off_t offset) {
  ASM::printLocalVariable(ostream, offset);
  isValue = false;
}

void Listener::listenField(const Env::Field &field) {
  if (!isValue) {
    ostream << "mov eax, [eax]\n";
  }
  ostream << "add eax, " << field.offset << '\n';
  isValue = false;
}

void Listener::listenStaticField(const Env::Field &) {}

void Listener::listenImplicit() {
  ostream << "mov eax, [ebp + " << offset << "]\n";
  isValue = true;
}

void Listener::listenMethod(const Env::Method &method) {
  if (!isValue) {
    ostream << "mov eax, [eax]\n";
  }
  ostream << "push eax\n";
  this->method = &method;
}

void Listener::listenStaticMethod(const Env::Method &method) {
  this->staticMethod = &method;
}

void Listener::listenArrayLength() { ostream << "mov eax, [eax]\n"; }

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

void Listener::listenConstructor(const Env::Constructor &constructor) {
  ASM::printCall(ostream, constructor.label);
  ostream << "add esp, " << constructor.args.size() * 4 << '\n';
}

void Listener::setOffset(off_t offset) { this->offset = offset; }

void Listener::setValue() { this->isValue = true; }

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
  if (binaryOperator == Type::BinaryOperator::And) {
    std::string label = labelService.getUniqueLabel();
    ostream << "cmp eax, 0\n";
    ostream << "je " << label << '\n';
    node.getChild(2).accept(*this);
    ASM::printLabel(ostream, label);
  } else if (binaryOperator == Type::BinaryOperator::Or) {
    std::string label = labelService.getUniqueLabel();
    ostream << "cmp eax, 0\n";
    ostream << "jne " << label << '\n';
    node.getChild(2).accept(*this);
    ASM::printLabel(ostream, label);
  } else {
    ostream << "push eax\n";
    node.getChild(2).accept(*this);
    ostream << "pop ebx\n";
    ASM::printBinaryOperator(ostream, binaryOperator);
  }
}

void Visitor::visit(const AST::UnaryExpression &node) {
  node.getChild(1).accept(*this);
  Type::UnaryOperatorVisitor unaryVisitor;
  node.getChild(0).accept(unaryVisitor);
  ASM::printUnaryOperator(ostream, unaryVisitor.getUnaryOperator());
}

void Visitor::visit(const AST::SingleVariableDeclaration &node) {
  Env::LocalTrackVisitor::visit(node);
}

void Visitor::visit(const AST::VariableDeclaration &node) {
  node.getChild(0).accept(*this);
  ASM::printLocalVariable(ostream, getLocal().getLastVariable().offset);
  ostream << "push eax\n";
  node.getChild(1).accept(*this);
  ASM::printAssignment(ostream);
}

void Visitor::visit(const AST::ReturnStatement &node) {
  dispatchChildren(node);
  ASM::printEpilogue(ostream);
}

void Visitor::visit(const AST::AssignmentExpression &node) {
  isAddress = true;
  nameVisitor = std::make_unique<FieldAddressVisitor>(
      ostream, resolverFactory.getField());
  node.getChild(0).accept(*this);
  nameVisitor =
      std::make_unique<FieldNameVisitor>(ostream, resolverFactory.getField());
  isAddress = false;
  ostream << "push eax\n";
  node.getChild(1).accept(*this);
  ASM::printAssignment(ostream);
}

void Visitor::visit(const AST::MethodNameInvocation &node) {
  Name::ResolverFactory resolver(getLocal(), typeLink);
  Type::ArgumentsVisitor argumentsVisitor(typeLink.getTree(), resolver,
                                          typeLink);
  argumentsVisitor.dispatchChildren(node);

  Listener listener(ostream);
  listener.setOffset(offset);
  Name::MethodResolver methodResolver = resolverFactory.getMethod(listener);
  methodResolver.setArgs(argumentsVisitor.getArgs());
  nameVisitor = std::make_unique<MethodNameVisitor>(ostream, methodResolver);
  dispatchChildren(node);
  listener.generateMethod();
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
  ostream << "push eax\n";
  dispatchChildren(node);
  Name::ResolverFactory resolver2(getLocal(), typeLink);
  Type::ArgumentsVisitor argumentsVisitor(typeLink.getTree(), resolver2,
                                          typeLink);
  argumentsVisitor.dispatchChildren(node);
  Name::ConstructorResolver resolver = resolverFactory.getConstructor();
  resolver.match(type, argumentsVisitor.getArgs());
  ostream << "pop eax\n";
}

void Visitor::visit(const AST::MethodPrimaryInvocation &node) {
  node.getChild(0).accept(*this);
  ostream << "push eax\n";
  Name::ResolverFactory resolver(getLocal(), typeLink);
  Type::ExpressionVisitor expressionVisitor(typeLink.getTree(), resolver,
                                            typeLink);
  expressionVisitor.dispatchChildren(node);
  AST::PropertiesVisitor propertiesVisitor;
  propertiesVisitor.dispatchChildren(node);
  Type::ArgumentsVisitor argumentsVisitor(typeLink.getTree(), resolver,
                                          typeLink);
  argumentsVisitor.dispatchChildren(node);

  Listener listener(ostream);
  listener.setOffset(offset);
  listener.setValue();
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

  if (node.getChildren().size() > 1) {
    ASM::printLabel(ostream, begin);
    node.getChild(0).accept(*this);
    ostream << "cmp eax, 0\n";
    ostream << "je " + end + "\n";
    node.getChild(1).accept(*this);
    ostream << "jmp " << begin << '\n';
    ASM::printLabel(ostream, end);
  }
}

void Visitor::visit(const AST::ForStatement &) {}

void Visitor::visit(const AST::FieldAccess &node) {
  node.getChild(0).accept(*this);
  Name::ResolverFactory resolver2(getLocal(), typeLink);
  Type::ExpressionVisitor expressionVisitor(typeLink.getTree(), resolver2,
                                            typeLink);
  expressionVisitor.dispatchChildren(node);
  AST::PropertiesVisitor propertiesVisitor;
  propertiesVisitor.dispatchChildren(node);

  listener.setValue();
  Name::FieldResolver fieldResolver = resolverFactory.getField();
  fieldResolver.match(expressionVisitor.getType(),
                      propertiesVisitor.getIdentifier());
  if (!isAddress) {
    ostream << "mov eax, [eax]\n";
  }
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

void Visitor::visit(const AST::ThisExpression &) {
  ostream << "mov eax, [ebp + " << offset << "]\n";
}

void Visitor::visit(const AST::ArrayCreation &node) {
  AST::TypeVisitor typeVisitor(typeLink);
  node.getChild(0).accept(typeVisitor);

  node.getChild(1).accept(*this);
  size_t size = 0;
  if (typeVisitor.getType().keyword != Env::TypeKeyword::Simple) {
    size = 4;
  } else {
    size = typeVisitor.getType().declare->contain.getFields().size() * 4;
  }
  ostream << "push eax\n";
  ostream << "imul eax, " << size << '\n';
  ostream << "add eax, 4\n";
  ASM::printCall(ostream, "__malloc");
  ostream << "pop ebx" << '\n';
  ostream << "mov [eax], ebx" << '\n';
}

void Visitor::setOffset(off_t offset) {
  listener.setOffset(offset);
  this->offset = offset;
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
}

void FrameStackVisitor::visit(const AST::SingleVariableDeclaration &) {
  ++declaration;
}

unsigned int FrameStackVisitor::getDeclaration() const { return declaration; }

} // namespace CodeGen

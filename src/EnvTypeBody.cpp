#include "EnvTypeBody.hpp"
#include "ASTVisitorUtil.hpp"

namespace Env {

Field::Field(std::set<Modifier> modifiers, Type type, std::string identifier,
             const AST::FieldDeclaration *astNode)
    : modifiers(std::move(modifiers)), type(std::move(type)),
      identifier(std::move(identifier)), astNode(astNode) {}

bool Field::operator==(const Field &field) const {
  return identifier == field.identifier;
}

bool Field::operator!=(const Field &field) const {
  return identifier != field.identifier;
}

std::ostream &operator<<(std::ostream &stream, const Field &field) {
  stream << "JOOSFIELD"
         << "\n";
  stream << "Modifiers: {";
  for (auto const &modifier : field.modifiers) {
    stream << modifierString.at(modifier) << " ";
  }
  stream << "}\n";
  stream << "Identifier: " << field.identifier << "\n";
  return stream << "\n";
}

Method::Method(std::set<Modifier> modifiers, Type returnType,
               std::string identifier, std::vector<Type> args,
               const AST::MethodDeclaration *astNode)
    : modifiers(std::move(modifiers)), returnType(std::move(returnType)),
      identifier(std::move(identifier)), args(std::move(args)),
      astNode(astNode) {}

bool Method::operator==(const Method &method) const {
  return identifier == method.identifier && args == method.args;
}

bool Method::operator!=(const Method &method) const {
  return identifier != method.identifier || args != method.args;
}

std::ostream &operator<<(std::ostream &stream, const Method &method) {
  stream << "JOOSMETHOD"
         << "\n";
  stream << "Modifiers: {";
  for (auto const &modifier : method.modifiers) {
    stream << modifierString.at(modifier) << " ";
  }
  stream << "}\n";
  stream << "ReturnType: ";
  stream << method.returnType;
  stream << "Identifier: " << method.identifier << "\n";
  for (auto const &arg : method.args) {
    stream << "Argument: " << arg;
  }
  return stream << "\n";
}

Constructor::Constructor(std::set<Modifier> modifiers, std::string identifier,
                         std::vector<Type> args,
                         const AST::ConstructorDeclaration *astNode)
    : modifiers(std::move(modifiers)), identifier(std::move(identifier)),
      args(std::move(args)), astNode(astNode) {}

bool Constructor::operator==(const Constructor &constructor) const {
  return identifier == constructor.identifier && args == constructor.args;
}

bool Constructor::operator!=(const Constructor &constructor) const {
  return identifier != constructor.identifier || args != constructor.args;
}

std::ostream &operator<<(std::ostream &stream, const Constructor &constructor) {
  stream << "JOOSCONSTRUCTOR"
         << "\n";
  stream << "Modifiers: {";
  for (auto const &modifier : constructor.modifiers) {
    stream << modifierString.at(modifier) << " ";
  }
  stream << "}\n";
  stream << "Identifier: " << constructor.identifier << "\n";
  for (auto const &arg : constructor.args) {
    stream << "Argument: " << arg;
  }
  return stream << "\n";
}

const std::vector<Field> &TypeBody::getFields() const { return fields; }

bool TypeBody::addField(Field field) {
  for (auto const &field : fields) {
    if (field == field) {
      return false;
    }
  }
  fields.emplace_back(std::move(field));
  return true;
}

const std::vector<Method> &TypeBody::getMethods() const { return methods; }

bool TypeBody::addMethod(Method method) {
  for (auto const &method : methods) {
    if (method == method) {
      return false;
    }
  }
  methods.emplace_back(std::move(method));
  return true;
}

const std::vector<Constructor> &TypeBody::getConstructors() const {
  return constructors;
}

bool TypeBody::addConstructor(Constructor constructor) {
  for (auto const &constructor : constructors) {
    if (constructor == constructor) {
      return false;
    }
  }
  constructors.emplace_back(std::move(constructor));
  return true;
}

void TypeBody::setAbstract() {
  for (auto &method : methods) {
    method.modifiers.emplace(Modifier::Abstract);
  }
}

std::ostream &operator<<(std::ostream &stream, const TypeBody &body) {
  for (auto const &field : body.fields) {
    stream << field;
  }
  for (auto const &method : body.methods) {
    stream << method;
  }
  for (auto const &constructor : body.constructors) {
    stream << constructor;
  }
  return stream << '\n';
}

TypeBodyVisitor::TypeBodyVisitor(const TypeLink &typeLink)
    : typeLink(typeLink) {}

void TypeBodyVisitor::visit(const AST::Start &start) {
  dispatchChildren(start);
}

void TypeBodyVisitor::visit(const AST::ClassDeclaration &decl) {
  dispatchChildren(decl);
}

void TypeBodyVisitor::visit(const AST::InterfaceDeclaration &decl) {
  dispatchChildren(decl);
}

void TypeBodyVisitor::visit(const AST::FieldDeclaration &decl) {
  AST::PropertiesVisitor propertiesVisitor;
  propertiesVisitor.dispatchChildren(decl);

  AST::TypeVisitor typeVisitor(typeLink);
  typeVisitor.dispatchChildren(decl);

  fields.emplace_back(propertiesVisitor.getModifiers(), typeVisitor.getType(),
                      propertiesVisitor.getIdentifier(), &decl);
}

void TypeBodyVisitor::visit(const AST::MethodDeclaration &decl) {
  AST::PropertiesVisitor propertiesVisitor;
  propertiesVisitor.dispatchChildren(decl);

  AST::ArgumentsVisitor argumentsVisitor(typeLink);
  argumentsVisitor.dispatchChildren(decl);

  AST::TypeVisitor typeVisitor(typeLink);
  typeVisitor.dispatchChildren(decl);

  methods.emplace_back(propertiesVisitor.getModifiers(), typeVisitor.getType(),
                       propertiesVisitor.getIdentifier(),
                       argumentsVisitor.getArgs(), &decl);
}

void TypeBodyVisitor::visit(const AST::ConstructorDeclaration &decl) {
  AST::PropertiesVisitor propertiesVisitor;
  propertiesVisitor.dispatchChildren(decl);

  AST::ArgumentsVisitor argumentsVisitor(typeLink);
  argumentsVisitor.dispatchChildren(decl);
  constructors.emplace_back(propertiesVisitor.getModifiers(),
                            propertiesVisitor.getIdentifier(),
                            argumentsVisitor.getArgs(), &decl);
}

std::vector<Field> TypeBodyVisitor::getFields() { return std::move(fields); }

std::vector<Method> TypeBodyVisitor::getMethods() { return std::move(methods); }

std::vector<Constructor> TypeBodyVisitor::getConstructors() {
  return std::move(constructors);
}

} // namespace Env

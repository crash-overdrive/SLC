#include "EnvLocal.hpp"
#include "ASTVisitorUtil.hpp"
#include <optional>

namespace Env {

std::optional<Type> VariableTable::findVariable(const std::string &name) const {
  auto it = variableMap.find(name);
  return (it != variableMap.end()) ? std::make_optional(it->second)
                                   : std::nullopt;
}

bool VariableTable::addVariable(const std::string &name, Type type) {
  if (findVariable(name)) {
    return false;
  }
  variableMap[name] = type;
  return true;
}

std::ostream &operator<<(std::ostream &stream, const VariableTable &table) {
  for (auto const &variable : table.variableMap) {
    stream << "Variable: " << variable.first << " with type" << variable.second
           << "\n";
  }
  return stream;
}

Local::Local(bool log) : log(log) { addVariableTable(); }

std::optional<Type> Local::findVariable(const std::string &name) const {
  for (auto &table : tables) {
    auto type = table.findVariable(name);
    if (type) {
      return type;
    }
  }
  return std::nullopt;
}

bool Local::addVariable(const std::string &name, Type type) {
  if (findVariable(name) || type == TypeKeyword::Void) {
    std::cerr << "ERROR!! Variable: " << name << " with descriptor: " << type
              << " could not be added\n";
    return false;
  }
  tables.back().addVariable(name, type);
  if (log) {
    std::cerr << "Added: Variable: " << name << " with type: " << type << "\n";
  }
  return true;
}

void Local::addVariableTable() {
  tables.emplace_back();
  if (log) {
    std::cerr << "Added: New VariableTable\n";
  }
}

void Local::removeVariableTable() {
  if (log) {
    auto &environment = tables.back();
    std::cerr << "Removed:\n"
              << environment << '\n'
              << "Removed: VariableTable\n";
  }
  tables.pop_back();
}

LocalTrackVisitor::LocalTrackVisitor(const TypeLink &typeLink, bool log)
    : typeLink(typeLink), local(log) {}

void LocalTrackVisitor::visit(const AST::SingleVariableDeclaration &decl) {
  AST::PropertiesVisitor propertiesVisitor;
  propertiesVisitor.dispatchChildren(decl);

  AST::TypeVisitor typeVisitor(typeLink);
  typeVisitor.dispatchChildren(decl);

  if (typeVisitor.isErrorState() ||
      !local.addVariable(propertiesVisitor.getIdentifier(),
                         typeVisitor.getType())) {
    setError();
    return;
  }
}

void LocalTrackVisitor::visit(const AST::SimpleType &simpleType) {
  AST::TypeVisitor typeVisitor(typeLink);
  simpleType.accept(typeVisitor);
  if (typeVisitor.isErrorState()) {
    setError();
    return;
  }
}

void LocalTrackVisitor::visit(const AST::Block &block) {
  local.addVariableTable();
  dispatchChildren(block);
  local.removeVariableTable();
}

const Local &LocalTrackVisitor::getLocal() const { return local; }

} // namespace Env

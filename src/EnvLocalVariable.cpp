#include "EnvLocalVariable.hpp"
#include "ASTVisitorUtil.hpp"

namespace Env {

Type *VariableTable::findVariable(const std::string &name) {
  auto it = variableMap.find(name);
  return (it != variableMap.end()) ? &it->second : nullptr;
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

LocalVariableAnalysis::LocalVariableAnalysis(bool log) : log(log) {
  addVariableTable();
}

Type *LocalVariableAnalysis::findVariable(const std::string &name) {
  for (auto &table : tables) {
    Type *type = table.findVariable(name);
    if (type) {
      return type;
    }
  }
  return nullptr;
}

bool LocalVariableAnalysis::addVariable(const std::string &name, Type type) {
  if (findVariable(name)) {
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

void LocalVariableAnalysis::addVariableTable() {
  tables.emplace_back();
  if (log) {
    std::cerr << "Added: New VariableTable\n";
  }
}

void LocalVariableAnalysis::removeVariableTable() {
  if (log) {
    auto &environment = tables.back();
    std::cerr << "Removed:\n"
              << environment << '\n'
              << "Removed: VariableTable\n";
  }
  tables.pop_back();
}

LocalVariableVisitor::LocalVariableVisitor(const TypeLink &typeLink, bool log)
    : typeLink(typeLink), localVariableAnalysis(LocalVariableAnalysis(log)){};

void LocalVariableVisitor::visit(const AST::SingleVariableDeclaration &decl) {
  AST::PropertiesVisitor propertiesVisitor;
  propertiesVisitor.dispatchChildren(decl);

  AST::TypeVisitor typeVisitor(typeLink);
  typeVisitor.dispatchChildren(decl);

  if (typeVisitor.isErrorState() ||
      !localVariableAnalysis.addVariable(propertiesVisitor.getIdentifier(),
                                         typeVisitor.getType())) {
    setError();
    return;
  }
}

void LocalVariableVisitor::visit(const AST::Block &block) {
  localVariableAnalysis.addVariableTable();
  dispatchChildren(block);
  localVariableAnalysis.removeVariableTable();
}
}; // namespace Env

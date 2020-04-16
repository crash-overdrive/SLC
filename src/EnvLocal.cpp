#include "EnvLocal.hpp"
#include "ASTVisitorUtil.hpp"
#include <utility>

namespace Env {

Variable::Variable(std::string identifier, Type type)
    : identifier(std::move(identifier)), type(std::move(type)) {}

VariableTable::VariableTable(bool args) : args(args) {
  if (args) {
    offset = 12;
  } else {
    offset = -4;
  }
}

std::optional<Variable>
VariableTable::findVariable(const std::string &identifier) const {
  auto it = variableMap.find(identifier);
  return (it != variableMap.end()) ? std::make_optional(it->second)
                                   : std::nullopt;
}

bool VariableTable::addVariable(std::string identifier, Type type) {
  auto [it, flag] = variableMap.emplace(identifier, Variable{identifier, type});
  if (!flag) {
    return false;
  }
  it->second.offset = offset;
  if (args) {
    offset += 4;
  } else {
    offset -= 4;
  }
  return true;
}

VariableTable VariableTable::createNextVariableTable() const {
  VariableTable table;
  if (!args) {
    table.offset = offset;
  }
  return table;
}

std::ostream &operator<<(std::ostream &stream, const VariableTable &table) {
  for (auto const &variable : table.variableMap) {
    stream << "Variable: " << variable.first << " with type"
           << variable.second.identifier << "\n";
  }
  return stream;
}

Local::Local(bool log) : log(log) { tables.emplace_back(true); }

std::optional<Variable>
Local::findVariable(const std::string &identifier) const {
  for (const auto &table : tables) {
    auto variable = table.findVariable(identifier);
    if (variable) {
      return variable;
    }
  }
  return std::nullopt;
}

bool Local::addVariable(std::string identifier, Type type) {
  lastVariable = Variable(identifier, type);
  if (findVariable(identifier) || type == TypeKeyword::Void) {
    std::cerr << "ERROR!! Variable: " << identifier
              << " with descriptor: " << type << " could not be added\n";
    return false;
  }
  if (log) {
    std::cerr << "Added: Variable: " << identifier << " with type: " << type
              << "\n";
  }
  tables.back().addVariable(std::move(identifier), type);
  return true;
}

void Local::addVariableTable() {
  tables.emplace_back(tables.back().createNextVariableTable());
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

const Variable &Local::getLastVariable() const { return lastVariable; }

LocalTrackVisitor::LocalTrackVisitor(const TypeLink &typeLink, bool log)
    : typeLink(typeLink), local(log) {}

void LocalTrackVisitor::visit(const AST::SingleVariableDeclaration &decl) {
  AST::DeclarationVisitor visitor(typeLink);
  decl.accept(visitor);

  if (visitor.isErrorState() ||
      !local.addVariable(visitor.getIdentifier(), visitor.getType())) {
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

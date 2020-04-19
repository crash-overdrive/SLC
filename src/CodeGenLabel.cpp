#include "CodeGenLabel.hpp"
#include "EnvTypeDeclaration.hpp"
#include <sstream>

namespace CodeGen {

LabelGenerator::LabelGenerator(std::vector<std::string> path)
    : path(std::move(path)) {}

void LabelGenerator::generateDeclaration(Env::TypeDeclaration &decl) {
  for (auto &method : decl.body.getMethods()) {
    generateMethod(method);
  }
  for (auto &constructor : decl.body.getConstructors()) {
    generateConstructor(constructor);
  }
  generateVTable(decl.contain);
  generateSelector(decl.contain);
}

// a general comment on the structure of the labels
/*
In order to encompass all potential names, we need an escape key
Just like how the combination of quotation marks and escape sequence allows us
to express the sum of all strings Having a dedicated delimiter/escape sequence
allows us to avoid collisions
*/

// Underscore that does not denote member reference always comes in pairs;
// a pair of underscore implies it is directly from the string of the class
// name, where as a single underscore reliably indicates memeber relationship
std::string sanitizedLabelName(const std::string &reference, char esc = '_') {
  std::string lname = "";
  for (unsigned i = 0; i < reference.length(); i++) {
    if (reference[i] == esc) {
      lname += esc;
    }
    lname += reference[i];
  }
  return lname;
}

void LabelGenerator::generateMethod(Env::Method &method) {
  std::ostringstream str;
  str << method.offset;

  std::string fullName;
  for (const auto &name : path) {
    fullName += name;
    fullName += '.';
  }
  std::string className = sanitizedLabelName(fullName);
  std::string offset = str.str();

  method.label = className + method.identifier + '_' + offset;
}

void LabelGenerator::generateConstructor(Env::Constructor &constructor) {
  std::string fullName;
  for (const auto &name : path) {
    fullName += name;
    fullName += '.';
  }
  std::string className = sanitizedLabelName(fullName);
  std::string argChain = "";
  for (const auto &arg : constructor.args) {
    std::string argName;
    if (arg.keyword == Env::TypeKeyword::Simple) {
      argName = arg.declare->identifier;
    } else {
      argName = Env::typeKeywordString.at(arg.keyword);
    }
    if (arg.isArray) {
      argName += "Array";
    }
    std::string sargs = sanitizedLabelName(argName);
    argChain += '_' + sargs;
  }

  constructor.label = className + constructor.identifier + argChain;
}

void LabelGenerator::generateVTable(Env::TypeContain &contain) {
  std::string fullName;
  for (const auto &name : path) {
    fullName += name;
    fullName += '.';
  }
  fullName += "$.vTable";
  contain.vtablelabel = fullName;
}

void LabelGenerator::generateSelector(Env::TypeContain &contain) {
  std::string fullName;
  for (const auto &name : path) {
    fullName += name;
    fullName += '.';
  }
  fullName += "$.selector";
  contain.selectorlabel = fullName;
}

} // namespace CodeGen

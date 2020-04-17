#include "CodeGenLabel.hpp"

namespace CodeGen {

LabelGenerator::LabelGenerator(std::vector<std::string> path)
    : path(std::move(path)) {}

void LabelGenerator::generateMethod(Env::Method &method) {
  method.label = path.at(0) + '.' + method.identifier;
}

} // namespace CodeGen

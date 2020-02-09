#ifndef WEEDER_HPP
#define WEEDER_HPP

#include "ParseTree.hpp"

namespace Weed {

bool FileNameCompare(const Parse::Tree &Tree, const std::string &Name);

bool AbstractFinalClass(const Parse::Tree &Tree);

typedef bool(*Check)(const Parse::Tree &Tree);
extern const std::vector<Check> JoosChecks;

} // namespace Parse

#endif // WEEDER_HPP

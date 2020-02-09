#ifndef WEEDER_HPP
#define WEEDER_HPP

#include "ParseTree.hpp"

namespace Weed {

bool FileNameCompare(Parse::Tree &T, const std::string &Name);

bool AbstractFinalClass(Parse::Tree &T);

typedef bool(*Check)(Parse::Tree &T);
extern const std::vector<Check> JoosChecks;

} // namespace Parse

#endif // WEEDER_HPP

#ifndef WEEDER_HPP
#define WEEDER_HPP

#include "ParseTree.hpp"

namespace Weed {

bool FileNameCompare(Parse::Tree &T, const std::string &Name);

bool AbstractFinalClass(const Parse::Tree &T);

typedef bool (*Check)(const Parse::Tree &T);
extern const std::vector<Check> JoosChecks;

} // namespace Weed

#endif // WEEDER_HPP

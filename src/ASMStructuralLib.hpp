#ifndef ASM_STRUCTURAL_LIB_H
#define ASM_STRUCTUAL_LIB_H

namespace ASM{

class AnonymousLabelService;

class StructuralLib {
	const AnonymousLabelService& label;
public:
	StructuralLib(const AnonymousLabelService& label);
	std::string IfThenElse(const std::string& condition, const std::string& s1, const std::string& s2);
	std::string WhileLoop(const std::string& condition, const std::string& statement);
	std::string ForLoop(const std::string& init, const std::string& condition, const std::string& increment, const std::string& statement);
};


}

#endif

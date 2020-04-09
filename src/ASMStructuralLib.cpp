#include "ASMStructuralLib.hpp"
#include "ASMServices.hpp"

namespace ASM{

StructuralLib::StructuralLib(AnonymousLabelService& label): label(label){}

std::string StructuralLib::IfThenElse(const std::string& condition, const std::string& s1, const std::string& s2){
	std::string s2label = label.getUniqueLabel();
	std::string end = label.getUniqueLabel();
	std::string cmpNjmp = "cmp eax, 0; \nje " + s2label + "\n";
	std::string jmp = "jmp " + end + "\n";
	return condition + cmpNjmp + s1 + jmp + s2label + ":\n" + s2 + end + ":\n";
}	

std::string StructuralLib::WhileLoop(const std::string& condition, const std::string& statement){
	std::string beg = label.getUniqueLabel();
	std::string end = label.getUniqueLabel();

	std::string cmpNjmp = "cmp eax, 0;\nje " + end + "\n";
	std::string jmp = "jmp " + beg + "\n";

	return beg + ":\n" + condition + cmpNjmp + statement + jmp + end + ":\n";
}

std::string StructuralLib::ForLoop(const std::string& init, const std::string& condition, const std::string& increment, const std::string& statement){
	std::string beg = label.getUniqueLabel();
	std::string end = label.getUniqueLabel();

	std::string cmpNjmp = "cmp eax, 0; \n je " + end + "\n";
	std::string jmp = "jmp " + beg + "\n";

	
	return init + beg + ":\n" + condition + cmpNjmp + statement + increment + jmp + end + ":\n";
}

}

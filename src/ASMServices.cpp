#include "ASMServices.hpp"
#include <stringstream>

namespace ASM{

	AnonymousLabelService::AnonyMouseLabelService(const std::string& prefix):prefix(prefix), instanceCount(0){
		
	}

	int getUniqueId(){
		return instanceCount++;
	}

	std::string getUniqueLabel(){
		std::ostringstream str;
		str << prefix << instanceCount++;
		return str.str();
	}

}

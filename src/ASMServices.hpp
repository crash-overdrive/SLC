#ifndef ASM_SERVICES_H
#define ASM_SERVICES_H

namespace ASM{

class AnonymousLabelService{
	const std::string prefix;
	int instanceCount;
	public:
	AnonymousLabelService(const std::string& prefix = ".L");
	int getUniqueId();
	std::string getUniqueLabel();
};

}
#endif

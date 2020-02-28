#pragma once

#include <map>

namespace type {

	//string identifier in this namespace are performed with fully qualified names, in order for us to link them correctly to the class types
	struct fieldType{
		std::string returnType;
		std::vector<std::string> args;
		bool isFunc;
	};


	struct classType{
		std::string name;
		std::map<std::string, std::vector<fieldType>> fields;
		std::string parent;
		std::vector<std::string> interfaces;
	};

	
	struct interfaceType{
		std::string name;
		std::map<std::string, std::vector<fieldType>> fields;
	}

}

#pragma once

#include <string>
#include <map>

namespace ENV{
	class Scope {
		public:
		enum Type {
			GLOBAL,
			PACKAGE,
			CLASS,
			INTERFACE,
			//optional if we want to extend to in class parsing
			VARIABLE,
			METHOD,
		};

		private:

		std::string name;
		//can change this to inheritance, but with significantly more coupling
		Type type;
		std::map<std::string, Scope> children;
		//std::map is exclusionary, so we will need a way to distinguish
		//optional for reverse lookup resolution
		Scope* parent;
		
		public:
		Scope();
		Scope(const std::string& name, Type type, Scope* parent);
		bool insertChildren(const std::string& key, Type type);
		bool contains(const std::string& key);
		Scope& operator[](const std::string& key);
		const std::string getName();
		const std::string getFullyQualifiedName();
		~Scope();
	};
}

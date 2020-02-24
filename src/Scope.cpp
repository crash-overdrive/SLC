#include "Scope.hpp"

Scope::Scope(){}

Scope::Scope(const std::string& name, Scope::Type type, Scope* parent): name(name), type(type), parent(parent){}

bool Scope::insertChildren(const std::string& key, Scope::Type type){
	std::pair<std::map<std::string, Scope>::iterator, bool> ret = children.emplace(std::make_pair(key, Scope(key, type, this)));
	return ret.second;
}

bool Scope::contains(const std::string& key){
	return children.count(key);
}

Scope& Scope::operator[](const std::string& key){
	return children[key];
}

const std::string Scope::getName(){
	return name;
}

const std::string Scope::getFullyQualifiedName(){
	if(!parent || parent->type == Type::GLOBAL)
		return name;
	return parent->getFullyQualifiedName() + "." + name;
}

Scope::~Scope(){

}

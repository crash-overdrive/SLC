#include "ENV::Scope.hpp"

ENV::Scope::Scope(){}

ENV::Scope::Scope(const std::string& name, ENV::Scope::Type type, ENV::Scope* parent): name(name), type(type), parent(parent){}

bool ENV::Scope::insertChildren(const std::string& key, ENV::Scope::Type type){
	std::pair<std::map<std::string, ENV::Scope>::iterator, bool> ret = children.emplace(std::make_pair(key, ENV::Scope(key, type, this)));
	return ret.second;
}

bool ENV::Scope::contains(const std::string& key){
	return children.count(key);
}

ENV::Scope& ENV::Scope::operator[](const std::string& key){
	return children[key];
}

const std::string ENV::Scope::getName(){
	return name;
}

const std::string ENV::Scope::getFullyQualifiedName(){
	if(!parent || parent->type == Type::GLOBAL)
		return name;
	return parent->getFullyQualifiedName() + "." + name;
}

ENV::Scope::~Scope(){

}

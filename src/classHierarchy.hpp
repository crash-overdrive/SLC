#pragma once
#include "dataType.hpp"
#include "Hierarchy.hpp"
#include <map>

namespace Env{
	class Scope;
}

//need this for name resolutions
//class TypeLink;

namespace type{
	class ClassHierarchyBuilder : public HierarchicalVisitor {
		//data structures for storing our results
		//we do not prevent class/interface clashes, that should be up to the scope pass
		//in an ideal world, i'd appreciate the use of 
		std::map<std::string, classType> classes;
		std::map<std::string, interfaceType> interface;	

		//to be implemented with TypeLink
		std::string findFullName();

		Env::Scope *rootScope;

		//stateful stuff, which I need to keep because of the lack of control over the ast visitor pattern
		classType* curClass;
		interfaceType* curInterface;

		public:

		ClassHierarchyBuillder(Env::Scope *root);

		//need this for resolving the fully qualified name for the present class
		virtual void visit(const AST::PackageDeclaration &Dec) override;
		//in case i cannot receive typed linking from external sources
		virtual void visit(const AST::SingleImportDeclaration &Dec) override;
		virtual void visit(const AST::DemandImportDeclaration &Dec) override;
		//the actual class declaration
		virtual void visit(const AST::ClassDeclaration &Dec) override;
		virtual void visit(const AST::InterfaceDeclaration &Dec) override;
		virtual void visit(const AST::MethodDeclaration &Dec) override;
		virtual void visit(const AST::FieldDeclaration &Dec) override;
		//it is unproductive to venture beyond field or method, as there are no structural reference to speak of
		~ClassHierarchyBuilder();
	};
}

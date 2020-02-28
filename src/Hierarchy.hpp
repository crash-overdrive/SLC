#pragma once
#include "ASTVisitor.hpp"

namespace type{
	
	class HierarchicalVisitor : public Visitor{
		virtual void visit(const Start &Start);
		virtual void visit(const PackageDeclaration &Decl);
		virtual void visit(const SingleImportDeclaration &Decl);
		virtual void visit(const DemandImportDeclaration &Decl);
		virtual void visit(const ClassDeclaration &Decl);
		virtual void visit(const InterfaceDeclaration &Decl);
		virtual void visit(const FieldDeclaration &Decl);
		virtual void visit(const MethodDeclaration &Decl);
		virtual void visit(const Modifier &Modifier);
		virtual void visit(const Identifier &Identifier);
		virtual void preHook(const Node &node);
		virtual void posHook(const Node &node);
		virtual ~HierachicalVisitor();
	};
}

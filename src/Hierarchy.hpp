#pragma once

#include "ASTVisitor.hpp"

namespace type{
	
	class HierarchicalVisitor : public AST::Visitor{
		virtual void visit(const AST::Start &Start);
		virtual void visit(const AST::PackageDeclaration &Decl);
		virtual void visit(const AST::SingleImportDeclaration &Decl);
		virtual void visit(const AST::DemandImportDeclaration &Decl);
		virtual void visit(const AST::ClassDeclaration &Decl);
		virtual void visit(const AST::InterfaceDeclaration &Decl);
		virtual void visit(const AST::FieldDeclaration &Decl);
		virtual void visit(const AST::MethodDeclaration &Decl);
		virtual void visit(const AST::Modifier &Modifier);
		virtual void visit(const AST::Identifier &Identifier);
		virtual void preHook(const AST::Node &node);
		virtual void postHook(const AST::Node &node);
		virtual ~HierarchicalVisitor();
	};
}

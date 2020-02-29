#include "Hierarchy.hpp"

void type::HierarchicalVisitor::visit(const AST::Start &Start){
	preHook(Start);
	(void)Start;
	postHook(Start);
}

void type::HierarchicalVisitor::visit(const AST::PackageDeclaration &Decl){
	preHook(Decl);
	(void)Decl;
	postHook(Decl);
}

void type::HierarchicalVisitor::visit(const AST::SingleImportDeclaration &Decl){
	preHook(Decl);
	(void)Decl;
	postHook(Decl);
}

void type::HierarchicalVisitor::visit(const AST::DemandImportDeclaration &Decl){
	preHook(Decl);
	(void)Decl;
	postHook(Decl);
}

void type::HierarchicalVisitor::visit(const AST::ClassDeclaration &Decl){
	preHook(Decl);
	(void)Decl;
	postHook(Decl);
}

void type::HierarchicalVisitor::visit(const AST::InterfaceDeclaration &Decl){
	preHook(Decl);
	(void)Decl;
	postHook(Decl);
}

void type::HierarchicalVisitor::visit(const AST::FieldDeclaration &Decl){
	preHook(Decl);
	(void)Decl;
	postHook(Decl);
}

void type::HierarchicalVisitor::visit(const AST::MethodDeclaration &Decl){
	preHook(Decl);
	(void)Decl;
	postHook(Decl);
}

void type::HierarchicalVisitor::visit(const AST::Modifier &Modifier){
	preHook(Modifier);
	(void)Modifier;
	postHook(Modifier);
}

void type::HierarchicalVisitor::visit(const AST::Identifier &Identifier){
	preHook(Identifier);
	(void)Identifier;
	postHook(Identifier);
}

void type::HierarchicalVisitor::preHook(const AST::Node &node){
	(void)node;
}

void type::HierarchicalVisitor::postHook(const AST::Node& node){
	for (const auto &child : node.getChildren()){
		child->accept(*this);
	}
}

type::HierarchicalVisitor::~HierarchicalVisitor(){}

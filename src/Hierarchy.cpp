#include "hierachy.hpp"

void type::HierarchicalVisitor::visit(const Start &Start){preHook();postHook();}

void type::HierarchicalVisitor::visit(const PackageDeclaration &Decl){preHook();postHook();}

void type::HierarchicalVisitor::visit(const SingleImportDeclaration &Decl){preHook();postHook();}

void type::HierarchicalVisitor::visit(const DemandImportDeclaration &Decl){preHook();postHook();}

void type::HierarchicalVisitor::visit(const ClassDeclaration &Decl){preHook();postHook();}

void type::HierarchicalVisitor::visit(const InterfaceDeclaration &Decl){preHook();postHook();}

void type::HierarchicalVisitor::visit(const FieldDeclaration &Decl){preHook();postHook();}

void type::HierarchicalVisitor::visit(const MethodDeclaration &Decl){preHook();postHook();}

void type::HierarchicalVisitor::visit(const Modifier &Modifier){preHook();postHook();}

void type::HierarchicalVisitor::visit(const Identifier &Identifier){preHook();postHook();}

void type::HierarchicalVisitor::preHook(const Node &node){
}

void type::HierarchicalVisitor::postHook(const Node& node){
	for (const child : node->getCHildren()){
		child->accept(*this);
	}
}

type::HierarchicalVisitor::~HierarchicalVisitor(){}

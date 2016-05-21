/* File: ast_decl.cc
 * -----------------
 * Implementation of Decl node classes.
 */
#include "ast_decl.h"
#include "ast_type.h"
#include "ast_stmt.h"
#include "symtable.h"
#include "irgen.h"

Decl::Decl (Identifier *n) : Node (*n->GetLocation ()) {
  Assert(n != NULL);
  (id = n)->SetParent (this);
}

VarDecl::VarDecl (Identifier *n, Type *t, Expr *e) : Decl (n) {
  Assert(n != NULL && t != NULL);
  (type = t)->SetParent (this);
  if(e) { (assignTo = e)->SetParent (this); }
  typeq = NULL;
}

VarDecl::VarDecl (Identifier *n, TypeQualifier *tq, Expr *e) : Decl (n) {
  Assert(n != NULL && tq != NULL);
  (typeq = tq)->SetParent (this);
  if(e) { (assignTo = e)->SetParent (this); }
  type = NULL;
}

VarDecl::VarDecl (Identifier *n, Type *t, TypeQualifier *tq, Expr *e) : Decl (n) {
  Assert(n != NULL && t != NULL && tq != NULL);
  (type = t)->SetParent (this);
  (typeq = tq)->SetParent (this);
  if(e) { (assignTo = e)->SetParent (this); }
}

void VarDecl::PrintChildren (int indentLevel) {
  if(typeq) { typeq->Print (indentLevel + 1); }
  if(type) { type->Print (indentLevel + 1); }
  if(id) { id->Print (indentLevel + 1); }
  if(assignTo) { assignTo->Print (indentLevel + 1, "(initializer) "); }
}

llvm::Value *VarDecl::Emit () {
  llvm::Value *value = NULL;
  char *name;
  bool isConstant;
  llvm::Constant *constant;
  llvm::GlobalVariable *InsertBefore = NULL;

  if(Node::symtable->symTable.empty()) {
    map<Decl*, DeclAssoc> newMap;

    // gets llvm type
    llvm::Type *type = Node::irgen->Converter(this->type);

    // sets the constant
    if(this->assignTo != NULL) {
      constant = llvm::cast<llvm::Constant>(this->assignTo->Emit());
    }
    else {
      constant = llvm::Constant::getNullValue(type);
    }

    if(this->typeq->constTypeQualifier != NULL) {
      isConstant = true;
    }
    else {
      isConstant = false;
    }

    name = this->GetIdentifier()->GetName();

    llvm::Module *mod = irgen->GetOrCreateModule("irgen.bc");

    new llvm::GlobalVariable(*mod, type, isConstant, llvm::GlobalValue::ExternalLinkage, constant, name);
  }
  
  if(this->typeq->constTypeQualifier != NULL) {
    isConstant = true;
  }
  else {
    isConstant = false;
  }

  return value;  
}

FnDecl::FnDecl (Identifier *n, Type *r, List<VarDecl *> *d) : Decl (n) {
  Assert(n != NULL && r != NULL && d != NULL);
  (returnType = r)->SetParent (this);
  (formals = d)->SetParentAll (this);
  body = NULL;
  returnTypeq = NULL;
}

FnDecl::FnDecl (Identifier *n, Type *r, TypeQualifier *rq, List<VarDecl *> *d) : Decl (n) {
  Assert(n != NULL && r != NULL && rq != NULL && d != NULL);
  (returnType = r)->SetParent (this);
  (returnTypeq = rq)->SetParent (this);
  (formals = d)->SetParentAll (this);
  body = NULL;
}

void FnDecl::SetFunctionBody (Stmt *b) {
  (body = b)->SetParent (this);
}

void FnDecl::PrintChildren (int indentLevel) {
  if(returnType) { returnType->Print (indentLevel + 1, "(return type) "); }
  if(id) { id->Print (indentLevel + 1); }
  if(formals) { formals->PrintAll (indentLevel + 1, "(formals) "); }
  if(body) { body->Print (indentLevel + 1, "(body) "); }
}

llvm::Value *FnDecl::Emit () {
  // TODO
}

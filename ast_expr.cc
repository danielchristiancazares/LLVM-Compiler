/* File: ast_expr.cc
 * -----------------
 * Implementation of expression node classes.
 */

#include <string.h>
#include "ast_expr.h"
#include "ast_type.h"
#include "ast_decl.h"
#include "symtable.h"

IntConstant::IntConstant (yyltype loc, int val) : Expr (loc) {
  value = val;
}
void IntConstant::PrintChildren (int indentLevel) {
  printf ("%d", value);
}

llvm::Value *IntConstant::Emit () {
  return llvm::ConstantInt::get (irgen->GetIntType (), value);
}

FloatConstant::FloatConstant (yyltype loc, double val) : Expr (loc) {
  value = val;
}
void FloatConstant::PrintChildren (int indentLevel) {
  printf ("%g", value);
}

llvm::Value *FloatConstant::Emit () {
  return llvm::ConstantFP::get (irgen->GetFloatType (), value);
}

BoolConstant::BoolConstant (yyltype loc, bool val) : Expr (loc) {
  value = val;
}
void BoolConstant::PrintChildren (int indentLevel) {
  printf ("%s", value ? "true" : "false");
}

llvm::Value *BoolConstant::Emit () {
  return llvm::ConstantInt::get (irgen->GetBoolType (), value);
}

VarExpr::VarExpr (yyltype loc, Identifier *ident) : Expr (loc) {
  Assert(ident != NULL);
  this->id = ident;
}

void VarExpr::PrintChildren (int indentLevel) {
  id->Print (indentLevel + 1);
}

llvm::Value *VarExpr::Emit () {
  llvm::Value *value = NULL;
  for (std::vector< map<string, DeclAssoc> >::iterator it = symtable->symTable.end(); it != symtable->symTable.begin() || value != NULL; --it) {
    value = it->at(this->GetIdentifier ()->GetName ()).second.value;
  }
  llvm::Twine *twine = new llvm::Twine(this->GetIdentifier ()->GetName ());
  return new llvm::LoadInst(mem, *twine, irgen->GetBasicBlock());
}

Operator::Operator (yyltype loc, const char *tok) : Node (loc) {
  Assert(tok != NULL);
  strncpy (tokenString, tok, sizeof (tokenString));
}

void Operator::PrintChildren (int indentLevel) {
  printf ("%s", tokenString);
}

bool Operator::IsOp (const char *op) const {
  return strcmp (tokenString, op) == 0;
}

CompoundExpr::CompoundExpr (Expr *l, Operator *o, Expr *r)
    : Expr (Join (l->GetLocation (), r->GetLocation ())) {
  Assert(l != NULL && o != NULL && r != NULL);
  (op = o)->SetParent (this);
  (left = l)->SetParent (this);
  (right = r)->SetParent (this);
}

CompoundExpr::CompoundExpr (Operator *o, Expr *r)
    : Expr (Join (o->GetLocation (), r->GetLocation ())) {
  Assert(o != NULL && r != NULL);
  left = NULL;
  (op = o)->SetParent (this);
  (right = r)->SetParent (this);
}

CompoundExpr::CompoundExpr (Expr *l, Operator *o)
    : Expr (Join (l->GetLocation (), o->GetLocation ())) {
  Assert(l != NULL && o != NULL);
  (left = l)->SetParent (this);
  (op = o)->SetParent (this);
}

void CompoundExpr::PrintChildren (int indentLevel) {
  if(left) { left->Print (indentLevel + 1); }
  op->Print (indentLevel + 1);
  if(right) { right->Print (indentLevel + 1); }
}

ConditionalExpr::ConditionalExpr (Expr *c, Expr *t, Expr *f)
    : Expr (Join (c->GetLocation (), f->GetLocation ())) {
  Assert(c != NULL && t != NULL && f != NULL);
  (cond = c)->SetParent (this);
  (trueExpr = t)->SetParent (this);
  (falseExpr = f)->SetParent (this);
}

void ConditionalExpr::PrintChildren (int indentLevel) {
  cond->Print (indentLevel + 1, "(cond) ");
  trueExpr->Print (indentLevel + 1, "(true) ");
  falseExpr->Print (indentLevel + 1, "(false) ");
}
ArrayAccess::ArrayAccess (yyltype loc, Expr *b, Expr *s) : LValue (loc) {
  (base = b)->SetParent (this);
  (subscript = s)->SetParent (this);
}

llvm::Value *ArithmeticExpr::Emit () {
  // TODO This is going to be such a pain in the butt
}

void ArrayAccess::PrintChildren (int indentLevel) {
  base->Print (indentLevel + 1);
  subscript->Print (indentLevel + 1, "(subscript) ");
}

FieldAccess::FieldAccess (Expr *b, Identifier *f)
    : LValue (b ? Join (b->GetLocation (), f->GetLocation ()) : *f->GetLocation ()) {
  Assert(f != NULL); // b can be be NULL (just means no explicit base)
  base = b;
  if(base) { base->SetParent (this); }
  (field = f)->SetParent (this);
}

void FieldAccess::PrintChildren (int indentLevel) {
  if(base) { base->Print (indentLevel + 1); }
  field->Print (indentLevel + 1);
}

Call::Call (yyltype loc, Expr *b, Identifier *f, List<Expr *> *a) : Expr (loc) {
  Assert(f != NULL && a != NULL); // b can be be NULL (just means no explicit base)
  base = b;
  if(base) { base->SetParent (this); }
  (field = f)->SetParent (this);
  (actuals = a)->SetParentAll (this);
}

void Call::PrintChildren (int indentLevel) {
  if(base) { base->Print (indentLevel + 1); }
  if(field) { field->Print (indentLevel + 1); }
  if(actuals) { actuals->PrintAll (indentLevel + 1, "(actuals) "); }
}

llvm::Value *EqualityExpr::Emit () {
  llvm::Value *l = left->Emit ();
  llvm::Value *r = right->Emit ();
  llvm::Type *ty = l->getType ();

  // TODO Unsure of what to do from here...
}

llvm::Value *LogicalExpr::Emit () {
  llvm::Value *l = left->Emit ();
  llvm::Value *r = right->Emit ();
  llvm::Value *retVal = NULL;

  // TODO Some sort of comparison depending on whether it's AND or OR

  return retVal;
}

llvm::Value *AssignExpr::Emit () {
  // TODO This is the worst.
  // Check if it's accessing a field or a variable then either swizzle
  // or perform the assignment which may be =, +=, *=, etc
}

llvm::Value *PostfixExpr::Emit () {
  // TODO Help
}

llvm::Value *FieldAccess::Emit () {
  // TODO Help
}

llvm::Value *RelationalExpr::Emit () {

}

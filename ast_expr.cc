/* File: ast_expr.cc
 * -----------------
 * Implementation of expression node classes.
 */

#include <string.h>
#include "ast_expr.h"
#include "ast_type.h"
#include "ast_decl.h"
#include "symtable.h"

IntConstant::IntConstant(yyltype loc, int val) : Expr(loc) {
  value = val;
}
void IntConstant::PrintChildren(int indentLevel) {
  printf("%d", value);
}

llvm::Value *IntConstant::Emit() {
  return llvm::ConstantInt::get(irgen->GetIntType(), value);
}

FloatConstant::FloatConstant(yyltype loc, double val) : Expr(loc) {
  value = val;
}
void FloatConstant::PrintChildren(int indentLevel) {
  printf("%g", value);
}

llvm::Value *FloatConstant::Emit() {
  return llvm::ConstantFP::get(irgen->GetFloatType(), value);
}

BoolConstant::BoolConstant(yyltype loc, bool val) : Expr(loc) {
  value = val;
}
void BoolConstant::PrintChildren(int indentLevel) {
  printf("%s", value ? "true" : "false");
}

llvm::Value *BoolConstant::Emit() {
  return llvm::ConstantInt::get(irgen->GetBoolType(), value);
}

VarExpr::VarExpr(yyltype loc, Identifier *ident) : Expr(loc) {
  Assert(ident != NULL);
  this->id = ident;
}

void VarExpr::PrintChildren(int indentLevel) {
  id->Print(indentLevel + 1);
}

llvm::Value *VarExpr::Emit() {
  llvm::Value *value = NULL;

  vector < map < string, SymbolTable::DeclAssoc > > ::reverse_iterator
  it = this->symtable->symTable.rbegin();
  for (; it != this->symtable->symTable.rend() && value == NULL; ++it) {
    value = it->at(this->GetIdentifier()->GetName()).value;
  }
  llvm::Twine *twine = new llvm::Twine(this->GetIdentifier()->GetName());
  return new llvm::LoadInst(value, *twine, irgen->GetBasicBlock());
}

Operator::Operator(yyltype loc, const char *tok) : Node(loc) {
  Assert(tok != NULL);
  strncpy(tokenString, tok, sizeof(tokenString));
}

void Operator::PrintChildren(int indentLevel) {
  printf("%s", tokenString);
}

bool Operator::IsOp(const char *op) const {
  return strcmp(tokenString, op) == 0;
}

CompoundExpr::CompoundExpr(Expr *l, Operator *o, Expr *r)
    : Expr(Join(l->GetLocation(), r->GetLocation())) {
  Assert(l != NULL && o != NULL && r != NULL);
  (op = o)->SetParent(this);
  (left = l)->SetParent(this);
  (right = r)->SetParent(this);
}

CompoundExpr::CompoundExpr(Operator *o, Expr *r)
    : Expr(Join(o->GetLocation(), r->GetLocation())) {
  Assert(o != NULL && r != NULL);
  left = NULL;
  (op = o)->SetParent(this);
  (right = r)->SetParent(this);
}

CompoundExpr::CompoundExpr(Expr *l, Operator *o)
    : Expr(Join(l->GetLocation(), o->GetLocation())) {
  Assert(l != NULL && o != NULL);
  (left = l)->SetParent(this);
  (op = o)->SetParent(this);
}

void CompoundExpr::PrintChildren(int indentLevel) {
  if(left) { left->Print(indentLevel + 1); }
  op->Print(indentLevel + 1);
  if(right) { right->Print(indentLevel + 1); }
}

ConditionalExpr::ConditionalExpr(Expr *c, Expr *t, Expr *f)
    : Expr(Join(c->GetLocation(), f->GetLocation())) {
  Assert(c != NULL && t != NULL && f != NULL);
  (cond = c)->SetParent(this);
  (trueExpr = t)->SetParent(this);
  (falseExpr = f)->SetParent(this);
}

void ConditionalExpr::PrintChildren(int indentLevel) {
  cond->Print(indentLevel + 1, "(cond) ");
  trueExpr->Print(indentLevel + 1, "(true) ");
  falseExpr->Print(indentLevel + 1, "(false) ");
}
ArrayAccess::ArrayAccess(yyltype loc, Expr *b, Expr *s) : LValue(loc) {
  (base = b)->SetParent(this);
  (subscript = s)->SetParent(this);
}

llvm::Value *ArithmeticExpr::Emit() {
  // TODO This is going to be such a pain in the butt
}

void ArrayAccess::PrintChildren(int indentLevel) {
  base->Print(indentLevel + 1);
  subscript->Print(indentLevel + 1, "(subscript) ");
}

FieldAccess::FieldAccess(Expr *b, Identifier *f)
    : LValue(b ? Join(b->GetLocation(), f->GetLocation()) : *f->GetLocation()) {
  Assert(f != NULL); // b can be be NULL (just means no explicit base)
  base = b;
  if(base) { base->SetParent(this); }
  (field = f)->SetParent(this);
}

void FieldAccess::PrintChildren(int indentLevel) {
  if(base) { base->Print(indentLevel + 1); }
  field->Print(indentLevel + 1);
}

Call::Call(yyltype loc, Expr *b, Identifier *f, List<Expr *> *a) : Expr(loc) {
  Assert(f != NULL && a != NULL); // b can be be NULL (just means no explicit base)
  base = b;
  if(base) { base->SetParent(this); }
  (field = f)->SetParent(this);
  (actuals = a)->SetParentAll(this);
}

void Call::PrintChildren(int indentLevel) {
  if(base) { base->Print(indentLevel + 1); }
  if(field) { field->Print(indentLevel + 1); }
  if(actuals) { actuals->PrintAll(indentLevel + 1, "(actuals) "); }
}

llvm::Value *EqualityExpr::Emit() {
  llvm::Value *lhs = left->Emit();
  llvm::Value *rhs = right->Emit();
  llvm::Type *type = lhs->getType();

  if(type == irgen->GetIntType()) {
    if(this->op->IsOp("==")) {
      return llvm::CmpInst::Create(llvm::CmpInst::ICmp, llvm::ICmpInst::ICMP_EQ, lhs, rhs, "", irgen->GetBasicBlock());
    } else if(this->op->IsOp("!=")) {
      return llvm::CmpInst::Create(llvm::CmpInst::ICmp, llvm::ICmpInst::ICMP_NE, lhs, rhs, "", irgen->GetBasicBlock());
    }
  } else if(type == irgen->GetFloatType()) {
    if(this->op->IsOp("==")) {
      return llvm::CmpInst::Create(llvm::CmpInst::FCmp, llvm::CmpInst::FCMP_OEQ, lhs, rhs, "", irgen->GetBasicBlock());
    } else if(this->op->IsOp("!=")) {
      return llvm::CmpInst::Create(llvm::CmpInst::FCmp, llvm::CmpInst::FCMP_ONE, lhs, rhs, "", irgen->GetBasicBlock());
    }
  }
  return NULL;
}

llvm::Value *LogicalExpr::Emit() {
  llvm::Value *lhs = left->Emit();
  llvm::Value *rhs = right->Emit();

  llvm::ConstantInt *lhsBool = (llvm::ConstantInt *) lhs;
  llvm::ConstantInt *rhsBool = (llvm::ConstantInt *) rhs;

  if(this->op->IsOp("&&")) {
    if(lhsBool->isOne() && rhsBool->isOne()) {
      return lhs;
    } else if(rhsBool->isZero()) {
      return rhs;
    } else {
      return lhs;
    }
  } else if(this->op->IsOp("||")) {
    if(lhsBool->isOne()) {
      return lhs;
    } else {
      return rhs;
    }
  }
  return NULL;
}

llvm::Value *AssignExpr::Emit() {
  VarExpr *lhsVar = dynamic_cast<VarExpr *>(left);
  llvm::Value *rhs = right->Emit();

  llvm::Value *lhs = NULL;
  vector < map < string, SymbolTable::DeclAssoc > > ::reverse_iterator
  it = this->symtable->symTable.rbegin();
  for (; it != this->symtable->symTable.rend() && lhs == NULL; ++it) {
    lhs = it->at(lhsVar->GetIdentifier()->GetName()).value;
  }

  if(this->op->IsOp("=")) {
    return new llvm::StoreInst(rhs, lhs, irgen->GetBasicBlock());
  } else if(this->op->IsOp("*=")) {
    llvm::Type *type = rhs->getType();
    llvm::Value *rhsMult = NULL;
    if(type == irgen->GetFloatType()) {
      rhsMult = llvm::BinaryOperator::CreateFMul(left->Emit(), right->Emit(), "", irgen->GetBasicBlock());
    } else {
      rhsMult = llvm::BinaryOperator::CreateMul(left->Emit(), right->Emit(), "", irgen->GetBasicBlock());
    }
    return new llvm::StoreInst(rhsMult, lhs, irgen->GetBasicBlock());
  } else if(this->op->IsOp("+=")) {
    llvm::Type *type = rhs->getType();
    llvm::Value *rhsAdd = NULL;
    if(type == irgen->GetFloatType()) {
      rhsAdd = llvm::BinaryOperator::CreateFAdd(left->Emit(), right->Emit(), "", irgen->GetBasicBlock());
    } else {
      rhsAdd = llvm::BinaryOperator::CreateAdd(left->Emit(), right->Emit(), "", irgen->GetBasicBlock());
    }
    return new llvm::StoreInst(rhsAdd, lhs, irgen->GetBasicBlock());
  }
}

llvm::Value *PostfixExpr::Emit() {
  // TODO Help
}

llvm::Value *FieldAccess::Emit() {
  // TODO Help
}

llvm::Value *RelationalExpr::Emit() {
  llvm::Value *lhs = left->Emit();
  llvm::Value *rhs = right->Emit();
  llvm::Type *type = lhs->getType();

  if(type == irgen->GetIntType()) {
    if(this->op->IsOp("<")) {
      return llvm::CmpInst::Create(llvm::CmpInst::ICmp, llvm::ICmpInst::ICMP_SLT, lhs, rhs, "", irgen->GetBasicBlock());
    } else if(this->op->IsOp(">")) {
      return llvm::CmpInst::Create(llvm::CmpInst::ICmp, llvm::ICmpInst::ICMP_SGT, lhs, rhs, "", irgen->GetBasicBlock());
    } else if(this->op->IsOp("<=")) {
      return llvm::CmpInst::Create(llvm::CmpInst::ICmp, llvm::ICmpInst::ICMP_SLE, lhs, rhs, "", irgen->GetBasicBlock());
    } else if(this->op->IsOp(">=")) {
      return llvm::CmpInst::Create(llvm::CmpInst::ICmp, llvm::ICmpInst::ICMP_SGE, lhs, rhs, "", irgen->GetBasicBlock());
    }

  } else if(type == irgen->GetFloatType()) {
    if(this->op->IsOp("<")) {
      return llvm::CmpInst::Create(llvm::CmpInst::FCmp, llvm::ICmpInst::FCMP_OLT, lhs, rhs, "", irgen->GetBasicBlock());
    } else if(this->op->IsOp(">")) {
      return llvm::CmpInst::Create(llvm::CmpInst::FCmp, llvm::ICmpInst::FCMP_OGT, lhs, rhs, "", irgen->GetBasicBlock());
    } else if(this->op->IsOp("<=")) {
      return llvm::CmpInst::Create(llvm::CmpInst::FCmp, llvm::ICmpInst::FCMP_OLE, lhs, rhs, "", irgen->GetBasicBlock());
    } else if(this->op->IsOp(">=")) {
      return llvm::CmpInst::Create(llvm::CmpInst::FCmp, llvm::ICmpInst::FCMP_OGE, lhs, rhs, "", irgen->GetBasicBlock());
    }
  }
  return NULL;
}

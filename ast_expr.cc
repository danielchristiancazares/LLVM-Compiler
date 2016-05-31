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
  // cerr << "[DEBUG] VarExpr::Emit()" << endl;
  llvm::Value *value = NULL;
  llvm::Module *mod = irgen->GetOrCreateModule("irgen.bc");
  string s = this->GetIdentifier()->GetName();
  vector < map < string, SymbolTable::DeclAssoc > > ::reverse_iterator
  it = Node::symtable->symTable.rbegin();
  for(; it != Node::symtable->symTable.rend(); ++it) {
    map <string, SymbolTable::DeclAssoc> currMap = *it;
    if(currMap.find(s) != currMap.end()) {
      // cerr << "[DEBUG] Loading value." << endl;
      value = currMap.find(s)->second.value;
      // cerr << "[DEBUG] Identifier: " << this->GetIdentifier()->GetName() << ", Value: " << value << endl;
      llvm::Twine *twine = new llvm::Twine(this->GetIdentifier()->GetName());
      return new llvm::LoadInst(value, "", irgen->GetBasicBlock());
    }
  }
  return NULL;
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
  llvm::Value *value = NULL;
  llvm::Value *binaryOp = NULL;
  llvm::Value *lhs = (this->left == NULL) ? NULL : this->left->Emit();
  llvm::Value *rhs = this->right->Emit();
  Operator *arithOp = this->op;
  llvm::Type *type = rhs->getType();
  llvm::Value *constZero = NULL;

  string rhsName;
  string lhsName;

  FieldAccess *rhsFieldAccess = dynamic_cast<FieldAccess *>(this->right);
  FieldAccess *lhsFieldAccess = dynamic_cast<FieldAccess *>(this->left);
  if(lhsFieldAccess && rhsFieldAccess) {
    if(this->left) {
      if(type == irgen->GetIntType()) {
        if(this->op->IsOp("-")) {
          return llvm::BinaryOperator::CreateSub(lhs, rhs, "", irgen->GetBasicBlock());
        }
        else if(this->op->IsOp("+")) {
          return llvm::BinaryOperator::CreateAdd(lhs, rhs, "", irgen->GetBasicBlock());
        }
        else if(this->op->IsOp("*")) {
          return llvm::BinaryOperator::CreateMul(lhs, rhs, "", irgen->GetBasicBlock());
        }
        else if(this->op->IsOp("/")) {
          return llvm::BinaryOperator::CreateSDiv(lhs, rhs, "", irgen->GetBasicBlock());
        }
      }
      else if(type == irgen->GetFloatType()) {
        if(this->op->IsOp("-")) {
          return llvm::BinaryOperator::CreateFSub(lhs, rhs, "", irgen->GetBasicBlock());
        }
        else if(this->op->IsOp("+")) {
          return llvm::BinaryOperator::CreateFAdd(lhs, rhs, "", irgen->GetBasicBlock());
        }
        else if(this->op->IsOp("*")) {
          return llvm::BinaryOperator::CreateFMul(lhs, rhs, "", irgen->GetBasicBlock());
        }
        else if(this->op->IsOp("/")) {
          return llvm::BinaryOperator::CreateFDiv(lhs, rhs, "", irgen->GetBasicBlock());
        }
      }
      else if(type == irgen->GetVec2Type() || type == irgen->GetVec3Type() || type == irgen->GetVec4Type()) {
        llvm::Value *lhsValue = new llvm::LoadInst(lhsFieldAccess->getPointer(), "loadvector", irgen->GetBasicBlock());
        llvm::Value *rhsValue = new llvm::LoadInst(lhsFieldAccess->getPointer(), "loadvector", irgen->GetBasicBlock());
        llvm::Value *newVector = llvm::UndefValue::get(type);
        llvm::Value *extractLeft;
        llvm::Value *extractRight;
        llvm::Value *arithValue;
        llvm::Value *insertOp;
        unsigned int size;
        if(type == irgen->GetVec2Type()) { size = 2; }
        if(type == irgen->GetVec3Type()) { size = 3; }
        if(type == irgen->GetVec4Type()) { size = 4; }
        if(this->op->IsOp("-")) {
          for(int i = 0; i < size; i++) {
            extractLeft = llvm::ExtractElementInst::Create(lhsValue, llvm::ConstantInt::get(irgen->GetIntType(), i), "extractLeft", irgen->GetBasicBlock());
            extractRight = llvm::ExtractElementInst::Create(rhsValue, llvm::ConstantInt::get(irgen->GetIntType(), i), "extractRight", irgen->GetBasicBlock());
            arithValue = llvm::BinaryOperator::CreateFSub(extractLeft, extractRight, "newValue", irgen->GetBasicBlock());
            newVector = llvm::InsertElementInst::Create(newVector, arithValue, llvm::ConstantInt::get(irgen->GetIntType(), i), "newVector", irgen->GetBasicBlock());
          }
          return newVector;
        }
        else if(this->op->IsOp("+")) {
          for(int i = 0; i < size; i++) {
            extractLeft = llvm::ExtractElementInst::Create(lhsValue, llvm::ConstantInt::get(irgen->GetIntType(), i), "extractLeft", irgen->GetBasicBlock());
            extractRight = llvm::ExtractElementInst::Create(rhsValue, llvm::ConstantInt::get(irgen->GetIntType(), i), "extractRight", irgen->GetBasicBlock());
            arithValue = llvm::BinaryOperator::CreateFAdd(extractLeft, extractRight, "newValue", irgen->GetBasicBlock());
            newVector = llvm::InsertElementInst::Create(newVector, arithValue, llvm::ConstantInt::get(irgen->GetIntType(), i), "newVector", irgen->GetBasicBlock());
          }
          return newVector;
        }
        else if(this->op->IsOp("*")) {
          for(int i = 0; i < size; i++) {
            extractLeft = llvm::ExtractElementInst::Create(lhsValue, llvm::ConstantInt::get(irgen->GetIntType(), i), "extractLeft", irgen->GetBasicBlock());
            extractRight = llvm::ExtractElementInst::Create(rhsValue, llvm::ConstantInt::get(irgen->GetIntType(), i), "extractRight", irgen->GetBasicBlock());
            arithValue = llvm::BinaryOperator::CreateFMul(extractLeft, extractRight, "newValue", irgen->GetBasicBlock());
            newVector = llvm::InsertElementInst::Create(newVector, arithValue, llvm::ConstantInt::get(irgen->GetIntType(), i), "newVector", irgen->GetBasicBlock());
          }
          return newVector;
        }
        else if(this->op->IsOp("/")) {
          for(int i = 0; i < size; i++) {
            extractLeft = llvm::ExtractElementInst::Create(lhsValue, llvm::ConstantInt::get(irgen->GetIntType(), i), "extractLeft", irgen->GetBasicBlock());
            extractRight = llvm::ExtractElementInst::Create(rhsValue, llvm::ConstantInt::get(irgen->GetIntType(), i), "extractRight", irgen->GetBasicBlock());
            arithValue = llvm::BinaryOperator::CreateFDiv(extractLeft, extractRight, "newValue", irgen->GetBasicBlock());
            newVector = llvm::InsertElementInst::Create(newVector, arithValue, llvm::ConstantInt::get(irgen->GetIntType(), i), "newVector", irgen->GetBasicBlock());
          }
          return newVector;
        }
      }

    }
    else {
      llvm::Value *constOne;

      rhsName = dynamic_cast<VarExpr *>(this->right)->GetIdentifier()->GetName();

      vector < map < string, SymbolTable::DeclAssoc > > ::reverse_iterator
      it = symtable->symTable.rbegin();
      for(; it != symtable->symTable.rend(); ++it) {
        map <string, SymbolTable::DeclAssoc> currMap = *it;
        if(currMap.find(rhsName) != currMap.end()) {
          value = currMap.find(rhsName)->second.value;
          break;
        }
      }

      if(type == irgen->GetIntType()) {
        constOne = llvm::ConstantInt::get(irgen->GetIntType(), 1);
        if(this->op->IsOp("--")) {
          binaryOp = llvm::BinaryOperator::CreateSub(rhs, constOne, "", irgen->GetBasicBlock());
        }
        else {
          binaryOp = llvm::BinaryOperator::CreateAdd(rhs, constOne, "", irgen->GetBasicBlock());
        }
      }
      else if(type == irgen->GetFloatType()) {
        constOne = llvm::ConstantFP::get(irgen->GetFloatType(), (float) 1.0);
        if(this->op->IsOp("--")) {
          binaryOp = llvm::BinaryOperator::CreateFSub(rhs, constOne, "", irgen->GetBasicBlock());
        }
        else {
          binaryOp = llvm::BinaryOperator::CreateFAdd(rhs, constOne, "", irgen->GetBasicBlock());
        }
      }
      new llvm::StoreInst(binaryOp, value, irgen->GetBasicBlock());
      return new llvm::LoadInst(value, "", irgen->GetBasicBlock());
    }
  }
  else if(lhsFieldAccess && !rhsFieldAccess) {
    if(type == irgen->GetIntType()) {
      if(this->op->IsOp("-")) {
        return llvm::BinaryOperator::CreateSub(lhs, rhs, "", irgen->GetBasicBlock());
      }
      else if(this->op->IsOp("+")) {
        return llvm::BinaryOperator::CreateAdd(lhs, rhs, "", irgen->GetBasicBlock());
      }
      else if(this->op->IsOp("*")) {
        return llvm::BinaryOperator::CreateMul(lhs, rhs, "", irgen->GetBasicBlock());
      }
      else if(this->op->IsOp("/")) {
        return llvm::BinaryOperator::CreateSDiv(lhs, rhs, "", irgen->GetBasicBlock());
      }
    }
    else if(type == irgen->GetFloatType()) {
      if(this->op->IsOp("-")) {
        return llvm::BinaryOperator::CreateFSub(lhs, rhs, "", irgen->GetBasicBlock());
      }
      else if(this->op->IsOp("+")) {
        return llvm::BinaryOperator::CreateFAdd(lhs, rhs, "", irgen->GetBasicBlock());
      }
      else if(this->op->IsOp("*")) {
        return llvm::BinaryOperator::CreateFMul(lhs, rhs, "", irgen->GetBasicBlock());
      }
      else if(this->op->IsOp("/")) {
        return llvm::BinaryOperator::CreateFDiv(lhs, rhs, "", irgen->GetBasicBlock());
      }
    }
  }
  else if(!lhsFieldAccess && rhsFieldAccess) {
    if(type == irgen->GetIntType()) {
      if(this->op->IsOp("-")) {
        return llvm::BinaryOperator::CreateSub(lhs, rhs, "", irgen->GetBasicBlock());
      }
      else if(this->op->IsOp("+")) {
        return llvm::BinaryOperator::CreateAdd(lhs, rhs, "", irgen->GetBasicBlock());
      }
      else if(this->op->IsOp("*")) {
        return llvm::BinaryOperator::CreateMul(lhs, rhs, "", irgen->GetBasicBlock());
      }
      else if(this->op->IsOp("/")) {
        return llvm::BinaryOperator::CreateSDiv(lhs, rhs, "", irgen->GetBasicBlock());
      }
    }
    else if(type == irgen->GetFloatType()) {
      if(this->op->IsOp("-")) {
        return llvm::BinaryOperator::CreateFSub(lhs, rhs, "", irgen->GetBasicBlock());
      }
      else if(this->op->IsOp("+")) {
        return llvm::BinaryOperator::CreateFAdd(lhs, rhs, "", irgen->GetBasicBlock());
      }
      else if(this->op->IsOp("*")) {
        return llvm::BinaryOperator::CreateFMul(lhs, rhs, "", irgen->GetBasicBlock());
      }
      else if(this->op->IsOp("/")) {
        return llvm::BinaryOperator::CreateFDiv(lhs, rhs, "", irgen->GetBasicBlock());
      }
    }
  }
  else if(!lhsFieldAccess && !rhsFieldAccess) {
    cerr << "neither are fieldaccess" << endl;
    if(this->left) {
      if(type == irgen->GetIntType()) {
        if(this->op->IsOp("-")) {
          return llvm::BinaryOperator::CreateSub(lhs, rhs, "", irgen->GetBasicBlock());
        }
        else if(this->op->IsOp("+")) {
          return llvm::BinaryOperator::CreateAdd(lhs, rhs, "", irgen->GetBasicBlock());
        }
        else if(this->op->IsOp("*")) {
          return llvm::BinaryOperator::CreateMul(lhs, rhs, "", irgen->GetBasicBlock());
        }
        else if(this->op->IsOp("/")) {
          return llvm::BinaryOperator::CreateSDiv(lhs, rhs, "", irgen->GetBasicBlock());
        }
      }
      else if(type == irgen->GetFloatType()) {
        if(this->op->IsOp("-")) {
          return llvm::BinaryOperator::CreateFSub(lhs, rhs, "", irgen->GetBasicBlock());
        }
        else if(this->op->IsOp("+")) {
          return llvm::BinaryOperator::CreateFAdd(lhs, rhs, "", irgen->GetBasicBlock());
        }
        else if(this->op->IsOp("*")) {
          return llvm::BinaryOperator::CreateFMul(lhs, rhs, "", irgen->GetBasicBlock());
        }
        else if(this->op->IsOp("/")) {
          return llvm::BinaryOperator::CreateFDiv(lhs, rhs, "", irgen->GetBasicBlock());
        }
      }
      else if((type == irgen->GetFloatType() && lhs->getType() == irgen->GetVec2Type())
         || (type == irgen->GetFloatType() && lhs->getType() == irgen->GetVec3Type())
         || (type == irgen->GetFloatType() && lhs->getType() == irgen->GetVec4Type())) {
        cerr << "left is float, right is vec" << endl;
        llvm::Value *newVector = llvm::UndefValue::get(type);
        llvm::Value *extractLeft;
        llvm::Value *extractRight;
        llvm::Value *arithValue;
        llvm::Value *insertOp;
        unsigned int size;
        if(lhs->getType() == irgen->GetVec2Type()) { size = 2; }
        if(lhs->getType() == irgen->GetVec3Type()) { size = 3; }
        if(lhs->getType() == irgen->GetVec4Type()) { size = 4; }
        if(this->op->IsOp("-")) {
          for(int i = 0; i < size; i++) {
            extractLeft = llvm::ExtractElementInst::Create(rhs, llvm::ConstantInt::get(irgen->GetIntType(), i), "extractLeft", irgen->GetBasicBlock());
            arithValue = llvm::BinaryOperator::CreateFSub(extractLeft, lhs, "newValue", irgen->GetBasicBlock());
            newVector = llvm::InsertElementInst::Create(newVector, arithValue, llvm::ConstantInt::get(irgen->GetIntType(), i), "newVector", irgen->GetBasicBlock());
          }
          return newVector;
        }
        else if(this->op->IsOp("+")) {
          for(int i = 0; i < size; i++) {
            extractLeft = llvm::ExtractElementInst::Create(rhs, llvm::ConstantInt::get(irgen->GetIntType(), i), "extractLeft", irgen->GetBasicBlock());
            arithValue = llvm::BinaryOperator::CreateFAdd(extractLeft, lhs, "newValue", irgen->GetBasicBlock());
            newVector = llvm::InsertElementInst::Create(newVector, arithValue, llvm::ConstantInt::get(irgen->GetIntType(), i), "newVector", irgen->GetBasicBlock());
          }
          return newVector;
        }
        else if(this->op->IsOp("*")) {
          for(int i = 0; i < size; i++) {
            extractLeft = llvm::ExtractElementInst::Create(rhs, llvm::ConstantInt::get(irgen->GetIntType(), i), "extractLeft", irgen->GetBasicBlock());
            arithValue = llvm::BinaryOperator::CreateFMul(extractLeft, lhs, "newValue", irgen->GetBasicBlock());
            newVector = llvm::InsertElementInst::Create(newVector, arithValue, llvm::ConstantInt::get(irgen->GetIntType(), i), "newVector", irgen->GetBasicBlock());
          }
          return newVector;
        }
        else if(this->op->IsOp("/")) {
          for(int i = 0; i < size; i++) {
            extractLeft = llvm::ExtractElementInst::Create(rhs, llvm::ConstantInt::get(irgen->GetIntType(), i), "extractLeft", irgen->GetBasicBlock());
            arithValue = llvm::BinaryOperator::CreateFDiv(extractLeft, lhs, "newValue", irgen->GetBasicBlock());
            newVector = llvm::InsertElementInst::Create(newVector, arithValue, llvm::ConstantInt::get(irgen->GetIntType(), i), "newVector", irgen->GetBasicBlock());
          }
          return newVector;
        }
      }
      else if((type == irgen->GetVec2Type() && lhs->getType() == type)
              || (type == irgen->GetVec3Type() && lhs->getType() == type)
              || (type == irgen->GetVec4Type() && lhs->getType() == type)) {
        cerr << "both are the same vec type" << endl;
        llvm::Value *newVector;
        llvm::Value *extractLeft;
        llvm::Value *extractRight;
        llvm::Value *arithValue;
        llvm::Value *insertOp;
        unsigned int size;
        if(type == irgen->GetVec2Type()) { size = 2; newVector = llvm::UndefValue::get(irgen->GetVec2Type()); }
        if(type == irgen->GetVec3Type()) { size = 3; newVector = llvm::UndefValue::get(irgen->GetVec3Type()); }
        if(type == irgen->GetVec4Type()) { size = 4; newVector = llvm::UndefValue::get(irgen->GetVec4Type()); }
        if(this->op->IsOp("-")) {
          for(int i = 0; i < size; i++) {
            extractLeft = llvm::ExtractElementInst::Create(lhs, llvm::ConstantInt::get(irgen->GetIntType(), i), "extractLeft", irgen->GetBasicBlock());
            extractRight = llvm::ExtractElementInst::Create(rhs, llvm::ConstantInt::get(irgen->GetIntType(), i), "extractRight", irgen->GetBasicBlock());
            arithValue = llvm::BinaryOperator::CreateFSub(extractLeft, extractRight, "newValue", irgen->GetBasicBlock());
            newVector = llvm::InsertElementInst::Create(newVector, arithValue, llvm::ConstantInt::get(irgen->GetIntType(), i), "newVector", irgen->GetBasicBlock());
          }
          return newVector;
        }
        else if(this->op->IsOp("+")) {
          for(int i = 0; i < size; i++) {
            extractLeft = llvm::ExtractElementInst::Create(lhs, llvm::ConstantInt::get(irgen->GetIntType(), i), "extractLeft", irgen->GetBasicBlock());
            extractRight = llvm::ExtractElementInst::Create(rhs, llvm::ConstantInt::get(irgen->GetIntType(), i), "extractRight", irgen->GetBasicBlock());
            arithValue = llvm::BinaryOperator::CreateFAdd(extractLeft, extractRight, "newValue", irgen->GetBasicBlock());
            newVector = llvm::InsertElementInst::Create(newVector, arithValue, llvm::ConstantInt::get(irgen->GetIntType(), i), "newVector", irgen->GetBasicBlock());
          }
          return newVector;
        }
        else if(this->op->IsOp("*")) {
          for(int i = 0; i < size; i++) {
            extractLeft = llvm::ExtractElementInst::Create(lhs, llvm::ConstantInt::get(irgen->GetIntType(), i), "extractLeft", irgen->GetBasicBlock());
            extractRight = llvm::ExtractElementInst::Create(rhs, llvm::ConstantInt::get(irgen->GetIntType(), i), "extractRight", irgen->GetBasicBlock());
            arithValue = llvm::BinaryOperator::CreateFMul(extractLeft, extractRight, "newValue", irgen->GetBasicBlock());
            newVector = llvm::InsertElementInst::Create(newVector, arithValue, llvm::ConstantInt::get(irgen->GetIntType(), i), "newVector", irgen->GetBasicBlock());
          }
          return newVector;
        }
        else if(this->op->IsOp("/")) {
          for(int i = 0; i < size; i++) {
            extractLeft = llvm::ExtractElementInst::Create(lhs, llvm::ConstantInt::get(irgen->GetIntType(), i), "extractLeft", irgen->GetBasicBlock());
            extractRight = llvm::ExtractElementInst::Create(rhs, llvm::ConstantInt::get(irgen->GetIntType(), i), "extractRight", irgen->GetBasicBlock());
            arithValue = llvm::BinaryOperator::CreateFDiv(extractLeft, extractRight, "newValue", irgen->GetBasicBlock());
            newVector = llvm::InsertElementInst::Create(newVector, arithValue, llvm::ConstantInt::get(irgen->GetIntType(), i), "newVector", irgen->GetBasicBlock());
          }
          return newVector;
        }
      }
    }
    else {
      llvm::Value *constOne;

      rhsName = dynamic_cast<VarExpr *>(this->right)->GetIdentifier()->GetName();

      vector < map < string, SymbolTable::DeclAssoc > > ::reverse_iterator
      it = symtable->symTable.rbegin();
      for(; it != symtable->symTable.rend(); ++it) {
        map <string, SymbolTable::DeclAssoc> currMap = *it;
        if(currMap.find(rhsName) != currMap.end()) {
          value = currMap.find(rhsName)->second.value;
          break;
        }
      }

      if(type == irgen->GetIntType()) {
        constOne = llvm::ConstantInt::get(irgen->GetIntType(), 1);
        if(this->op->IsOp("--")) {
          binaryOp = llvm::BinaryOperator::CreateSub(rhs, constOne, "", irgen->GetBasicBlock());
        }
        else {
          binaryOp = llvm::BinaryOperator::CreateAdd(rhs, constOne, "", irgen->GetBasicBlock());
        }
      }
      else if(type == irgen->GetFloatType()) {
        constOne = llvm::ConstantFP::get(irgen->GetFloatType(), (float) 1.0);
        if(this->op->IsOp("--")) {
          binaryOp = llvm::BinaryOperator::CreateFSub(rhs, constOne, "", irgen->GetBasicBlock());
        }
        else {
          binaryOp = llvm::BinaryOperator::CreateFAdd(rhs, constOne, "", irgen->GetBasicBlock());
        }
      }
      new llvm::StoreInst(binaryOp, value, irgen->GetBasicBlock());
      return new llvm::LoadInst(value, "", irgen->GetBasicBlock());
    }
  }
  return NULL;
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
    }
    else if(this->op->IsOp("!=")) {
      return llvm::CmpInst::Create(llvm::CmpInst::ICmp, llvm::ICmpInst::ICMP_NE, lhs, rhs, "", irgen->GetBasicBlock());
    }
  }
  else if(type == irgen->GetFloatType()) {
    if(this->op->IsOp("==")) {
      return llvm::CmpInst::Create(llvm::CmpInst::FCmp, llvm::CmpInst::FCMP_OEQ, lhs, rhs, "", irgen->GetBasicBlock());
    }
    else if(this->op->IsOp("!=")) {
      return llvm::CmpInst::Create(llvm::CmpInst::FCmp, llvm::CmpInst::FCMP_ONE, lhs, rhs, "", irgen->GetBasicBlock());
    }
  }
  return NULL;
}

llvm::Value *LogicalExpr::Emit() {
  /*
  llvm::BinaryOperator::CreateAnd(Value *S1, Value *S2, const Twine &Name, BasicBlock *InsertAtEnd)
  llvm::BinaryOperator::CreateOr(Value *S1, Value *S2, const Twine &Name, BasicBlock *InsertAtEnd)
  */
  llvm::Value *lhs = left->Emit();
  llvm::Value *rhs = right->Emit();

  llvm::ConstantInt *lhsBool = (llvm::ConstantInt *) lhs;
  llvm::ConstantInt *rhsBool = (llvm::ConstantInt *) rhs;

  if(this->op->IsOp("&&")) {
    return llvm::BinaryOperator::CreateAnd(lhs, rhs, "LogicalAnd", irgen->GetBasicBlock());
    /*
    if(lhsBool->isOne() && rhsBool->isOne()) {
      return lhs;
    } else if(rhsBool->isZero()) {
      return rhs;
    } else {
      return lhs;
    }
    */
  }
  else if(this->op->IsOp("||")) {
    return llvm::BinaryOperator::CreateOr(lhs, rhs, "LogicalAnd", irgen->GetBasicBlock());
    /*
    if(lhsBool->isOne()) {
      return lhs;
    } else {
      return rhs;
    }
    */
  }
  return NULL;
}

llvm::Value *FieldAccess::getPointer() {
  llvm::Value *lhs = NULL;
  VarExpr *lhsVar = dynamic_cast<VarExpr *>(this->base);
  string s = lhsVar->GetIdentifier()->GetName();
  vector < map < string, SymbolTable::DeclAssoc > > ::reverse_iterator
  it = this->symtable->symTable.rbegin();
  for(; it != this->symtable->symTable.rend() && lhs == NULL; ++it) {
    map <string, SymbolTable::DeclAssoc> currMap = *it;
    if(currMap.find(s) != currMap.end()) {
      lhs = currMap.find(s)->second.value;
      break;
    }
  }
  return lhs;
}

llvm::Value *AssignExpr::Emit() {
  cerr << "assignexpr" << endl;
  llvm::Value *lhs = NULL;
  llvm::Value *rhs = NULL;
  FieldAccess *lhsFieldAccess = NULL;
  llvm::Value *retVal = NULL;
  VarExpr *lhsVar = dynamic_cast<VarExpr *>(left);
  llvm::Value *binaryOp = NULL;
  llvm::Value *insertOp = NULL;
  if(lhsVar) {
    lhs = this->left->Emit();
    cerr << "assignexpr var: " << lhsVar->GetIdentifier()->GetName() << endl;
    lhs = llvm::cast<llvm::LoadInst>(lhs)->getPointerOperand();
  }
  else if(dynamic_cast<FieldAccess *>(left)) {
    lhsFieldAccess = dynamic_cast<FieldAccess *>(left);
    lhs = lhsFieldAccess->getPointer();
  }
  else if(dynamic_cast<ArrayAccess *>(left)) {
    lhs = dynamic_cast<ArrayAccess *>(left)->Emit();
    lhs = llvm::cast<llvm::LoadInst>(lhs)->getPointerOperand();
  }
  llvm::Type *type = lhs->getType();
  if(this->op->IsOp("=")) {
    if(lhsFieldAccess) {
      rhs = this->right->Emit();
      llvm::Value *lhsValue = new llvm::LoadInst(lhsFieldAccess->getPointer(), "loadvector", irgen->GetBasicBlock());
      string swizzle = lhsFieldAccess->GetSwizzle();
      llvm::Constant *swizzleIdx;
      llvm::Value *extractedValue;
      for(int i = 0; i < swizzle.size(); i++) {
        if(swizzle[i] == 'x') {
          swizzleIdx = llvm::ConstantInt::get(irgen->GetIntType(), 0);
        }
        else if(swizzle[i] == 'y') {
          swizzleIdx = llvm::ConstantInt::get(irgen->GetIntType(), 1);
        }
        else if(swizzle[i] == 'z') {
          swizzleIdx = llvm::ConstantInt::get(irgen->GetIntType(), 2);
        }
        else if(swizzle[i] == 'w') {
          swizzleIdx = llvm::ConstantInt::get(irgen->GetIntType(), 3);
        }
        if(rhs->getType() == irgen->GetFloatType()) {
          insertOp = llvm::InsertElementInst::Create(lhsValue, rhs, swizzleIdx, "singleFloat", irgen->GetBasicBlock());
          break;
        }
        else {
          extractedValue = llvm::ExtractElementInst::Create(rhs, swizzleIdx, "extractedVal", irgen->GetBasicBlock());
          insertOp = llvm::InsertElementInst::Create(lhsValue, extractedValue, swizzleIdx, "newValInserted", irgen->GetBasicBlock());
        }
      }
      new llvm::StoreInst(insertOp, lhs, irgen->GetBasicBlock());
      return rhs;
    }
    retVal = this->right->Emit();
    cerr << "assignexpr: done emitting" << endl;
    new llvm::StoreInst(retVal, lhs, irgen->GetBasicBlock());
    return retVal;
  }
  else if(this->op->IsOp("*=")) {
    retVal = this->right->Emit();
    llvm::Type *type = lhs->getType();
    if(type == irgen->GetFloatType()) {
      binaryOp = llvm::BinaryOperator::CreateFMul(left->Emit(), retVal, "mulFAssign", irgen->GetBasicBlock());
    }
    else {
      binaryOp = llvm::BinaryOperator::CreateMul(left->Emit(), retVal, "mulAssign", irgen->GetBasicBlock());
    }
    new llvm::StoreInst(binaryOp, lhs, irgen->GetBasicBlock());
    return retVal;
  }
  else if(this->op->IsOp("+=")) {
    retVal = this->right->Emit();
    if(type == irgen->GetFloatType()) {
      binaryOp = llvm::BinaryOperator::CreateFAdd(left->Emit(), retVal, "addFAssign", irgen->GetBasicBlock());
    }
    else {
      binaryOp = llvm::BinaryOperator::CreateAdd(left->Emit(), retVal, "addAssign", irgen->GetBasicBlock());
    }
    new llvm::StoreInst(binaryOp, lhs, irgen->GetBasicBlock());
    return retVal;
  }
  else if(this->op->IsOp("-=")) {
    retVal = this->right->Emit();
    if(type == irgen->GetFloatType()) {
      binaryOp = llvm::BinaryOperator::CreateFSub(left->Emit(), retVal, "subFAssign", irgen->GetBasicBlock());
    }
    else {
      binaryOp = llvm::BinaryOperator::CreateSub(left->Emit(), retVal, "subAssign", irgen->GetBasicBlock());
    }
    new llvm::StoreInst(binaryOp, lhs, irgen->GetBasicBlock());
    return retVal;
  }
  else if(this->op->IsOp("/=")) {
    retVal = this->right->Emit();
    if(type == irgen->GetFloatType()) {
      binaryOp = llvm::BinaryOperator::CreateFDiv(left->Emit(), retVal, "divFAssign", irgen->GetBasicBlock());
    }
    else {
      binaryOp = llvm::BinaryOperator::CreateSDiv(left->Emit(), retVal, "divAssign", irgen->GetBasicBlock());
    }
    new llvm::StoreInst(binaryOp, lhs, irgen->GetBasicBlock());
    return retVal;
  }

  return NULL;
}

llvm::Value *PostfixExpr::Emit() {
  llvm::Value *value = NULL;
  llvm::Value *lhs = left->Emit();
  llvm::Type *type = lhs->getType();
  llvm::Value *binaryOp = NULL;

  string s = dynamic_cast<VarExpr *>(left)->GetIdentifier()->GetName();
  vector < map < string, SymbolTable::DeclAssoc > > ::reverse_iterator
  it = Node::symtable->symTable.rbegin();
  for(; it != Node::symtable->symTable.rend(); ++it) {
    map <string, SymbolTable::DeclAssoc> currMap = *it;
    if(currMap.find(s) != currMap.end()) {
      value = currMap.find(s)->second.value;
      break;
    }
  }

  if(type == irgen->GetIntType()) {
    if(this->op->IsOp("--")) {
      binaryOp = llvm::BinaryOperator::CreateSub(lhs, llvm::ConstantInt::get(irgen->GetIntType(), 1), "", irgen->GetBasicBlock());
    }
    else {
      binaryOp = llvm::BinaryOperator::CreateAdd(lhs, llvm::ConstantInt::get(irgen->GetIntType(), 1), "", irgen->GetBasicBlock());
    }
  }
  else if(type == irgen->GetFloatType()) {
    if(this->op->IsOp("--")) {
      binaryOp = llvm::BinaryOperator::CreateFSub(lhs, llvm::ConstantFP::get(irgen->GetFloatType(), 1.0), "", irgen->GetBasicBlock());
    }
    else {
      binaryOp = llvm::BinaryOperator::CreateFAdd(lhs, llvm::ConstantFP::get(irgen->GetFloatType(), 1.0), "", irgen->GetBasicBlock());
    }
  }
  new llvm::StoreInst(binaryOp, value, irgen->GetBasicBlock());
  return lhs;
}

llvm::Value *ArrayAccess::Emit() {
  // cerr << "[ArrayAccess] Emit is being called" << endl;
  /*
  // Ptr should be address of the base
  // IdxList should be an ArrayRef<Value*> where the first element is a llvm::ConstantInt(0) and
  // the second element is the llvm:: corresponding to the subscript
  llvm::GetElementPtrInst::Create(Value *Ptr, ArrayRef<Value*> IdxList, const Twine &NameStr, BasicBlock *InsertAtEnd);
  */
  vector < llvm::Value * > arrayVal;
  arrayVal.push_back(llvm::ConstantInt::get(irgen->GetIntType(), 0));
  llvm::Value *arraySubscript = this->subscript->Emit();
  arrayVal.push_back(arraySubscript);
  llvm::Value *arrBase = this->base->Emit();
  llvm::Value *baseAddress = llvm::cast<llvm::LoadInst>(arrBase)->getPointerOperand();
  llvm::ArrayRef<llvm::Value *> arrayOffset(arrayVal);
  llvm::Value *value = llvm::GetElementPtrInst::Create(baseAddress, arrayOffset, "ArrayAccess", irgen->GetBasicBlock());
  return new llvm::LoadInst(value, "ArrayAccessed", irgen->GetBasicBlock());
}

llvm::Value *FieldAccess::Emit() {
  llvm::Value *fieldBase = this->base->Emit();
  llvm::Value *value = NULL;
  string fieldName = this->field->GetName();
  vector < llvm::Constant * > maskIdx;
  llvm::Constant *mask;
  llvm::Value *retVal = NULL;
  if(fieldName.size() == 1) {
    if(fieldName == "x") {
      retVal = llvm::ExtractElementInst::Create(fieldBase, llvm::ConstantInt::get(irgen->GetIntType(), 0), "vecfloat", irgen->GetBasicBlock());
    }
    else if(fieldName == "y") {
      retVal = llvm::ExtractElementInst::Create(fieldBase, llvm::ConstantInt::get(irgen->GetIntType(), 1), "vecfloat", irgen->GetBasicBlock());
    }
    else if(fieldName == "z") {
      retVal = llvm::ExtractElementInst::Create(fieldBase, llvm::ConstantInt::get(irgen->GetIntType(), 2), "vecfloat", irgen->GetBasicBlock());
    }
    else if(fieldName == "w") {
      retVal = llvm::ExtractElementInst::Create(fieldBase, llvm::ConstantInt::get(irgen->GetIntType(), 3), "vecfloat", irgen->GetBasicBlock());
    }
  }
  else if(fieldName.size() <= 4) {
    vector < llvm::Constant * > swizzles;
    llvm::Constant *swizzleIdx;
    for(int i = 0; i < fieldName.size(); ++i) {
      if(fieldName[i] == 'x') {
        swizzleIdx = llvm::ConstantInt::get(irgen->GetIntType(), 0);
      }
      else if(fieldName[i] == 'y') {
        swizzleIdx = llvm::ConstantInt::get(irgen->GetIntType(), 1);
      }
      else if(fieldName[i] == 'z') {
        swizzleIdx = llvm::ConstantInt::get(irgen->GetIntType(), 2);
      }
      else if(fieldName[i] == 'w') {
        swizzleIdx = llvm::ConstantInt::get(irgen->GetIntType(), 3);
      }
      swizzles.push_back(swizzleIdx);
    }
    llvm::ArrayRef<llvm::Constant *> swizzleArrayRef(swizzles);
    llvm::Constant *mask = llvm::ConstantVector::get(swizzleArrayRef);
    retVal = new llvm::ShuffleVectorInst(fieldBase, llvm::UndefValue::get(fieldBase->getType()), mask, "vector", irgen->GetBasicBlock());
  }
  return retVal;
}

llvm::Value *RelationalExpr::Emit() {
  // cerr << "RealtionalExpr called" << endl;
  llvm::Value *lhs = left->Emit();
  llvm::Value *rhs = right->Emit();
  llvm::Type *type = lhs->getType();

  if(type == irgen->GetIntType()) {
    if(this->op->IsOp("<")) {
      return llvm::CmpInst::Create(llvm::CmpInst::ICmp, llvm::ICmpInst::ICMP_SLT, lhs, rhs, "", irgen->GetBasicBlock());
    }
    else if(this->op->IsOp(">")) {
      return llvm::CmpInst::Create(llvm::CmpInst::ICmp, llvm::ICmpInst::ICMP_SGT, lhs, rhs, "", irgen->GetBasicBlock());
    }
    else if(this->op->IsOp("<=")) {
      return llvm::CmpInst::Create(llvm::CmpInst::ICmp, llvm::ICmpInst::ICMP_SLE, lhs, rhs, "", irgen->GetBasicBlock());
    }
    else if(this->op->IsOp(">=")) {
      return llvm::CmpInst::Create(llvm::CmpInst::ICmp, llvm::ICmpInst::ICMP_SGE, lhs, rhs, "", irgen->GetBasicBlock());
    }

  }
  else if(type == irgen->GetFloatType()) {
    if(this->op->IsOp("<")) {
      return llvm::CmpInst::Create(llvm::CmpInst::FCmp, llvm::ICmpInst::FCMP_OLT, lhs, rhs, "", irgen->GetBasicBlock());
    }
    else if(this->op->IsOp(">")) {
      return llvm::CmpInst::Create(llvm::CmpInst::FCmp, llvm::ICmpInst::FCMP_OGT, lhs, rhs, "", irgen->GetBasicBlock());
    }
    else if(this->op->IsOp("<=")) {
      return llvm::CmpInst::Create(llvm::CmpInst::FCmp, llvm::ICmpInst::FCMP_OLE, lhs, rhs, "", irgen->GetBasicBlock());
    }
    else if(this->op->IsOp(">=")) {
      return llvm::CmpInst::Create(llvm::CmpInst::FCmp, llvm::ICmpInst::FCMP_OGE, lhs, rhs, "", irgen->GetBasicBlock());
    }
  }
  return NULL;
}

llvm::Value *Call::Emit() {
  // cerr << "[Call] Emit is being called" << endl;
  /*
    Used for "Call" expression
    Func should be the address of the function
    Args is an ArrayRef<Value*> of the Actuals LLVM::Value
    llvm::CallInst::Create( Value *Func, ArrayRef<Value*> Args, const Twine &NameStr, BasicBlock *InsertAtEnd );
  */
  llvm::Function *f = NULL;
  string s = this->field->GetName();
  vector < map < string, SymbolTable::DeclAssoc > > ::reverse_iterator
  it = this->symtable->symTable.rbegin();
  for(; it != this->symtable->symTable.rend() && f == NULL; ++it) {
    map <string, SymbolTable::DeclAssoc> currMap = *it;
    if(currMap.find(s) != currMap.end()) {
      if(!llvm::cast<llvm::Function>((currMap.find(s)->second.value))) {
        // cerr << "[Call] Function was not found" << endl;
      }
      // cerr << "[Call] Found the function!" << endl;
      f = llvm::cast<llvm::Function>(currMap.find(s)->second.value);
      break;
    }
  }

  vector < llvm::Value * > actualStack;
  for(int i = 0; i < this->actuals->NumElements(); i++) {
    // cerr << "[Call] Actuals are being populated: " << i << endl;
    llvm::Value *temp = this->actuals->Nth(i)->Emit();
    actualStack.push_back(temp);
  }

  llvm::ArrayRef<llvm::Value *> arrayOfActuals(actualStack);
  llvm::Value *value = llvm::CallInst::Create(f, arrayOfActuals, "FunctionCall", irgen->GetBasicBlock());
  return value;
}

llvm::Value *ConditionalExpr::Emit() {
  // Expr *cond, *trueExpr, *falseExpr;
  // Create (Type *Ty, unsigned NumReservedValues, const Twine &NameStr="", Instruction *InsertBefore=nullptr)
  cerr << "[ConditionalExpr] Emit is called" << endl;
  llvm::Value *cond = this->cond->Emit();
  llvm::LLVMContext *context = irgen->GetContext();
  llvm::Value *trueBranch = NULL;
  llvm::Value *falseBranch = NULL;
  llvm::BasicBlock *trueBB = llvm::BasicBlock::Create(*context, "TrueBB", irgen->GetFunction());
  llvm::BasicBlock *falseBB = llvm::BasicBlock::Create(*context, "FalseBB", irgen->GetFunction());
  llvm::BasicBlock *footerBB = llvm::BasicBlock::Create(*context, "CondFooterBB", irgen->GetFunction());

  cerr << "[ConditionalExpr] BasicBlocks have been made, making a SelectInst branch now" << endl;
  llvm::BranchInst::Create(trueBB, falseBB, cond, irgen->GetBasicBlock());
  cerr << "[ConditionalExpr] BasicBlocks have been made, making a SelectInst branch now" << endl;

  irgen->SetBasicBlock(trueBB);
  trueBranch = this->trueExpr->Emit();
  if(irgen->GetBasicBlock()->getTerminator() == NULL) {
    llvm::BranchInst::Create(footerBB, irgen->GetBasicBlock());
  }

  irgen->SetBasicBlock(footerBB);

  irgen->SetBasicBlock(falseBB);
  falseBranch = this->falseExpr->Emit();
  if(irgen->GetBasicBlock()->getTerminator() == NULL) {
    llvm::BranchInst::Create(footerBB, irgen->GetBasicBlock());
  }
  irgen->SetBasicBlock(footerBB);

  cerr << "[ConditionalExpr] Making PHINode" << endl;
  llvm::PHINode *phiNode = llvm::PHINode::Create(falseBranch->getType(), 0, "PHINode", irgen->GetBasicBlock());
  //void  addIncoming (Value *V, BasicBlock *BB)
  cerr << "[ConditionalExpr] Adding incoming statements" << endl;
  phiNode->addIncoming(trueBranch, trueBB);
  phiNode->addIncoming(falseBranch, falseBB);

  cerr << "[ConditionalExpr] Returning PHINode" << endl;
  return phiNode;
}
/* File: ast_stmt.cc
 * -----------------
 * Implementation of statement node classes.
 */
#include "ast_stmt.h"
#include "ast_type.h"
#include "ast_decl.h"
#include "ast_expr.h"
#include "symtable.h"

#include "irgen.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/Support/raw_ostream.h"

Program::Program (List<Decl *> *d) {
  Assert(d != NULL);
  (decls = d)->SetParentAll (this);
}

void Program::PrintChildren (int indentLevel) {
  decls->PrintAll (indentLevel + 1);
  printf ("\n");
}

llvm::Value *Program::Emit () {
  if(decls->NumElements () <= 0) {
    return NULL;
  }

  llvm::Module *mod = irgen->GetOrCreateModule ("irgen.bc");

  for (int i = 0; i < decls->NumElements (); ++i) {
    Decl *decl = decls->Nth (i);
    decl->Emit ();
  }



// create a function signature
//    std::vector<llvm::Type *> argTypes;
//    llvm::Type *intTy = irgen.GetIntType();
//    argTypes.push_back(intTy);
//    llvm::ArrayRef<llvm::Type *> argArray(argTypes);
//    llvm::FunctionType *funcTy = llvm::FunctionType::get(intTy, argArray, false);

// llvm::Function *f = llvm::cast<llvm::Function>(mod->getOrInsertFunction("foo", intTy, intTy, (Type *)0));
//    llvm::Function *f = llvm::cast<llvm::Function>(mod->getOrInsertFunction("Name_the_function", funcTy));
//    llvm::Argument *arg = f->arg_begin();
//    arg->setName("x");

// insert a block into the runction
//    llvm::LLVMContext *context = irgen.GetContext();
//    llvm::BasicBlock *bb = llvm::BasicBlock::Create(*context, "entry", f);

// create a return instruction
//    llvm::Value *val = llvm::ConstantInt::get(intTy, 1);
//    llvm::Value *sum = llvm::BinaryOperator::CreateAdd(arg, val, "", bb);
//    llvm::ReturnInst::Create(*context, sum, bb);

// write the BC into standard output
//    llvm::WriteBitcodeToFile(mod, llvm::outs());
  return NULL;
}

StmtBlock::StmtBlock (List<VarDecl *> *d, List<Stmt *> *s) {
  Assert(d != NULL && s != NULL);
  (decls = d)->SetParentAll (this);
  (stmts = s)->SetParentAll (this);
}

void StmtBlock::PrintChildren (int indentLevel) {
  decls->PrintAll (indentLevel + 1);
  stmts->PrintAll (indentLevel + 1);
}

llvm::Value *StmtBlock::Emit () {
  // TODO Need to figure out the logic for this.
  /*
    Get the current scope
    For each  element in scope 
    Create  local variable  (as in  VarDecl::Emit)
    Pop the current scope
    Create  new scope  
    For each  statement 's' in  body  of  funcCon:    
    s->Emit()   
    Delete  scope 
  */
<<<<<<< HEAD
    // get formals for local variables
    map<string, SymbolTable::DeclAssoc> tempScope = Node::symtable->symTable.back();
    map<string, SymbolTable::DeclAssoc> newScope;
=======
  map <string, SymbolTable::DeclAssoc> newScope;
>>>>>>> df915d86a66396f7587e1a61c4b2277606185e14

    FnDecl *f = dynamic_cast<FnDecl*>(tempScope.rbegin()->second.decl);
    if (f == NULL) {
      cout << "SHIT IS NULL" << endl;
    }
    List<VarDecl*> *formalList = f->GetFormals();
    string name;
    SymbolTable::DeclAssoc declassoc;

    for(int i = 0; i < formalList->NumElements(); i++) {
      VarDecl *v = formalList->Nth(i);
      llvm::Type *varType = irgen->Converter(v->GetType());
      name = v->GetIdentifier()->GetName();
      llvm::Value *value = new llvm::AllocaInst(varType, name, irgen->GetBasicBlock());
      declassoc.decl = v;
      declassoc.value = value;
      newScope.insert(pair<string, SymbolTable::DeclAssoc>(name, declassoc));
    }

    Node::symtable->symTable.push_back(newScope);
    for(int i  = 0; i < this->stmts->NumElements(); i++) {
      Stmt *s = this->stmts->Nth(i);
      s->Emit();
    }

    Node::symtable->symTable.pop_back();

<<<<<<< HEAD
=======
llvm::Value *StmtBlock::EmitFromFunc () {
>>>>>>> df915d86a66396f7587e1a61c4b2277606185e14
  return NULL;
}

DeclStmt::DeclStmt (Decl *d) {
  Assert(d != NULL);
  (decl = d)->SetParent (this);
}

void DeclStmt::PrintChildren (int indentLevel) {
  decl->Print (indentLevel + 1);
}

llvm::Value *DeclStmt::Emit () {
  decl->Emit ();
  return NULL;
}

ConditionalStmt::ConditionalStmt (Expr *t, Stmt *b) {
  Assert(t != NULL && b != NULL);
  (test = t)->SetParent (this);
  (body = b)->SetParent (this);
}

llvm::Value *ConditionalStmt::Emit () {
  test->Emit ();
  body->Emit ();
  return NULL;
}

ForStmt::ForStmt (Expr *i, Expr *t, Expr *s, Stmt *b) : LoopStmt (t, b) {
  Assert(i != NULL && t != NULL && b != NULL);
  (init = i)->SetParent (this);
  step = s;
  if(s) {
    (step = s)->SetParent (this);
  }
}

void ForStmt::PrintChildren (int indentLevel) {
  init->Print (indentLevel + 1, "(init) ");
  test->Print (indentLevel + 1, "(test) ");
  if(step) {
    step->Print (indentLevel + 1, "(step) ");
  }
  body->Print (indentLevel + 1, "(body) ");
}

llvm::Value *ForStmt::Emit () {
  //TODO Logic for this is intense
  return NULL;
}

void WhileStmt::PrintChildren (int indentLevel) {
  test->Print (indentLevel + 1, "(test) ");
  body->Print (indentLevel + 1, "(body) ");
}

llvm::Value *WhileStmt::Emit () {
  //TODO Logic for this is intense
  return NULL;
}

IfStmt::IfStmt (Expr *t, Stmt *tb, Stmt *eb) : ConditionalStmt (t, tb) {
  Assert(t != NULL && tb != NULL); // else can be NULL
  elseBody = eb;
  if(elseBody) { elseBody->SetParent (this); }
}

void IfStmt::PrintChildren (int indentLevel) {
  if(test) { test->Print (indentLevel + 1, "(test) "); }
  if(body) { body->Print (indentLevel + 1, "(then) "); }
  if(elseBody) { elseBody->Print (indentLevel + 1, "(else) "); }
}

llvm::Value *IfStmt::Emit () {
  //TODO Logic for this is intense
  llvm::Value *cond = this->test->Emit();
  llvm::LLVMContext *context = irgen->GetContext();
  llvm::BasicBlock *footBB = llvm::BasicBlock::Create(*context, "footer", irgen->GetFunction());
  llvm::BasicBlock *elseBB;
  if (this->elseBody != NULL) {
    elseBB = llvm::BasicBlock::Create(*context, "elseBB", irgen->GetFunction());
  }
  llvm::BasicBlock *thenBB = llvm::BasicBlock::Create(*context, "thenBB", irgen->GetFunction());
  llvm::BranchInst::Create(thenBB, elseBody ? elseBB : footBB, cond, irgen->GetBasicBlock());

  irgen->SetBasicBlock(thenBB);
  this->body->Emit();
  llvm::BranchInst::Create(footBB, irgen->GetBasicBlock());

  if (this->elseBody != NULL) {
    irgen->SetBasicBlock(elseBB);
    this->elseBody->Emit();
    llvm::BranchInst::Create(elseBB, irgen->GetBasicBlock());
  }

  irgen->SetBasicBlock(footBB);

  return NULL;
}

ReturnStmt::ReturnStmt (yyltype loc, Expr *e) : Stmt (loc) {
  expr = e;
  if(e != NULL) { expr->SetParent (this); }
}

void ReturnStmt::PrintChildren (int indentLevel) {
  if(expr) {
    expr->Print (indentLevel + 1);
  }
}

llvm::Value *ReturnStmt::Emit () {
  // TODO Check the expression and perform something depending on that?
  return NULL;
}

SwitchLabel::SwitchLabel (Expr *l, Stmt *s) {
  Assert(l != NULL && s != NULL);
  (label = l)->SetParent (this);
  (stmt = s)->SetParent (this);
}

SwitchLabel::SwitchLabel (Stmt *s) {
  Assert(s != NULL);
  label = NULL;
  (stmt = s)->SetParent (this);
}

void SwitchLabel::PrintChildren (int indentLevel) {
  if(label) { label->Print (indentLevel + 1); }
  if(stmt) { stmt->Print (indentLevel + 1); }
}

llvm::Value *SwitchLabel::Emit () {
  stmt->Emit ();
  return NULL;
}

SwitchStmt::SwitchStmt (Expr *e, List<Stmt *> *c, Default *d) {
  Assert(e != NULL && c != NULL && c->NumElements () != 0);
  (expr = e)->SetParent (this);
  (cases = c)->SetParentAll (this);
  def = d;
  if(def) { def->SetParent (this); }
}

void SwitchStmt::PrintChildren (int indentLevel) {
  if(expr) { expr->Print (indentLevel + 1); }
  if(cases) { cases->PrintAll (indentLevel + 1); }
  if(def) { def->Print (indentLevel + 1); }
}

llvm::Value *SwitchStmt::Emit () {
  //TODO OMG What do here
  return NULL;
}

llvm::Value *BreakStmt::Emit () {
  //TODO Create a BasicBlock within SwitchStmt to declare whether or not
  //TODO to exit.
//  llvm::BranchInst::Create (????, irgen->GetBasicBlock ());
  return NULL;
}

llvm::Value *ContinueStmt::Emit () {
  //TODO Same as above.
//  llvm::BranchInst::Create (?????, irgen->GetBasicBlock ());
  return NULL;
}


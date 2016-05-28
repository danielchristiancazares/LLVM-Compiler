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

Program::Program(List<Decl *> *d) {
  Assert(d != NULL);
  (decls = d)->SetParentAll(this);
}

void Program::PrintChildren(int indentLevel) {
  decls->PrintAll(indentLevel + 1);
  printf("\n");
}

llvm::Value *Program::Emit() {
  if(decls->NumElements() <= 0) {
    return NULL;
  }

  llvm::Module *mod = irgen->GetOrCreateModule("irgen.bc");

  for (int i = 0; i < decls->NumElements(); ++i) {
    Decl *decl = decls->Nth(i);
    decl->Emit();
    mod->dump();
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
  llvm::WriteBitcodeToFile(mod, llvm::outs());
  return NULL;
}

StmtBlock::StmtBlock(List<VarDecl *> *d, List<Stmt *> *s) {
  Assert(d != NULL && s != NULL);
  (decls = d)->SetParentAll(this);
  (stmts = s)->SetParentAll(this);
}

void StmtBlock::PrintChildren(int indentLevel) {
  decls->PrintAll(indentLevel + 1);
  stmts->PrintAll(indentLevel + 1);
}

llvm::Value *StmtBlock::Emit() {
  // TODO Need to figure out the logic for this.
  //cerr << "StmtBlock Emit called" << endl;
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
  // get formals for local variables
  cerr << "[Global::StmtBlock] StmtBlock::EmitFromFunc()" << endl;
  map <string, SymbolTable::DeclAssoc> newScope;
  Node::symtable->symTable.push_back(newScope);
  for (int i = 0; i < this->stmts->NumElements(); i++) {
    Stmt *s = this->stmts->Nth(i);
    s->Emit();
  }

  vector< map<string, SymbolTable::DeclAssoc> >::iterator it = Node::symtable->symTable.begin();
  for(; it != Node::symtable->symTable.end(); it++) {
    map<string, SymbolTable::DeclAssoc> currMap = *it;
    for(map<string, SymbolTable::DeclAssoc>::iterator it2 = currMap.begin(); it2 != currMap.end(); it2++) {
      cerr << "[Global::StmtBlock] String: " << it2->first << ", Value: " << it2->second.value << endl;
    }
  }
  Node::symtable->symTable.pop_back();

  return NULL;
}

llvm::Value *StmtBlock::EmitFromFunc() {
  cerr << "[FnDecl::StmtBlock] StmtBlock::EmitFromFunc()" << endl;

  for (int i = 0; i < this->stmts->NumElements(); i++) {
    Stmt *s = this->stmts->Nth(i);
    s->Emit();
  }

  vector< map<string, SymbolTable::DeclAssoc> >::iterator it = Node::symtable->symTable.begin();
  for(; it != Node::symtable->symTable.end(); it++) {
    map<string, SymbolTable::DeclAssoc> currMap = *it;
    for(map<string, SymbolTable::DeclAssoc>::iterator it2 = currMap.begin(); it2 != currMap.end(); it2++) {
      cerr << "[FnDecl::StmtBlock] String: " << it2->first << ", Value: " << it2->second.value << endl;
    }
  }
  Node::symtable->symTable.pop_back();

  return NULL;
}

DeclStmt::DeclStmt(Decl *d) {
  Assert(d != NULL);
  (decl = d)->SetParent(this);
}

void DeclStmt::PrintChildren(int indentLevel) {
  decl->Print(indentLevel + 1);
}

llvm::Value *DeclStmt::Emit() {
  //cerr << "DeclStmt Emit called" << endl;
  decl->Emit();
  return NULL;
}

ConditionalStmt::ConditionalStmt(Expr *t, Stmt *b) {
  Assert(t != NULL && b != NULL);
  (test = t)->SetParent(this);
  (body = b)->SetParent(this);
}

llvm::Value *ConditionalStmt::Emit() {
  test->Emit();
  body->Emit();
  return NULL;
}

ForStmt::ForStmt(Expr *i, Expr *t, Expr *s, Stmt *b) : LoopStmt(t, b) {
  Assert(i != NULL && t != NULL && b != NULL);
  (init = i)->SetParent(this);
  step = s;
  if(s) {
    (step = s)->SetParent(this);
  }
}

void ForStmt::PrintChildren(int indentLevel) {
  init->Print(indentLevel + 1, "(init) ");
  test->Print(indentLevel + 1, "(test) ");
  if(step) {
    step->Print(indentLevel + 1, "(step) ");
  }
  body->Print(indentLevel + 1, "(body) ");
}

llvm::Value *ForStmt::Emit() {
  //cerr << "ForStmt Emit called" << endl;
  llvm::LLVMContext *context = irgen->GetContext();
  // creating the basicblocks
  llvm::BasicBlock *next = llvm::BasicBlock::Create(*context, "FORnext", irgen->GetFunction());
  llvm::BasicBlock *headerBB = llvm::BasicBlock::Create(*context, "FORheaderBB", irgen->GetFunction());
  llvm::BasicBlock *bodyBB = llvm::BasicBlock::Create(*context, "FORbodyBB", irgen->GetFunction());
  llvm::BasicBlock *stepBB = llvm::BasicBlock::Create(*context, "FORstepBB", irgen->GetFunction());
  llvm::BasicBlock *footerBB = llvm::BasicBlock::Create(*context, "FORfooterBB", irgen->GetFunction());

  llvm::BranchInst::Create(next, irgen->GetBasicBlock());
  irgen->SetBasicBlock(next);
  // emit init
  this->init->Emit();

  // create branch to terminate current BB and start loop header
  //cerr << "branch for header" << endl;
  llvm::BranchInst::Create(headerBB, irgen->GetBasicBlock());
  irgen->SetBasicBlock(headerBB);
  // emit test
  llvm::Value *cond = this->test->Emit();

  // irgen headerBB
  //cerr << "branch for main" << endl;

  llvm::BranchInst::Create(bodyBB, footerBB, cond, irgen->GetBasicBlock());

  // jump to footer
  //cerr << "branch for footer" << endl;

  /*
  llvm::BranchInst::Create(footerBB, irgen->GetBasicBlock());
  irgen->SetBasicBlock(footerBB);
  */

  // saving footerBB for break statement
  Node::breakStack->push_back(footerBB);

  // saving headerBB for continue statement
  Node::continueStack->push_back(stepBB);

  // emit body
  //llvm::BranchInst::Create(bodyBB, irgen->GetBasicBlock());
  irgen->SetBasicBlock(bodyBB);
  llvm::Value *body = this->body->Emit();
  //irgen->SetBasicBlock(bodyBB);
  // check terminator instruction
  if(irgen->GetBasicBlock()->getTerminator() == NULL) {
    //cerr << "branch for step" << endl;
    llvm::BranchInst::Create(stepBB, irgen->GetBasicBlock());
  }

  irgen->SetBasicBlock(stepBB);
  llvm::Value *step = this->step->Emit();
  //cerr << "branch back to header" << endl;
  llvm::BranchInst::Create(headerBB, irgen->GetBasicBlock());

  // pop break stack
  irgen->SetBasicBlock(footerBB);
  Node::breakStack->pop_back();

  // pop continue stack
  Node::continueStack->pop_back();

  return NULL;
}

void WhileStmt::PrintChildren(int indentLevel) {
  test->Print(indentLevel + 1, "(test) ");
  body->Print(indentLevel + 1, "(body) ");
}

llvm::Value *WhileStmt::Emit() {
  //TODO Logic for this is intense
  // making the basic blocks
  llvm::LLVMContext *context = irgen->GetContext();
  llvm::BasicBlock *headerBB = llvm::BasicBlock::Create(*context, "WHILEheaderBB", irgen->GetFunction());
  llvm::BasicBlock *bodyBB = llvm::BasicBlock::Create(*context, "WHILEbodyBB", irgen->GetFunction());
  llvm::BasicBlock *footerBB = llvm::BasicBlock::Create(*context, "WHILEfooterBB", irgen->GetFunction());

  // create branch to terminate current BB and start loop header
  llvm::BranchInst::Create(headerBB, irgen->GetBasicBlock());
  irgen->SetBasicBlock(headerBB);

  // branch according to test condition
  llvm::Value *cond = this->test->Emit();
  llvm::BranchInst::Create(bodyBB, footerBB, cond, irgen->GetBasicBlock());

  // saving footerBB for break statement
  Node::breakStack->push_back(footerBB);

  // saving headerBB for continue statement
  Node::continueStack->push_back(headerBB);

  // emit body
  irgen->SetBasicBlock(bodyBB);
  llvm::Value *body = this->body->Emit();

  // check terminator instruction
  if (irgen->GetBasicBlock()->getTerminator() == NULL) {
    llvm::BranchInst::Create(headerBB, irgen->GetBasicBlock());
  }

  irgen->SetBasicBlock(footerBB);

  // pop break stack
  Node::breakStack->pop_back();

  // pop the continue stack
  Node::continueStack->pop_back();

  return NULL;
}

IfStmt::IfStmt(Expr *t, Stmt *tb, Stmt *eb) : ConditionalStmt(t, tb) {
  Assert(t != NULL && tb != NULL); // else can be NULL
  elseBody = eb;
  if(elseBody) { elseBody->SetParent(this); }
}

void IfStmt::PrintChildren(int indentLevel) {
  if(test) { test->Print(indentLevel + 1, "(test) "); }
  if(body) { body->Print(indentLevel + 1, "(then) "); }
  if(elseBody) { elseBody->Print(indentLevel + 1, "(else) "); }
}

llvm::Value *IfStmt::Emit() {
  //TODO Logic for this is intense
  llvm::Value *cond = this->test->Emit();
  llvm::LLVMContext *context = irgen->GetContext();
  llvm::BasicBlock *thenBB = llvm::BasicBlock::Create(*context, "IFthenBB", irgen->GetFunction());
  llvm::BasicBlock *elseBB = NULL;
  if(this->elseBody != NULL) {
    cerr << "[IfStmt] Made a else BB" << endl;
    elseBB = llvm::BasicBlock::Create(*context, "IFelseBB", irgen->GetFunction());
  }
  llvm::BasicBlock *footerBB = llvm::BasicBlock::Create(*context, "IFfooterBB", irgen->GetFunction());
  llvm::BranchInst::Create(thenBB, elseBody ? elseBB : footerBB, cond, irgen->GetBasicBlock());

  irgen->SetBasicBlock(thenBB);
  map<string, SymbolTable::DeclAssoc> newScope;
  Node::symtable->symTable.push_back(newScope);
  this->body->Emit();
  if(irgen->GetBasicBlock()->getTerminator() == NULL) {
    llvm::BranchInst::Create(footerBB, irgen->GetBasicBlock());
  }
  
  irgen->SetBasicBlock(footerBB); 

  if(this->elseBody != NULL) {
    irgen->SetBasicBlock(elseBB);
    map<string, SymbolTable::DeclAssoc> newScope2;
    Node::symtable->symTable.push_back(newScope2);
    this->elseBody->Emit();
    if(irgen->GetBasicBlock()->getTerminator() == NULL) {
      llvm::BranchInst::Create(footerBB, irgen->GetBasicBlock());
    }
    irgen->SetBasicBlock(footerBB);
  }

  return NULL;
}

ReturnStmt::ReturnStmt(yyltype loc, Expr *e) : Stmt(loc) {
  expr = e;
  if(e != NULL) { expr->SetParent(this); }
}

void ReturnStmt::PrintChildren(int indentLevel) {
  if(expr) {
    expr->Print(indentLevel + 1);
  }
}

llvm::Value *ReturnStmt::Emit() {
  // TODO Check the expression and perform something depending on that?
  //cerr << "ReturnEmit called" << endl;
  llvm::LLVMContext *context = irgen->GetContext();
  llvm::Value *val;
  if (this->expr != NULL) {
    val = this->expr->Emit();
    ////cerr << "expr is not null" << endl;
    ////cerr << "return type is " << val << endl;
    llvm::ReturnInst::Create(*context, val, irgen->GetBasicBlock());
  }
  else {
    ////cerr << "expr is null" << endl;
    llvm::ReturnInst::Create(*context, irgen->GetBasicBlock());
  }
  return NULL;
}

SwitchLabel::SwitchLabel(Expr *l, Stmt *s) {
  Assert(l != NULL && s != NULL);
  (label = l)->SetParent(this);
  (stmt = s)->SetParent(this);
}

SwitchLabel::SwitchLabel(Stmt *s) {
  Assert(s != NULL);
  label = NULL;
  (stmt = s)->SetParent(this);
}

void SwitchLabel::PrintChildren(int indentLevel) {
  if(label) { label->Print(indentLevel + 1); }
  if(stmt) { stmt->Print(indentLevel + 1); }
}

llvm::Value *SwitchLabel::Emit() {
  stmt->Emit();
  return NULL;
}

SwitchStmt::SwitchStmt(Expr *e, List<Stmt *> *c, Default *d) {
  Assert(e != NULL && c != NULL && c->NumElements() != 0);
  (expr = e)->SetParent(this);
  (cases = c)->SetParentAll(this);
  def = d;
  if(def) { def->SetParent(this); }
}

void SwitchStmt::PrintChildren(int indentLevel) {
  if(expr) { expr->Print(indentLevel + 1); }
  if(cases) { cases->PrintAll(indentLevel + 1); }
  if(def) { def->Print(indentLevel + 1); }
}

llvm::Value *SwitchStmt::Emit() {
  //TODO OMG What do here
  /*
    Find  all the cases /default  case  and create  BB  for each  of  them
    Emit  of  Expression
    Create  Switch  instrucCon
    For each  case
      ‘addCase’ to  Switch  instrucCon
      Emit  for statement in  case  statement
      Create  terminator  instrucCon
  */
  cerr << "Switch Emit() is called" << endl;
  vector<llvm::BasicBlock*> caseStack;
  vector<llvm::BasicBlock*> caseTerminator;
  int numOfCases = 0;
  cerr << "There are " << this->cases->NumElements() << " in the list of cases" << endl;
  llvm::LLVMContext *context = irgen->GetContext();
  llvm::BasicBlock *footerBB = llvm::BasicBlock::Create(*context, "footerBB", irgen->GetFunction());
  cerr << "pushing footer block to breakstack" << endl;
  breakStack->push_back(footerBB);

  for(int i = 0; i < this->cases->NumElements(); i++) {
    if(dynamic_cast<Case*>(this->cases->Nth(i)) != NULL) {
      cerr << "pushing case block to stack: case" << i << endl;
      llvm::BasicBlock *caseBB = llvm::BasicBlock::Create(*context, "", irgen->GetFunction());
      caseStack.push_back(caseBB);
      caseTerminator.push_back(caseBB);
      numOfCases++;
    }
    else {
      cerr << "casestack is not being populated" << endl;
    }
  }
  cerr << "Number of cases: " << numOfCases << endl;
  cerr << "pushing default block to casestack" << endl;
  llvm::BasicBlock *defaultBB = llvm::BasicBlock::Create(*context, "default", irgen->GetFunction());
  caseStack.push_back(defaultBB);
  //caseTerminator.push_back(defaultBB);
  reverse(caseStack.begin(), caseStack.end());
  reverse(caseTerminator.begin(), caseTerminator.end());
  llvm::Value *value = this->expr->Emit();
  llvm::SwitchInst *switchStmt = llvm::SwitchInst::Create(value, defaultBB, numOfCases, irgen->GetBasicBlock());
  /*
  llvm::BasicBlock *headerBB = llvm::BasicBlock::Create(*context, "headerBB", irgen->GetFunction());
  llvm::BranchInst::Create(headerBB, irgen->GetBasicBlock());
  irgen->SetBasicBlock(headerBB);
  */
  // void   addCase (ConstantInt *OnVal, BasicBlock *Dest)
  for(int i = 0; i < this->cases->NumElements(); i++) {
    cerr << "caseStack is at size " << caseStack.size() << endl;
    if(dynamic_cast<Case*>(this->cases->Nth(i)) != NULL && caseStack.size() > 0) {
      cerr << "adding case to switch" << endl;
      llvm::BasicBlock *tempBB = caseStack.back();
      llvm::ConstantInt* caseLabel = llvm::cast<llvm::ConstantInt>((((SwitchLabel*)this->cases->Nth(i))->getLabel()->Emit()));
      switchStmt->addCase(caseLabel, tempBB);
      /*
      if(irgen->GetBasicBlock()->getTerminator() == NULL) {
        llvm::BranchInst::Create(tempBB, irgen->GetBasicBlock());
      }
      */
      irgen->SetBasicBlock(tempBB);
      this->cases->Nth(i)->Emit();

      caseStack.pop_back();
      /*
      llvm::BasicBlock *nextBlock = caseStack.back();
      if(irgen->GetBasicBlock()->getTerminator() == NULL) {
        llvm::BranchInst::Create(nextBlock, irgen->GetBasicBlock());
      }
      */
    }
    else if(dynamic_cast<Default*>(this->cases->Nth(i)) != NULL && caseStack.size() == 1) {
      cerr << "Default stmt: adding default" << endl;
      llvm::BasicBlock *tempBB = caseStack.back();
      /*
      if(irgen->GetBasicBlock()->getTerminator() == NULL) {
        llvm::BranchInst::Create(tempBB, irgen->GetBasicBlock());
      }
      */
      irgen->SetBasicBlock(tempBB);
      this->cases->Nth(i)->Emit();
      caseStack.pop_back();
    }
    else {
      cerr << "Not case or default" << endl;
      this->cases->Nth(i)->Emit();
    }
  }

  while(caseTerminator.size() > 0) {
    llvm::BasicBlock *tempBB = caseTerminator.back();
    if(tempBB->getTerminator() == NULL && caseTerminator.size() == 1) {
      llvm::BranchInst::Create(defaultBB, tempBB);
      caseTerminator.pop_back();
    }
    else if(tempBB->getTerminator() == NULL && caseTerminator.size() > 0) {
      caseTerminator.pop_back();
      llvm::BasicBlock *nextBlock = caseTerminator.back();
      llvm::BranchInst::Create(nextBlock, tempBB);
    }
    else if(caseTerminator.size() > 0){
      caseTerminator.pop_back();
    }
  }

  if(defaultBB->getTerminator() == NULL) {
    llvm::BranchInst::Create(footerBB, defaultBB);
  }

  breakStack->pop_back();
  irgen->SetBasicBlock(footerBB);

  return NULL;
}

llvm::Value *Case::Emit() {
  this->stmt->Emit();
  return NULL;
}

llvm::Value *Default::Emit() {
  this->stmt->Emit();
  return NULL;
}

llvm::Value *BreakStmt::Emit() {
  llvm::BranchInst::Create (Node::breakStack->back(), irgen->GetBasicBlock ());
  //irgen->SetBasicBlock(Node::breakStack->back());
  return NULL;
}

llvm::Value *ContinueStmt::Emit() {
  llvm::BranchInst::Create (Node::continueStack->back(), irgen->GetBasicBlock ());
  return NULL;
}


/**
 * File: symtable.h
 * ----------- 
 *  Header file for Symbol table implementation.
 */

#include <stdlib.h>
#include "ast_decl.h"
#include <iostream>
#include <map>
#include <vector>
#include "ast_type.h"

using namespace std;

class SymbolTable {
 public:
  struct DeclAssoc {
      bool isGlobal;
      Decl *decl = NULL;
      llvm::Value *value = NULL;
  };

  vector< map<string, DeclAssoc> > symTable;
  int loopCounter;
  bool insideSwitch;
  bool returnIsSeen;
  Type *returnType;
  void PrintTable();
};

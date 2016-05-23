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

class Scope {
 protected:
  bool isGlobal;
  map<string, llvm::Value*> table;
};

class SymbolTable {
  List<Scope*> scopes;
  int loopCounter;
  bool insideSwitch;
  bool returnIsSeen;
  Type *returnType;
  void PrintTable();
};

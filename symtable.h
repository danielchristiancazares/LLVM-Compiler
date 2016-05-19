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
		vector< map<string, Decl*> > symTable;
		int loopCounter;
		bool insideSwitch;
		bool returnIsSeen;
		Type *returnType;
		void PrintTable();

		SymbolTable() {
			loopCounter = 0;
		}
};
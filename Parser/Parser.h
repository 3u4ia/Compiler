#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "../TokensAndStates/TokensAndStates.h"
#include "../TreeClass/TreeNodeStruct.h"
#include "../ScannerClass/Scanner.h"
#include "../TreeClass/Tree.h"


using namespace std;

class Parser {
	private:
		Token tk;
		char *fileName;
		Scanner scanner;
		TreeNode *program();
		TreeNode *vars();
		TreeNode *varList();
		TreeNode *block();
		TreeNode *stats();
		TreeNode *mStat();
		TreeNode *stat();
		TreeNode *read();
		TreeNode *print();
		TreeNode *cond();
		TreeNode *loop();
		TreeNode *assign();
		TreeNode *relational();
		TreeNode *exp();
		TreeNode *m();
		TreeNode *n();
		TreeNode *r();
		void errorHandler(const char*);
		void keywordHandler(const char*);
		

	public:
		Parser(char *fileName, const char *textBuffer): fileName(fileName), scanner(textBuffer) {
		}

		void parse();

		
};











#endif

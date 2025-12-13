#include <stdio.h>
#include "Parser.h"

//Token tk;
//Scanner scanner;
//struct Token {
//	TokenID tokenID;
//	char lexeme[9];
//	int lineNum;
//};


//struct TreeNode {
//	int label;
//	Token token;
//	TreeNode *left;
//	TreeNode *middle;
//	TreeNode *right;
//};
static const Token initTk = {OPTK, "", -1};



void Parser::parse() {
	tk = scanner.scanToken();
	Tree treeObj(fileName, program());
	//program();
	if (tk.tokenID != EOFTK) {
		printf("ERROR EOF NOT REACHED\n");
		exit(1);
	}
	else {
		printf("Success in parsing!\n");
	}
	treeObj.displayPreOrder();
	treeObj.processNode();
	return;
}



TreeNode* Parser::program() {
	TreeNode *programNode = new TreeNode;
	Token initTk = {OPTK, "", -1};
	programNode->label = PROGRAM;
	programNode->tokenArr[0] = initTk;
	programNode->tokenArr[1] = initTk;
	programNode->tokenArr[2] = initTk;
	programNode->nodeArr[0] = nullptr;
	programNode->nodeArr[1] = nullptr;
	programNode->nodeArr[2] = nullptr;


	if (tk.tokenID == KEYWORD) {
		if (strcmp(tk.lexeme, "go") == 0) {
			tk = scanner.scanToken();
		}
		else {
			errorHandler("go lexeme");
		}
	} else {
		errorHandler(tokenNames[KEYWORD - 1000]);
	}
	programNode->nodeArr[0] = vars();
	programNode->nodeArr[1] = block();
	if(tk.tokenID == KEYWORD) {
		if (strcmp(tk.lexeme, "exit") == 0) {
			tk = scanner.scanToken();
		}
		else {
			errorHandler("exit lexeme");
		}
	}
	else {
		errorHandler("exit");
	}
	return programNode;

}
//struct Token {
//	TokenID tokenID;
//	char lexeme[9];
//	int lineNum;
//};
//
//struct TreeNode {
//	int label;
//	Token tokenArr[3];
//	TreeNode *nodeArr[3];
//};


TreeNode* Parser::vars() { // LIKELY INCORRECT DUE TO NOT BEING DEEPLY NESTED
	TreeNode *varsNode = new TreeNode;
	varsNode->label = VARS;
	varsNode->tokenArr[0] = initTk;
	varsNode->tokenArr[1] = initTk;
	varsNode->tokenArr[2] = initTk;
	varsNode->nodeArr[0] = nullptr;
	varsNode->nodeArr[1] = nullptr;
	varsNode->nodeArr[2] = nullptr;

	if(tk.tokenID == KEYWORD) {
		if(strcmp(tk.lexeme, "int") == 0) {
			tk = scanner.scanToken();
			if(tk.tokenID == IDTK) {
				varsNode->tokenArr[0] = tk;
				tk = scanner.scanToken();
			} else {
				errorHandler(tokenNames[IDTK - 1000]);
			}
			if(tk.tokenID == ASSIGNOPTK) {
				tk = scanner.scanToken();
			}
			else {
				errorHandler(tokenNames[ASSIGNOPTK - 1000]);
			}
			if(tk.tokenID == NUMTK) {
				varsNode->tokenArr[1] = tk;
				tk = scanner.scanToken();
			}
			else {
				errorHandler(tokenNames[NUMTK - 1000]);
			}
			varsNode->nodeArr[0] = varList();
			if(tk.tokenID == DELOPTK) {
				tk = scanner.scanToken();
				return varsNode;
			}
			else {
				errorHandler(tokenNames[DELOPTK - 1000]);
			}
		}
		return varsNode; // for the empty case
	}
	return varsNode; // for the empty case
}

TreeNode* Parser::varList() {
	TreeNode *varListNode = new TreeNode;
	varListNode->label = VARLIST;
	varListNode->tokenArr[0] = initTk;
	varListNode->tokenArr[1] = initTk;
	varListNode->tokenArr[2] = initTk;
	varListNode->nodeArr[0] = nullptr;
	varListNode->nodeArr[1] = nullptr;
	varListNode->nodeArr[2] = nullptr;

	if(tk.tokenID == IDTK) {
		varListNode->tokenArr[0] = tk;
		tk = scanner.scanToken();
		if(tk.tokenID == ASSIGNOPTK) {
			tk = scanner.scanToken();
		} else {
			errorHandler(tokenNames[ASSIGNOPTK - 1000]);
		}
		if(tk.tokenID == NUMTK) {
			varListNode->tokenArr[1] = tk;
			tk = scanner.scanToken();
			varListNode->nodeArr[0] = varList();
			return varListNode;
		} else {
			errorHandler(tokenNames[NUMTK - 1000]);
		}
	} else {
		return varListNode;
	}
}

TreeNode* Parser::block() {
	TreeNode *blockNode = new TreeNode;
	blockNode->label = BLOCK;
	blockNode->tokenArr[0] = initTk;
	blockNode->tokenArr[1] = initTk;
	blockNode->tokenArr[2] = initTk;
	blockNode->nodeArr[0] = nullptr;
	blockNode->nodeArr[1] = nullptr;
	blockNode->nodeArr[2] = nullptr;

	if (tk.tokenID == LFTCURLYDELIM) {
		tk = scanner.scanToken();
		blockNode->nodeArr[0] = vars();
		blockNode->nodeArr[1] = stats();
		if(tk.tokenID == RGHTCURLYDELIM) {
			tk = scanner.scanToken();
			return blockNode;
		} else {
			errorHandler(tokenNames[RGHTCURLYDELIM - 1000]);
		}
	} else {
		errorHandler(tokenNames[LFTCURLYDELIM - 1000]);
	}
}

TreeNode* Parser::stats() {
	TreeNode *statsNode = new TreeNode;
	statsNode->label = STATS;
	statsNode->tokenArr[0] = initTk;
	statsNode->tokenArr[1] = initTk;
	statsNode->tokenArr[2] = initTk;
	statsNode->nodeArr[0] = nullptr;
	statsNode->nodeArr[1] = nullptr;
	statsNode->nodeArr[2] = nullptr;

	statsNode->nodeArr[0] = stat();
	statsNode->nodeArr[1] = mStat();
	return statsNode;
}

TreeNode* Parser::mStat() {
	TreeNode *mStatNode = new TreeNode;
	mStatNode->label = MSTAT;
	mStatNode->tokenArr[0] = initTk;
	mStatNode->tokenArr[1] = initTk;
	mStatNode->tokenArr[2] = initTk;
	mStatNode->nodeArr[0] = nullptr;
	mStatNode->nodeArr[1] = nullptr;
	mStatNode->nodeArr[2] = nullptr;

	if(tk.tokenID == KEYWORD) {
		if(strcmp(tk.lexeme, "scan") == 0 ||
			strcmp(tk.lexeme, "output") == 0 ||
			strcmp(tk.lexeme, "cond") == 0 ||
			strcmp(tk.lexeme, "loop") == 0 ||
			strcmp(tk.lexeme, "set") == 0) {
			

			mStatNode->nodeArr[0] = stat();
			mStatNode->nodeArr[1] = mStat();
			return mStatNode;
		}
	} else if(tk.tokenID == LFTCURLYDELIM) {
		mStatNode->nodeArr[0] = stat();
		mStatNode->nodeArr[1] = mStat();
		return mStatNode;
	}


	return mStatNode;
}

TreeNode* Parser::stat() {
	TreeNode *statNode = new TreeNode;
	statNode->label = STAT;
	statNode->tokenArr[0] = initTk;
	statNode->tokenArr[1] = initTk;
	statNode->tokenArr[2] = initTk;
	statNode->nodeArr[0] = nullptr;
	statNode->nodeArr[1] = nullptr;
	statNode->nodeArr[2] = nullptr;

	if(tk.tokenID == KEYWORD) {
		if(strcmp(tk.lexeme, "scan") == 0) {
			statNode->nodeArr[0] = read();
			return statNode;
		} else if(strcmp(tk.lexeme, "output") == 0) {
			statNode->nodeArr[0] = print();
			return statNode;
		} else if(strcmp(tk.lexeme, "cond") == 0) {
			statNode->nodeArr[0] = cond();
			return statNode;
		} else if(strcmp(tk.lexeme, "loop") == 0) {
			statNode->nodeArr[0] = loop();
			return statNode;
		} else if(strcmp(tk.lexeme, "set") == 0) {
			statNode->nodeArr[0] = assign();
			return statNode;
		} else {
			errorHandler("scan, output, cond, loop or set");
		}
			
	} 
	else if(tk.tokenID == LFTCURLYDELIM) {
		statNode->nodeArr[0] = block();
		return statNode;
	}
	else {
		errorHandler("scan, output, cond, loop, set or {");
	}


}

TreeNode* Parser::read() {
	TreeNode *readNode = new TreeNode;
	readNode->label = READ;
	readNode->tokenArr[0] = initTk;
	readNode->tokenArr[1] = initTk;
	readNode->tokenArr[2] = initTk;
	readNode->nodeArr[0] = nullptr;
	readNode->nodeArr[1] = nullptr;
	readNode->nodeArr[2] = nullptr;

	if(tk.tokenID == KEYWORD) {
		if(strcmp(tk.lexeme, "scan") == 0) {
			tk = scanner.scanToken();
		} else {
			errorHandler("scan");
		}
		if(tk.tokenID == IDTK) {
			readNode->tokenArr[0] = tk;
			tk = scanner.scanToken();

		} else {
			errorHandler(tokenNames[IDTK - 1000]);
		}
		if(tk.tokenID == DELOPTK) {
			tk = scanner.scanToken();
			return readNode;
		} else {
			errorHandler(tokenNames[DELOPTK - 1000]);
		}


	} else {
		errorHandler(tokenNames[KEYWORD - 1000]);
	}
}



TreeNode* Parser::print() {
	TreeNode *printNode = new TreeNode;
	printNode->label = PRINT;
	printNode->tokenArr[0] = initTk;
	printNode->tokenArr[1] = initTk;
	printNode->tokenArr[2] = initTk;
	printNode->nodeArr[0] = nullptr;
	printNode->nodeArr[1] = nullptr;
	printNode->nodeArr[2] = nullptr;


	if(tk.tokenID == KEYWORD) {
		if(strcmp(tk.lexeme, "output") == 0) {
			tk = scanner.scanToken();
			printNode->nodeArr[0] = exp();

		}
		else {
			errorHandler("output");
		}	
	} else {
		errorHandler(tokenNames[KEYWORD - 1000]);
	}
	if(tk.tokenID == DELOPTK) {
		tk = scanner.scanToken();
		return printNode;
	} else {
		errorHandler(tokenNames[DELOPTK - 1000]);
	}

}


TreeNode* Parser::cond() {
	TreeNode *condNode = new TreeNode;
	condNode->label = COND;
	condNode->tokenArr[0] = initTk;
	condNode->tokenArr[1] = initTk;
	condNode->tokenArr[2] = initTk;
	condNode->nodeArr[0] = nullptr;
	condNode->nodeArr[1] = nullptr;
	condNode->nodeArr[2] = nullptr;


	if(tk.tokenID == KEYWORD) {
		if(strcmp(tk.lexeme, "cond") == 0) {
			tk = scanner.scanToken();

		} else {
			errorHandler("cond");
		}
	} else {
		errorHandler(tokenNames[KEYWORD - 1000]);
	}

	if(tk.tokenID == LFTSQREDELIM) {
		tk = scanner.scanToken();
	} else {
		errorHandler(tokenNames[LFTSQREDELIM - 1000]);
	}

	if(tk.tokenID == IDTK) {
		condNode->tokenArr[0] = tk;
		tk = scanner.scanToken();
		condNode->nodeArr[0] = relational();
		condNode->nodeArr[1] = exp();
	} else {
		errorHandler(tokenNames[IDTK - 1000]);
	}

	if(tk.tokenID == RGHTSQREDELIM) {
		tk = scanner.scanToken();
		condNode->nodeArr[2] = stat();
		return condNode;
	} else {
		errorHandler(tokenNames[RGHTSQREDELIM - 1000]);
	}



}


TreeNode* Parser::loop() {
	TreeNode *loopNode = new TreeNode;
	loopNode->label = LOOP;
	loopNode->tokenArr[0] = initTk;
	loopNode->tokenArr[1] = initTk;
	loopNode->tokenArr[2] = initTk;
	loopNode->nodeArr[0] = nullptr;
	loopNode->nodeArr[1] = nullptr;
	loopNode->nodeArr[2] = nullptr;


	if(tk.tokenID == KEYWORD) {
		if(strcmp(tk.lexeme, "loop") == 0) {
			tk = scanner.scanToken();
		} else {
			errorHandler("loop");
		}
	} else {
		errorHandler(tokenNames[KEYWORD - 1000]);
	}
	if(tk.tokenID == LFTSQREDELIM) {
		tk = scanner.scanToken();
	} else {
		errorHandler(tokenNames[LFTSQREDELIM - 1000]);
	}
	if(tk.tokenID == IDTK) {
		loopNode->tokenArr[0] = tk;
		tk = scanner.scanToken();
		loopNode->nodeArr[0] = relational();
		loopNode->nodeArr[1] = exp();
	} else {
		errorHandler(tokenNames[IDTK - 1000]);
	}
	if(tk.tokenID == RGHTSQREDELIM) {
		tk = scanner.scanToken();
		loopNode->nodeArr[2] = stat();
		return loopNode;
	} else {
		errorHandler(tokenNames[RGHTSQREDELIM - 1000]);
	}



}


TreeNode* Parser::assign() {
	TreeNode *assignNode = new TreeNode;
	assignNode->label = ASSIGN;
	assignNode->tokenArr[0] = initTk;
	assignNode->tokenArr[1] = initTk;
	assignNode->tokenArr[2] = initTk;
	assignNode->nodeArr[0] = nullptr;
	assignNode->nodeArr[1] = nullptr;
	assignNode->nodeArr[2] = nullptr;


	if(tk.tokenID == KEYWORD) {
		if(strcmp(tk.lexeme, "set") == 0) {
			tk = scanner.scanToken();

		} else {
			errorHandler("set");
		}
	} else {
		errorHandler(tokenNames[KEYWORD - 1000]);
	}

	if(tk.tokenID == IDTK) {
		assignNode->tokenArr[0] = tk;
		tk = scanner.scanToken();
	} else {
			errorHandler(tokenNames[IDTK - 1000]);
	}

	if(tk.tokenID == ASSIGNOPTK) {
		tk = scanner.scanToken();
		assignNode->nodeArr[0] = exp();
	} else {
		errorHandler(tokenNames[ASSIGNOPTK - 1000]);
	}
	if(tk.tokenID == DELOPTK) {
		tk = scanner.scanToken();
		return assignNode;
	} else {
		errorHandler(tokenNames[DELOPTK - 1000]);
	}



}

TreeNode* Parser::relational() {
	TreeNode *relationalNode = new TreeNode;
	relationalNode->label = RELATIONAL;
	relationalNode->tokenArr[0] = initTk;
	relationalNode->tokenArr[1] = initTk;
	relationalNode->tokenArr[2] = initTk;
	relationalNode->nodeArr[0] = nullptr;
	relationalNode->nodeArr[1] = nullptr;
	relationalNode->nodeArr[2] = nullptr;


	if(tk.tokenID == LEOPTK) {
		relationalNode->tokenArr[0] = tk;
		tk = scanner.scanToken();
		return relationalNode;
	} else if(tk.tokenID == GEOPTK) {
		relationalNode->tokenArr[0] = tk;
		tk = scanner.scanToken();
		return relationalNode;
	} else if(tk.tokenID == LTOPTK) {
		relationalNode->tokenArr[0] = tk;
		tk = scanner.scanToken();
		return relationalNode;
	} else if(tk.tokenID == EQOPTK) {
		relationalNode->tokenArr[0] = tk;
		tk = scanner.scanToken();
		return relationalNode;
	} else if(tk.tokenID == ASSIGNOPTK) {		//SHOULD I ADD THE TOKEN TO THE NODE HERE????
		relationalNode->tokenArr[0] = tk;
		tk = scanner.scanToken();

		if(tk.tokenID == ASSIGNOPTK) {
			relationalNode->tokenArr[1] = tk;
			tk = scanner.scanToken();
			return relationalNode;
		} else {
			errorHandler(tokenNames[ASSIGNOPTK - 1000]);
		}
	} else {
		errorHandler("Any relational token");
	}
}

TreeNode* Parser::exp() {
	TreeNode *expNode = new TreeNode;
	expNode->label = EXP;
	expNode->tokenArr[0] = initTk;
	expNode->tokenArr[1] = initTk;
	expNode->tokenArr[2] = initTk;
	expNode->nodeArr[0] = nullptr;
	expNode->nodeArr[1] = nullptr;
	expNode->nodeArr[2] = nullptr;


	expNode->nodeArr[0] = m();
	if(tk.tokenID == DBLESTAR) {
		expNode->tokenArr[0] = tk;
		tk = scanner.scanToken();
		expNode->nodeArr[1] = exp();
		return expNode;
	} else if (tk.tokenID == DBLESLASH) {
		expNode->tokenArr[0] = tk;
		tk = scanner.scanToken();
		expNode->nodeArr[1] = exp();
		return expNode;
	} else {
		return expNode;
	}
}


TreeNode* Parser::m() {
	TreeNode *mNode = new TreeNode;
	mNode->label = M;
	mNode->tokenArr[0] = initTk;
	mNode->tokenArr[1] = initTk;
	mNode->tokenArr[2] = initTk;
	mNode->nodeArr[0] = nullptr;
	mNode->nodeArr[1] = nullptr;
	mNode->nodeArr[2] = nullptr;


	mNode->nodeArr[0] = n();
	if(tk.tokenID == OPTK) {
		if(strcmp(tk.lexeme, "+") == 0) {
			mNode->tokenArr[0] = tk;
			tk = scanner.scanToken();
			mNode->nodeArr[1] = m();
			return mNode;
		} else {
			return mNode; // In case M -> N opt
		}
	} else {
		return mNode;
	}
}


TreeNode* Parser::n() {
	TreeNode *nNode = new TreeNode;
	nNode->label = N;
	nNode->tokenArr[0] = initTk;
	nNode->tokenArr[1] = initTk;
	nNode->tokenArr[2] = initTk;
	nNode->nodeArr[0] = nullptr;
	nNode->nodeArr[1] = nullptr;
	nNode->nodeArr[2] = nullptr;


	if(tk.tokenID == OPTK) {
		if(strcmp(tk.lexeme, "-") == 0) {
			nNode->tokenArr[0] = tk;
			tk = scanner.scanToken();
			nNode->nodeArr[0] = n();
			return nNode;
		}	
	}

	nNode->nodeArr[0] = r();
	
	if(tk.tokenID == OPTK) {
		if(strcmp(tk.lexeme, "-") == 0) {
			nNode->tokenArr[0] = tk;
			tk = scanner.scanToken();
			nNode->nodeArr[1] = n();
			return nNode;
		} else {
			return nNode; // In case N -> R option
		}
	} else {
		return nNode; // In case N -> R option
	}
	
}

TreeNode* Parser::r() {
	TreeNode *rNode = new TreeNode;
	rNode->label = R;
	rNode->tokenArr[0] = initTk;
	rNode->tokenArr[1] = initTk;
	rNode->tokenArr[2] = initTk;
	rNode->nodeArr[0] = nullptr;
	rNode->nodeArr[1] = nullptr;
	rNode->nodeArr[2] = nullptr;


	if(tk.tokenID == LFTPARENDELIM) {
		tk = scanner.scanToken();
		rNode->nodeArr[0] = exp();
		if(tk.tokenID == RGHTPARENDELIM) {
			tk = scanner.scanToken();
			return rNode;
		} else {
			errorHandler(tokenNames[RGHTPARENDELIM - 1000]);
		}
	} else if(tk.tokenID == IDTK) {
		rNode->tokenArr[0] = tk;
		tk = scanner.scanToken();
		return rNode;
	} else if(tk.tokenID == NUMTK) {
		rNode->tokenArr[0] = tk;
		tk = scanner.scanToken();
		return rNode;
	} else {
		errorHandler("(, IDTK or NUMTK");
	}
}



[[noreturn]] void Parser::errorHandler(const char *expectedTk) {
	printf("Error: %s expected but received %s with lexeme %s on line %d\n", expectedTk, tokenNames[tk.tokenID - 1000], tk.lexeme, tk.lineNum); 
	exit(1);
}

































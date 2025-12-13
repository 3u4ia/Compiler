#ifndef TREENODESTRUCT_H 
#define TREENODESTRUCT_H
#include "../TokensAndStates/TokensAndStates.h"

//struct Token {
//	TokenID tokenID = IDTK;
//	char lexeme[9] = {0};
//	int lineNum = 0;
//};

struct TreeNode {
	int label;
	Token tokenArr[3];
	TreeNode *nodeArr[3];
};
#endif

#include "Tree.h"

typedef enum {VAR, LABEL} nameType;
static unsigned int LabelCntr = 0;
static unsigned int VarCntr = 0;
static char Name[20]; /* for creation of unique names*/

void Tree::fileInitHelper(FILE **filePtr, const char *extension) {
	size_t newSize = strlen(baseFileName) + strlen(extension);
	char *fullFileName = (char *)malloc(newSize + 1);
	strcpy(fullFileName, baseFileName);
	strcat(fullFileName, extension);
	*filePtr = fopen(fullFileName, "w");
	if(!*filePtr) {
		perror("fopen failed");
	}
	free(fullFileName);
}



void Tree::displayPreOrder(TreeNode *nodePtr, size_t depth) const {
	if(nodePtr) {
		for(size_t i = 0; i < depth * 2; i++) fprintf(preOrderFile, " ");
		int tokenID = -1;
		fprintf(preOrderFile, "%s", nonTerminalNames[nodePtr->label]);
		for(size_t i = 0; i < 3; i++) {
			tokenID = nodePtr->tokenArr[i].tokenID;
			if (nodePtr->tokenArr[i].lineNum != -1) {
				fprintf(preOrderFile, " %s:%s:%d", tokenNames[tokenID - 1000], nodePtr->tokenArr[i].lexeme, nodePtr->tokenArr[i].lineNum);
			} else {
				break;
			}
		}
		fprintf(preOrderFile, "\n");
		for(int i = 0; i < 3; i++) {
			displayPreOrder(nodePtr->nodeArr[i], depth+1);
		}

	}
}


void Tree::processNode(TreeNode *nodePtr) {
	if(!nodePtr) {
		return;
	}

	switch(nodePtr->label) {
		case VARS:
			handleVars(nodePtr);
			break;
		case READ:
			handleVarUsage(nodePtr);
			break;
		case COND:
			handleVarUsage(nodePtr);
			break;
		case LOOP:
			handleVarUsage(nodePtr);
			break;
		case ASSIGN:
			handleVarUsage(nodePtr);
			break;
		case R:
			handleVarUsage(nodePtr);
			break;
		default:
			break;
	}

	for(int i = 0; i < 3; i++) {
		processNode(nodePtr->nodeArr[i]);
	}
}

void Tree::handleVarUsage(TreeNode *nodePtr) {
	if(!nodePtr){
		printf("handleCarUsage: nodePtr is null and it shouldn't be\n");
		exit(1);
	}
	if(nodePtr->tokenArr[0].tokenID != IDTK) {
		if(nodePtr->label != R) { // Because R's identifier is an option not a requirement.
			printf("handleVarUsage: token %s isn't IDTK when it should be on line %d\n\tCurrentNode: %s\n", nodePtr->tokenArr[0].lexeme, nodePtr->tokenArr[0].lineNum, nonTerminalNames[nodePtr->label]);
			exit(1);
		}
	}

	if(nodePtr->tokenArr[0].tokenID == IDTK) {
		if(!apiObj.verify(nodePtr->tokenArr[0].lexeme)) {
			printf("ERROR: token %s instance %s was not previously defined on line number %d\n", tokenNames[nodePtr->tokenArr[0].tokenID - 1000], nodePtr->tokenArr[0].lexeme, nodePtr->tokenArr[0].lineNum);
			exit(1);

		}
	}
}



void Tree::handleVars(TreeNode *varNode) {
	if(varNode->tokenArr[0].tokenID == IDTK) {
		apiObj.insert(varNode->tokenArr[0].lexeme);
	}
	TreeNode *varListNode = varNode->nodeArr[0];
	handleVarList(varListNode);	
}


void Tree::handleVarList(TreeNode *varListNode) {
	if(!varListNode) {
		return;
	}
	if(varListNode->tokenArr[0].lineNum == -1) {
		return;
	}
	else {
		apiObj.insert(varListNode->tokenArr[0].lexeme);
		handleVarList(varListNode->nodeArr[0]);
	}

}


void Tree::generateCode(TreeNode *nodePtr) {
	char argR[20]; // local storage for temp or label
	if(!nodePtr) {
		return;
	}

	switch(nodePtr->label) {
		case READ:
			fprintf(asmFile, "READ %s\n", nodePtr->tokenArr[0].lexeme);
			break;
		case PRINT:
			generateCode(nodePtr->nodeArr[0]);
			argR = newName(VAR);
			fprintf(asmFile, "STORE t%s\n", argR);
			fprintf(asmFile, "WRITE t%s\n", argR);
			break;
		case COND:
			generateCode(nodePtr->nodeArr[1]);
			argR = newName(VAR);
			fprintf(asmFile, "\tSTORE \t%s\n", argR);
			generateCode(nodePtr->nodeArr[0]);
			fprintf(asmFile, "SUB %s\n");	
			char *compOp = getRelationalOp(nodePtr);
			
			//cond [ identifier <relational> <exp> ] <stat>
			break;
		case ASSIGN:
			generateCode(nodePtr->nodeArr[0]);
			fprintf(out, "\tSTORE \t%s\n", nodePtr->tokenArr[0].lexeme);
			break;
		default:
			printf("Visiting %s\n", nonTerminalNames[nodePtr->label]);
			break;
	}

	for(int i = 0; i < 3; i++) {
		generateCode(nodePtr->nodeArr[i]);
	}
}

char *getRelationalOp(TreeNode *nodePtr) {
	TreeNode *relationalNode = nodePtr->nodeArr[0];
	char *s = malloc(10 * sizeof(char));
	Token *firstToken = relationalNode->tokenArr[0];

	// There is only one production that has two tokens for a relational and that's = =
	if(firstToken.tokenID == ASSIGNOPTK && relationalNode->tokenArr[1].tokenID == ASSIGNOPTK) {
		strcpy(s, "BRZERO");
		return;
	}
	else if (firstToken.tokenID == LEOPTK) {
		strcpy(s, "BRZNEG");
		return;
	}
	else if (firstToken.tokenID == GEOPTK) {
		strcpy(s, "BRZPOS");
		return;
	} else if (firstToken.tokenID == 
}



void Tree::handleDef(TreeNode *varNode) {
	/*if(varNode->tokenArr[0].tokenID == IDTK) {
		apiObj.insert(varNode->tokenArr[0].lexeme);
	}
	*/
	TreeNode *varListNode = varNode->nodeArr[0];
	
	handleDefVarList(varListNode);	
}


void Tree::handleDefVarList(TreeNode *varListNode) {
	if(!varListNode) {
		return;
	}
	if(varListNode->tokenArr[0].lineNum == -1) {
		return;
	}
	else {
		handleDefVarList(varListNode->nodeArr[0]);
	}

}



static char *newName(nameType what) {
	if (what == VAR) // creating new temporary
		sprintf(Name, "t%d", VarCntr++); /* generate a new label as T0, T1, etc*/
	else // creating new label
		sprintf(Name, "L%d", LabelCntr++); /* new labels as L0, L1, etc */
	return(Name);
}






























#include "Tree.h"

static unsigned int LabelCntr = 0;
static unsigned int VarCntr = 0;
static char Name[20]; /* for creation of unique names*/

using namespace std;

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
	int nameIndex;
	if(!nodePtr) {
		return;
	}

	switch(nodePtr->label) {
		case READ:
			fprintf(asmFile, "READ %s\n", nodePtr->tokenArr[0].lexeme);
			break;
		case PRINT:
			generateCode(nodePtr->nodeArr[0]);
			nameIndex = newName(VAR);
			printf("storing and writing\n");
			fprintf(asmFile, "STORE \t%s\n", tempVarVec.at(nameIndex));
			fprintf(asmFile, "WRITE \t%s\n", tempVarVec.at(nameIndex));
			return;
		//case BLOCK:
			//generateCode(nodePtr->nodeArr[0]);
		//	generateCode(nodePtr->nodeArr[1]);
		//	break;
		case ASSIGN:// DONE
			generateCode(nodePtr->nodeArr[0]);
			fprintf(asmFile, "\tSTORE \t%s\n", nodePtr->tokenArr[0].lexeme);
			break;
		case EXP:
			expHandler(nodePtr);
			break;
		case M:
			mHandler(nodePtr);
			break;
		case N:
			nHandler(nodePtr);
			break;
		case R:
			rHandler(nodePtr);
			break;
		case COND:
			generateCode(nodePtr->nodeArr[1]);
			nameIndex = newName(VAR);
			fprintf(asmFile, "\tSTORE \t%s\n", tempVarVec.at(nameIndex));
			generateCode(nodePtr->nodeArr[0]);
			fprintf(asmFile, "SUB %s\n", tempVarVec.at(nameIndex));
			printRelationalOp(nodePtr, tempVarVec.at(nameIndex)); // maybe need to set argR to a temp variable so that the label can be placed after the statement
			generateCode(nodePtr->nodeArr[2]);
			fprintf(asmFile, "%s: NOOP", tempVarVec.at(nameIndex)); // may have error because of the comment right above this one
			
			//cond [ identifier <relational> <exp> ] <stat>
			break;
		//case STAT: // DONE
		//	generateCode(nodePtr->nodeArr[0]); // NONE OF THIS IS NEEDED BECAUSE IT WILL GO AND CALL IT ANYWAY AFTER THE SWITCH CASE 
		//	break;
		default:
			printf("Visiting %s\n", nonTerminalNames[nodePtr->label]);
			break;
	}

	for(int i = 0; i < 3; i++) {
		generateCode(nodePtr->nodeArr[i]);
	}
}

void Tree::rHandler(TreeNode *nodePtr) {
	Token firstToken = nodePtr->tokenArr[0];
	if(firstToken.tokenID == LFTPARENDELIM) {
		//generateCode(nodePtr->nodeArr[0]);
		fprintf(asmFile, "(exp) still in prog\n");
	} 
	else if (firstToken.tokenID == IDTK) {
		fprintf(asmFile, "LOAD %s\n", firstToken.lexeme);
	}
	else if (firstToken.tokenID == NUMTK) {
		//strcpy(argR, newName(VAR));
		printf("WRITING LOAD\n");
		fprintf(asmFile, "LOAD %s\n", firstToken.lexeme);
	}
}

void Tree::nHandler(TreeNode *nodePtr) {
	Token firstToken = nodePtr->tokenArr[0];

	if(firstToken.tokenID == OPTK && strcmp(firstToken.lexeme, "-") == 0) {

		if(nodePtr->nodeArr[1]) {
			fprintf(asmFile, "something - something in prog\n");
		} else {
			fprintf(asmFile, "-something in prog\n");
		}
	}
}


void Tree::mHandler(TreeNode *nodePtr) {
	Token firstToken = nodePtr->tokenArr[0];

	if(firstToken.tokenID == OPTK && strcmp(firstToken.lexeme, "+") == 0) {
		fprintf(asmFile, "+ in prog\n");
	}
	else {
		return;
	}
}


void Tree::expHandler(TreeNode *nodePtr) {
	Token firstToken = nodePtr->tokenArr[0];

	if(firstToken.tokenID == DBLESTAR) {
		fprintf(asmFile, "DBLESTR in prog\n");
	}
	else if(firstToken.tokenID == DBLESLASH) {
		fprintf(asmFile, "DBLESLAHS in prog\n");
	}
	else {
		return;
	}
}

void Tree::printRelationalOp(TreeNode *nodePtr, char *argR) {
	string str;
	int indexOfName;
	TreeNode *relationalNode = nodePtr->nodeArr[0];
	Token firstToken = relationalNode->tokenArr[0];
	//argR = newName(LABEL);
	indexOfName = newName(LABEL);
	

	// There is only one production that has two tokens for a relational and that's = =
	if(firstToken.tokenID == ASSIGNOPTK && relationalNode->tokenArr[1].tokenID == ASSIGNOPTK) {
		fprintf(asmFile, "BRNEG %s\nBRPOS %s", tempLabelVec.at(indexOfName), tempLabelVec.at(indexOfName));
	}
	else if (firstToken.tokenID == LEOPTK) {
		fprintf(asmFile, "BRPOS %s\n", tempLabelVec.at(indexOfName));
	}
	else if (firstToken.tokenID == GEOPTK) {
		fprintf(asmFile, "BRNEG %s\n", tempLabelVec.at(indexOfName));
	}
	else if (firstToken.tokenID == LTOPTK) {
		fprintf(asmFile, "BRZPOS %s\n", tempLabelVec.at(indexOfName));
	}
	else if (firstToken.tokenID == GTOPTK) {
		fprintf(asmFile, "BRZNEG %s\n", tempLabelVec.at(indexOfName));
	}
	else if (firstToken.tokenID == SEMICOLON) { // how should i go about !=???
		fprintf(asmFile, "BRZERO %s\n", tempLabelVec.at(indexOfName)); // NEXT PART IS FIGURING OUT IF I SHOULD/HOW TO EVALUATE THE STMT
	}
	
	return;
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






int Tree::newName(nameType what) {
	if (what==VAR) // creating new temporary
		sprintf(Name,"t%d",VarCntr++); /* generate a new label as T0, T1, etc */
	else // creating new Label
		sprintf(Name,"L%d",LabelCntr++); /* new lables as L0, L1, etc */

	char *nameCopy = new char[strlen(Name) + 1];
	strcpy(nameCopy, Name);

	if(what == VAR) {
		tempVarVec.push_back(nameCopy);
		return tempVarVec.size() - 1;
	}
	else {
		tempLabelVec.push_back(nameCopy);
		return tempLabelVec.size() - 1;
	}
}



















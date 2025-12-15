#ifndef TREE_H
#define TREE_H
#include "TreeNodeStruct.h"
#include "../TokensAndStates/TokensAndStates.h"
#include "../STAPI/STAPI.h"
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <string.h>

class Tree {
	private:
		TreeNode *root;
		STAPI apiObj;
		const char *baseFileName;
		FILE *preOrderFile = NULL;
		FILE *asmFile = NULL;
		void displayPreOrder(TreeNode *, size_t) const;
		int smallChange = 0;
		void fileInitHelper(FILE **, const char *);
		void processNode(TreeNode *);
		void generateCode(TreeNode *);

		void handleVarUsage( TreeNode *);
		void handleVars(TreeNode *);
		void handleVarList(TreeNode *);

		void handleDef(TreeNode *);
		void handleDefVarList(TreeNode *);
		

		
	public:
		Tree(char *fileName) {
			root = nullptr;
			if(fileName == nullptr){
				baseFileName = "out";
			} else {
				baseFileName = fileName; // Allocating new memory and copying the contents of the str
			}
			
			fileInitHelper(&preOrderFile, ".preorder");
			fileInitHelper(&asmFile, ".asm");
		}
		Tree(char *fileName, TreeNode *root) {
			this->root = root;
			if(fileName == nullptr) {
				baseFileName = "out";
			} else {
				baseFileName = fileName;
			}

			fileInitHelper(&preOrderFile, ".preorder");
			fileInitHelper(&asmFile, ".asm");
		}
		~Tree() {
			if(preOrderFile) {
				fclose(preOrderFile);
			}
		}

		void displayPreOrder()const {
			displayPreOrder(root, 0);
		}

		void processNode() {
			processNode(root);
			apiObj.checkVars();
			apiObj.displayVarCount();
			
		}

		void generateCode() {
			generateCode(root);
			fprintf(asmFile, "STOP\n");
		}

		
};


#endif


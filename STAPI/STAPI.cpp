#include "STAPI.h"
#include <string.h>
//vector<char *> varVec;	

/*
struct STAPIStruct {
	char *var
	int varUse = 0;
}

*/

void STAPI::insert(char *var) {
	for (auto it = varVec.begin(); it != varVec.end(); ++it) {
		if(strcmp(var, it->var) == 0){
			printf("ERROR: identifier %s is already defined\n", it->var);
			exit(1);
		}
	}
	varVec.push_back(STAPIStruct{var, 0});
	return;
}


bool STAPI::verify(char *var) {
	for (auto it = varVec.begin(); it != varVec.end(); ++it) {
		if(strcmp(var, it->var) == 0) {
			it->varUse++;
			return true;
		}
	}
	return false;
}

void STAPI::checkVars(void) {
	for(auto it = varVec.begin(); it != varVec.end(); ++it) {
		if(it->varUse == 0) {
			printf("WARNING: identifier %s is unused\n", it->var);
		}
	}
	return;
}


void STAPI::displayVarCount(void) {
	for (auto it = varVec.begin(); it != varVec.end(); ++it) {
		printf("%s: %d\n", it->var, it->varUse);
	}

}

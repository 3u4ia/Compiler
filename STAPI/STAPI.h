#ifndef STAPI_H
#define STAPI_H
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "../StructsAndInterfaces/STAPIStruct.h"

using namespace std;

class STAPI {
	private:
		vector<STAPIStruct> varVec;
	public:
		void insert(char *);
		bool verify(char *);
		void checkVars(void);
		void displayVarCount(void);
};


#endif


#ifndef TOKENSANDSTATES_H
#define TOKENSANDSTATES_H

enum TokenID {
	IDTK = 1000,
	NUMTK,
	OPTK,
	DELIMTK,
	EOFTK,
	KEYWORD,
	DELOPTK,
	LEOPTK,
	GEOPTK,
	LTOPTK,
	GTOPTK,
	NEOPTK,
	EQOPTK,
	ASSIGNOPTK,
	DBLESTAR,
	DBLESLASH,
	LFTPARENDELIM,
	RGHTPARENDELIM,
	LFTCURLYDELIM,
	RGHTCURLYDELIM,
	LFTSQREDELIM,
	RGHTSQREDELIM,
	SEMICOLON
};


struct Token {
	TokenID tokenID = IDTK;
	char lexeme[9] = {0};
	int lineNum = 0;
};


extern const char *tokenNames[];

enum NonTerminals {
	PROGRAM,
	VARS,
	VARLIST,
	BLOCK,
	STATS,
	MSTAT,
	STAT,
	READ,
	PRINT,
	COND,
	LOOP,
	ASSIGN,
	RELATIONAL,
	EXP,
	M,
	N,
	R,
};

extern const char *nonTerminalNames[];

enum States {
	S1,
	S2,
	S3,
	S4,
	S5,
	S6,		
	S7,
	S8,
	S9,
	S10,
	S11,
	S12,
	S13
};

enum Errors {
	EXPLETTER = -1000,
	EXPSLASH,
	EXPSTAR,
};

enum INVALIDS {
	INVALID = -2000,
	INVDLETTER,
	INVDSLASH,
	INVDSTAR,
};




#endif

#include "TokensAndStates.h"

const char *tokenNames[]= {"IDTk", "NumTk", "OpTk", "DelimTk", "EOFTk", "KeywordTk", "DelOpTk", "LEOpTk", "GEOpTk", "LTOpTk", "GTOpTk", "NEOpTk", "EQOpTk", "AssignOpTk", "DoubleStarOpTk", "DoubleSlashOpTk", "LeftParenDelimTk", "RightParenDelimTk", "LeftCurlyDelimTk", "RightCurlyDelimTk", "LeftSquareDelimTk", "RightSqureDelimTk", "SemicolonTk"};

const char *nonTerminalNames[] = {"Program", "Vars", "VarList", "Block", "Stats", "MStat", "Stat", "Read", "Print", "Cond", "Loop", "Assign", "Relational", "Exp", "M", "N", "R"};


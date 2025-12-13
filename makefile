CC = g++ -g3 -O0
CFLAGS = -g3 -Wall
TARGET = statSem

OBJS1 = P3.o
OBJS2 = Scanner.o
OBJS3 = TokensAndStates.o
OBJS4 = Parser.o
OBJS5 = Tree.o
OBJS6 = STAPI.o

CLSFLDR = ScannerClass
PRSRCLASS = Parser
TREECLS = TreeClass
STAPICLS = STAPI

OBJS = $(OBJS1) \
	$(CLSFLDR)/$(OBJS2) \
	TokensAndStates/$(OBJS3) \
	$(PRSRCLASS)/$(OBJS4) \
	$(TREECLS)/$(OBJS5) \
	$(STAPICLS)/$(OBJS6)


all: $(TARGET)
main: $(OBJS1)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)
 
$(OBJS1): P3.cpp $(CLSFLDR)/Scanner.h TokensAndStates/TokensAndStates.h $(PRSRCLASS)/Parser.h
	$(CC) $(CFLAGS) -c P3.cpp -o $(OBJS1)

$(CLSFLDR)/$(OBJS2): $(CLSFLDR)/Scanner.cpp $(CLSFLDR)/Scanner.h TokensAndStates/TokensAndStates.h
	$(CC) $(CFLAGS) -c $(CLSFLDR)/Scanner.cpp -o $(CLSFLDR)/$(OBJS2)

TokensAndStates/$(OBJS3): TokensAndStates/TokensAndStates.cpp TokensAndStates/TokensAndStates.h
	$(CC) $(CFLAGS) -c TokensAndStates/TokensAndStates.cpp -o TokensAndStates/$(OBJS3)

$(PRSRCLASS)/$(OBJS4): $(PRSRCLASS)/Parser.cpp $(PRSRCLASS)/Parser.h TokensAndStates/TokensAndStates.h $(CLSFLDR)/Scanner.h $(TREECLS)/TreeNodeStruct.h $(TREECLS)/Tree.h
	$(CC) $(CFLAGS) -c $(PRSRCLASS)/Parser.cpp -o $(PRSRCLASS)/$(OBJS4)

$(TREECLS)/$(OBJS5): $(TREECLS)/Tree.cpp $(TREECLS)/Tree.h $(TREECLS)/TreeNodeStruct.h TokensAndStates/TokensAndStates.h $(STAPICLS)/STAPI.h
	$(CC) $(CFLAGS) -c $(TREECLS)/Tree.cpp -o $(TREECLS)/$(OBJS5)


$(STAPICLS)/$(OBJS6): $(STAPICLS)/STAPI.cpp $(STAPICLS)/STAPI.h StructsAndInterfaces/STAPIStruct.h
	$(CC) $(CFLAGS) -c $(STAPICLS)/STAPI.cpp -o $(STAPICLS)/$(OBJS6)

clean:
	/bin/rm -f *.o ./ScannerClass/*.o ./TokensAndStates/*.o ./Parser/*.o ./STAPI/*.o ./TreeClass/*.o $(TARGET)

cleanTest: 
	/bin/rm -f *.fs25s1

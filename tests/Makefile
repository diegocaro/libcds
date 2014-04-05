
#CPP=g++

#CPPFLAGS=-g3 -Wall -O0 -I../includes/ 
#CPPFLAGS=-O3 -Wall -DNDEBUG -I../includes/
INCS=-I../includes/
LIB=../lib/libcds.a

OBJECTS= testArray.o testBitSequence.o testSequence.o testQuantile.o testHuffman.o timeSequence.o toArray.o toArray2.o testRange.o
BIN= testArray testBitSequence testSequence testHuffman timeSequence toArray toArray2 testQuantile testRange

%.o: %.cpp
	@echo " [C++] Compiling $<"
	@$(CXX) $(CPPFLAGS) $(INCS) -c $< -o $@

all: clean $(OBJECTS) $(BIN)
	@echo " [MSG] Done compiling tests"
	@echo " [FLG] $(CPPFLAGS)"

testHuffman:
	@echo " [LNK] Building testHuffman"
	@$(CXX) $(CPPFLAGS) -o testHuffman testHuffman.o $(LIB)

testSequence:
	@echo " [LNK] Building testSequence"
	@$(CXX) $(CPPFLAGS) -o testSequence testSequence.o $(LIB)

testQuantile:
	@echo " [LNK] Building testSequence"
	@$(CXX) $(CPPFLAGS) -o testQuantile testQuantile.o $(LIB)

testBitSequence:
	@echo " [LNK] Building testBitSequence"
	@$(CXX) $(CPPFLAGS) -o testBitSequence testBitSequence.o $(LIB)

testArray:
	@echo " [LNK] Building testArray"
	@$(CXX) $(CPPFLAGS) -o testArray testArray.o $(LIB)

testTextIndex:
	 @echo " [LNK] Building testTextIndex"
	 @$(CXX) $(CPPFLAGS) -o testTextIndex testTextIndex.o $(LIB)

testLCP:
	@echo " [LNK] Building testLCP"
	@$(CXX) $(CPPFLAGS) -o testLCP testLCP.o $(LIB)

testNPR:
	@echo " [LNK] Building testNPR"
	@$(CXX) $(CPPFLAGS) -o testNPR testNPR.o $(LIB)

toArray2:
	@echo " [LNK] Building toArray2"
	@$(CXX) $(CPPFLAGS) -o toArray2 toArray2.o $(LIB)

toArray:
	@echo " [LNK] Building toArray"
	@$(CXX) $(CPPFLAGS) -o toArray toArray.o $(LIB)

testSuffixTree:
	@echo " [LNK] Building testSuffixTree"
	@$(CXX) $(CPPFLAGS) -o testSuffixTree testSuffixTree.o $(LIB)

timeSequence:
	@echo " [LNK] Building timeSequence"
	@$(CXX) $(CPPFLAGS) -o timeSequence timeSequence.o $(LIB)

testRange:
	@echo " [LNK] Building testRange"
	@$(CXX) $(CPPFLAGS) -o testRange testRange.o $(LIB)
	

clean:
	@echo " [CLN] Cleaning object and binary files"
	@rm -f $(OBJECTS) $(BIN)

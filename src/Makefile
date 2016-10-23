CXXFLAGS = -I. -Wall -Wextra -Wno-unused-parameter -Wno-return-type -O2 -g -std=c++11
EXECUTABLE=minidb
OBJECTS=LinkedList.o BinFileHandler.o DenseIndex.o BplusIndex.o HashIndex.o Relation.o Index.o bpt.o Join.o

all: $(EXECUTABLE)

$(EXECUTABLE): main.cpp $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

LinkedList.o: LinkedList.cpp LinkedList.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

BinFileHandler.o: BinFileHandler.cpp BinFileHandler.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

DenseIndex.o: DenseIndex.cpp DenseIndex.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

BplusIndex.o: BplusIndex.cpp BplusIndex.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

HashIndex.o: HashIndex.cpp HashIndex.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

Relation.o: Relation.cpp Relation.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

Index.o: Index.cpp Index.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

bpt.o: bpt.cc bpt.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

Join.o: Join.cpp Join.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(EXECUTABLE) *.o

.PHONY: clean all

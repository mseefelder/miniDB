CC=gcc
CFLAGS=-O3 -Wall

test: testBTree
	time ./testBTree

testBTree: bTree.o testBTree.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) testBTree *.o

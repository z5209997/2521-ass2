 # Makefile for COMP1917 15s2 Week 08 Lab

CC=gcc
CFLAGS=-Wall -Werror -g
PGRK=graph.o pagerank.o readData.o searchPagerank.o
INV= BSTree.o ListNode.o inverted.o readData.o graph.o

# crawl : crawl.o $(LIBS)
# 	gcc -g -o crawl crawl.o $(LIBS) -lcurl
pagerank : $(PGRK)
		$(CC) -o pagerank $(OBJS)
inverted : $(INV)
		$(CC) -o inverted $(INV)

pagerank.o : pagerank.c pagerank.h graph.h
invertedIndex.o : invertedIndex.c invertedIndex.h
pagerank.o : pagerank.c pagerank.h
readData.o : readData.c readData.h
searchPagerank.o : searchPagerank.c searchPagerank.h
graph.o : graph.c graph.h



clean :
	rm -fr $(BINS) $(OBJS) core

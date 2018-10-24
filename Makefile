 # Makefile for COMP1917 15s2 Week 08 Lab

CC=gcc
CFLAGS=-Wall -Werror -g -std=c11
PGRK=graph.o pagerank.o readData.o 
INV= BSTree.o ListNode.o inverted.o readData.o graph.o
SRCH = searchPagerank.o
TFIDF = searchTfIdf.o readData.o graph.o
SFR = readData.o scaledFootrule.o graph.o

# crawl : crawl.o $(LIBS)
# 	gcc -g -o crawl crawl.o $(LIBS) -lcurl
pagerank : $(PGRK)
		$(CC) $(CFLAGS) -o pagerank $(PGRK)
inverted : $(INV)
		$(CC) $(CFLAGS) -o inverted $(INV)
searchPagerank : $(SRCH)
		$(CC) $(CFLAGS) -o searchPagerank $(SRCH)
searchTfIdf : $(TFIDF)
		$(CC) $(CFLAGS) -o searchTfIdf $(TFIDF)
footrule   : $(SFR)
		$(CC) $(CFLAGS) -o footrule $(SFR)

pagerank.o : pagerank.c pagerank.h graph.h
invertedIndex.o : invertedIndex.c invertedIndex.h
pagerank.o : pagerank.c pagerank.h
readData.o : readData.c readData.h
searchPagerank.o : searchPagerank.c searchPagerank.h
searchTfIdf.o : searchTfIdf.c
graph.o : graph.c graph.h
scaledFootrule.o : scaledFootrule.c



clean :
	rm -fr $(BINS) $(OBJS) core

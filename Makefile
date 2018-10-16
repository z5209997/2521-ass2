 # Makefile for COMP1917 15s2 Week 08 Lab

CC=gcc
CFLAGS=-Wall -Werror -g
OBJS=graph.o invertedIndex.o pagerank.o readData.o searchPagerank.o

# crawl : crawl.o $(LIBS)
# 	gcc -g -o crawl crawl.o $(LIBS) -lcurl
all : $(OBJS)
		$(CC) -o pgrk $(OBJS)

pagerank.o : pagerank.c pagerank.h graph.h
invertedIndex.o : invertedIndex.c invertedIndex.h
pagerank.o : pagerank.c pagerank.h
readData.o : readData.c readData.h
searchPagerank.o : searchPagerank.c searchPagerank.h
graph.o : graph.c graph.h

ts : ts.o set.o
	gcc -o ts ts.o set.o
ts.o : ts.c set.h

tq : tq.o queue.o
	gcc -o tq tq.o queue.o
tq.o : tq.c queue.h

tk : tk.o stack.o
	gcc -o tk tk.o stack.o
tk.o : tk.c stack.h

tg : tg.o graph.o
	gcc -o tg tg.o graph.o
tg.o : tg.c graph.h

clean :
	rm -fr $(BINS) $(OBJS) core

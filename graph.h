// graph.h ... Interface to Graph of strings
// Written by John Shepherd, September 2015
// Sourced from lab08

#ifndef GRAPH_H
#define GRAPH_H

typedef struct GraphRep *Graph;

// Function signatures

Graph newGraph(int);
void  disposeGraph(Graph);
int   addEdge(Graph,char *,char *);
int   nVertices(Graph);
int   isConnected(Graph, char *, char *);
char *getVertex(Graph g, int idx);
int getEdge(Graph g, int i, int j);
void  showGraph(Graph,int);

#endif

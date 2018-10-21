#include "graph.h"
#include <stdio.h>

typedef struct urlPagerank;

urlPagerank *calculatePagerank(Graph g, float d, double diffPR, int maxIterations);
void orderPageranks(urlPagerank *pagerankList, int numRanks);
void printPageranks(urlPagerank *pageranks, int numUrls, FILE *f);
float PR(Graph g, urlPagerank *pageranks, int numRanks, char *p, int t, float d, int N);
float W(Graph g, int N, int u, int p);
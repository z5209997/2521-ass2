#include "graph.h"
#include <stdio.h>

typedef struct urlPagerank urlPagerank;

urlPagerank *calculatePageranks(Graph g, float d, double diffPR, int maxIterations);
void freeUrlPageRank(urlPagerank *pagerankList, int numUrls);
float PR(Graph g, urlPagerank *pageranks, int numRanks, char *p, int t, float d, float N);
float Wout(Graph g, int N, int v, int u);
float Win(Graph g, int N, int u, int p);
void orderPageranks(urlPagerank *pagerankList, int numRanks);
void printPageranks(urlPagerank *pageranks, int numUrls, FILE *f);
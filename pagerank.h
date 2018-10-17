#include "graph.h"

typedef struct urlPagerank;

urlPagerank *calculatePagerank(Graph g, float d, double diffPR, int maxIterations);
void orderPageranks(urlPagerank *pagerankList);
void printPageranks(urlPagerank *pageranks, int numUrls);
float PR(Graph g, char *p, int t, float d, int N);
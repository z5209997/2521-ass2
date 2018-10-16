#include "graph.h"

typedef struct urlPagerank;

urlPagerank *orderPageranks(urlPagerank *pagerankList);
urlPagerank *calculatePagerank(Graph g, double diffPR, int maxIterations);
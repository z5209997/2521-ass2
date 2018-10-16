// Interface for readData
#include "graph.h"

char ** GetCollection(FILE *f);
void freeUrls(char **urls);
Graph GetGraph(char ** urls);

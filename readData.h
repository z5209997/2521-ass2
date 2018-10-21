// Interface for readData
#include "graph.h"

char ** GetCollection(FILE *f);
void freeUrls(char **urls);
Graph GetGraph(char ** urls);
void normaliseWord(char *str);
void deleteFromArray(char *str, int idx);

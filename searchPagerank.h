#include <stdio.h>

typedef struct urlData urlData;

urlData *calculateUrls(FILE *f, int argc, char *argv[], int *numberUrls);
void freeUrlData(urlData *url);
void addPageranks(urlData *urlList, FILE *f, int numberUrls);
int compareUrls(const void *a, const void *b);
void printUrls(urlData *urlList, int n);

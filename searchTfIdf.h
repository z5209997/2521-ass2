#include <stdio.h>

typedef struct urlData urlData;

urlData *setTfIdfValues(char **collection, FILE * finverted, int argc, char *argv[], float N);
void freeUrlData(urlData *url);
float numUrls(FILE *f);
float findTotalWordCount(FILE *f);
float findTCount(FILE *f, char *t);
float findDocuments(FILE *f, char *t);
void printTfIfd(urlData *urlCounts, float N);
int compareTfIdf(const void *a, const void *b);
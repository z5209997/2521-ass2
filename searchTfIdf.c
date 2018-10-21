// ranks pages based on corresponding tf-idf values

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "readData.h"
#include "graph.h"

typedef struct urlCount {
    char url[BUFSIZ];
    int totalWordCount;
    int inputWordCount;
    float tfIdf;
} urlCount;

int main(int argc, char *argv[]){
    if (argc < 2) { 
        printf("useage: ./search <term 1> <term 2> ...\n"); 
        exit(1); 
    }
    
}

// idf
// N = total documents: read this from collection.txt
// documents with t: read this from invertedIndex.txt


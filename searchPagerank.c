// matched_Url_list <- findMatchedUrls(“invertedIndex.txt”,	queryWords)
// matched_Urls_with_PR <- findPagerank(“pagerankList.txt”,	matched_Url_list)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "readData.h"
#include "graph.h"

int main(int argc, char *argv[]){
    if (argc < 2) { 
        printf("usage: ./pagerank <damping> <difference> <maxIterations>\n"); 
        exit(1); 
    }

    FILE *f = fopen("invertedIndex.txt", "r");
    if (f == NULL) { 
        fprintf(stderr, "Could not open collection.txt\n"); 
        exit(1); 
    }


}
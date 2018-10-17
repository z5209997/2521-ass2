// 1. Get	args :	d,	diffPR,	maxIterations

// 2. List_of_Urls <- GetCollection(	)
// Graph	g	<- GetGraph(List_of_Urls)

// 3. List_Urls_PageRanks =	calculatePageRank(g,	d,	diffPR,	maxIterations );	
// Ordered_List_Urls_PageRanks =	order	(List_Urls_PageRanks )

// 4. Output	Ordered_List_Urls_PageRanks to	“pagerankList.txt”

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readData.h"
#include "graph.h"

#define MAX_URLS 50

typedef struct urlPagerank {
    char *url;
    int degree;
    double pagerank;
} urlPagerank;


urlPagerank *calculatePagerank(Graph g, float d, double diffPR, int maxIterations);
void orderPageranks(urlPagerank *pagerankList);
void printPageranks(urlPagerank *pageranks, int numUrls);


// typedef struct Graph Graph;

int main(int argc, char *argv[]){
    if (argc < 4) { 
        printf("usage: ./pagerank <damping> <difference> <maxIterations>\n"); 
        exit(1); 
    }

    float d = atof(argv[1]);
    double diffPR;
    sscanf(argv[2], "%lf", &diffPR);
    int maxIterations = atoi(argv[3]);

    FILE *f = fopen("collection.txt", "r");
    if (f == NULL) { 
        fprintf(stderr, "Could not open collection.txt\n"); 
        exit(1); 
    }
    
    char **urls = GetCollection(f);

    Graph g = GetGraph(urls);

    urlPagerank *pagerankList = calculatePagerank(g, d, diffPR, maxIterations);

    printPageranks(pagerankList, nVertices(g));

    freeUrls(urls);
    disposeGraph(g);
    fclose(f);
    return 0;
}

urlPagerank *calculatePagerank(Graph g, float d, double diffPR, int maxIterations){
    int N = nVertices(g);
   
    urlPagerank *pageranks = malloc(sizeof(urlPagerank) * N);
    
    // make pagerank stucture for each url and set PR = 1/N
    int i;
    for (i=0; i < N; i++){
        urlPagerank *newPagerank = malloc(sizeof(urlPagerank));
        newPagerank->url = getVertex(g, i);
        //printf("%s\n", newPagerank->url);
        newPagerank->pagerank = 1/N;
        newPagerank->degree = 0;

        pageranks[i] = *newPagerank;
    }

    int iteration = 0;
    double diff = diffPR;

    while (iteration < maxIterations && diff >= diffPR) {
        // sum = 0;
        // for (..){

        // }
        // pageranks[iteration + 1].pagerank = (1-d)/N + d * sum;

        
        iteration++;
    }

    return pageranks;
}

float PR(Graph g, char *p, int t, float d, int N){
    if (t == 0){
        return 1/N;
    }
    int sum = 0;
    int i;
    for (i=0; i < N; i++){
        if (strcmp(p, g->vertex[i]) == 0){
            int j;
            for (j=0; j < N; j++){
                if (g->edges[i][j]){
                    sum = sum + PR(g, g->vertex[j], t-1, d, N);
                }
            }
        }
    }

    return (1-d)/N + d * sum;
}

void orderPageranks(urlPagerank *pagerankList){
}

void printPageranks(urlPagerank *pageranks, int numUrls){
    int i;
    for (i = 0; i < numUrls; i++){
        printf("%s %d %.7lf\n", pageranks[i].url, pageranks[i].degree, pageranks[i].pagerank);
    }
}
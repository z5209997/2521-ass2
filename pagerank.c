// 1. Get	args :	d,	diffPR,	maxIterations

// 2. List_of_Urls <- GetCollection(	)
// Graph	g	<- GetGraph(List_of_Urls)

// 3. List_Urls_PageRanks =	calculatePageRank(g,	d,	diffPR,	maxIterations );	
// Ordered_List_Urls_PageRanks =	order	(List_Urls_PageRanks )

// 4. Output	Ordered_List_Urls_PageRanks to	“pagerankList.txt”

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
float PR(Graph g, urlPagerank *pageranks, int numRanks, char *p, int t, float d, int N);
float Win(Graph g, int N, int u, int p);
float Wout(Graph g, int N, int u, int p);



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
        newPagerank->degree = nEdges(g, i);

        int iteration = 0;
        double diff = diffPR;
        int numRanks = 0;

        double currPagerank = 0;
        while (iteration < maxIterations) && diff >= diffPR) {
            
            currPagerank = PR(g, pageranks, numRanks, newPagerank->url, iteration + 1, d, N);
            printf("CURR: %0.7lf\n", currPagerank);
            
            int sum;
            int k;
            for (k=1; k < N; k++){
                sum = sum + fabsf(PR(g, pageranks, numRanks, newPagerank->url, iteration+1, d, N) - PR(g, pageranks, numRanks, newPagerank->url, iteration, d, N));
            }
            diff = sum;

            iteration++;
        }
        numRanks++;
        newPagerank->pagerank = currPagerank;
        pageranks[i] = *newPagerank;
    }

    return pageranks;
}

float PR(Graph g, urlPagerank *pageranks, int numRanks, char *p, int t, float d, int N){
    printf("%d\n", numRanks);
    int i;
    for (i=0; i < numRanks; i++){
        printf("%s\n", pageranks[i].url);
        if (strcmp(pageranks[i].url, p) == 0){
            printf("FOUND\n");
            return pageranks[i].pagerank;
        }
    }
    
    if (t == 0){
        return 1/N;
    }
    int sum = 0;
    for (i=0; i < N; i++){
        if (strcmp(p, getVertex(g, i)) == 0){
            int j;
            for (j=0; j < N; j++){
                if (getEdge(g, i, j)){
                    sum = sum + PR(g, pageranks, numRanks, getVertex(g, j), t-1, d, N) * Win(g, N, i, j) * Wout(g, N, i, j);
                }
            }
        }
    }
    return (1-d)/N + d * sum;
}

float Win(Graph g, int N, int u, int p){
    // float Iu;
    // float Iv;

    return 1;
}

float Wout(Graph g, int N, int u, int p){
    float Ou = nEdges(g, u);
    float Ov = 0;
    int i;
    int j;
    for (i=0; i < N; i++) {
        if(getEdge(g, p, i)) {
            for(j=0; j < N; j++)
                Ov += getEdge(g, i, j);
        }
    }
    if (Ov == 0) return 0;
    //printf("%f\n", Ou/Ov);
    return Ou/Ov;
}

void orderPageranks(urlPagerank *pagerankList){
}

void printPageranks(urlPagerank *pageranks, int numUrls){
    int i;
    for (i = 0; i < numUrls; i++){
        printf("%s %d %.7lf\n", pageranks[i].url, pageranks[i].degree, pageranks[i].pagerank);
    }
}
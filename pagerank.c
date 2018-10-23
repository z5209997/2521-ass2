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

typedef struct urlPagerank {
    char *url;
    int degree;
    double pagerank;
} urlPagerank;


urlPagerank *calculatePagerank(Graph g, float d, double diffPR, int maxIterations);
void freeUrlPageRank(urlPagerank *pagerankList, int numUrls);
void orderPageranks(urlPagerank *pagerankList, int numRanks);
void printPageranks(urlPagerank *pageranks, int numUrls, FILE *f);
float PR(Graph g, urlPagerank *pageranks, int numRanks, char *p, int t, float d, float N);
float Wout(Graph g, int N, int v, int u);
float Win(Graph g, int N, int u, int p);



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

    orderPageranks(pagerankList, nVertices(g));

    FILE *pr = fopen("pagerankList.txt", "w");
    printPageranks(pagerankList, nVertices(g), pr);

    freeUrls(urls);
    disposeGraph(g);
    freeUrlPageRank(pagerankList, nVertices(g));
    
    fclose(f);
    fclose(pr);
    return 0;
}

urlPagerank *calculatePagerank(Graph g, float d, double diffPR, int maxIterations){
    float N = nVertices(g);
   
    urlPagerank *pageranks = malloc(sizeof(urlPagerank) * N);
    
    // make pagerank stucture for each url and set PR = 1/N
    int i;
    int numRanks = 0;
    for (i=0; i < N; i++){
        urlPagerank *newPagerank = malloc(sizeof(urlPagerank));
        newPagerank->url = getVertex(g, i);
        newPagerank->degree = outgoingEdges(g, i);

        
        int iteration = 0;
        double diff = diffPR;
        

        double currPagerank = 1/N;
        while (iteration < maxIterations && diff >= diffPR) {
            currPagerank = PR(g, pageranks, numRanks, newPagerank->url, iteration + 1, d, N);
            //printf("CURR: %0.7lf\n", currPagerank);
            
            double sum;
            int k;
            for (k=1; k <= N; k++){
                sum = sum + fabsf(PR(g, pageranks, numRanks, newPagerank->url, iteration +1, d, N)
                                - PR(g, pageranks, numRanks, newPagerank->url, iteration, d, N));
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

void freeUrlPageRank(urlPagerank *pagerankList, int numUrls) {
    int i;
    for (i = 0; i < numUrls; i++){
        //free(pagerankList[i]);
    }
    free(pagerankList);
}



float PR(Graph g, urlPagerank *pageranks, int numRanks, char *p, int t, float d, float N){
    int i;
    for(i=0; i<numRanks; i++){
        if(strcmp(pageranks[i].url, p) == 0) {
            return pageranks[i].pagerank;
        }
    }

    if (t == 0){
        return 1/N;
    }
    float sum = 0;
    for (i=0; i < N; i++){
        if (strcmp(p, getVertex(g, i)) == 0){
            int j;
            for (j=0; j < N; j++){
                if (getEdge(g, j, i)){
                    float win = Win(g, N, j, i);
                    float wout = Wout(g, N, j, i);
                    sum = sum + PR(g, pageranks, numRanks, getVertex(g, j), t-1, d, N) * win * wout;
                }
            }
        }
    }
    return (1-d)/N + d * sum;
}

float Win(Graph g, int N, int v, int u){
    float Iu = incomingEdges(g, u);
    float Iv = 0;
    int i;
    for (i=0; i < N; i++) {
        if(getEdge(g, v, i)) {
            Iv += incomingEdges(g, i);
        }
    }
    return Iu/Iv;
}

float Wout(Graph g, int N, int v, int u){
    float Ou = outgoingEdges(g, u);
    if (Ou == 0) Ou = 0.5;

    float Ov = 0;
    int i;
    for (i=0; i < N; i++) {
        if(getEdge(g, v, i)) {
            
            int oe = outgoingEdges(g, i);
            Ov += oe;
            if (oe == 0) Ov += 0.5;
        }
    }
    return Ou/Ov;
}

int cmpFunc (const void * a, const void *b) {
    urlPagerank *pagerankA = (urlPagerank *)a;
    urlPagerank *pagerankB = (urlPagerank *)b;

    int value = (pagerankB->pagerank - pagerankA->pagerank) * pow(10,7);

    return value;
}

void orderPageranks(urlPagerank *pagerankList, int numUrls){
    qsort(pagerankList, numUrls, sizeof(urlPagerank), cmpFunc);
}

void printPageranks(urlPagerank *pageranks, int numUrls, FILE *f){
    int i;
    for (i = 0; i < numUrls; i++){
        fprintf(f, "%s %d %.7lf\n", pageranks[i].url, pageranks[i].degree, pageranks[i].pagerank);
    }
}
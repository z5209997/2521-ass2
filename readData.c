// 1. List_of_Urls <- GetCollection(	)
// Create	a	set	(list)	of	urls to	process	by	reading	data	from	file “collection.txt”

// 2. Graph	 g	<- GetGraph(List_of_Urls )
// Create	empty	graph	(use	graph	ADT	in	say	graph.h and	graph.c)
// For	each	url in	the	above	list
// • read	<url>.txt	file,	and	update	graph	by	adding	a	node	and	outgoing links

// 3. InvertedList <- GetInvertedList(List_of_Urls )
// Create	empty	inverted	list	(use	say	List	of	lists,	BST	where	values	are	lists,	etc)
// For	each	url in	List_of_Urls
// • read	<url>.txt	file,	and	update	inverted	index

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "readData.h"
#include "graph.h"

#define MAX_URLS 50
#define TRUE 1
#define FALSE 0

// Reads list of urls from files into an array
char **GetCollection(FILE *f)
{
    char **urls = malloc(MAX_URLS * sizeof(char *));
    char str[BUFSIZ];
    int i = 0;
    while (fscanf(f, "%s", str) == 1)
    {
        urls[i] = malloc(strlen(str) + 1);
        strcpy(urls[i], str);
        i++;
    }
    urls[i] = NULL;
    return urls;
}

// frees associated memory with urls
void freeUrls(char **urls)
{
    int i;
    for (i = 0; urls[i] != NULL; i++)
        free(urls[i]);
    free(urls);
}

static int isUrl(char **urls, char *url, char *currUrl){
    int i;
    for(i=0; urls[i] != NULL; i++){
        if (strcmp(urls[i], url) == 0 && strcmp(urls[i], currUrl) != 0) return TRUE; 
    }
    return FALSE;
}

// Converts list of urls into graph using corresponding .txt files
Graph GetGraph(char **urls)
{
    Graph g = newGraph(MAX_URLS);
    int i;
    for (i=0; urls[i] != NULL; i++){
        // add .txt to url and open file
        char urlFile[strlen(urls[i]) + 1];
        strcpy(urlFile, urls[i]);
        strcat(urlFile, ".txt");
        FILE *f = fopen(urlFile, "r");
        // check file exists
        if (f == NULL) { fprintf(stderr, "Invalid file name %s\n", urls[i]); exit(1); }
        char str[BUFSIZ];
        while(fscanf(f, "%s", str) == 1){
            // check if str is one of the urls and add to graph
            if (isUrl(urls, str, urls[i])){
                addEdge(g, urls[i], str);
            }
        }
        fclose(f);
    }
    return g;
}

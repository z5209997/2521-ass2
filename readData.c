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
#include <ctype.h>
#include "readData.h"
#include "graph.h"

#define MAX_URLS 50
#define TRUE 1
#define FALSE 0

// Reads list of urls from files into an array
char **GetCollection(FILE *f)
{
    // find size of file and malloc
    fseek(f, 0L, SEEK_END);
    int size = ftell(f);
    rewind(f);
    char **urls = malloc(size*sizeof(char*));

    char str[BUFSIZ];
    int i = 0;
    while (fscanf(f, "%s", str) == 1)
    {
        urls[i] = malloc(strlen(str) + 1);
        strcpy(urls[i], str);
        i++;
    }
    urls[i] = NULL;
    rewind(f);
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

void deleteFromArray(char *str, int idx){
    int i;
    for(i=idx; i < strlen(str) + 1; i++)
        str[i] = str[i+1];
}

// converts word to lowercase, removes spaces and removes  ',', '.', ';' and '?'
void normaliseWord(char *str){
    int i;
    for(i=0; str[i]; i++){
         str[i] = tolower(str[i]);
         if (strchr(" ,.;?", str[i])){
             deleteFromArray(str, i);
         }
    }
}

// returns the number of urls in collection.txt
float numUrls(FILE *f){
    int count = 0;
    char word[BUFSIZ];
    while (fscanf(f, "%s", word) == 1) count++;
    rewind(f);
    return count;
}

// sorts an array of strings alphebetically
char **sortUrls(char **arr, int n) {
    int i, j;
    char **sorted = malloc(n * BUFSIZ);
    for (i=0; i<n; i++)
        sorted[i] = strdup(arr[i]);
    
    char temp[BUFSIZ];
    
    for (i = 0; i < n - 1 ; i++) {
        for (j = i + 1; j < n; j++){
            if (strcmp(sorted[i], sorted[j]) > 0 && strlen(sorted[i]) >= strlen(sorted[j])) {
                strcpy(temp, sorted[i]);
                strcpy(sorted[i], sorted[j]);
                strcpy(sorted[j], temp);
            }
        }
    }
    return sorted;
}

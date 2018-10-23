// List_of_Urls <- GetCollection()
// InvertedIndex invertedIdx <- GetInvertedList (List_of_Urls)

// Output	invertedIdx to	“invertedIndex.txt”

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include "readData.h"
#include "graph.h"
#include "BSTree.h"

#define MAX_URLS 50
#define TRUE 1
#define FALSE 0

BSTree makeInvertedTree(char **urls);
void deleteFromArray(char *str, int idx);

int main(int argc, char *argv[]){
    
    FILE *f = fopen("collection.txt", "r");
    if (f == NULL) { 
        fprintf(stderr, "Could not open collection.txt\n"); 
        exit(1); 
    }
    
    char **urls = GetCollection(f);
    fclose(f);

    BSTree invertedIdx = makeInvertedTree(urls);
    
    showBSTree(invertedIdx);

    // clear contents of invertedIndex.txt then open to append
    fclose(fopen("file.txt", "w"));
    FILE *invFile = fopen("invertedIndex.txt", "a");
    if (f == NULL) { 
        fprintf(stderr, "Could not open invertedIndex.txt\n"); 
        exit(1); 
    }

    getInvertedList(invertedIdx, invFile);
    fclose(invFile);

    freeUrls(urls);
    dropBSTree(invertedIdx);
    return 0;
}

BSTree makeInvertedTree(char **urls){
    BSTree t;
    t = newBSTree();

    int i;
    for (i=0; urls[i] != NULL; i++) {
        char urlFile[strlen(urls[i]) + 1];
        strcpy(urlFile, urls[i]);
        strcat(urlFile, ".txt");
        FILE *f = fopen(urlFile, "r");
        if (f == NULL) { fprintf(stderr, "Invalid file name %s\n", urls[i]); exit(1); }
        
        int sec2 = FALSE;
        char str[BUFSIZ];
        while(fscanf(f, "%s", str) == 1){
            if (strcmp(str, "#end") == 0){
                sec2 = FALSE; 
            } else if (sec2) {
                normaliseWord(str);
                t = BSTreeInsert(t, str, urls[i]);
            } else if (strcmp(str, "Section-2") == 0){
                sec2 = TRUE;
            } 
        }
        fclose(f);
    }
    return t;
}


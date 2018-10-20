// List_of_Urls <- GetCollection()
// InvertedIndex invertedIdx <- GetInvertedList (List_of_Urls)

// Output	invertedIdx to	“invertedIndex.txt”

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "readData.h"
#include "graph.h"
#include "BSTree.h"

#define MAX_URLS 50

// typedef struct ListNode {
// 	char   *url;  // url
// 	struct ListNode *prev; // pointer previous node in list
// 	struct ListNode *next; // pointer to next node in list
// } ListNode;

// typedef struct TreeNode *TreeLink;
// typedef struct TreeNode {
// 	char *word; //key
//     ListNode *urlList; //linked list of the urls containing word
// 	TreeLink left, right;
// } TreeNode;

int main(int argc, char *argv[]){
    
    FILE *f = fopen("collection.txt", "r");
    if (f == NULL) { 
        fprintf(stderr, "Could not open collection.txt\n"); 
        exit(1); 
    }
    
    char **urls = GetCollection(f);
    int i;
    for (i=0; i < MAX_URLS; i++) {
        FILE *f = fopen(urls[i], "r");
        
    }
    
    freeUrls(urls);
    fclose(f);
    return 0;
}
// matched_Url_list <- findMatchedUrls(“invertedIndex.txt”,	queryWords)
// matched_Urls_with_PR <- findPagerank(“pagerankList.txt”,	matched_Url_list)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "readData.h"
#include "graph.h"

#define TRUE 1
#define FALSE 0
#define MAX_URL 4000


typedef struct urlData {
    char url[BUFSIZ];
    int wordCount;
    float rank;
} urlData;

urlData *calculateUrls(FILE *f, int argc, char *argv[], int *numberUrls);
void addPageranks(urlData *urlList, FILE *f, int numberUrls);
int compareUrls(const void *a, const void *b);
void printUrls(urlData *urlList, int n);


int main(int argc, char *argv[]){
    if (argc < 2) { 
        printf("useage: ./search <term 1> <term 2> ...\n"); 
        exit(1); 
    }

    FILE *invertedIdx = fopen("invertedIndex.txt", "r");
    if (invertedIdx == NULL) { 
        fprintf(stderr, "Could not open invertedIndex.txt\n"); 
        exit(1); 
    }
    
    int numberUrls;
    
    // makes a list of all the urls and adds data
    urlData *urlList = calculateUrls(invertedIdx, argc, argv, &numberUrls);
    
    FILE *pageranks = fopen("pagerankList.txt", "r");
    if (pageranks == NULL) { 
        fprintf(stderr, "Could not open pagerankList.txt\n"); 
        exit(1); 
    }

    // add rank to urls and sort them
    addPageranks(urlList, pageranks, numberUrls);
    qsort(urlList, numberUrls, sizeof(urlData), compareUrls);

    printUrls(urlList, numberUrls);
    
    fclose(invertedIdx);
    fclose(pageranks);

    return 0;
}

// makes an array of urls with their corresponding number of word matches
urlData *calculateUrls(FILE *f, int argc, char *argv[], int *numberUrls){

    urlData *urlList = malloc(sizeof(urlData) * MAX_URL);

    char line[BUFSIZ];
    char *word;
    int urlIdx = 0;
    int i;
    for (i=1; i < argc; i++){
        while (fgets(line, BUFSIZ, f)){
            if (strstr(line, argv[i])){
                char temp[BUFSIZ];
                strcpy(temp, line);
                int searchWord = TRUE;
                for (word = strtok(temp, " "); word; word = strtok(NULL, " ")) {
                    if (searchWord) { 
                        searchWord=FALSE;
                        continue; 
                    }
                    if (strcmp(word, "\n") == 0) continue; // check it is not a newline
                    // find if already in array and add to counter
                    int j;
                    int inArray = FALSE;
                    for (j=0; j <= urlIdx; j++){
                        if (strcmp(urlList[j].url, word) == 0) {
                            urlList[j].wordCount++;
                            inArray = TRUE;
                        } 
                    }
                    // if it is not add a new word
                    if (!inArray){
                        urlData *new = malloc(sizeof(urlData));
                        strcpy(new->url, word);
                        new->wordCount = 1;
                        urlList[urlIdx] = *new;
                        urlIdx++;
                    }
                }
            }
        }
        rewind(f); 
    }
    *numberUrls = urlIdx;
    return urlList;
}

// adds corresponding pageranks to the urlList list
void addPageranks(urlData *urlList, FILE *f, int numberUrls){
    char word[BUFSIZ];
    while (fscanf(f, "%s", word) == 1){
        int i;
        for (i=0; i < numberUrls; i++){
            if (strcmp(urlList[i].url, word) == 0)  {
                // scan to pagerank
                fscanf(f, "%s", word);
                float pr;
                fscanf(f, "%f", &pr);
                urlList[i].rank = pr;
            }
        }
    }
}

// function to compare urlData structs
// compares on word count first then rank
int compareUrls(const void *a, const void *b)
{
    urlData *countA = (urlData *)a;
    urlData *countB = (urlData *)b;
    int countCompare =  countB->wordCount - countA->wordCount;
    if (countCompare == 0)
        return (countB->rank - countA->rank) * pow(10, 7);
    else
        return countCompare;
}

// prints out each url in order
void printUrls(urlData *urlList, int n){
    int i;
    for (i=0; i < n; i++) {
        printf("%s\n", urlList[i].url);
    }
}

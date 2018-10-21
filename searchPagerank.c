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


typedef struct urlCount {
    char url[BUFSIZ];
    int wordCount;
    float rank;
} urlCount;

urlCount *countUrls(FILE *f, int argc, char *argv[], int *numberUrls);
void addPageranks(urlCount *urlCounts, FILE *f, int numberUrls);
int compareUrls(const void *a, const void *b);
int numUrls(FILE *f);


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
    
    urlCount *urlCounts = countUrls(invertedIdx, argc, argv, &numberUrls);
    
    FILE *pageranks = fopen("pagerankList.txt", "r");
    if (pageranks == NULL) { 
        fprintf(stderr, "Could not open pagerankList.txt\n"); 
        exit(1); 
    }
    addPageranks(urlCounts, pageranks, numberUrls);
    qsort(urlCounts, numberUrls, sizeof(urlCount), compareUrls);

    int i;
    
    for (i=0; i < numberUrls; i++) 
        printf("%s\n", urlCounts[i].url);
    
    
    fclose(invertedIdx);
    fclose(pageranks);

    return 0;
}

// makes an array of urls with their corresponding "counts" of the number of word matches
urlCount *countUrls(FILE *f, int argc, char *argv[], int *numberUrls){

    urlCount *urlCounts = malloc(sizeof(urlCount) * MAX_URL);

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
                    if (searchWord) { searchWord=FALSE; continue; }
                    if (strcmp(word, "\n") == 0) continue;
                    // find if already in array and add to counter
                    int j;
                    int inArray = FALSE;
                    for (j=0; j <= urlIdx; j++){
                        if (strcmp(urlCounts[j].url, word) == 0) {
                            urlCounts[j].wordCount++;
                            inArray = TRUE;
                        } 
                    }
                    if (!inArray){
                        urlCount *new = malloc(sizeof(urlCount));
                        strcpy(new->url, word);
                        new->wordCount = 1;
                        urlCounts[urlIdx] = *new;
                        urlIdx++;
                    }
                }
            }
        }
        rewind(f); 
    }
    *numberUrls = urlIdx;
    return urlCounts;
}

void addPageranks(urlCount *urlCounts, FILE *f, int numberUrls){
    char word[BUFSIZ];
    while (fscanf(f, "%s", word) == 1){
        int i;
        for (i=0; i < numberUrls; i++){
            if (strcmp(urlCounts[i].url, word) == 0)  {
                // scan to pagerank
                fscanf(f, "%s", word);
                float pr;
                fscanf(f, "%f", &pr);
                urlCounts[i].rank = pr;
            }
        }
    }
}

int compareUrls(const void *a, const void *b)
{
    urlCount *countA = (urlCount *)a;
    urlCount *countB = (urlCount *)b;
    //printf("wordcounts: %d %d \n", countA->wordCount, countB->wordCount);
    int countCompare =  countB->wordCount - countA->wordCount;
    if (countCompare == 0)
        return (countB->rank - countA->rank) * pow(10, 7);
    else
        return countCompare;
}

// int numUrls(FILE *f){
//     char word[BUFSIZ];
//     int count = 0;
//     while (fscanf(f, "%s", word) == 1) {
//         if (strstr(word, "url")) {
//             count++;
//         }
//     }
//     return count;
// }

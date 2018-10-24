// ranks pages based on corresponding tf-idf values

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "readData.h"
#include "graph.h"

#define MAX_URL 4000
#define TRUE 1
#define FALSE 0

typedef struct urlCount {
    char url[BUFSIZ]; 
    // int totalWordCount;
    // int inputWordCount;
    float tfIdf;
} urlCount;

urlCount *setTfIdfValues(char **collection, FILE * finverted, int argc, char *argv[], float N);
float numUrls(FILE *f);
float findTotalWordCount(FILE *f);
float findTCount(FILE *f, char *t);
float findDocuments(FILE *f, char *t);
void printTfIfd(urlCount *urlCounts, float N);
int compareTfIdf(const void *a, const void *b);

int main(int argc, char *argv[]){
    if (argc < 2) {
        printf("useage: ./search <term 1> <term 2> ...\n"); 
        exit(1); 
    }
    
    FILE *fcollection = fopen("collection.txt", "r");
    if (fcollection == NULL) { 
        fprintf(stderr, "Could not open collection.txt\n"); 
        exit(1); 
    }

    FILE *finverted = fopen("invertedIndex.txt", "r");
    if (finverted == NULL) { 
        fprintf(stderr, "Could not open invertedIndex.txt\n"); 
        exit(1); 
    }


    char **collection = GetCollection(fcollection);
    float N = numUrls(fcollection); //total number of urls    

    urlCount *urlCounts = setTfIdfValues(collection, finverted, argc, argv, N);                                  

    printTfIfd(urlCounts, N);

    fclose(fcollection);
    fclose(finverted);    
    return 0;
}

urlCount *setTfIdfValues(char **collection, FILE * finverted, int argc, char *argv[], float N) {
    urlCount *urlCounts = malloc(sizeof(urlCount) * MAX_URL);
    
    int i; //reading all the urls
    for (i=0; i<N; i++) {
        // add .txt to url and open file
        char urlFile[strlen(collection[i]) + 1];
        strcpy(urlFile, collection[i]);
        strcat(urlFile, ".txt");
        FILE *f = fopen(urlFile, "r");
        // check file exists
        if (f == NULL) { fprintf(stderr, "Invalid file name %s\n", urlFile); exit(1); }

        float totalWordCount = findTotalWordCount(f);
        
        int j; //reading each word to search, and calculate tf-idf for each word
        // the tf-idf for each url is the sum of the tf-idf per word
        float tfIdfSum = 0;                
        for(j=1; j < argc; j++){
            char *t = argv[j];
            normaliseWord(t);
            //tf = (frequency of word) / (totalWordCount)
            float tCount = findTCount(f, t); //in each document
            float tf = tCount/totalWordCount; 

            // idf = log(N/number of documents containing word)
            float documentCount = findDocuments(finverted, t); // finds number of documents containing T
            double idf = log10(N/documentCount);
            //printf("totalN = %.1f documentCount = %.2f idf = %.7f\n", N, documentCount, idf);
            float tfIdf = tf * idf;
            tfIdfSum += tfIdf;
        }

        //set urlCount[].url = collection[i] and urlCount[].tfIdf = tfIdfSum  
        urlCount *new = malloc(sizeof(urlCount));
        strcpy(new->url, collection[i]);
        new->tfIdf = tfIdfSum;

        urlCounts[i] = *new;                                 
    }
    return urlCounts;
}

float findTotalWordCount(FILE *f) {
    float totalWords = 0;

    int sec2 = FALSE;
    char word[BUFSIZ];
    while (fscanf(f, "%s", word) == 1){
        if (strcmp("Section-2", word) == 0) sec2 = TRUE;
        else if(strcmp("#end", word) == 0) sec2 = FALSE;
        if (sec2) {
            if ((strcmp("Section-2", word)) != 0) totalWords++;
        }
    }
    rewind(f);
    return totalWords;
}

float findTCount(FILE *f, char *t){
    float numOccurences = 0;
    char word[BUFSIZ];
    while(fscanf(f, "%s", word) == 1){
        normaliseWord(word);
        //printf("%s %s\n", word, t);
        if (strcmp(word, t) == 0) numOccurences++;
    }
    rewind(f);
    return numOccurences;
}

float findDocuments(FILE *f, char *t){
    float numDocs = 0;
    char word[BUFSIZ];
    int lineFound = FALSE;
    while(fscanf(f, "%s", word) == 1){
        normaliseWord(word);

        if (strcmp(word, t) == 0) lineFound = TRUE;
        else if (lineFound && strstr(word, "url")) {
            numDocs++;
        }
        else lineFound = FALSE;
    }
    rewind(f);
    return numDocs;
}

void printTfIfd(urlCount *urlCounts, float N){
    qsort(urlCounts, N, sizeof(urlCount), compareTfIdf);

    int i;
    for (i=0; i < N && i < 30; i++){
        if (urlCounts[i].tfIdf != 0) {
            printf("%s %f\n", urlCounts[i].url, urlCounts[i].tfIdf);
        }
    }
}

int compareTfIdf(const void *a, const void *b){
    urlCount *countA = (urlCount *)a;
    urlCount *countB = (urlCount *)b;
    return  (countB->tfIdf - countA->tfIdf) * pow(10,7);
}



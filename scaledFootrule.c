#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "readData.h"
#include "graph.h"

#define MAX_FOOTRULE 100
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MAX_STR 20
#define MAX_FILES 50
#define MAX_URLS 500

typedef struct minFootrule{
    char **permutation;
    float footrule;
} minFootrule;


minFootrule *findMin(char ***collectionList, int numCollections);
void freeMinFootrule (minFootrule *min);
char getLength(char **arr);
void recursivePermute(int *p, int l, int r, char ***collectionList, int numCollections, minFootrule *min);
char **unionMultiArrays(char ***arrList, int numArrs);
char **unionTwoArrays(char **T1, char **T2);
int findIndex(char *str, char **arr, int size);
void calculateFootrule(int *p, char ***collectionList, int numCollections, minFootrule *min);
void copyArr(char **dest, char **src);
void printMin(minFootrule *min);



int main(int argc, char *argv[]){
    if (argc < 3) { 
        printf("usage: ./scaledFootrule <file1> <file2>...\n"); 
        exit(1); 
    }

    minFootrule *min;
    char ***collectionList = malloc(MAX_FILES * MAX_URLS * MAX_STR);
    int i;

    // loop through each file and add to collection list
    for(i=1; i<argc; i++){
        collectionList[i-1] = malloc(MAX_URLS*MAX_STR);

        FILE *f = fopen(argv[i], "r");
        if (f == NULL){
            fprintf(stderr, "Could not open file %s\n", argv[i]); 
            exit(1); 
        }
        copyArr(collectionList[i-1], GetCollection(f));
        fclose(f); 
    }
    
    min = findMin(collectionList, argc-1);
    printMin(min);
    
    free(collectionList);
    freeMinFootrule(min);
    
    return 0;
}

// uses an array of arrays of urls to find best footrule
// returns a struct with minumum footrule and corresponding order
minFootrule *findMin(char ***collectionList, int numCollections){
    int i;
    int sizePerm = getLength(unionMultiArrays(collectionList, numCollections));
    
    int basePerm[sizePerm];
    for (i=1; i<=sizePerm; i++)
        basePerm[i-1] = i;

    minFootrule *min = malloc(sizeof(minFootrule) + sizeof(int) * sizePerm);
    min->footrule = MAX_FOOTRULE;
    
    min->permutation = malloc(sizePerm * MAX_STR);
    recursivePermute(basePerm, 0, sizePerm-1, collectionList, numCollections, min);
    
    return min;
}

void freeMinFootrule (minFootrule *min) {
    free(min->permutation);
    free(min);
}

// swaps a[i] with a[j]
void swap(int *a, int i, int j){
    int temp = a[i];
    a[i] = a[j];
    a[j] = temp;
}

// finds all permutations given a base permutation and calculates footrule for each
// p -> array to permute, l -> start index, r -> end index
void recursivePermute(int *p, int l, int r, char ***collectionList, int numCollections, minFootrule *min){ 
   
   int i; 
   if (l == r) {
        // calculates footrule of perm and checks if less than minumum
        calculateFootrule(p, collectionList, numCollections, min);
   }
   else { 
       for (i = l; i <= r; i++) { 
          swap(p, l, i); 
          recursivePermute(p, l+1, r, collectionList, numCollections, min); 
          swap(p, l, i); 
       } 
   } 
} 

// uses formula to calculate footrule
// p -> current permutation, T1 & T2 -> two rank lists
void calculateFootrule(int *p, char ***collectionList, int numCollections, minFootrule *min){
    int i;

    // make c array -> union between T1 and T2
    char **cArr = unionMultiArrays(collectionList, numCollections);
    int lenC = getLength(cArr);
    
    float sum = 0;
    int c;
    for (c=0; c < lenC; c++){
        for(i=0; i<numCollections; i++){
            float len = getLength(collectionList[i]);

            // find 'a' such that cArr[c] == T1[a]
            float a = findIndex(cArr[c], collectionList[i], len) + 1;
            if (a > 0)
                sum += fabs(a/len - (float)p[c]/lenC);
        }
    }
    // compare with current minimum
    if (sum < min->footrule){
        min->footrule = sum;
        for(i=0; i<lenC; i++)
            min->permutation[i] = cArr[p[i]-1];
    }
    free(cArr);
}

// finds the length of a 2D char array
char getLength(char **arr){
    int count = 0;
    while (arr[count]){count++;}
    return count;
}

// calculates the union between numArrs arrays
char **unionMultiArrays(char ***arrList, int numArrs){
    char **finalUnion = malloc(MAX_URLS*MAX_STR);
    finalUnion = unionTwoArrays(arrList[0], arrList[1]);
    int i;
    for(i=2; i<numArrs; i++)
        finalUnion = unionTwoArrays(finalUnion, arrList[i]);
    return finalUnion;
}
// returns an array which is the union of T1 and T2
char **unionTwoArrays(char **T1, char **T2){
    
    int lenT1 = getLength(T1);
    int lenT2 = getLength(T2);
    char **unionArr = malloc((lenT1 + lenT2) * MAX_STR);

    // sort T1 and T2
    sortUrls(T1, lenT1);
    sortUrls(T2, lenT2);

    // compare index by index and add the lesser value (works as they are already sorted)
    int i = 0, j = 0, k = 0; 
    while (i < lenT1 && j < lenT2) { 
        if (strcmp(T1[i], T2[j]) < 0 && strlen(T1[i]) <= strlen(T2[i])) 
            unionArr[k++] = strdup(T1[i++]); 
        else if (strcmp(T2[j], T1[i]) < 0 && strlen(T2[i]) <= strlen(T1[i])) 
            unionArr[k++] = strdup(T2[j++]); 
        else { 
            unionArr[k++] = strdup(T2[j++]); 
            i++;  
        } 
    } 
  
    // add remaining elements of the larger array 
    while(i < lenT1) 
        unionArr[k++] = strdup(T1[i++]); 
    while(j < lenT2) 
        unionArr[k++] = strdup(T2[j++]); 

    return unionArr;
}

// returns index of str in arr 
int findIndex(char *str, char **arr, int size){
    int i;
    for (i=0; i < size; i++){
        if (strcmp(str, arr[i]) == 0) return i;
    }
    return -1;
}

// copies 2D char array to a destination array
void copyArr(char **dest, char **src){
    int len = getLength(src);
    int i;
    for (i=0; i<len; i++) {
        dest[i]= strdup(src[i]);
    }
}

// prints the footrule and final ranking of the minimum
void printMin(minFootrule *min){
    printf("%.6f\n", min->footrule);

    int len = getLength(min->permutation);
    int j;
    for(j=0; j<len; j++){
        printf("%s \n", min->permutation[j]);
    }
}
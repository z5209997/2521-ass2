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
#define MAX_URLS 50

typedef struct minFootrule{
    char **permutation;
    float footrule;
} minFootrule;


minFootrule *findMin(char ***collectionList, int numCollections);
char getLength(char **arr);
void recursivePermute(int *p, int l, int r, char ***collectionList, minFootrule *min);
char **sortUrls(char **arr, int n);
char **unionMultiArrays(char ***arrList, int numArrs);
char **unionTwoArrays(char **T1, char **T2);
int findIndex(char *str, char **arr, int size);
void calculateFootrule(int *p, char ***collectionList, minFootrule *min);
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

    for(i=1; i<argc; i++){
        collectionList[i-1] = malloc(MAX_URLS*MAX_STR);

        FILE *f = fopen(argv[i], "r");
        if (f == NULL){
            fprintf(stderr, "Could not open file %s\n", argv[i]); 
            exit(1); 
        }
        copyArr(collectionList[i-1], GetCollection(f));
        // printf("%lu\n", sizeof(GetCollection(f)));
        fclose(f); 
    }
    min = findMin(collectionList, argc-1);
    printMin(min);
    // free min footrule 

    return 0;
}

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
    recursivePermute(basePerm, 0, sizePerm-1, collectionList, min);
    
    return min;
}

// swaps a[i] with a[j]
void swap(int *a, int i, int j){
    int temp = a[i];
    a[i] = a[j];
    a[j] = temp;
}

// finds all permutations given a base permutation and calculates footrule for each
// p -> array to permute, l -> start index, r -> end index
void recursivePermute(int *p, int l, int r, char ***collectionList, minFootrule *min){ 
   
   int i; 
   if (l == r) {
        // calculates footrule of perm and checks if less than minumum
        calculateFootrule(p, collectionList, min);
   }
   else { 
       for (i = l; i <= r; i++) { 
          swap(p, l, i); 
          recursivePermute(p, l+1, r, collectionList, min); 
          swap(p, l, i); 
       } 
   } 
} 

// uses formula to calculate footrule
// p -> current permutation, T1 & T2 -> two rank lists
void calculateFootrule(int *p, char ***collectionList, minFootrule *min){
    int i;
    float lenT1 = 2;
    float lenT2 = 2;

    float maxSize = MAX(lenT1, lenT2);

    // make c array -> union between T1 and T2
    char **cArr = unionMultiArrays(collectionList);
    int lenC = getLength(cArr);
    
    float sum = 0;
    int c;
    for (c=0; c < lenC; c++){
        // find 'a' such that cArr[c] == T1[a]
        float a = findIndex(cArr[c], T1, lenT1) + 1;
        
        // find 'b' such that cArr[cc] == T2[b]
        float b = findIndex(cArr[c], T2, lenT2) + 1;
        if (a > 0)
            sum += fabs(a/lenT1 - (float)p[c]/maxSize);
        if (b > 0)
            sum += fabs(b/lenT2 - (float)p[c]/maxSize);
    }
    // compare with current minimum
    if (sum < min->footrule){
        min->footrule = sum;
        for(i=0; i<lenC; i++)
            min->permutation[i] = cArr[p[i]-1];
    }
}

// finds the length of a 2D char array
char getLength(char **arr){
    int count = 0;
    while (arr[count]){count++;}
    //printf(" %d\n", count);
    return count;
}

// calculates the union between numArrs arrays
char **unionMultiArrays(char ***arrList, int numArrs){
    char **finalUnion = unionTwoArrays(arrList[0], arrList[1]);
    int i;
    for(i=2; i<numArrs; i++)
        finalUnion = unionTwoArrays(finalUnion, arrList[i]);

    return sortUrls(finalUnion, getLength(finalUnion));
}
// returns an array which is the union of T1 and T2
char **unionTwoArrays(char **T1, char **T2){
    int lenT1 = getLength(T1);
    int lenT2 = getLength(T2);
    char **unionArr = malloc((lenT1 + lenT2) * MAX_STR);

    // sort T1 and T2
    char **sortedT1 = sortUrls(T1, lenT1);
    char **sortedT2 = sortUrls(T2, lenT2);

    // compare index by index and add the lesser value (works as they are already sorted)
    int i = 0, j = 0, k = 0; 
    while (i < lenT1 && j < lenT2) { 
        if (strcmp(sortedT1[i], sortedT2[j]) < 0 && strlen(sortedT1[i]) <= strlen(sortedT2[i])) 
            unionArr[k++] = strdup(sortedT1[i++]); 
        else if (strcmp(sortedT2[j], sortedT1[i]) < 0 && strlen(sortedT2[i]) <= strlen(sortedT1[i])) 
            unionArr[k++] = strdup(sortedT2[j++]); 
        else { 
            unionArr[k++] = strdup(sortedT2[j++]); 
            i++;  
        } 
    } 
  
    // add remaining elements of the larger array 
    while(i < lenT1) 
        unionArr[k++] = strdup(sortedT1[i++]); 
    while(j < lenT2) 
        unionArr[k++] = strdup(sortedT2[j++]); 

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

        // calculate the first pagerank using the first two files
    //     if (i == 1){
    //         FILE *file1 = fopen(argv[1], "r");
    //         FILE *file2 = fopen(argv[2], "r");
    //         if (file1 == NULL || file2 == NULL){
    //             fprintf(stderr, "Could not open file\n"); 
    //             exit(1); 
    //         }
    //         T1 = GetCollection(file1);
    //         T2 = GetCollection(file2);
    //         fclose(file2);
    //         fclose(file1);
    //    } else if (i == 2) 
    //        continue;
    //     else { // otherwise use existing pagerank
    //         FILE *f = fopen(argv[i], "r");
    //         if (f == NULL){
    //             fprintf(stderr, "Could not open file\n"); 
    //             exit(1); 
    //         }
    //         copyArr(T1, min->permutation);
    //         T2 = GetCollection(f);
            
    //         fclose(f);
    //     }
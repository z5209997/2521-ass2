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

typedef struct minFootrule{
    char **permutation;
    float footrule;
} minFootrule;


minFootrule *findPermutations(char **T1, char **T2);
char getLength(char **arr);
long factorial(int n);
void recursivePermute(int *a, int l, int r, char **T1, char **T2, minFootrule *min);
char **sortUrls(char **arr, int n);
char **arrayUnion(char **T1, char **T2);
int findIndex(char *str, char **arr, int size);
void calculateFootrule(int *p, char **T1, char **T2, minFootrule *min);
void swap(int *a, int i, int j);
void copyArr(char **src, char **dest);
void printMin(minFootrule *min);



int main(int argc, char *argv[]){
    if (argc < 3) { 
        printf("usage: ./scaledFootrule <file1> <file2>...\n"); 
        exit(1); 
    }

    minFootrule *min;
    char **T1;
    char **T2;

    int i;
    for(i=1; i<argc; i++){
        if (i == 1){
            FILE *file1 = fopen(argv[1], "r");
            FILE *file2 = fopen(argv[2], "r");
            if (file1 == NULL || file2 == NULL){
                fprintf(stderr, "Could not open file\n"); 
                exit(1); 
            }
            T1 = GetCollection(file1);
            T2 = GetCollection(file2);
            fclose(file2);
            fclose(file1);
       } else if (i == 2) 
           continue;
        else {
            FILE *f = fopen(argv[i], "r");
            if (f == NULL){
                fprintf(stderr, "Could not open file\n"); 
                exit(1); 
            }

            copyArr(T1, min->permutation);
            T2 = GetCollection(f);
            
            fclose(f);
        }

        min = findPermutations(T1, T2);

        if (i == argc - 1)
            printMin(min);

        if (i==2) 
            freeUrls(T1);
        freeUrls(T2);
        
    }
    // free min footrule 

    return 0;
}

minFootrule *findPermutations(char **T1, char **T2){
    // int lenT1 = getLength(T1);
    // int lenT2 = getLength(T2);

    int sizePerm = getLength(arrayUnion(T1, T2));
    int basePerm[sizePerm];
    int i;
    for (i=1; i<=sizePerm; i++)
        basePerm[i-1] = i;
        // printf("PERM: %d\n", basePerm[i-1]);
    printf("\n");

    minFootrule *min = malloc(sizeof(minFootrule) + 4 * sizePerm);
    min->footrule = MAX_FOOTRULE;
    
    min->permutation = malloc(sizePerm * MAX_STR);
    recursivePermute(basePerm, 0, sizePerm-1, T1, T2, min);
    
    return min;
}

// takes array to permute, start index and end index
void recursivePermute(int *a, int l, int r, char **T1, char **T2, minFootrule *min){ 
   
   int i; 
   if (l == r) {
        // calculates footrule of perm and checks if less than minumum
        calculateFootrule(a, T1, T2, min);
   }
   else { 
       for (i = l; i <= r; i++) { 
          swap(a, l, i); 
          recursivePermute(a, l+1, r, T1, T2, min); 
          swap(a, l, i); 
       } 
   } 
} 

void swap(int *a, int i, int j){
    int temp = a[i];
    a[i] = a[j];
    a[j] = temp;
}

void calculateFootrule(int *p, char **T1, char **T2, minFootrule *min){
    int i;
    float lenT1 = getLength(T1);
    float lenT2 = getLength(T2);

    float maxSize = MAX(lenT1, lenT2);

    // make c array -> union between T1 and T2
    char **cArr = arrayUnion(T1, T2);
    
    int lenC = getLength(cArr);
    // for(i=0; i<lenC; i++) printf("%s\n", cArr[i]);
    // printf("\n");
    
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
    if (sum < min->footrule){
        min->footrule = sum;
        for(i=0; i<lenC; i++)
            min->permutation[i] = cArr[p[i]-1];
    }
}

char getLength(char **arr){
    int count = 0;
    while (arr[count]!=NULL) count++;
    return count;
}

char **arrayUnion(char **T1, char **T2){
    int lenT1 = getLength(T1);
    int lenT2 = getLength(T2);
    char **unionArr = malloc((lenT1 + lenT2) * MAX_STR);
    char **sortedT1 = sortUrls(T1, lenT1);
    char **sortedT2 = sortUrls(T2, lenT2);

    // find union of T1 and T2
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
  
    // Print remaining elements of the larger array 
    while(i < lenT1) 
        unionArr[k++] = strdup(sortedT1[i++]); 
    while(j < lenT2) 
        unionArr[k++] = strdup(sortedT2[j++]); 

    return unionArr;
}

int findIndex(char *str, char **arr, int size){
    int i;
    for (i=0; i < size; i++){
        if (strcmp(str, arr[i]) == 0) return i;
    }
    return -1;
}

void copyArr(char **dest, char **src){
    int len = getLength(src);
    int i;
    for (i=0; i<len; i++) 
        dest[i] = strdup(src[i]);
}

long factorial(int n)
{
  if (n == 0)
    return 1;
  else
    return(n * factorial(n-1));
}

void printMin(minFootrule *min){
    printf("%.6f\n", min->footrule);

    int len = getLength(min->permutation);
    int j;
    for(j=0; j<len; j++){
        printf("%s \n", min->permutation[j]);
    }
}
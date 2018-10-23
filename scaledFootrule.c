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
    int *permutation;
    float footrule;
} minFootrule;


minFootrule *findPermutations(char **T1, char **T2);
char getLength(char **arr);
long factorial(int n);
void permute(int *a, int l, int r, char **T1, char **T2, minFootrule *min);
char **sortUrls(char **arr, int n);
char **arrayUnion(char **T1, char **T2);
int findIndex(char *str, char **arr, int size);
void calculateFootrule(int *p, char **T1, char **T2, minFootrule *min);
void swap(int *a, int i, int j);
void printUrls(char **urls);
void populatePerms(int *perm, int size, int noPerms, int perms[size][noPerms]);



int main(int argc, char *argv[]){
    if (argc < 3) { 
        printf("usage: ./scaledFootrule <file1> <file2>...\n"); 
        exit(1); 
    }

    FILE *file1 = fopen(argv[1], "r");
    FILE *file2 = fopen(argv[2], "r");
    if (file1 == NULL || file2 == NULL){
        fprintf(stderr, "Could not open file\n"); 
        exit(1); 
    }

    char **T1 = GetCollection(file1);
    char **T2 = GetCollection(file2);

    minFootrule *min = findPermutations(T1, T2);
    printf("%f\n", min->footrule);

    freeUrls(T1);
    freeUrls(T2);
    fclose(file2);
    fclose(file1);
    return 0;
}

minFootrule *findPermutations(char **T1, char **T2){
    int lenT1 = getLength(T1);
    int lenT2 = getLength(T2);

    int sizePerm = MAX(lenT1, lenT2);
    int basePerm[sizePerm];
    int i;
    for (i=1; i<=sizePerm; i++){
        basePerm[i-1] = (float)i;
    }

    minFootrule *min = malloc(sizeof(minFootrule) + 4 * sizePerm);
    min->footrule = MAX_FOOTRULE;
    min->permutation = basePerm;

    permute(basePerm, 0, sizePerm-1, T1, T2, min);

    return min;
}

// takes array to permute, start index and end index
void permute(int *a, int l, int r, char **T1, char **T2, minFootrule *min){ 
   int i; 
   if (l == r) {
        int k; 
        for (k=0; k<=r; k++){
            printf("%d ", a[k]);
        }
        printf("\n");
        // calculates footrule of perm and checks if less than minumum
        calculateFootrule(a, T1, T2, min);
   }
   else { 
       for (i = l; i <= r; i++) { 
          swap(a, l, i); 
          permute(a, l+1, r, T1, T2, min); 
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
    float lenT1 = getLength(T1);
    float lenT2 = getLength(T2);

    float maxSize = MAX(lenT1, lenT2);

    // make c array -> union between T1 and T2
    char **cArr = arrayUnion(T1, T2);
    int lenC = getLength(cArr);
    
    float sum = 0;

    int c;
    for (c=0; c < lenC; c++){
        // find 'a' such that cArr[c] == T1[a]
        float a = findIndex(cArr[c], T1, lenT1) + 1;
        float b = findIndex(cArr[c], T2, lenT2) + 1;
        
        if (a >= 0) {
            printf("a: %f\n", a);
            sum += fabs(a/lenT1 - p[c]/maxSize);
        }
        if (b >= 0) {
            sum += fabs(b/lenT2 - p[c-1]/maxSize);
        }
    }

    if (sum < min->footrule){
        min->footrule = sum;
        printf("%f\n", sum);
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
        if (strcmp(sortedT1[i], sortedT2[j]) < 0) 
            unionArr[k++] = strdup(sortedT1[i++]); 
        else if (strcmp(sortedT2[j], sortedT1[i]) < 0) 
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

// sorts an array of strings alphebetically
char **sortUrls(char **arr, int n) {
    int i, j;
    char **sorted = malloc(n * MAX_STR);
    for (i=0; i<n; i++){
        sorted[i] = strdup(arr[i]);
    }
    
    char temp[MAX_STR];
    
    for (i = 0; i < n - 1 ; i++) {
        for (j = i + 1; j < n; j++){
            if (strcmp(sorted[i], sorted[j]) > 0) {
                strcpy(temp, sorted[i]);
                strcpy(sorted[i], sorted[j]);
                strcpy(sorted[j], temp);
            }
        }
    }
    return sorted;
}

int findIndex(char *str, char **arr, int size){
    int i;
    for (i=0; i < size; i++){
        if (strcmp(str, arr[i]) == 0) return i;
    }
    return -1;
}

long factorial(int n)
{
  if (n == 0)
    return 1;
  else
    return(n * factorial(n-1));
}

// #ifndef DLLIST_H
// #define DLLIST_H

#include <stdio.h>

typedef struct ListRep *List;

List newList();
void freeList(List);
void ListInsert(List, char*);
int ListLength (List L);
int ListFind(List L, char *str);
void printList (List L, FILE *f);

// #endif
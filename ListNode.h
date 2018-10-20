// #ifndef DLLIST_H
// #define DLLIST_H

#include <stdio.h>

typedef struct ListRep *List;
//typedef struct ListNode *ListNode;

List newList();
void freeList(List);
void ListInsert(List, char*);
int ListLength (List L);

// #endif
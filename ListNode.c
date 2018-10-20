// Based off DLList.c from lab03; edited
// BSTree.c ... implementation of binary search tree ADT

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "ListNode.h"

#define FALSE 0
#define TRUE 1

typedef struct ListNode {
	char   *word;  // url
	struct ListNode *next; // pointer to next node in list
} ListNode;

typedef struct ListRep {
	int  nitems;      // count of items in list
	ListNode *first; // first node in list
} ListRep;

// create a new ListNode (private function)
static ListNode *newListNode(char *it)
{
	ListNode *new;
	new = malloc(sizeof(ListNode));
	assert(new != NULL);
	new->word = strdup(it);
	new->next = NULL;
	return new;
}

// create a new empty List
List newList()
{
    List L = malloc(sizeof (struct ListRep) * 20);
	assert (L != NULL);
	L->nitems = 0;
	L->first = NULL;
	return L;
}

// free up all space associated with list
void freeList(List L)
{
	assert(L != NULL);
	ListNode *curr;
	curr = L->first;
	while (curr != NULL) {
        ListNode *tmp = curr;
		curr = curr->next;
		free(tmp->word);
		free(tmp);
	}
	free(L);
}

// Insert new node in list
void ListInsert(List L, char *it)
{
    assert(L!=NULL);
   
	ListNode *new = newListNode(it);
    // first item in list
    if (L->first == NULL) {
        L->first = new;
    }
    else {
        ListNode *curr = L->first;
        while (curr->next != NULL) {
            curr= curr->next;
        }
        curr->next = new;
    }
    L->nitems++;
}

int ListLength (List L) {
    return L->nitems;
}

int ListFind(List L, char *str){
    ListNode *curr;
    for (curr = L->first; curr; curr = curr->next) {
        if (strcmp(curr->word, str) == 0)
            return TRUE;
    }
    return FALSE;
}

void printList (List L, FILE *f){
    ListNode *curr;
    for (curr = L->first; curr; curr = curr->next)
        fprintf(f, "%s ", curr->word);
}


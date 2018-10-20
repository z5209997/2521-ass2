// Sourced from lab10; edited
// BSTree.h ... interface to binary search tree ADT

//#ifndef BSTREE_H
//#define BSTREE_H

typedef struct BSTNode *BSTree;

// create an empty BSTree
BSTree newBSTree();
// free memory associated with BSTree
void dropBSTree(BSTree);
// display a BSTree
void showBSTree(BSTree);
// display BSTree root node
void showBSTreeNode(BSTree);

// count #nodes in BSTree
int BSTreeNumNodes(BSTree);
// count #leaves in BSTree
int BSTreeNumLeaves(BSTree);

// insert a new value into a BSTree
BSTree BSTreeInsert(BSTree t, char *w, char *url);
// check whether a value is in a BSTree
int BSTreeFind(BSTree, char*);
void getInvertedList(BSTree t, FILE *f);

//#endif
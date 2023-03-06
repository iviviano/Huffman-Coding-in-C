#include <limits.h>
#include <stdio.h>

#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

typedef struct Node Node;
typedef struct Tree Tree;

struct Node {
    int c;
    int count;
    Node* next;
    Node* left;
    Node* right;
};

struct Tree {
    Node* head;
    int size;
};

Tree* newTree(void);
Node* newNode(int, int);
void addNodes(Tree*, int[CHAR_MAX]);
void makeTree(Tree*);
void freeLinkedList(Tree*);
void insertNode(Tree*, Node*);

//void printList(Tree*);
void printTree(Node*, FILE* wfile);
char** getBitstrings(Tree*);

Tree* buildTree(FILE*);
void decodeFile(Tree*, FILE*, FILE*);

#endif
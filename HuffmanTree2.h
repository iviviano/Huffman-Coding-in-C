#include <limits.h>
#include <stdio.h>

#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#define NUM_CHAR UCHAR_MAX + 1

/*The nodes used in the HuffmanTree data structure*/
typedef struct Node Node;
/*The tree that represents the HuffmanTree data stucture*/
typedef struct Tree Tree;

struct Node {
  int c1;
  int c2;
  int count;
  Node* next;
  Node* left;
  Node* right;
};

struct Tree {
    Node* head;
    int size;
};

/*Creates a new blank tree and returns it*/
Tree* newTree(int);
/*Creates a new node and returns it. The first int and second int specify the characters of that node, abd the second is the frequency of that character*/
Node* newNode(int, int, int);
/*Adds nodes to the tree using the array of frequencies*/
void addNodes(Tree*, int**);
/*Converts a linked list into a binary tree using Huffman's algorithm*/
void makeTree(Tree*);
/*Frees all the memory associated with a Huffman Tree*/
void freeLinkedList(Tree*);
/*Inserts the node into the tree using insertion sort based on the frequency of the node*/
void insertNode(Tree*, Node*);

/*Prints a preoder traversal of the HuffmanTree to the file in bits. The int* keeps track of how many bits have been printed for padding purposes*/
void printTree(Node*, FILE*, int*);
/*Puts the bitstring for wach character into an array and returns it*/
char*** getBitstrings(Tree*);

/*Builds a Huffman Tree from a preorder traversal represented in bits in the file*/
Tree* buildTree(FILE*);
/*Uses the tree to decode the file of bits into the output file*/
void decodeFile(Tree*, FILE*, FILE*);

#endif

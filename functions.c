#include "functions.h"
#include "HuffmanTree.h"

FILE* openFile(const char* filename, const char* op) {
    FILE* file = fopen(filename, op);
    if (file == NULL) {
        fprintf(stderr, "The file %s could not be found\n", filename);
        exit(EXIT_FAILURE);
    }
    return file;
}

void* doMalloc(size_t size) {
    void* ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

Node* newNode(int c, int count) {
    Node* newNode = doMalloc(sizeof(Node));
    newNode->c = c;
    newNode->count = count;
    return newNode;
}

void recFree(Node*);

void freeLinkedList(Tree* tree) {
    recFree(tree->head);
    tree->head = NULL;
    free(tree);
}

void recFree(Node* node) {
    if (node->left != NULL) {
        recFree(node->left);
        node->left = NULL;
    }
    if (node->right != NULL) {
        recFree(node->right);
        node->right = NULL;
    }
    free(node);
}
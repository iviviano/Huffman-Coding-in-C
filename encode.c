#include "functions.h"
#include "HuffmanTree.h"

int runEncode(const char* rfilename, const char* wfilename) {
  FILE* rfile = openFile(rfilename, "r");
  FILE* wfile = !strcmp(wfilename, STDOUT) ? NULL : openFile(wfilename, "w");
  
  int frequencies[CHAR_MAX];
  
  getFrequencies(rfile, frequencies);
  
  Tree* tree = newTree();
  addNodes(tree, frequencies);
  
  makeTree(tree);
  
  printTree(tree->head, wfile);
  
  fclose(rfile);
  rfile = openFile(rfilename, "r");
  
  char** bitstrings = getBitstrings(tree);
  
  encodeFile(bitstrings, rfile, wfile);
  
  for (int i = EOF; i < CHAR_MAX; i++) {
    if (bitstrings[i] != NULL) 
      free(bitstrings[i]);
    bitstrings[i] = NULL;
  }
  
  free(&bitstrings[-1]); 
  
  freeLinkedList(tree);
  
  fclose(rfile);
  fclose(wfile);
  
  return EXIT_SUCCESS;
}

void getFrequencies(FILE* rfile, int frequencies[CHAR_MAX]) {
  for (int i = 0; i < CHAR_MAX; i++) {
    frequencies[i] = 0;
  }
  
  for (int c = fgetc(rfile); c != EOF; c = fgetc(rfile)) {
    frequencies[c]++;
  }
}

Tree* newTree(void) {
  Tree* newTree = doMalloc(sizeof(Tree));
  newTree->head = newNode(EOF, 1);
  newTree->size = 1;
  return newTree;
}

void addNodes(Tree* list, int frequencies[CHAR_MAX]) {
  for (int i = 0; i < CHAR_MAX; i++) {
    if (frequencies[i] != 0) 
      insertNode(list, newNode(i, frequencies[i]));
  }
}

void insertNode(Tree* list, Node* node) {
  Node* cur = list->head;
  
  if (node->count <= cur->count) {
    node->next = cur;
    list->head = node;
    list->size++;
    return;
  }
  
  while (cur->next != NULL) {
    if (node->count <= cur->next->count)
      break;
    cur = cur->next;
  }
  Node* temp = cur->next;
  cur->next = node;
  node->next = temp;
  
  list->size++;
}

void makeTree(Tree* list) {
  while (list->size > 1) {
    Node* cur = newNode(-2, list->head->count + list->head->next->count);
    cur->left = list->head;
    cur->right = list->head->next;
    insertNode(list, cur);
    list->head = list->head->next->next;
    list->size -= 2;
  }
}

void printTree(Node* node, FILE* wfile) {
  if (node->left == NULL) {
    PUTC(wfile, '1');
    char c = node->c;
    int bit[__CHAR_BIT__];
    for (int i = 0; i < __CHAR_BIT__; i++) {
      bit[i] = c % 2;
      c = c / 2;
    }
    for (int i = 1; i <= __CHAR_BIT__; i++) {
      PUTC(wfile, '0' + bit[__CHAR_BIT__ - i]);
    }
  }
  else {
    PUTC(wfile, '0');
    printTree(node->left, wfile);
    printTree(node->right, wfile);
  }
}

void recGetBitstrings(Node*, char**, char*, int);

char** getBitstrings(Tree* tree) {
  char** array = doMalloc((CHAR_MAX + 1) * sizeof(char*));
  
  char** bitstrings = &array[1];
  
  for (int i = EOF; i < CHAR_MAX; i++) {
    bitstrings[i] = NULL;
  }
  
  char* bitstring = doMalloc(CHAR_BIT * CHAR_BIT * sizeof(char));
  
  recGetBitstrings(tree->head, bitstrings, bitstring, 0);
  
  free(bitstring);
  
  return bitstrings;
  
}

void recGetBitstrings(Node* node, char** bitstrings, char* bitstring, int level) {
  if (node->left == NULL) {
    bitstring[level] = '\0';
    bitstrings[node->c] = doMalloc(CHAR_BIT * CHAR_BIT * sizeof(char));
    strcpy(bitstrings[node->c], bitstring);
  }
  else {
    bitstring[level] = '0';
    recGetBitstrings(node->left, bitstrings, bitstring, level + 1);
    bitstring[level] = '1';
    recGetBitstrings(node->right, bitstrings, bitstring, level + 1);
  }
}

void encodeFile(char** bitstrings, FILE* rfile, FILE* wfile) {
  int bit_counter = 0;
  PRINT(wfile, bitstrings[EOF]);
  for (int c = fgetc(rfile); c != EOF; c = fgetc(rfile)) {
    PRINT(wfile, bitstrings[c]);
    bit_counter += strlen(bitstrings[c]);
  }
  PRINT(wfile, bitstrings[EOF]);
  for (; bit_counter % CHAR_BIT != 0; bit_counter++)
    PUTC(wfile, '0');
}

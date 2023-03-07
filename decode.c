#include "HuffmanTree.h"
#include "functions.h"

int runDecode(const char* rfilename, const char* wfilename) {
  FILE* rfile = openFile(rfilename, "r");
  FILE* wfile = !strcmp(wfilename, STDOUT) ? NULL : openFile(wfilename, "w");
  
  Tree* tree = buildTree(rfile);
  
  decodeFile(tree, rfile, wfile);
  
  fprintf(stderr, "Freeing everything\n");
  freeLinkedList(tree);

  fclose(rfile);
  fclose(wfile);
  
  return EXIT_SUCCESS;
}

Node* recBuild(FILE*);

Tree* buildTree(FILE* rfile) {
  Tree* tree = doMalloc(sizeof(Tree));
  
  tree->head = recBuild(rfile);
  
  return tree;
}

Node* recBuild(FILE* file) {
  if (fgetc(file) == '0') { //interior node 
    Node* node = newNode(-2, 0);
    //printf("creating blank node\n");
    node->left = recBuild(file);
    node->right = recBuild(file);
    return node;
  }
  //leaf node:
  int c = 0;
  for (int i = 0; i < CHAR_BIT; i++) 
    c = 2 * c + fgetc(file) - '0';
  //printf("Creating leaf node for %c\n", c);
  return newNode(c, 0);
}

void decodeFile(Tree* tree, FILE* rfile, FILE* wfile) {
  Node* node = tree->head;
  int c = fgetc(rfile);
  for (; node->left != NULL; c = fgetc(rfile)) {//might not need this
    if (c == '0')
      node = node->left;
    else
      node = node->right;
  }
  node->c = EOF;
  int character = '\0';
  while (character != EOF) {
    if (character != '\0')
      PUTC(wfile, character);
    //putchar(character); //for debugging
    node = tree->head;
    for (; node->left != NULL; c = fgetc(rfile)) {
      if (c == '0')
	node = node->left;
      else
	node = node->right;
    }
    character = node->c;
  }
}

#include "HuffmanTree2.h"
#include "functions2.h"

int runDecode(const char* rfilename, const char* wfilename) {
  FILE* rfile = openFile(rfilename, "r");
  FILE* bits = openFile(BITS, "w"); 
  FILE* wfile = !strcmp(wfilename, STDOUT) ? NULL : openFile(wfilename, "w");

  //fprintf(stderr, "Putting input into bits\n");
  encode_bits(rfile, bits);

  fclose(bits);
  
  //fprintf(stderr, "Reopening bits to read\n");
  bits = openFile(BITS, "r");

  //fprintf(stderr, "Building the tree using bits\n");
  Tree* tree = buildTree(bits);

  //fprintf(stderr, "decoding the file\n");
  decodeFile(tree, bits, wfile);
  
  //fprintf(stderr, "Freeing everything\n");
  freeLinkedList(tree);

  fclose(rfile);
  fclose(bits);

  bits = openFile(BITS, "w"); //clear bits to reduce storage
  fclose(bits);

  if (wfile != NULL) {
    //fprintf(stderr, "wfile is not stdout");
    fclose(wfile);
  }
  
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
    Node* node = newNode(-2, -2, 0);
    //printf("creating blank node\n");
    node->left = recBuild(file);
    node->right = recBuild(file);
    return node;
  }
  //leaf node:
  int c1 = 0;
  for (int i = 0; i < CHAR_BIT; i++) {
    char ch = fgetc(file);
    if (ch != '0' && ch != '1')
      fputc(ch, stderr);
    c1 = 2 * c1 + ch - '0';
  }
  int c2 = 0;
  for (int i = 0; i < CHAR_BIT; i++) {
    char ch = fgetc(file);
    if (ch != '0' && ch != '1')
      fputc(ch, stderr);
    c2 = 2 * c2 + ch - '0';
  }
  printf("Creating leaf node for %c, %c: %d, %d\n", c1, c2, c1, c2);
  return newNode(c1, c2, 0);
}

void decodeFile(Tree* tree, FILE* rfile, FILE* wfile) {
  Node* node = tree->head;
  int c = fgetc(rfile);
  for (; node->left != NULL; c = fgetc(rfile)) {
    if (c == '0')
      node = node->left;
    else
      node = node->right;
  }
  //  c = fgetc(rfile);
  fprintf(stderr, "Fixing EOF: node->c1 is %d and node->c2 is %d\n c is %c\n", node->c1, node->c2, c);
  if (c == '0') {
    node->c1 = EOF;
    fprintf(stderr, "fixed node->c1; node->c1 is now %d\n", node->c1);
  }
  node->c2 = EOF;
  fprintf(stderr, "fixed node->c2; node->c2 is now %d\n", node->c2);
  //int acc = 0; //for testing
  c = fgetc(rfile);
  while (1) {
    node = tree->head;
    //fputc('\n', stderr);
    for (; node->left != NULL; c = fgetc(rfile)) {
      if (c == '0') 
	//fprintf(stderr, "c is %c\n", c);
	node = node->left;
      else 
       	//fprintf(stderr, "c is %c\n", c);
	node = node->right;
    }
    if (node->c1 != EOF)
      PUTC(wfile, node->c1);
    if (node->c2 != EOF)
      PUTC(wfile, node->c2);
    else
      break;
  }
}

void encode_bits(FILE* rfile, FILE* bits) {
  for (int c = fgetc(rfile); c != EOF; c = fgetc(rfile)) {
    for (int i = 0; i < CHAR_BIT; i++) {
      fputc(((c & 0x80) >> (CHAR_BIT - 1)) + '0', bits);
      c = c << 1;
    }
  }
}

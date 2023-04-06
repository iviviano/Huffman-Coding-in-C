#include "HuffmanTree.h"
#include "functions.h"

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
    Node* node = newNode(-2, 0);
    //printf("creating blank node\n");
    node->left = recBuild(file);
    node->right = recBuild(file);
    return node;
  }
  //leaf node:
  int c = 0;
  for (int i = 0; i < CHAR_BIT; i++) {
    char ch = fgetc(file);
    if (ch != '0' && ch != '1')
      fputc(ch, stderr);
    c = 2 * c + ch - '0';
  }
  //printf("Creating leaf node for %c : %d\n", c, c);
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
  //fprintf(stderr, "Fixing EOF: node->c is %d\n", node->c);
  node->c = EOF;
  int character = UCHAR_MAX + 1;
  //int acc = 0; //for testing
  while (character != EOF) {
    if (character <= UCHAR_MAX)
      PUTC(wfile, character);
    //fputc(character, stderr); //for debugging
    //fputc(acc++ + '0', stderr);
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
    character = node->c;
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

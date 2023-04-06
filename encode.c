#include "functions.h"
#include "HuffmanTree.h"

#define NUM_CHAR UCHAR_MAX + 1

int runEncode(const char* rfilename, const char* wfilename) {
  FILE* rfile = openFile(rfilename, "r");
  FILE* bits = openFile(BITS, "w");
  FILE* wfile = !strcmp(wfilename, STDOUT) ? NULL : openFile(wfilename, "w");
  
  int frequencies[NUM_CHAR];
  
  getFrequencies(rfile, frequencies);
  
  Tree* tree = newTree();
  addNodes(tree, frequencies);
  
  makeTree(tree);

  int* bit_counter = doMalloc(sizeof(int));

  *bit_counter = 0;
  
  printTree(tree->head, bits, bit_counter);
  
  fclose(rfile);
  rfile = openFile(rfilename, "r");
  
  char** bitstrings = getBitstrings(tree);
  
  encodeFile(bitstrings, rfile, bits, bit_counter);

  fprintf(stderr, "%-10scount bitstring\n", "char");
  fprintf(stderr, "%-10s_____ _________\n", "____");
  for (int i = EOF; i < NUM_CHAR; i++) {
    if (bitstrings[i] != NULL) {
      fprintf(stderr, "%-5d%-6c", i, i);
      Node* node = tree->head;
      for (int j = 0; bitstrings[i][j] != '\0'; j++) {
	if (bitstrings[i][j] == '1')
	  node = node->right;
	else
	  node = node->left;
      }
      fprintf(stderr, "%-6d%s\n", node->count, bitstrings[i]);
    }
  }
  
  for (int i = EOF; i < NUM_CHAR; i++) {
    if (bitstrings[i] != NULL) 
      free(bitstrings[i]);
    bitstrings[i] = NULL;
  }
  
  free(&bitstrings[-1]); 
  
  freeLinkedList(tree);
  
  fclose(rfile);
  fclose(bits);
  bits = openFile(BITS, "r");
  
  decode_bits(bits, wfile);

  fclose(bits);
  
  if (wfile != NULL)
    fclose(wfile);

  free(bit_counter);
  
  return EXIT_SUCCESS;
}

void getFrequencies(FILE* rfile, int frequencies[UCHAR_MAX]) {
  for (int i = 0; i < NUM_CHAR; i++) 
    frequencies[i] = 0;
  
  for (int c = fgetc(rfile); c != EOF; c = fgetc(rfile)) 
    frequencies[c]++;
}

Tree* newTree(void) {
  Tree* newTree = doMalloc(sizeof(Tree));
  newTree->head = newNode(EOF, 1);
  newTree->size = 1;
  return newTree;
}

void addNodes(Tree* list, int frequencies[UCHAR_MAX]) {
  for (int i = 0; i < NUM_CHAR; i++) 
    if (frequencies[i] != 0) 
      insertNode(list, newNode(i, frequencies[i]));
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

void printTree(Node* node, FILE* bits, int* bit_counter) {
  if (node->left == NULL) {
    fputc('1', bits);
    *bit_counter = *bit_counter + 1;
    char c = node->c;
    
    /* int bit[CHAR_BIT]; */
    /* for (int i = 0; i < CHAR_BIT; i++) { */
    /*   bit[i] = c % 2; */
    /*   c = c / 2; */
    /* } */
    /* for (int i = 1; i <= CHAR_BIT; i++) { */
    /*   fputc('0' + bit[CHAR_BIT - i], bits); */
      
    /*   *bit_counter = *bit_counter + 1; */
    /*} */

    for (int i = 0; i < CHAR_BIT; i++) {
      if (c & 0x80)
	fputc('1', bits);
      else
	fputc('0', bits);
      c = c << 1;
      *bit_counter = *bit_counter + 1;
    }

  }
  else {
    fputc('0', bits);
    *bit_counter = *bit_counter + 1;
    printTree(node->left, bits, bit_counter);
    printTree(node->right, bits, bit_counter);
  }
}

void recGetBitstrings(Node*, char**, char*, int);

char** getBitstrings(Tree* tree) {
  char** array = doMalloc((NUM_CHAR + 1) * sizeof(char*));
  
  char** bitstrings = &array[1];
  
  for (int i = EOF; i < NUM_CHAR; i++) 
    bitstrings[i] = NULL;
  
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

void encodeFile(char** bitstrings, FILE* rfile, FILE* bits, int* bit_counter) {
  fprintf(bits, "%s", bitstrings[EOF]);
  fprintf(stderr, "Printed first EOF bitstring\n");
  *bit_counter = *bit_counter + 2* strlen(bitstrings[EOF]);
  for (int c = fgetc(rfile); c != EOF; c = fgetc(rfile)) {
    fprintf(bits, "%s", bitstrings[c]);
    *bit_counter = *bit_counter + strlen(bitstrings[c]);
  }
  fprintf(bits, "%s", bitstrings[EOF]);
  fprintf(stderr, "Printed second EOF bitstring\n");
  for (; *bit_counter % CHAR_BIT != 0; *bit_counter = *bit_counter + 1)
    fputc('0', bits);
  fprintf(stderr, "Total bits printed: %d\n", *bit_counter);
}

void decode_bits(FILE* bits, FILE* wfile) {
  int c = 0;
  int bit_counter = 0;

  for (int ch = fgetc(bits); ch != EOF; ch = fgetc(bits)) {
    //putchar(ch);
    if (bit_counter % CHAR_BIT == 0 && bit_counter != 0) {
      PUTC(wfile, c);
      c = 0;
    }
    c = (c << 1) + (ch - '0');
    bit_counter++;
  }
  PUTC(wfile, c);
  //PUTC(wfile, '\n');
}

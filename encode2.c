#include "functions2.h"
#include "HuffmanTree2.h"

int runEncode(const char* rfilename, const char* wfilename) {
  FILE* rfile = openFile(rfilename, "r");
  FILE* bits = openFile(BITS, "w");
  FILE* wfile = !strcmp(wfilename, STDOUT) ? NULL : openFile(wfilename, "w");

  int** arr = doMalloc((NUM_CHAR + 1) * sizeof(int*));
  int** frequencies = &arr[1];
  for (int i = EOF; i < NUM_CHAR; i++) {
    int* num = doMalloc((NUM_CHAR + 1) * sizeof(int));
    frequencies[i] = &num[1];
    for (int j = EOF; j < NUM_CHAR; j++) {
      frequencies[i][j] = 0;
    }
  }
  
  getFrequencies(rfile, frequencies);

  int EOF_index = EOF;
  for (; frequencies[EOF_index][EOF] == 0; EOF_index++);
  //    printf("EOF_index is %d\n frequencies[EOF_index][EOF] = %d\n", EOF_index, frequencies[EOF_index][EOF]);
  
  Tree* tree = newTree(EOF_index);
  addNodes(tree, frequencies);
  
  makeTree(tree);

  int* bit_counter = doMalloc(sizeof(int));

  *bit_counter = 0;
  
  printTree(tree->head, bits, bit_counter);
  
  fclose(rfile);
  rfile = openFile(rfilename, "r");
  
  char*** bitstrings = getBitstrings(tree);

  /* fprintf(stderr, "%-12scount bitstring\n", "String"); */
  /* fprintf(stderr, "%-12s_____ _________\n", "______"); */
  /* for (int i = EOF; i < NUM_CHAR; i++) { */
  /*   for (int j = EOF; j < NUM_CHAR; j++) { */
  /*     if (bitstrings[i][j] != NULL) { */
  /* 	fprintf(stderr, "%-4d%-4d%c%-5c", i, j, i, j); */
  /* 	Node* node = tree->head; */
  /* 	for (int k = 0; bitstrings[i][j][k] != '\0'; k++) { */
  /* 	  if (bitstrings[i][j][k] == '1') */
  /* 	    node = node->right; */
  /* 	  else */
  /* 	    node = node->left; */
  /* 	} */
  /* 	fprintf(stderr, "%-6d%s\n", node->count, bitstrings[i][j]); */
  /*     } */
  /*   } */
  /* } */

  encodeFile(bitstrings, rfile, bits, bit_counter);

  for (int i = EOF; i < NUM_CHAR; i++) {
    for (int j = EOF; j < NUM_CHAR; j++) {
      if (bitstrings[i][j] != NULL) {
	free(bitstrings[i][j]);
	bitstrings[i][j] = NULL;
      }
    }
    free(&(frequencies[i])[-1]);
    free(&(bitstrings[i])[-1]);
  }
  free(&bitstrings[-1]);
  free(&frequencies[-1]);
  
  freeLinkedList(tree);
  
  fclose(rfile);
  fclose(bits);
  bits = openFile(BITS, "r");
  
  decode_bits(bits, wfile);

  fclose(bits);

  bits = openFile(BITS, "w"); //clear bits file to reduce storage
  fclose(bits);
  
  if (wfile != NULL)
    fclose(wfile);

  free(bit_counter);
  
  return EXIT_SUCCESS;
}

void getFrequencies(FILE* rfile, int** frequencies) {
  int c1;
  int c2 = '0';
  for (c1 = fgetc(rfile); c2 != EOF; c1 = fgetc(rfile)) { 
    c2 = c1 == EOF ? EOF : fgetc(rfile);
    frequencies[c1][c2]++;
    /* if (c1 == EOF || c2 == EOF) { */
    /*   fprintf(stderr, "c1 is %d and c2 is %d; freq[%d][%d] = %d\n", c1, c2, c1, c2, frequencies[c1][c2]); */
    /* }  */
  } 
}

Tree* newTree(int EOF_index) {
  Tree* newTree = doMalloc(sizeof(Tree));
  newTree->head = newNode(EOF_index, EOF, 1);
  newTree->size = 1;
  return newTree;
}

void addNodes(Tree* list, int** frequencies) {
  for (int i = 0; i < NUM_CHAR; i++)
    for (int j = 0; j < NUM_CHAR; j++)
      if (frequencies[i][j] != 0) 
	insertNode(list, newNode(i, j, frequencies[i][j]));
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
    Node* cur = newNode(-2, -2, list->head->count + list->head->next->count);
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
    char c = node->c1;
    
    for (int i = 0; i < CHAR_BIT; i++) {
      if (c & 0x80)
	fputc('1', bits);
      else
	fputc('0', bits);
      c = c << 1;
      *bit_counter = *bit_counter + 1;
    }
    
    c = node->c2;
    
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

void recGetBitstrings(Node*, char***, char*, int);

char*** getBitstrings(Tree* tree) {
  char*** array = doMalloc((NUM_CHAR + 1) * sizeof(char**));
  
  char*** bitstrings = &array[1];

  for (int i = EOF; i < NUM_CHAR; i++) {
    char** arr = doMalloc((NUM_CHAR + 1) * sizeof(char*));
    bitstrings[i] = &arr[1];
    for (int j = EOF; j < NUM_CHAR; j++) 
      bitstrings[i][j] = NULL;
  }
  
  char* bitstring = doMalloc(CHAR_BIT * CHAR_BIT * sizeof(char));
  
  recGetBitstrings(tree->head, bitstrings, bitstring, 0);
  
  free(bitstring);
  
  return bitstrings;
}

void recGetBitstrings(Node* node, char*** bitstrings, char* bitstring, int level) {
  if (node->left == NULL) {
    bitstring[level] = '\0';
    bitstrings[node->c1][node->c2] = doMalloc(CHAR_BIT * CHAR_BIT * sizeof(char));
    strcpy(bitstrings[node->c1][node->c2], bitstring);
  }
  else {
    bitstring[level] = '0';
    recGetBitstrings(node->left, bitstrings, bitstring, level + 1);
    bitstring[level] = '1';
    recGetBitstrings(node->right, bitstrings, bitstring, level + 1);
  }
}

void encodeFile(char*** bitstrings, FILE* rfile, FILE* bits, int* bit_counter) {
  int EOF_index = EOF;
  for (; bitstrings[EOF_index][EOF] == NULL; EOF_index++);
  /* fprintf(stderr, "bitstrings[EOF_index][EOF] == NULL is %s\n", bitstrings[EOF_index][EOF] == NULL ? "true" : "false"); */
  /* fprintf(stderr, "EOF_index is %d\n", EOF_index); //for testing */
  /* fputc(EOF_index == EOF ? '0' : '1', stderr); //for testing */
  fprintf(bits, "%s", bitstrings[EOF_index][EOF]);

  fputc(EOF_index == EOF ? '0' : '1', bits);

  //fprintf(stderr, "Printed first EOF bitstring\n");
  *bit_counter = *bit_counter + 2* strlen(bitstrings[EOF_index][EOF]) + 1;
  for (int c1 = fgetc(rfile); c1 != EOF; c1 = fgetc(rfile)) {
    int c2 = fgetc(rfile);
    fprintf(bits, "%s", bitstrings[c1][c2]);
    *bit_counter = *bit_counter + strlen(bitstrings[c1][c2]);
  }
  fprintf(bits, "%s", bitstrings[EOF_index][EOF]);
  //fprintf(stderr, "Printed second EOF bitstring\n");
  for (; *bit_counter % CHAR_BIT != 0; *bit_counter = *bit_counter + 1)
    fputc('0', bits);
  //fprintf(stderr, "Total bits printed: %d\n", *bit_counter);
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

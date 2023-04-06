#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define STDOUT "stdout" 
#define ENCODE (const char*) "./encode"
#define DECODE "./decode"
#define BITS "bits"

/*Macros to print to a file or stdout*/
#define PRINT(file, string) file == NULL ? printf("%s", (char*) string) : fprintf(file, "%s", (char*) string)
#define PUTC(file, c) file == NULL ? putchar((char) c) : fputc((char) c, file)

/*Runs the encode program using the given filenames*/
int runEncode(const char* rfilename, const char* wfilename);
/*Records the frequency of each character in the file*/
void getFrequencies(FILE*, int[UCHAR_MAX]);
/*Encodes the file using an array of the bitstrings generated from the Huffman Tree. The first file is the source file. The second is the bits file. The int* keeps track of how many bits have been printed for padding purposes.*/
void encodeFile(char**, FILE*, FILE*, int*);
/*Decodes the first file of bits into the second file in ascii, compressing it*/
void decode_bits(FILE*, FILE*);

/*Runs the decode program using the given filenames*/
int runDecode(const char*, const char*);
/*Encodes the first file of ascii into the second file as bits*/
void encode_bits(FILE*, FILE*);

/*Opens the given filename with the mode given. Returns the file if it is found, or exits with status EXIT_FAILURE if the file could not be opened*/
FILE* openFile(const char*, const char*);
/*Allocates memory of the given size with malloc. Returns the memory address if malloc was successful and exits with status EXIT_FAILURE otherwise*/
void* doMalloc(size_t);

#endif

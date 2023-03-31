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
#define PRINT(file, string) file == NULL ? printf("%s", string) : fprintf(file, "%s", string)
#define PUTC(file, c) file == NULL ? putchar(c) : fputc(c, file)

int runEncode(const char* rfilename, const char* wfilename);
void getFrequencies(FILE*, int[CHAR_MAX]);
void encodeFile(char**, FILE*, FILE*, int*);
void decode_bits(FILE*, FILE*);

int runDecode(const char*, const char*);
void encode_bits(FILE*, FILE*);

FILE* openFile(const char*, const char*);
void* doMalloc(size_t);

#endif

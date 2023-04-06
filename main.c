#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

/*The main function for both encode and decode programs. The first command line argument specifies which program to run. The second is the name of the input file. A third, optional parameter is the name of the output file. If no filename is given, the program writes to stdout.*/
int main(int argc, char** argv) {
  char* rfile;
  char* wfile;
  
  if (argc < 2) { //need input filename
    fprintf(stderr, "Not enough arguments\n");
    return EXIT_FAILURE;
  }
  
  rfile = argv[1];
  
  if (argc == 3) //ouput filename given
    wfile = argv[2];
  else 
    wfile = STDOUT; //write to stdout
  
  if (!strcmp(argv[0], ENCODE)) { 
    runEncode(rfile, wfile);
    return EXIT_SUCCESS;
  }
  else if (!strcmp(argv[0], DECODE)) {
    runDecode(rfile, wfile);
    return EXIT_SUCCESS;
  }
  else { //if first argument is not encode or decode. Can't occur using make commands
    fprintf(stderr, "Unkown command: %s\n", argv[0]);
    return EXIT_FAILURE;
  }
}

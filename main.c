#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

int main(int argc, char** argv) {
    char* rfile;
    char* wfile;

    if (argc < 2) {
        fprintf(stderr, "Not enough arguments\n");
        return EXIT_FAILURE;
    }

    rfile = argv[1];

    if (argc == 3)
        wfile = argv[2];
    else 
        wfile = STDOUT;

    if (!strcmp(argv[0], ENCODE)) {
        runEncode(rfile, wfile);
        return EXIT_SUCCESS;
    }
    else if (!strcmp(argv[0], DECODE)) {
        runDecode(rfile, wfile);
        return EXIT_SUCCESS;
    }
    else {
        fprintf(stderr, "Unkown command: %s\n", argv[0]);
        return EXIT_FAILURE;
    }
}

This is a class project implementing Huffman's algorithm for lossless compression and decomprssion in C. 

# Isaac Viviano

### Programs:

encode:
Reads a file specified as the first command-line argument, and encodes it using Huffman's algorithm. It writes output to the file given as the second comand line argument or stdout if no file is given.

decode:
Reads a file specified as the first command-line argument that was encoded by encode. Decodes the file and writes output to the file given as the second comand line argument or stdout if no file is given.

encode2:
Runs the Huffman encode algorithm using 2-character sequences.

decode2:
Runs the Huffman decode algorithm on files encoded by encode2.

compare.sh
Takes an input file specifies as the first command-line argument. Encodes the input file using encode and encode2. Prints the size of the encoded file after encoding to compare the compression from 1 and 2 character sequences.

### Files:

main.c:
Contains the main function for the encode and decode programs.
encode.c:
Contains definitions for functions used exclusively by the encode program.
decode.c:
Contains definitions for functions used exclusively by the decode program.
HuffmanTree.h:
Contains the type definition and function declarations for using the HuffmanTree data type.
functions.h:
Contains the function declarations for both encode and decode programs.
functions.c:
Contains definitions for the functions used by both programs.
*2*:
The equivalent versions of each file for the encode2 and decode2 programs.
compare.sh:
Contains the linux script for the compare program.


As far as I know, there are no errors or bugs. It runs under valgrind with no errors or memory leaks.


Extra Credit:
I implemented compression with 2-character sequences and a compare program to show how effective the compression algorithms are. For very large files (>5MB), the 2-character compression becomes significantly better. For a 1GB file, it was about twice as good as the single character compression program: the output for encode was 126M and the output for encode2 was 63M.

I have adhered to the Honor Code on this assignment.
Isaac Viviano

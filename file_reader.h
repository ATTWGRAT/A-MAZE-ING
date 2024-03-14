#ifndef INCLUDEfile_reader_h_
#define INCLUDEfile_reader_h_

#include <stdio.h>
#include <stdlib.h>
#include "node.h"

#define NODE_OFFSET 36 //Each node is 9 ints or 36 bytes
#define DJ_LENGTH_OFFSET 32 //Not skipping last int

typedef struct f{
    FILE* file;
    int exit_node;
    int curr_node;
} file_line;

node read_single_node(int nr, file_line* f);
file_line* open_processed_file(char* name);
int write_dj_length(int nr, int length, file_line* f);
#endif

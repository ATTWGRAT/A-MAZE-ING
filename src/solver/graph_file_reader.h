#ifndef INCLUDEfile_reader_h_
#define INCLUDEfile_reader_h_

#include <stdio.h>
#include <stdlib.h>

#define NODE_OFFSET 36 //Each node is 9 ints or 36 bytes
#define DJ_LENGTH_OFFSET 32 //Not skipping last int

typedef struct e {
    int next;
    int length;
} edge;

typedef struct n {
    int nr;
    edge nextN;
    edge nextE;
    edge nextS;
    edge nextW;
    unsigned int dj_length;
} node;

typedef struct f{
    FILE* file;
    int exit_node;
    int curr_node;
    int nodes_amount;
} graph_file;

node read_single_node(int nr, graph_file* f);
graph_file* open_processed_file(char* name);
int write_dj_length(int nr, int length, graph_file* f);
int read_dj_length(int nr, graph_file* gf);
#endif

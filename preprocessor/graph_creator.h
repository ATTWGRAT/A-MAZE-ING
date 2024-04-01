#ifndef INCLUDE_graph_creator_h
#define INCLUDE_graph_creator_h

#include <stdio.h>
#include <stdlib.h>
#include "map_struct.h"

typedef enum _preprocessor_directions{
    N,
    E,
    S,
    W,
} directions;

typedef struct _preprocessor_edge{
    int length;
    int next;
} edge;

typedef struct _preprocessor_node{
    coords coords;
    edge N,E,S,W;
} node;

typedef struct _preprocessor_graph{
    node* nodes;
    int size;
    int length;
    int exit_index;
} graph;

graph* graphize(maze_map*);

int write_graph(graph*, FILE*);

#endif

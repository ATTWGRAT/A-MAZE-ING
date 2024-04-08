#ifndef INCLUDE_graph_creator_h
#define INCLUDE_graph_creator_h

#include <stdio.h>
#include <stdlib.h>
#include "map_struct.h"

#define INTERSECTION 1
#define VISITED 2
#define DEAD_END 3
#define WALL 4
#define EXIT 5

#define IGNORE_NODE -1
#define FALSE_VISITED_ERROR -2
#define MEMORY_REALLOCATION_ERROR -3

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

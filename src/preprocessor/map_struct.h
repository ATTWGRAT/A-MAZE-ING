#ifndef INCLUDE_map_struct_h_
#define INCLUDE_map_struct_h_
#include <stdio.h>
#include <stdlib.h>

typedef struct _preprocessor_coords{
    int x, y;
}coords;

typedef struct _preprocessor_map{
    char** maze;
    int x, y;
    coords entrance, exit;
    char wall, path;
}maze_map;

void free_maze_map(maze_map*);
#endif



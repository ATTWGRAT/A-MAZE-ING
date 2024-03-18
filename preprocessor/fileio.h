#ifndef _fileio_h_include_
#define _fileio_h_include_

#include <stdio.h>
#include <stdlib.h>

typedef struct m{
    char** map;
    int enx, eny, exx, exy;
}mazemap;

mazemap read_map(FILE* f, int x, int y);

#endif

#ifndef INCLUDE_map_file_h_
#define INCLUDE_map_file_h_
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "map_struct.h"

maze_map* read_uncompressed(int, int, FILE*);

maze_map* read_compressed(FILE*);

#endif  

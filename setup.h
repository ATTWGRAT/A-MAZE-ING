#ifndef A_MAZE_ING_SETUP_H
#define A_MAZE_ING_SETUP_H

#include <stdlib.h>
#include <unistd.h>

typedef struct a{
    int is_pre;
    int x;
    int y;
    char* path;
}*arguments;

arguments get_flags(int argc, char** argv);

#endif //A_MAZE_ING_SETUP_H

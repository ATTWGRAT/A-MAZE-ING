#include "map_struct.h"
#include <stdio.h>

void free_maze_map(maze_map* map)
{
    for(int i = 0; i < map->y; i++)
        free(map->maze[i]);

    free(map->maze);
    free(map);

    printf("Zwolniono mapę\n");
}

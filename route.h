#ifndef A_MAZE_ING_STACK_H
#define A_MAZE_ING_STACK_H

#include <stdlib.h>

enum direction{
    N, E, S, W
};

typedef struct ln {
    int nr;
    enum direction dir;
    int length;
} linked_node;

typedef struct s {
  linked_node *array;
  int size;
  int last;
} route;

int is_route_empty(route *r);
route make_route();
int push_route(route *proute, linked_node p);
linked_node pop_route(route *proute);
linked_node peek_route(route *proute);

#endif // A_MAZE_ING_STACK_H

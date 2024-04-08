#ifndef A_MAZE_ING_STACK_H
#define A_MAZE_ING_STACK_H

#include <stdlib.h>

enum direction{
    N = 0,
    E = 1,
    S = 2,
    W = 3
} __attribute__ ((__packed__));

struct ln {
    enum direction dir;
    short length;
} __attribute__ ((__packed__));

typedef struct ln linked_node;

struct s {
  linked_node *array;
  int size;
  int last;
} __attribute__ ((__packed__));

typedef struct s route;

int is_route_empty(route *r);
route* make_route();
int push_route(route *proute, linked_node p);
linked_node pop_route(route *proute);
linked_node peek_route(route *proute);

#endif // A_MAZE_ING_STACK_H

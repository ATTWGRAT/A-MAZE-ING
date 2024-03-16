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
} stack;

int is_stack_empty(stack *s);
stack make_stack();
int push_stack(stack *stack, linked_node p);
linked_node pop_stack(stack *stack);
linked_node peek_stack(stack *stack);

#endif // A_MAZE_ING_STACK_H

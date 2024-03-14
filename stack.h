#ifndef A_MAZE_ING_STACK_H
#define A_MAZE_ING_STACK_H

#include "node.h"
#include <stdlib.h>

typedef struct s {
  node *array;
  int size;
  int last;
} stack;

int is_stack_empty(stack *s);
stack make_stack();
int push_stack(stack *stack, node p);
node pop_stack(stack *stack);
node peek_stack(stack *stack);

#endif // A_MAZE_ING_STACK_H

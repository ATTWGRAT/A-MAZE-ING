#ifndef A_MAZE_ING_STACK_H
#define A_MAZE_ING_STACK_H

#include <stdlib.h>

typedef struct p {
  int x, y;
} point;

typedef struct s {
  point *array;
  int size;
  int last;
} stack;

int is_stack_empty(stack *s);
stack make_stack();
int push_stack(stack *stack, point p);
point pop_stack(stack *stack);
point peek_stack(stack *stack);

#endif // A_MAZE_ING_STACK_H

#include "stack.h"

int is_stack_empty(stack *s) { return s->last == -1; }

stack make_stack() {
  stack s;
  s.size = 10;
  s.last = -1;
  s.array = malloc(10 * sizeof *s.array);
  return s;
}

int push_stack(stack *s, point p) {
  if (s->last == s->size - 1) {
    s->array = realloc(s->array, s->size * 2 * sizeof *(s->array));
    if (s->array == NULL) {
      return -1;
    }
    s->size *= 2;
  }

  s->array[++(s->last)] = p;

  return 0;
}

point pop_stack(stack *stack) {
  point p = stack->array[stack->last];

  stack->last--;

  return p;
}

point peek_stack(stack *stack) { return stack->array[stack->last]; }

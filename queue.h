#ifndef A_MAZE_ING_QUEUE_H
#define A_MAZE_ING_QUEUE_H

#include "stack.h"

typedef struct q {
  point *array;
  int size;
  int first;
  int last;
} queue;

queue make_queue();
point peek_queue(queue *queue);
int push_queue(queue *queue, point p);
point pop_queue(queue *queue);

#endif

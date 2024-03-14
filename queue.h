#ifndef A_MAZE_ING_QUEUE_H
#define A_MAZE_ING_QUEUE_H

#include "node.h"
#include <stdlib.h>
typedef struct q {
  node *array;
  int size;
  int first;
  int last;
} queue;

queue make_queue();
node peek_queue(queue *queue);
int push_queue(queue *queue, node p);
node pop_queue(queue *queue);

#endif

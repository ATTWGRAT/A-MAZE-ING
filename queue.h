#ifndef A_MAZE_ING_QUEUE_H
#define A_MAZE_ING_QUEUE_H

#include <stdlib.h>

typedef struct sn{
    int nr;
    int dj_length;
} snode;

typedef struct q {
  snode *array;
  int size;
  int first;
  int last;
} queue;

queue make_queue();
snode peek_queue(queue *queue);
int push_queue(queue *queue, snode p);
snode pop_queue(queue *queue);
int is_queue_empty(queue *queue); 
#endif

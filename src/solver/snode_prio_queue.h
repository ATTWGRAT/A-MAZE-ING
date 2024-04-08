#ifndef A_MAZE_ING_QUEUE_H
#define A_MAZE_ING_QUEUE_H

#include <stdlib.h>

typedef struct sn{
    unsigned int nr;
    unsigned int dj_length;
} snode;

typedef struct q {
    snode *array;
    unsigned int size;
    unsigned int first;
    unsigned int last;
} queue;

queue make_queue();
snode peek_queue(queue *queue);
char push_queue(queue *queue, snode p);
snode pop_queue(queue *queue);
char is_queue_empty(queue *queue); 
#endif

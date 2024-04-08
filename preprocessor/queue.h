#ifndef A_MAZE_ING_QUEUE_H
#define A_MAZE_ING_QUEUE_H

#include <stdlib.h>

typedef struct q {
    int *array;
    int size;
    int first;
    int last;
} queue;

queue* make_queue();
int peek_queue(queue *queue);
int push_queue(queue *queue, int p);
int pop_queue(queue *queue);
int is_queue_empty(queue *queue); 
#endif

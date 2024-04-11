#ifndef A_MAZE_ING_QUEUE_H
#define A_MAZE_ING_QUEUE_H

#include <stdlib.h>

typedef struct _solver_node_header{
    unsigned int nr;
    unsigned int dj_length;
} node_header;

typedef struct q {
    node_header *array;
    unsigned int size;
    unsigned int first;
    unsigned int last;
} nh_queue;

nh_queue* make_queue();
node_header peek_queue(nh_queue *queue);
char push_queue(nh_queue *queue, node_header p);
node_header pop_queue(nh_queue *queue);
char is_queue_empty(nh_queue *queue); 
#endif

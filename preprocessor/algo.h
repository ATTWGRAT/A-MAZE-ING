#ifndef _algo_h_include_
#define _algo_h_include_

#include "fileio.h"

enum direction{
    N,E,S,W
};

int is_node(enum direction d, int x, int y, mazemap m);

typedef struct n {
    int x, y, nr;
} node;

typedef struct e {
    int next;
    int length;
} edge;

typedef struct b {
    node n;
    edge nextN;
    edge nextE;
    edge nextS;
    edge nextW;
} big_node;

typedef struct v {
    int size;
    int amount;
    big_node* array;
} vec;

typedef struct q {
  node *array;
  int size;
  int first;
  int last;
} queue;

vec make_vec();
queue make_queue();
node peek_queue(queue *queue);
int push_queue(queue *queue, node p);
node pop_queue(queue *queue);
int is_queue_empty(queue *queue); 
int push_vec(vec* v, big_node n);

#endif

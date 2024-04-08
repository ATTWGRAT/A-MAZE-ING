#include "queue.h"

queue* make_queue() 
{
    queue* q = malloc(sizeof *q);
    q->size = 10;
    q->last = -1;
    q->first = -1;
    q->array = malloc(10 * sizeof *q->array);
    return q;
}

int push_queue(queue *queue, int p)
{
    if (queue->first == -1) 
    {
        queue->array[0] = p;
        queue->first++;
        queue->last++;

    } 
    else if (queue->last == queue->size - 1) 
    {
        if (queue->first == 0) 
        {
            queue->array =
                realloc(queue->array, queue->size * 2 * sizeof *(queue->array));
            if (queue->array == NULL) 
            {
                return -1;
            }
            queue->size *= 2;
            queue->last++;
        } 
        else 
        {
            queue->last = 0;
        }

    } 
    else if (queue->first - 1 == queue->last) 
    {
        int *temp = malloc(queue->size * 2 * sizeof *(queue->array));

        if (temp == NULL) 
        {
            return -1;
        }

        for (int i = queue->first; i < queue->size; i++) 
        {
            temp[i - queue->first] = queue->array[i];
        }

        for (int i = 0; i <= queue->last; i++) 
        {
            temp[queue->size - queue->first + i] = queue->array[i];
        }

        free(queue->array);
        queue->array = temp;
        queue->first = 0;
        queue->last = queue->size;
        queue->size *= 2;
    } 
    else 
    {
        queue->last++;
    }



    queue->array[queue->last] = p;
    return 0;
}

int peek_queue(queue *queue) { return queue->array[queue->first]; }

int pop_queue(queue *queue)
{
    if (queue->first == queue->last) {
        int p = queue->array[queue->first];
        queue->first = -1;
        queue->last = -1;
        return p;
    }
    if (queue->first == queue->size - 1) {
        queue->first = 0;
        return queue->array[queue->size - 1];
    }
    return queue->array[queue->first++];
}

int is_queue_empty(queue *queue) 
{
    if (queue->first == -1) {
        return 1;
    }
    return 0;
}

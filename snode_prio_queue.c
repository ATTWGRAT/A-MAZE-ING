#include "snode_prio_queue.h"

queue make_queue() 
{
    queue q;
    q.size = 10;
    q.last = -1;
    q.first = -1;
    q.array = malloc(10 * sizeof *q.array);
    return q;
}

int push_queue(queue *queue, snode p) 
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
        snode *temp = malloc(queue->size * 2 * sizeof *(queue->array));

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

    int final_index = queue->last;
    int check_index;

    while(final_index != queue->first)
    {
        if(final_index == 0)
            check_index = queue->size - 1;
        else
            check_index = final_index - 1;

        if(queue->array[check_index].dj_length > p.dj_length)
        {
            queue->array[final_index] = queue->array[check_index];
            final_index = check_index;
        }
        else
            break;

    }

    queue->array[final_index] = p;
    return 0;
}

snode peek_queue(queue *queue) { return queue->array[queue->first]; }

snode pop_queue(queue *queue) 
{
    if (queue->first == queue->last) {
        snode p = queue->array[queue->first];
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

#include "nodeheader_prio_queue.h"

nh_queue* make_queue() 
{
    nh_queue* q = malloc(sizeof *q);

    if(q == NULL)
        return NULL;

    q->size = 200;
    q->last = -1;
    q->first = -1;
    q->array = malloc(200 * sizeof *q->array);

    if(q->array == NULL)
    {
        free(q);
        return NULL;
    }

    return q;
}

char push_queue(nh_queue *queue, node_header p) 
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
                realloc(queue->array, (queue->size + 200)   * sizeof *(queue->array));
            if (queue->array == NULL) 
            {
                return -1;
            }
            queue->size += 200;
            queue->last++;
        } 
        else 
        {
            queue->last = 0;
        }

    } 
    else if (queue->first - 1 == queue->last) 
    {
        node_header *temp = malloc((queue->size + 200) * sizeof *(queue->array));

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
        queue->size += 200;
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

node_header peek_queue(nh_queue *queue) { return queue->array[queue->first]; }

node_header pop_queue(nh_queue *queue) 
{
    if (queue->first == queue->last) {
        node_header p = queue->array[queue->first];
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

char is_queue_empty(nh_queue *queue) 
{
    if (queue->first == -1) {
        return 1;
    }
    return 0;
}

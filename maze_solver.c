#include "maze_solver.h"
#include "graph_file_reader.h"
#include "snode_prio_queue.h"

int dj_solve(graph_file* gf)
{
    queue snq = make_queue();

    write_dj_length(1, 0, gf);

    node curr_node, temp_node;

    unsigned int temp_length;

    unsigned int ex_length = 0xffffffff;

    snode curr_small_node = {.nr = 1, .dj_length = 0};

    push_queue(&snq, curr_small_node);

    while(!is_queue_empty(&snq))
    {
        curr_small_node = pop_queue(&snq);

        if(curr_small_node.dj_length > ex_length)
            break;

        curr_node = read_single_node(curr_small_node.nr, gf);

        if(curr_node.dj_length < curr_small_node.dj_length)
            continue;

        if(curr_node.nextN.next != 0 && (temp_length = read_dj_length(curr_node.nextN.next, gf)) > curr_node.dj_length + curr_node.nextN.length)
        {
            unsigned int new_len = curr_node.dj_length + curr_node.nextN.length;

            write_dj_length(curr_node.nextN.next, new_len, gf);

            snode new = {.nr = curr_node.nextN.next, .dj_length = new_len};

            push_queue(&snq, new);

            if(new.nr == gf->exit_node)
                ex_length = new_len;
        }

        if(curr_node.nextE.next != 0 && (temp_length = read_dj_length(curr_node.nextE.next, gf)) > curr_node.dj_length + curr_node.nextE.length)
        {
            unsigned int new_len = curr_node.dj_length + curr_node.nextE.length;

            write_dj_length(curr_node.nextE.next, new_len, gf);

            snode new = {.nr = curr_node.nextE.next, .dj_length = new_len};

        push_queue(&snq, new);

            if(new.nr == gf->exit_node)
                ex_length = new_len;
 
        }

        if(curr_node.nextS.next != 0 && (temp_length = read_dj_length(curr_node.nextS.next, gf)) > curr_node.dj_length + curr_node.nextS.length)
        {
            unsigned int new_len = curr_node.dj_length + curr_node.nextS.length;

            write_dj_length(curr_node.nextS.next, new_len, gf);

            snode new = {.nr = curr_node.nextS.next, .dj_length = new_len};

            push_queue(&snq, new);

            if(new.nr == gf->exit_node)
                ex_length = new_len;

        }

        if(curr_node.nextW.next != 0 && (temp_length = read_dj_length(curr_node.nextW.next, gf)) > curr_node.dj_length + curr_node.nextW.length)
        {
            unsigned int new_len = curr_node.dj_length + curr_node.nextW.length;

            write_dj_length(curr_node.nextW.next, new_len, gf);

            snode new = {.nr = curr_node.nextW.next, .dj_length = new_len};

            push_queue(&snq, new);

            if(new.nr == gf->exit_node)
                ex_length = new_len;
 
        }
    }

    free(snq.array);

    return 0;

}

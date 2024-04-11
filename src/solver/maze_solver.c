#include "maze_solver.h"

int dj_solve(graph_file* gf)
{
    int exit_code;

    node curr_node;

    edge north, east, south, west;

    int length_from_file, new_len;

    node_header new_nh;

    nh_queue* snq = make_queue();

    if(snq == NULL)
    {
        fprintf(stderr, "Błąd podczas alokacji pamięci na kolejkę!\n");
        return 1;
    }

    if(write_dj_length(1, 0, gf) < 0)
    {
        exit_code = 1;
        goto fileerror;
    }

    int ex_length = INT_MAX;

    node_header curr_node_header = {.nr = 1, .dj_length = 0};

    if(push_queue(snq, curr_node_header) == -1)
        goto memerror;

    while(!is_queue_empty(snq))
    {
        curr_node_header = pop_queue(snq);

        if(curr_node_header.dj_length > ex_length)
            break;

        curr_node = read_single_node(curr_node_header.nr, gf);

        if(curr_node.nr < 0)
        {
            exit_code = curr_node_header.nr;
            goto fileerror;
        }

        if(curr_node.dj_length < curr_node_header.dj_length)
            continue;

        north = curr_node.nextN;
        east = curr_node.nextE;
        south = curr_node.nextS;
        west = curr_node.nextW;

        if(north.next != 0)
        {
            length_from_file = read_dj_length(north.next, gf);

            if(length_from_file < 0)
            {
                exit_code = curr_node_header.nr;
                goto fileerror;
            }

            new_len = curr_node.dj_length + north.length;

            if(length_from_file > new_len)
            {
                if(write_dj_length(north.next, new_len, gf) < 0)
                {
                    exit_code = north.next;
                    goto fileerror;
                }

                new_nh.nr = north.next;
                new_nh.dj_length = new_len;

                if(push_queue(snq, new_nh) == -1)
                    goto memerror;

                if(new_nh.nr == gf->exit_node)
                    ex_length = new_len;
            }
        }

        if(east.next != 0)
        {
            length_from_file = read_dj_length(east.next, gf);

            if(length_from_file < 0)
            {
                exit_code = curr_node_header.nr;
                goto fileerror;
            }

            new_len = curr_node.dj_length + east.length;

            if(length_from_file > new_len)
            {
                if(write_dj_length(east.next, new_len, gf) < 0)
                {
                    exit_code = east.next;
                    goto fileerror;
                }

                new_nh.nr = east.next;
                new_nh.dj_length = new_len;

                if(push_queue(snq, new_nh) == -1)
                    goto memerror;

                if(new_nh.nr == gf->exit_node)
                    ex_length = new_len;
            }
        }

        if(south.next != 0)
        {
            length_from_file = read_dj_length(south.next, gf);

            if(length_from_file < 0)
            {
                exit_code = curr_node_header.nr;
                goto fileerror;
            }

            new_len = curr_node.dj_length + south.length;

            if(length_from_file > new_len)
            {
                if(write_dj_length(south.next, new_len, gf) < 0)
                {
                    exit_code = south.next;
                    goto fileerror;
                }

                new_nh.nr = south.next;
                new_nh.dj_length = new_len;

                if(push_queue(snq, new_nh) == -1)
                    goto memerror;

                if(new_nh.nr == gf->exit_node)
                    ex_length = new_len;
            }
        }

        if(west.next != 0)
        {
            length_from_file = read_dj_length(west.next, gf);
 
            if(length_from_file < 0)
            {
                exit_code = curr_node_header.nr;
                goto fileerror;
            }

            new_len = curr_node.dj_length + west.length;

            if(length_from_file > new_len)
            {
                if(write_dj_length(west.next, new_len, gf) < 0)
                {
                    exit_code = west.next;
                    goto fileerror;
                }

                new_nh.nr = west.next; 
                new_nh.dj_length = new_len;

                if(push_queue(snq, new_nh) == -1)
                    goto memerror;

                if(new_nh.nr == gf->exit_node)
                    ex_length = new_len;
            }
        }
    }

    free(snq->array);
    free(snq);

    return 0;

fileerror:
    fprintf(stderr, "Błąd podczas interakcji z plikiem! Węzeł nr: %d\n", exit_code);
    free(snq->array);
    free(snq);
    return -1;

memerror:
    fprintf(stderr, "Błąd podczas realokacji pamięci!\n");
    free(snq->array);
    free(snq);
    return -1;

}

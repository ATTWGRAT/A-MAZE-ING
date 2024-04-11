#include "way_out.h"

void print_way_out(route *s){
    printf("START\n");
    linked_node starting_node = peek_route(s);
    enum direction last_dir = starting_node.dir;
    int last_length = 0;
    while(!is_route_empty(s)){
        linked_node node = pop_route(s);
        enum direction dir = node.dir;
        int lenght = node.length;
        if(last_dir == dir){
            lenght += last_length;
        }else{
            printf("FORWARD %d\n",last_length);
            switch (last_dir) {
                case N:
                    switch (dir) {
                        case E:
                            printf("TURN RIGHT\n");
                            break;
                        case W:
                            printf("TURN LEFT\n");
                            break;
                    }
                    break;
                case E:
                    switch (dir) {
                        case N:
                            printf("TURN LEFT\n");
                            break;
                        case S:
                            printf("TURN RIGHT\n");
                            break;
                    }
                    break;
                case S:
                    switch (dir) {
                        case E:
                            printf("TURN LEFT\n");
                            break;
                        case W:
                            printf("TURN RIGHT\n");
                            break;
                    }
                    break;
                case W:
                    switch (dir) {
                        case N:
                            printf("TURN RIGHT\n");
                            break;
                        case S:
                            printf("TURN LEFT\n");
                            break;
                    }
                    break;
            }
        }
        last_dir = node.dir;
        last_length = lenght;
    }
    printf("FORWARD %d\n",last_length);
    printf("END\n");
}

route* find_route(graph_file* gf)
{
    int exit_code;
    unsigned int temp_len;
    unsigned int curr_len;

    route* r = make_route();

    if(r == NULL)
    {
        fprintf(stderr, "Błąd podczas alokacji pamięci!\n");
        return NULL;
    }

    node temp_node = read_single_node(gf->exit_node, gf);

    if(temp_node.nr < 0)
    {
        exit_code = gf->exit_node;
        goto fileerror;
    }

    curr_len = temp_node.dj_length;
    
    while(curr_len != 0)
    {

        if(temp_node.nextN.next != 0) 
        {
            temp_len = read_dj_length(temp_node.nextN.next, gf);

            if(temp_len < 0)
            {
                exit_code = temp_node.nextN.next;
                goto fileerror;
            }

            if(temp_len == curr_len - temp_node.nextN.length)
            {
                linked_node ln = {.length = temp_node.nextN.length, .dir = S};

                if(push_route(r, ln) == -1)
                    goto memerror;

                curr_len = temp_len;

                temp_node = read_single_node(temp_node.nextN.next, gf);

                if(temp_node.nr < 0)
                {
                    exit_code = temp_node.nextN.next;
                    goto fileerror;
                }

                continue;
            }
        }
            
        if(temp_node.nextE.next != 0 )
        {
            temp_len = read_dj_length(temp_node.nextE.next, gf);

            if(temp_len < 0)
            {
                exit_code = temp_node.nextE.next;
                goto fileerror;
            }

            if(temp_len == curr_len - temp_node.nextE.length)
            {
                linked_node ln = {.length = temp_node.nextE.length, .dir = W};

                if(push_route(r, ln) == -1)
                    goto memerror;

                curr_len = temp_len;

                temp_node = read_single_node(temp_node.nextE.next, gf);

                if(temp_node.nr < 0)
                {
                    exit_code = temp_node.nextE.next;
                    goto fileerror;
                }

                continue;
            }
        }


        if(temp_node.nextS.next != 0)
        {
            temp_len = read_dj_length(temp_node.nextS.next, gf);

            if(temp_len < 0)
            {
                exit_code = temp_node.nextS.next;
                goto fileerror;
            }

            if(temp_len == curr_len - temp_node.nextS.length)
            {
                linked_node ln = {.length = temp_node.nextS.length, .dir = N};

                if(push_route(r, ln) == -1)
                    goto memerror;

                curr_len = temp_len;

                temp_node = read_single_node(temp_node.nextS.next, gf);

                if(temp_node.nr < 0)
                {
                    exit_code = temp_node.nextS.next;
                    goto fileerror;
                }

                continue;
            }
        }


        if(temp_node.nextW.next != 0)
        {
            temp_len = read_dj_length(temp_node.nextW.next, gf);

            if(temp_len < 0)
            {
                exit_code = temp_node.nextW.next;
                goto fileerror;
            }

            if(temp_len == curr_len - temp_node.nextW.length)
            {
                linked_node ln = {.length = temp_node.nextW.length, .dir = E};

                if(push_route(r, ln) == -1)
                    goto memerror;

                curr_len = temp_len;

                temp_node = read_single_node(temp_node.nextW.next, gf);

                if(temp_node.nr < 0)
                {
                    exit_code = temp_node.nextW.next;
                    goto fileerror;
                }

                continue;
            }
        }

        fprintf(stderr, "Nie znaleziono drogi od wejścia do wyjścia! Czy na pewno podano poprawny plik?\n");
        free(r->array);
        free(r);
        return NULL;
    }

    return r;

fileerror:
    fprintf(stderr, "Błąd podczas interakcji z plikiem! Węzeł nr: %d\n", exit_code);
    free(r->array);
    free(r);
    return NULL;

memerror:
    fprintf(stderr, "Błąd podczas realokacji pamięci!\n");
    free(r->array);
    free(r);
    return NULL;
}

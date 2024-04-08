#include "way_out.h"
#include "graph_file_reader.h"
#include "route.h"

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
    route* r = make_route();
    node temp_node = read_single_node(gf->exit_node, gf);
    unsigned int curr_len = temp_node.dj_length;
    unsigned int temp_len;
    
    while(curr_len != 0)
    {

        if(temp_node.nextN.next != 0 && (temp_len = read_dj_length(temp_node.nextN.next, gf)) == curr_len - temp_node.nextN.length)
        {
            linked_node ln = {.length = temp_node.nextN.length, .dir = S};

            push_route(r, ln);

            curr_len = temp_len;

            temp_node = read_single_node(temp_node.nextN.next, gf);

            continue;
        }

        if(temp_node.nextE.next != 0 && (temp_len = read_dj_length(temp_node.nextE.next, gf)) == curr_len - temp_node.nextE.length)
        {
            linked_node ln = {.length = temp_node.nextE.length, .dir = W};

            push_route(r, ln);

            curr_len = temp_len;

            temp_node = read_single_node(temp_node.nextE.next, gf);

            continue;
        }


        if(temp_node.nextS.next != 0 && (temp_len = read_dj_length(temp_node.nextS.next, gf)) == curr_len - temp_node.nextS.length)
        {
            linked_node ln = {.length = temp_node.nextS.length, .dir = N};

            push_route(r, ln);

            curr_len = temp_len;

            temp_node = read_single_node(temp_node.nextS.next, gf);

            continue;
        }


        if(temp_node.nextW.next != 0 && (temp_len = read_dj_length(temp_node.nextW.next, gf)) == curr_len - temp_node.nextW.length)
        {
            linked_node ln = {.length = temp_node.nextW.length, .dir = E};

            push_route(r, ln);

            curr_len = temp_len;

            temp_node = read_single_node(temp_node.nextW.next, gf);

            continue;
        }

        return NULL;
    }

    return r;
}

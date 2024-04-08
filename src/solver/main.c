#include "graph_file_reader.h"
#include "maze_solver.h"
#include "way_out.h"

int main(int argc, char** argv) {
    //tests    
    /*stack s = make_stack();

    linked_node n1 = {.nr = 1, .dir = E, .length = 3};
    linked_node n2 = {.nr = 2, .dir = E, .length = 4};
    linked_node n3 = {.nr = 3, .dir = E, .length = 3};
    linked_node n4 = {.nr = 4, .dir = S, .length = 3};
    linked_node n5 = {.nr = 5, .dir = W, .length = 4};
    linked_node n6 = {.nr = 6, .dir = W, .length = 3};

    push_stack(&s, n6);
    push_stack(&s, n5);
    push_stack(&s, n4);
    push_stack(&s, n3);
    push_stack(&s, n2);
    push_stack(&s, n1);

    print_way_out(&s);
*/
   /* snode n1 = {.nr = 1, .dj_length = 3};
    snode n2 = {.nr = 2, .dj_length = 12};
    snode n3 = {.nr = 3, .dj_length = 54};
    snode n4 = {.nr = 4, .dj_length = 23};
    snode n5 = {.nr = 5, .dj_length = 8};
    snode n6 = {.nr = 6, .dj_length = 1};
    snode n7 = {.nr = 7, .dj_length = 34};
    snode n8 = {.nr = 8, .dj_length = 65};
    snode n9 = {.nr = 9, .dj_length = 90};
    snode n10 = {.nr = 10, .dj_length = 90};
    snode n11 = {.nr = 11, .dj_length = 85};
    snode n12 = {.nr = 12, .dj_length = 12};
    snode n13 = {.nr = 13, .dj_length = 32};

    queue q = make_queue();
    push_queue(&q, n1);
    push_queue(&q, n2);
    push_queue(&q, n3);
    push_queue(&q, n4);
    push_queue(&q, n5);
    push_queue(&q, n6);
    push_queue(&q, n7);
    push_queue(&q, n8);
    push_queue(&q, n9);
    push_queue(&q, n10);
    push_queue(&q, n11);
    push_queue(&q, n12);
    push_queue(&q, n13);

    while(!is_queue_empty(&q))
    {
        snode popped = pop_queue(&q);
        printf("popped: (%d, %d)\n", popped.nr, popped.dj_length);
    }
    return 0;*/

    graph_file* gf = open_processed_file(argv[1]);

    dj_solve(gf);

    route* r = find_route(gf);

    if(r == NULL){
        printf("Fuck\n");
        return 1;
    }

    print_way_out(r);

}

#include "stack.h"
#include "way_out.h"

int main() {
    
    stack s = make_stack();

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

    return 0;
}

#include "stack.h"
#include "way_out.h"
#include <stdio.h>
void print_way_out(stack *s){
    printf("START\n");
    linked_node starting_node = pop_stack(s);
    printf("FORWARD %d\n",starting_node.length);
    enum direction last_dir = starting_node.dir;
    int last_length = 0;
    while(is_stack_empty(s) != -1){
        linked_node node = pop_stack(s);
        enum direction dir = node.dir;
        int lenght = node.length;
        if(last_dir == dir){
            lenght += last_length;
        }else{
            printf("FORWARD %d\n",lenght);
            switch (last_dir) {
                case N:
                    switch (dir) {
                        case E:
                            printf("TURN RIGHT\n");
                            break;
                        case S:
                            printf("TURN RIGHT\n");
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
                        case W:
                            printf("TURN RIGHT\n");
                            printf("TURN RIGHT\n");
                            break;
                    }
                    break;
                case S:
                    switch (dir) {
                        case N:
                            printf("TURN RIGHT\n");
                            printf("TURN RIGHT\n");
                            break;
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
                        case E:
                            printf("TURN RIGHT\n");
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
        last_length = node.length;
    }
    printf("END");
}
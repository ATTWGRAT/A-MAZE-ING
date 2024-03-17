#include "stack.h"
#include "way_out.h"
#include <stdio.h>
void print_way_out(stack *s){
    printf("START\n");
    linked_node starting_node = peek_stack(s);
    enum direction last_dir = starting_node.dir;
    int last_length = 0;
    while(is_stack_empty(s) != -1){
        linked_node node = pop_stack(s);
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
    printf("END");
}
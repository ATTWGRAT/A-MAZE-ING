#include <stdio.h>
#include "fileio.h"
#include "algo.h"

int main(int argc, char** argv)
{
    if(argc != 4)
    {
        fprintf(stderr, "%s: Zła liczba argumentów (%d/3)\n", argv[0], argc-1);
        return 1;
    }
    
    FILE* f = fopen(argv[1], "r");

    if(f == NULL)
    {
        fprintf(stderr, "%s: Błąd podczas otwierania pliku: %s\n", argv[0], argv[1]);
        return 1;
    }

    mazemap m = read_map(f, atoi(argv[2]), atoi(argv[3]));    

    queue q = make_queue();       

    node n = {.x = m.enx, .y = m.eny, .nr = 0};

    vec v = make_vec();

    edge edgeN = {.next = 0, .length = 0};
    edge edgeE = {.next = 1, .length = 1};
    edge edgeS = {.next = 0, .length = 1};
    edge edgeW = {.next = 0, .length = 0};

    big_node start = {.n = n, .nextE = edgeE,
    .nextN = edgeN, .nextS = edgeS, .nextW = edgeW}; 

    push_vec(&v, start);
    node n1 = {.x = m.enx + 1, .y = m.eny, .nr = 1};
    big_node bn1 = {.n = n1, .nextN = edgeN, .nextE = edgeN, .nextW = edgeS, .nextS = edgeN};
    push_vec(&v, bn1);
    push_queue(&q, n1);

    while(!is_queue_empty(&q))
    {
        node current = pop_queue(&q);
        int code;
        int offset = 0;

        while((code = is_node(N, current.x, current.y - offset, m)) == 0)
            offset++;

        switch(code)
        {
            case 1:
                
        }
    }
}

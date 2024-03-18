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

    node n = {.x = m.enx, .y = m.eny};

    vec v = make_vec();

    
}

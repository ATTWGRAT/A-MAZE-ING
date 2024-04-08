#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "map_reader.h"
#include "graph_creator.h"

#define HELP_FLAG (void*) 0xffffffffffffffff

typedef struct _preprocessorargs{
    int x, y;
    FILE* plik;
    char is_txt;
    char* name_ptr;
} args;

args* parse_args(int argc, char** argv)
{
    args* parsed = malloc(sizeof*parsed);
    parsed->is_txt = 1;
    parsed->x = -1;
    parsed->y = -1;
    parsed->plik = NULL;
    parsed->name_ptr = NULL;

    char temp;

    while((temp = getopt (argc, argv, "x:y:f:ch")) != -1){
        switch (temp) {
            case 'x':
                if(atoi(optarg) < 2 || (atoi(optarg) > 1024)){
                    fprintf(stderr, "Podana wartość nie jest liczbą z przedziału <2; 1024>: -%c %s\n", temp, optarg);
                    return NULL;
                }
                parsed->x = atoi(optarg);
                break;

            case 'y':
                if(atoi(optarg) < 2 || (atoi(optarg) > 1024)){
                    fprintf(stderr, "Podana wartość nie jest liczbą z przedziału <2; 1024>: -%c %s\n", temp, optarg);
                    return NULL;
                }
                parsed->y = atoi(optarg);
                break;

            case 'f':
                parsed->plik = fopen(optarg, "r");
                if(parsed->plik == NULL)
                {
                    fprintf(stderr, "Błąd podczas otwierania pliku %s\n", optarg);
                    return NULL;
                }
                parsed->name_ptr = optarg;
                break;

            case 'c':
                parsed->is_txt = 0;
                break;

            case 'h':
                printf("Argumenty:\n\t-x <liczba naturalna> #liczba kolumn w labiryncie\n\t-y <liczba naturalna> #liczba wierszy w labiryncie\n\t-f <ścieżka do pliku z labiryntem>\nPrzykładowe wywołanie:\n\t./preprocessor -x 512 -y 512 -f maze-512x512.txt\n");
                return HELP_FLAG;

            case '?':
                if(optopt == 'x' || optopt == 'y' || optopt == 'f')
                    fprintf(stderr, "Opcja -%c wymaga argumentu!\n", optopt);
                else if(isprint(optopt))
                    fprintf(stderr, "Nieznana opcja: %c\n", optopt);
                else
                    fprintf(stderr, "Nieznany znak: %x\n", optopt);

                return NULL;
        }
    }
    
    if(parsed->x == -1)
    {
        fprintf(stderr, "Brak wymaganej opcji: -x\n");
        return NULL;
    }
    if(parsed->y == -1)
    {
        fprintf(stderr, "Brak wymaganej opcji: -y\n");
        return NULL;
    }
    if(parsed->plik == NULL)
    {
        fprintf(stderr, "Brak wymaganej opcji: -f\n");
    }
    return parsed; 
}

int main(int argc, char** argv)
{
    args* parsed_arguments = parse_args(argc, argv);

    if(parsed_arguments == NULL)
        return 1;
    if(parsed_arguments == HELP_FLAG)
        return 0;

    maze_map* map = read_uncompressed(parsed_arguments->x, parsed_arguments->y, parsed_arguments->plik);
    
    if(map == NULL)
        return 1;

    graph* g = graphize(map);

    if(g == NULL)
        return 1;

    char* new_name = malloc(strlen(parsed_arguments->name_ptr) + 6);
    
    if(new_name == NULL)
    {
        fprintf(stderr, "Błąd podczas alokowania pamięci\n");
        return 1;
    }

    strcpy(new_name, parsed_arguments->name_ptr);

    strcat(new_name, ".pped");

    FILE* out = fopen(new_name, "wb");

    if(out == NULL)
    {
        fprintf(stderr, "Błąd podczas tworzenia pliku o nazwie %s\n", new_name);
        return 1;
    }

    if(write_graph(g, out) == -1)
    {
        fprintf(stderr, "Błąd podczas wypisywania do pliku\n");
        return 1;
    }

    return 0;
}

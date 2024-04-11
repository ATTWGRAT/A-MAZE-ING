#include <ctype.h>
#include <unistd.h>
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
                    free(parsed);
                    return NULL;
                }
                parsed->x = atoi(optarg);
                break;

            case 'y':
                if(atoi(optarg) < 2 || (atoi(optarg) > 1024)){
                    fprintf(stderr, "Podana wartość nie jest liczbą z przedziału <2; 1024>: -%c %s\n", temp, optarg);
                    free(parsed);
                    return NULL;
                }
                parsed->y = atoi(optarg);
                break;

            case 'f':
                parsed->name_ptr = optarg;
                break;

            case 'c':
                parsed->is_txt = 0;
                break;

            case 'h':
                printf("Argumenty:\n\t-x <liczba naturalna> #liczba kolumn w labiryncie\n\t-y <liczba naturalna> #liczba wierszy w labiryncie\n\t-f <ścieżka do pliku z labiryntem>\n\t-c (jeśli plik jest skompresowany. Wtedy argumenty -x i -y nie są potrzebne.)\nPrzykładowe wywołanie:\n\t./preprocessor -x 512 -y 512 -f maze-512x512.txt\n");
                return HELP_FLAG;

            case '?':
                if(optopt == 'x' || optopt == 'y' || optopt == 'f')
                    fprintf(stderr, "Opcja -%c wymaga argumentu!\n", optopt);
                else if(isprint(optopt))
                    fprintf(stderr, "Nieznana opcja: %c\n", optopt);
                else
                    fprintf(stderr, "Nieznany znak: %x\n", optopt);

                free(parsed);
                return NULL;
        }
    }
    
    if(parsed->is_txt && parsed->x == -1)
    {
        fprintf(stderr, "Brak wymaganej opcji: -x\n");
        free(parsed);
        return NULL;
    }

    if(parsed->is_txt && parsed->y == -1)
    {
        fprintf(stderr, "Brak wymaganej opcji: -y\n");
        free(parsed);
        return NULL;
    }

    if(parsed->name_ptr == NULL)
    {
        fprintf(stderr, "Brak wymaganej opcji: -f\n");
        free(parsed);
        return NULL;
    }

    if(parsed->is_txt)
        parsed->plik = fopen(parsed->name_ptr, "r");
    else
        parsed->plik = fopen(parsed->name_ptr, "rb");

    return parsed; 
}

int main(int argc, char** argv)
{
    args* parsed_arguments = parse_args(argc, argv);

    if(parsed_arguments == NULL)
        return 1;
    if(parsed_arguments == HELP_FLAG)
        return 0;

    maze_map* map;
    
    if(parsed_arguments->is_txt)
         map = read_uncompressed(parsed_arguments->x, parsed_arguments->y, parsed_arguments->plik);
    else
         map = read_compressed(parsed_arguments->plik);

    fclose(parsed_arguments->plik);
    
    if(map == NULL)
    {
        free(parsed_arguments);
        return 1;
    }

    graph* g = graphize(map);

    free(map);

    if(g == NULL)
    {
        free(parsed_arguments);
        return 1;
    }

    char* new_name = malloc(strlen(parsed_arguments->name_ptr) + 6);
    
    if(new_name == NULL)
    {
        fprintf(stderr, "Błąd podczas alokowania pamięci\n");
        free(parsed_arguments);
        free(g);
        return 1;
    }

    strcpy(new_name, parsed_arguments->name_ptr);

    strcat(new_name, ".pped");

    FILE* out = fopen(new_name, "wb");

    if(out == NULL)
    {
        free(g);
        free(parsed_arguments);
        fprintf(stderr, "Błąd podczas tworzenia pliku o nazwie %s\n", new_name);
        return 1;
    }

    if(write_graph(g, out) == -1)
    {
        free(g);
        free(parsed_arguments);
        fclose(out);
        fprintf(stderr, "Błąd podczas wypisywania do pliku\n");
        return 1;
    }

    fclose(out);
    free(g);
    free(parsed_arguments);

    printf("Wypisano graf do pliku %s\n", new_name);

    return 0;
}

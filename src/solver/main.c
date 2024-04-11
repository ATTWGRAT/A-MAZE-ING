#include "graph_file_reader.h"
#include "maze_solver.h"
#include "way_out.h"

int main(int argc, char** argv) 
{
    graph_file* gf = open_processed_file(argv[1]);

    if(gf == NULL)
    {
        fprintf(stderr, "Nie udało się otworzyć pliku %s\n", argv[1]);
        return 1;
    }

    if(dj_solve(gf) == -1)
    {
        fclose(gf->file);
        free(gf);
        fprintf(stderr, "Błąd podczas przechodzenia po grafie.\n");
        return 1;
    }

    route* r = find_route(gf);

    if(r == NULL)
    {
        fclose(gf->file);
        free(gf);
        fprintf(stderr, "Błąd podczas szukania ścieżki. Kończę działanie.\n");
        return 1;
    }

    print_way_out(r);

    fclose(gf->file);
    free(gf);
    free(r);

    return 0;

}

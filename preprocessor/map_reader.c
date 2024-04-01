#include "map_reader.h"
#include "map_struct.h"

/* Helper functions */
int check_first_or_last_line(int x, int y, int line_nr, char* buffer, maze_map* pmap)
{
    for(int i = 0; i < 2*x+1; i++)
    {
        if(buffer[i] == 'P')
        {
            if(pmap->entrance.x != -1)
            {
                fprintf(stderr, "Błędny znak napotkany podczass wczytywania pliku: %d linia %d znak: %c\n",line_nr+1, i+1, buffer[i]);
                free(buffer);
                free_maze_map(pmap);
                return -1;
            }
            coords entrance = {.x = i, .y = line_nr};
            pmap->entrance = entrance;
        } 
        else if(buffer[i] == 'K')
        {
            if(pmap->exit.x != -1)
            {
                fprintf(stderr, "Błędny znak napotkany podczass wczytywania pliku: %d linia %d znak: %c\n", line_nr+1, i+1, buffer[i]);
                free(buffer);
                free_maze_map(pmap);
                return -1;
            }
            coords exit = {.x = i, .y = line_nr};
            pmap->exit = exit;
        }
        else if(buffer[i] != 'X')
        {
            fprintf(stderr, "Błędny znak napotkany podczas wczytywania pliku: %d linia %d znak: %c\n", line_nr+1, i+1, buffer[i]);
            free(buffer);
            free_maze_map(pmap);
            return -1;
        }
        pmap->maze[line_nr][i] = buffer[i];
    }

    return 0;
}

int check_line_start_end(int x, int y, int line_nr, char* buffer, maze_map* pmap)
{
    if(buffer[0] == 'P')
    {
        if(pmap->entrance.x != -1)
        {
            fprintf(stderr, "Znak wejścia napotkany 2 razy podczas wczytywania pliku: %d linia 1 znak\n", line_nr+1);
            free(buffer);
            free_maze_map(pmap);
            return -1;
        }
        coords entrance = {.x = 0, .y = line_nr};
        pmap->entrance = entrance;
    } 
    else if(buffer[0] == 'K')
    {
        if(pmap->exit.x != -1)
        {
            fprintf(stderr, "Znak wyjścia napotkany 2 razy podczas wczytywania pliku: %d linia 1 znak\n", line_nr+1);
            free(buffer);
            free_maze_map(pmap);
            return -1;
        }
        coords exit = {.x = 0, .y = line_nr};
        pmap->exit = exit;
    }
    else if(buffer[0] != 'X')
    {
        fprintf(stderr, "Błędny znak napotkany podczass wczytywania pliku: %d linia 1 znak: %c\n", line_nr+1, buffer[0]);
        free(buffer);
        free_maze_map(pmap);
        return -1;
    }

    pmap->maze[line_nr][0] = buffer[0];
    
    if(buffer[2*x] == 'P')
    {
        if(pmap->entrance.x != -1)
        {
            fprintf(stderr, "Znak wejścia napotkany 2 razy podczas wczytywania pliku: %d linia %d znak\n", line_nr+1, 2*x+1);
            free(buffer);
            free_maze_map(pmap);
            return -1;
        }
        coords entrance = {.x = 2*x, .y = line_nr};
        pmap->entrance = entrance;
    } 
    else if(buffer[2*x] == 'K')
    {
        if(pmap->exit.x != -1)
        {
            fprintf(stderr, "Znak wyjścia napotkany 2 razy podczas wczytywania pliku: %d linia %d znak\n", line_nr+1, 2*x+1);
            free(buffer);
            free_maze_map(pmap);
            return -1;
        }
        coords exit = {.x = 2*x, .y = line_nr};
        pmap->exit = exit;
    }
    else if(buffer[2*x] != 'X')
    {
        fprintf(stderr, "Błędny znak napotkany podczas wczytywania pliku: %d linia %d znak: %c\n", line_nr+1, 2*x+1, buffer[2*x]);
        free(buffer);
        free_maze_map(pmap);
        return -1;
    }

    pmap->maze[line_nr][2*x] = buffer[2*x];
    
    return 0;    
}


/* Public Functions */
maze_map* read_uncompressed(int x, int y, FILE* file)
{
    maze_map* pmap = malloc(sizeof*pmap);
    
    if(pmap == NULL || (pmap->maze = malloc((2*y + 1) * sizeof *pmap->maze)) == NULL)
    {
        fprintf(stderr, "Błąd podczas alokowania pamięci na mapę\n");
        return NULL;
    }
    
    coords bad_cords = {.x = -1, .y = -1};
    pmap->entrance = bad_cords;
    pmap->exit = bad_cords;
    pmap->x = 2*x+1;
    pmap->y = 2*y+1;

    for(int i = 0; i < 2*y+1; i++)
    {
        pmap->maze[i] = malloc(2*x + 1);

        if(pmap->maze[i] == NULL)
        {
            for(int j = 0; j < i; j++)
                free(pmap->maze[j]);

            fprintf(stderr, "Błąd podczas alokowania pamięci na mapę\n");
            return NULL;
        }
    }

    char* buffer = malloc(2*x + 3); //2 * x + 1 znaków, \n i \0.

    if(buffer == NULL)
    {
        fprintf(stderr, "Błąd podczas alokowania pamięci na mapę\n");
        free_maze_map(pmap);
        return NULL;
    }

    if(fgets(buffer, 2*x + 3, file) == NULL || strlen(buffer) != 2*x+2 || buffer[2*x + 1] != '\n')
    {
        fprintf(stderr, "Błąd podczas wczytywania pliku: 1 linia\n");
        free(buffer);
        free_maze_map(pmap);
        return NULL;
    }

    if(check_first_or_last_line(x, y, 0, buffer, pmap) == -1)
        return NULL;

    for(int i = 1; i < 2*y; i++)
    {
        if(fgets(buffer, 2*x + 3, file) == NULL || strlen(buffer) != 2*x+2 || buffer[2*x + 1] != '\n')
        {
            fprintf(stderr, "Błąd podczas wczytywania pliku: %d linia\n", i+1);
            free(buffer);
            free_maze_map(pmap);
            return NULL;
        }
        
        if(check_line_start_end(x, y, i, buffer, pmap) == -1)
            return NULL;
        
        for(int j = 1; j < 2*x; j++)
        {
            if(buffer[j] != 'X' && buffer[j] != ' ')
            {
                fprintf(stderr, "Błędny znak napotkany podczas wczytywania pliku: %d linia %d znak: %c\n", i+1, j+1, buffer[j]);
                free(buffer);
                free_maze_map(pmap);
                return NULL;
            }
            pmap->maze[i][j] = buffer[j];
        }
    }

    if(fgets(buffer, 2*x + 3, file) == NULL || strlen(buffer) != 2*x+2 || buffer[2*x + 1] != '\n')
    {
        fprintf(stderr, "Błąd podczas wczytywania pliku: %d linia\n", 2*y+1);
        free(buffer);
        free_maze_map(pmap);
        return NULL;
    }
    
    if(check_first_or_last_line(x, y, 2*y, buffer, pmap) == -1)
        return NULL;

    return pmap;

}



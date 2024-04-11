#include "map_reader.h"

struct binary_file_header{
    int File_ID;
    char ESC;
    short columns;
    short lines;
    short entryX;
    short entryY;
    short exitX;
    short exitY;
    char reserved[12];
    int counter;
    int solution_offset;
    char separator;
    char wall;
    char path;
}__attribute__((packed));

struct code_word{
    char separator;
    char value;
    unsigned char count;
}__attribute__((packed));


/* Helper functions */
int check_first_or_last_line(int x, int y, int line_nr, char* buffer, maze_map* pmap)
{
    /* Sprawdza całą linię w poszukiwaniu wejścia i wyjścia (znaku 'P' lub 'K'). Jeśli znak 
     * to nie 'P', 'K' lub 'X', funkcja zwraca -1 oznaczające błąd.
     * To samo funkcja zwraca w przypadku napotkania 2 wejścia lub wyjścia w mapie. */

    for(int i = 0; i < 2*x+1; i++)
    {
        switch(buffer[i])
        {
            case 'P':
                if(pmap->entrance.x != -1)
                {
                    fprintf(stderr, "Błędny znak napotkany podczass wczytywania pliku: %d linia %d znak: %c\n",line_nr+1, i+1, buffer[i]);
                    free(buffer);
                    free_maze_map(pmap);
                    return -1;
                }
                coords entrance = {.x = i, .y = line_nr};
                pmap->entrance = entrance;
                break;

            case 'K':
                if(pmap->exit.x != -1)
                {
                    fprintf(stderr, "Błędny znak napotkany podczass wczytywania pliku: %d linia %d znak: %c\n", line_nr+1, i+1, buffer[i]);
                    free(buffer);
                    free_maze_map(pmap);
                    return -1;
                }
                coords exit = {.x = i, .y = line_nr};
                pmap->exit = exit;
                break;

            case 'X':
                break;

            default:
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
    /* Sprawdza czy początek lub koniec linii jest wejściem lub wyjściem. W przeciwnym wypadku jeśli nie ma tam
     * ściany, to zwraca -1 (błąd). Jeśli napotkano wejście lub wyjście 2 raz również zwracane jest -1. */


    // Początek linii
    
    switch(buffer[0])
    {
        case 'P':
            if(pmap->entrance.x != -1)
            {
                fprintf(stderr, "Znak wejścia napotkany 2 razy podczas wczytywania pliku: %d linia 1 znak\n", line_nr+1);
                free(buffer);
                free_maze_map(pmap);
                return -1;
            }
            coords entrance = {.x = 0, .y = line_nr};
            pmap->entrance = entrance;
            break;

        case 'K':
            if(pmap->exit.x != -1)
            {
                fprintf(stderr, "Znak wyjścia napotkany 2 razy podczas wczytywania pliku: %d linia 1 znak\n", line_nr+1);
                free(buffer);
                free_maze_map(pmap);
                return -1;
            }
            coords exit = {.x = 0, .y = line_nr};
            pmap->exit = exit;
            break;

        case 'X':
            break;

        default:
            fprintf(stderr, "Błędny znak napotkany podczass wczytywania pliku: %d linia 1 znak: %c\n", line_nr+1, buffer[0]);
            free(buffer);
            free_maze_map(pmap);
            return -1;
        
    }

    pmap->maze[line_nr][0] = buffer[0];

    
    // Koniec linii  2*x - ostatni znak (przed \n)
    switch(buffer[2*x])
    { 
        case 'P':
            if(pmap->entrance.x != -1)
            {
                fprintf(stderr, "Znak wejścia napotkany 2 razy podczas wczytywania pliku: %d linia %d znak\n", line_nr+1, 2*x+1);
                free(buffer);
                free_maze_map(pmap);
                return -1;
            }
            coords entrance = {.x = 2*x, .y = line_nr};
            pmap->entrance = entrance;
            break;

        case 'K':
            if(pmap->exit.x != -1)
            {
                fprintf(stderr, "Znak wyjścia napotkany 2 razy podczas wczytywania pliku: %d linia %d znak\n", line_nr+1, 2*x+1);
                free(buffer);
                free_maze_map(pmap);
                return -1;
            }
            coords exit = {.x = 2*x, .y = line_nr};
            pmap->exit = exit;
            break;

        case 'X':
            break;
        
        default:
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

    pmap->wall = 'X';
    pmap->path = ' ';
    
    //Początkowe wartości dla wejścia i wyjścia
    coords bad_cords = {.x = -1, .y = -1};
    pmap->entrance = bad_cords; 
    pmap->exit = bad_cords;

    //Rzeczywiste rozmiary 2 wymiarowej tablicy znaków
    pmap->x = 2*x+1;
    pmap->y = 2*y+1;

    for(int i = 0; i < 2*y+1; i++)
    {
        pmap->maze[i] = malloc(2*x + 1);

        if(pmap->maze[i] == NULL) //Zwalnia wszystkie wiersze do tej pory
        {
            for(int j = 0; j < i; j++)
                free(pmap->maze[j]);

            free(pmap->maze);
            free(pmap);

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

    //fgets nie zwraca informacji o ilości wczytanych znaków. 
    //Musimy więc wykorzystać strlen do sprawdzenia czy 
    //wczytano dokładnie 2*x + 2 znaki (włącznie z znakiem \n) 
    //oraz czy linia nie jest dłuższa niż 2*x + 1 (buffer[2*x + 1] == '\n')

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
        
        //Sprawdza i wczytuje początek i koniec linii, a następnie jej resztę.

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

    //To samo co przy pierwszej
    if(fgets(buffer, 2*x + 3, file) == NULL || strlen(buffer) != 2*x+2 || buffer[2*x + 1] != '\n')
    {
        fprintf(stderr, "Błąd podczas wczytywania pliku: %d linia\n", 2*y+1);
        free(buffer);
        free_maze_map(pmap);
        return NULL;
    }
    
    if(check_first_or_last_line(x, y, 2*y, buffer, pmap) == -1)
        return NULL;

    free(buffer);

    return pmap;

}

maze_map* read_compressed(FILE* f)
{
    struct binary_file_header bfh;

    if(fread(&bfh, 40, 1, f) != 1)
    {
        fprintf(stderr, "Błąd podczas wczytywania nagłówka pliku\n");
        return NULL;
    };

    if(bfh.File_ID != 0x52524243 || bfh.ESC != 0x1B)
    {
        fprintf(stderr, "Błędny nagłówek pliku!\n");
        return NULL;
    }

    if(bfh.exitX < 1 || bfh.exitX > bfh.columns || bfh.exitY < 1 || bfh.exitY > bfh.lines)
    {
        fprintf(stderr, "Błędna lokalizacja wyjścia: (%d, %d)\n", bfh.exitX, bfh.exitY);
        return NULL;
    }

    if(bfh.entryX < 1 || bfh.entryX > bfh.columns || bfh.entryY < 1 || bfh.entryY > bfh.lines)
    {
        fprintf(stderr, "Błędna lokalizacja wejścia: (%d, %d)\n", bfh.entryX, bfh.entryY);
        return NULL;
    }

    maze_map* pmap = malloc(sizeof *pmap);

    if(pmap == NULL || (pmap->maze = malloc(bfh.lines * sizeof *pmap->maze)) == NULL)
    {
        fprintf(stderr, "Błąd podczas alokowania pamięci\n");
        return NULL;
    }

    pmap->path = bfh.path;
    pmap->wall = bfh.wall;

    for(int i = 0; i < bfh.lines; i++)
    {
        pmap->maze[i] = malloc(bfh.columns);

        if(pmap->maze[i] == NULL)
        {
            for(int j = 0; j < i; j++)
                free(pmap->maze[j]);

            free(pmap->maze);
            free(pmap);

            fprintf(stderr, "Błąd podczas alokowania pamięci\n");
            return NULL;
        }
    }

    struct code_word* super_buffer = malloc(bfh.counter * sizeof * super_buffer);

    if(super_buffer == NULL)
    {
        fprintf(stderr, "Błąd podczas alokowania pamięci\n");
        free_maze_map(pmap);
        return NULL;
    }

    if(fread(super_buffer, sizeof *super_buffer, bfh.counter, f) != bfh.counter)
    {
        fprintf(stderr, "Błąd podczas wczytywania słów kodowych\n");
        free_maze_map(pmap);
        free(super_buffer);
        return NULL;
    }

    int curr_column = 0;
    int curr_line = 0;

    struct code_word current;

    for(int i = 0; i < bfh.counter; i++)
    {
        current = super_buffer[i];

        if(current.separator != bfh.separator || (current.value != bfh.path && current.value != bfh.wall))
        {
            fprintf(stderr, "Błąd podczas przetwarzania słów kodowych - słowo %d - błędny znak\n", i+1);
            free_maze_map(pmap);
            free(super_buffer);
            return NULL;
        }

        for(int j = 0; j <= current.count; j++)
        {
            pmap->maze[curr_line][curr_column++] = current.value;

            if(curr_column == bfh.columns)
            {
                curr_line++;
                curr_column = 0;

                if(curr_line == bfh.lines && (j < current.count || i+1 < bfh.counter))
                {
                    fprintf(stderr, "Błąd podczas przetwarzania słów kodowych. Liczba wczytanych znaków > Pojemność labiryntu!\n");
                    free_maze_map(pmap);
                    free(super_buffer);
                    return NULL;
                } 
            }
        }
    }

    if(curr_line != bfh.lines)
    {
        fprintf(stderr, "Błąd podczas wczytywania. Liczba wczytanych znaków < Pojemność labiryntu!\n");
        free_maze_map(pmap);
        free(super_buffer);
        return NULL;
    }

    pmap->maze[bfh.entryY - 1][bfh.entryX - 1] = 'P';
    pmap->maze[bfh.exitY - 1][bfh.exitX - 1] = 'K';

    coords entry = {.x = bfh.entryX - 1, .y = bfh.entryY - 1};
    coords exit = {.x = bfh.exitX - 1, .y = bfh.exitY - 1};

    pmap->entrance = entry;
    pmap->exit = exit;

    pmap->x = bfh.columns;
    pmap->y = bfh.lines;

    free(super_buffer);

    return pmap;
}

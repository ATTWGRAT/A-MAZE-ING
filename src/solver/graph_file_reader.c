#include "graph_file_reader.h"
#include <stdio.h>

FILE* copy_to_temp(FILE* src)
{
    int read;

    char buffer[1024];

    FILE* new_file = tmpfile();

    if(new_file == NULL)
        return NULL;

    while(!feof(src))
    {
        read = fread(buffer, 1, 1024, src);

        if(fwrite(buffer, 1, read, new_file) != read)
            return NULL;
    }

    rewind(new_file);

    return new_file;
}

graph_file* open_processed_file(char* name)
{
    FILE* file = fopen(name, "rb");

    graph_file* gf = malloc(sizeof *gf);
    
    if(file == NULL)
    {
        fprintf(stderr, "Błąd podczas otwierania pliku %s\n", name);
        goto stop;
    }

    if(fread(&(gf->exit_node), 4, 1, file ) != 1  || fread( &(gf->nodes_amount), 4, 1, file ) !=1 )
    {
        fprintf(stderr, "Błąd podczas czytania z pliku!\n");
        fclose(file);
        goto stop;
    }

    rewind(file);

    FILE* temporary_file = copy_to_temp(file);

    fclose(file);

    if(temporary_file == NULL)
    {
        fprintf(stderr, "Błąd podczas tworzenia pliku tymczasowego\n");
        goto stop;
    }
    
    gf->file = temporary_file;
    gf->curr_node = 0;

    return gf;

stop:
    free(gf);
    return NULL;

}

node read_single_node(int nr, graph_file* f) //NODES START FROM 1
{
    //🧙

    int temp_buffer[8];
    node ret = {.nr = -1};

    if(fseek(f->file, (nr - f->curr_node)*NODE_OFFSET, SEEK_CUR))
        return ret;

    if(fread(temp_buffer, 4, 8, f->file) != 8 || fread(&ret.dj_length, 4, 1, f->file) != 1)
    {
        ret.nr = -2;
        return ret;
    }

    edge N = {.next = temp_buffer[0], .length = temp_buffer[1]};
    edge E = {.next = temp_buffer[2], .length = temp_buffer[3]};
    edge S = {.next = temp_buffer[4], .length = temp_buffer[5]};
    edge W = {.next = temp_buffer[6], .length = temp_buffer[7]};

    ret.nextN = N;
    ret.nextE = E;
    ret.nextS = S;
    ret.nextW = W;

    ret.nr = nr;

    if(f->nodes_amount == nr)
    {
        rewind(f->file);
        f->curr_node = 0;
    } 
    else
    {
        f->curr_node = nr+1;
    }

    return ret;
}

int write_dj_length(int nr, unsigned int length, graph_file* f)
{
    if(fseek(f->file, (nr - f->curr_node)*NODE_OFFSET + DJ_LENGTH_OFFSET, SEEK_CUR))
        return -1;

    if(fwrite(&length, 4, 1, f->file) != 1)
        return -2;

    if(f->nodes_amount == nr)
    {
        rewind(f->file);
        f->curr_node = 0;
    } 
    else
    {
        f->curr_node = nr+1;
    }

    return 0;
}

int read_dj_length(int nr, graph_file *gf)
{
    if(fseek(gf->file, (nr - gf->curr_node)*NODE_OFFSET + DJ_LENGTH_OFFSET, SEEK_CUR))
        return -1;

    int dj;

    if(fread(&dj, 4, 1, gf->file) != 1)
        return -2;

    if(gf->nodes_amount == nr)
    {
        rewind(gf->file);
        gf->curr_node = 0;
    } 
    else
    {
        gf->curr_node = nr+1;
    }

    return dj;

}

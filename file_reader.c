#include "file_reader.h"

file_line* open_processed_file(char* name)
{
    file_line* fl = malloc(sizeof *fl);
    
    FILE* file = fopen(name, "rb+");

    if(file == NULL)
        goto stop;
    if(fread(&(fl->exit_node), 4, 1, file) != 1)
        goto stop;

    rewind(file);

    fl->file = file;
    fl->curr_node = 0;
    return fl;

stop: 
     free(fl);
     return NULL;

}

node read_single_node(int nr, file_line* f)
{
    //🧙

    int temp_buffer[9];
    node ret = {.nr = -1};

    if(fseek(f->file, (nr - f->curr_node)*NODE_OFFSET, SEEK_CUR))
        return ret;

    if(fread(temp_buffer, 4, 9, f->file) != 9)
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

    ret.dj_length = temp_buffer[8];

    f->curr_node = nr+1;

    return ret;
}

int write_dj_length(int nr, int length, file_line* f)
{
    if(fseek(f->file, (nr - f->curr_node)*NODE_OFFSET + DJ_LENGTH_OFFSET, SEEK_CUR))
        return -1;

    if(fwrite(&length, 4, 1, f->file) != 1)
        return -2;

    f->curr_node = nr+1;

    return 0;
}

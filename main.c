#include <stdio.h>
#include "file_reader.h"
int main() {
    /*FILE* f = fopen("test", "wb");
    int buffer[18] = {1, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 3, 3, 1, 4, 3, 0, 0, 521};
    fwrite(buffer, 4, 18, f);
    fflush(f);
    fclose(f);*/

    file_line* fl = open_processed_file("test");
    node n = read_single_node(1, fl);
    printf("%d, %d, %d, %d, %d, %d, %d, %d, %d\n", n.nextN.next, n.nextN.length, n.nextE.next, n.nextE.length, n.nextS.next, n.nextS.length, n.nextW.next, n.nextW.length, n.dj_length);
    
    write_dj_length(1, 123, fl);
    
}

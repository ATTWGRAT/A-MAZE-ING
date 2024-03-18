#include "fileio.h"

mazemap read_map(FILE* f, int x, int y)
{
    mazemap m = {.enx = -1, .eny = -1, .exx = -1, .exy = -1};
    
    m.map = malloc(y*sizeof*m.map);

    for(int i = 0; i < y; i++)
    {
        m.map[i] = malloc(x);

        char temp[x+2];

        fgets(temp, x+2, f);

        if(temp[0] == 'P')
        {
            m.enx = 0; m.eny = i;
        }

        if(temp[x-1] == 'K')
        {
            m.exx = x-1; m.exy = i;
        }

        for(int j = 0; j < x; j++)
            m.map[i][j] = temp[j];

    }

    return m;
}

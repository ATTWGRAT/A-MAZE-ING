#include "graph_creator.h"

graph* init_graph()
{
    graph* pgraph = malloc(sizeof *pgraph);
    pgraph->length = 0;
    pgraph->size = 8;
    pgraph->exit_index = -1;
    pgraph->nodes = malloc(8 * sizeof *(pgraph->nodes)); 

    return pgraph;
}

node init_node(coords node_coords)
{
    edge edgy = {.length = -1, .next = -1};
    node n = {.coords = node_coords, .N = edgy, .E = edgy, .S = edgy, .W = edgy};
    return n;
}

int push_graph(graph* pgraph, node new_node)
{
    if(pgraph->length == pgraph->size)
    {
        node* temp = realloc(pgraph->nodes, pgraph->size * 2 * sizeof *temp);

        if(temp == NULL)
            return -1;

        pgraph->size *= 2;
        pgraph->nodes = temp;
    }

    pgraph->nodes[pgraph->length++] = new_node;

    return pgraph->length - 1;
}

int search_for_node(coords location, graph* pgraph)
{
    int ret = -1;
    
    for(int i = 0; i < pgraph->length; i++)
        if(pgraph->nodes[i].coords.x == location.x && pgraph->nodes[i].coords.y == location.y)
            ret = i;

    return ret;
}

char is_node(coords location, directions dir, maze_map* pmap)
{
    char temp = pmap->maze[location.y][location.x];

    if(temp == 'V')
        return 2;
    else if(temp == 'X')
        return 4;
    else if(temp == 'K')
        return 5;

    char north_char = pmap->maze[location.y - 1][location.x];
    char south_char = pmap->maze[location.y + 1][location.x];
    char east_char = pmap->maze[location.y][location.x + 1];
    char west_char = pmap->maze[location.y][location.x - 1];

    switch(dir)
    {
        case N:
            if(east_char != 'X' || west_char != 'X')
                return 1;
            
            if(north_char == 'X')
                return 3;
            
            return 0;

        case E:
            if(south_char != 'X' || north_char != 'X')
                return 1;
            
            if(east_char == 'X')
                return 3;
            
            return 0;

        case S:
            if(west_char != 'X' || east_char != 'X')
                return 1;
            
            if(south_char == 'X')
                return 3;
            
            return 0;

        case W:
            if(north_char != 'X' || south_char != 'X')
                return 1;
            
            if(west_char == 'X')
                return 3;
            
            return 0;
    }

}

int search_direction(int nr, graph* pgraph, directions dir, maze_map* pmap)
{
    coords current = pgraph->nodes[nr].coords;
    int counter = 0;
    int new_nr;
    edge forwards;
    edge backwards;
    node new_node;

    int code = 0;

    while(1)
    {
        counter++;

        switch(dir)
        {
            case N:
                current.y -= 1;
                break;
            
            case E:
                current.x += 1;
                break;

            case S:
                current.y += 1;
                break;

            case W:
                current.x -= 1;
                break;
        }

        switch(is_node(current, dir, pmap))
        {
            case 4:
                return -4;

            case 2:
                new_nr = search_for_node(current, pgraph);

                forwards.next = new_nr;
                forwards.length = counter;

                backwards.next = nr;
                backwards.length = counter;

                code = -2; 
                
                break;

            case 5:
                pgraph->exit_index = pgraph->length;
                code = -5;

            case 3:
                if(!code)
                    code = -3;

            case 1:
                new_node = init_node(current); 
                
                new_nr = push_graph(pgraph, new_node);
                
                forwards.next = new_nr;
                forwards.length = counter;

                backwards.next = nr;
                backwards.length = counter;

                if(!code)
                    code = new_nr;
                
                break;

            case 0:
                break;

        }

        if(!code)
            continue;

        switch(dir)
        {
            case N:
                pgraph->nodes[nr].N = forwards;
                pgraph->nodes[new_nr].S = backwards;
                break;

            case E:
                pgraph->nodes[nr].E = forwards;
                pgraph->nodes[new_nr].W = backwards;
                break;

            case S:
                pgraph->nodes[nr].S = forwards;
                pgraph->nodes[new_nr].N = backwards;
                break;

            case W:
                pgraph->nodes[nr].W = forwards;
                pgraph->nodes[new_nr].E = backwards;
                break;
        }

        return code;
    }
}

graph* graphize(maze_map * pmap)
{
    graph* pgraph = init_graph();

    node first_node = init_node(pmap->entrance);

    int first_number = push_graph(pgraph, first_node);

    //Zrób kolejkę, wrzuć do niej first_number.
    //Następnie w loopie kolejno zdejmuj element z kolejki,
    //sprawdź wszystkie kierunki (używając search_direction) dla których edge ma .nr równy -1
    //i jeśli search_direction zwróci liczbę większą od 0, to wrzuć
    //ją na kolejkę. 
    //Loop ma się kończyć jak kolejka jest pusta.
    //Wtedy funkcja zwraca pgraph.

    return pgraph;
}


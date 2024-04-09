#include "graph_creator.h"
#include "queue.h"

graph* init_graph()
{
    graph* pgraph = malloc(sizeof *pgraph);

    if(pgraph == NULL)
        return NULL;

    pgraph->length = 0;
    pgraph->size = 8;
    pgraph->exit_index = -1;
    pgraph->nodes = malloc(8 * sizeof *(pgraph->nodes)); 

    if(pgraph->nodes == NULL)
    {
        free(pgraph);
        return NULL;
    }

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
    //Sprawdza wszystkie węzły w grafie w poszukiwaniu tego o określonych koordynatach
    int ret = -1;
    
    for(int i = 0; i < pgraph->length; i++)
        if(pgraph->nodes[i].coords.x == location.x && pgraph->nodes[i].coords.y == location.y)
            ret = i;

    return ret;
}

char is_node(coords location, directions dir, maze_map* pmap)
{
    char temp;

    //Sprawdza czy nie wychodzimy poza mapę
    if(location.x >= pmap->x || location.y >= pmap->y || location.x < 0 || location.y < 0)
        temp = 'X';
    else
        temp = pmap->maze[location.y][location.x];

    if(temp == 'V')
        return VISITED;
    else if(temp == 'K')
        return EXIT;
    else if(temp == 'X')
        return WALL;

    char north_char = pmap->maze[location.y - 1][location.x];
    char south_char = pmap->maze[location.y + 1][location.x];
    char east_char = pmap->maze[location.y][location.x + 1];
    char west_char = pmap->maze[location.y][location.x - 1];

    switch(dir)
    {
        case N:
            if(east_char != 'X' || west_char != 'X')
                return INTERSECTION;
            
            if(north_char == 'X')
                return DEAD_END;
            
            return STRAIGHT ;

        case E:
            if(south_char != 'X' || north_char != 'X')
                return INTERSECTION;
            
            if(east_char == 'X')
                return DEAD_END;
            
            return STRAIGHT;

        case S:
            if(west_char != 'X' || east_char != 'X')
                return INTERSECTION;
            
            if(south_char == 'X')
                return DEAD_END;
            
            return STRAIGHT;

        case W:
            if(north_char != 'X' || south_char != 'X')
                return INTERSECTION;
            
            if(west_char == 'X')
                return DEAD_END;
            
            return STRAIGHT;
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
        //Następna iteracja = droga dłuższa o 1
        //Loop kończy się kiedy napotkamy ścianę lub węzeł.

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
            case WALL:
                return IGNORE_NODE;

            case VISITED:
                new_nr = search_for_node(current, pgraph);
                
                if(new_nr == -1)
                    return FALSE_VISITED_ERROR;

                forwards.next = new_nr;
                forwards.length = counter;

                backwards.next = nr;
                backwards.length = counter;

                code = IGNORE_NODE; 
                
                break;

            case EXIT:
                pgraph->exit_index = pgraph->length;
            case DEAD_END:
                code = IGNORE_NODE;
            case INTERSECTION:
                new_node = init_node(current); 
                
                new_nr = push_graph(pgraph, new_node);

                if(new_nr == -1)
                    return MEMORY_REALLOCATION_ERROR;
                
                forwards.next = new_nr;
                forwards.length = counter;

                backwards.next = nr;
                backwards.length = counter;

                if(!code){
                    pmap->maze[current.y][current.x]='V';
                    code = new_nr;
                }

                break;

            case STRAIGHT:
                break;

        }

        if(code == STRAIGHT)
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

    if(pgraph == NULL)
    {
        fprintf(stderr, "Błąd podczas tworzenia grafu\n");
        return NULL;
    }

    //Tworzymy początkowy węzeł i dodajemy do grafu

    node temp_node = init_node(pmap->entrance);

    int return_code = push_graph(pgraph, temp_node);

    if(return_code == -1)
    {
        fprintf(stderr, "Błąd podczas realokacji pamięci na graf\n");
        free(pgraph->nodes);
        free(pgraph);
        return NULL;
    }

    queue* q = make_queue();

    if(push_queue(q, return_code) == -1)
    {
        return_code = MEMORY_REALLOCATION_ERROR;
        goto messg;
    }

    int temp;
    
    while (is_queue_empty(q) != 1){

        temp = pop_queue(q);

        temp_node = pgraph->nodes[temp];

        if(temp_node.N.next == -1)
        {
            if((return_code = search_direction(temp,pgraph,N,pmap)) > 0)
            {
                if(push_queue(q, return_code) == -1)
                {
                    return_code = MEMORY_REALLOCATION_ERROR;
                    goto messg;
                }
            } 
            else if(return_code != IGNORE_NODE)
                goto messg;
        }

        if(temp_node.E.next == -1)
        {
            if((return_code = search_direction(temp,pgraph,E,pmap)) > 0)
            {
                if(push_queue(q, return_code) == -1)
                {
                    return_code = MEMORY_REALLOCATION_ERROR;
                    goto messg;
                }
            } 
            else if(return_code != IGNORE_NODE)
                goto messg;
        }

        if(temp_node.S.next == -1)
        {
            if((return_code = search_direction(temp,pgraph,S,pmap)) > 0)
            {
                if(push_queue(q, return_code) == -1)
                {
                    return_code = MEMORY_REALLOCATION_ERROR;
                    goto messg;
                }
            } 
            else if(return_code != IGNORE_NODE)
                goto messg;
        }

        if(temp_node.W.next == -1)
        {
            if((return_code = search_direction(temp,pgraph,W,pmap)) > 0)
            {
                if(push_queue(q, return_code) == -1)
                {
                    return_code = MEMORY_REALLOCATION_ERROR;
                    goto messg;
                }
            }
            else if(return_code != IGNORE_NODE)
                goto messg;
        }

    }

    return pgraph;



messg:
    switch(return_code){
        case -2:
            fprintf(stderr, "GRATULACJE!!! Odkryłeś sekretny błąd. Jesteś z siebie dumny?\n");
            break;
        case -3:
            fprintf(stderr, "Błąd podczas realokacji pamięci\n");

    }


freeall:
    free(pgraph->nodes);
    free(pgraph);
    free(q->array);
    free(q);
    return NULL;


}

int write_graph(graph* pgraph, FILE* out)
{
    int buffer[9] = {pgraph->exit_index+1, pgraph->length, 0, 0, 0, 0, 0, 0};
    buffer[8] = 0xFFFFFFFF;

    node temp;

    if(fwrite(buffer, 4, 9, out) != 9)
        return -1;

    for(int i = 0; i < pgraph->length; i++)
    {
        temp = pgraph->nodes[i];

        buffer[0] = temp.N.next + 1;
        buffer[1] = temp.N.length;

        buffer[2] = temp.E.next + 1;
        buffer[3] = temp.E.length;

        buffer[4] = temp.S.next + 1;
        buffer[5] = temp.S.length;

        buffer[6] = temp.W.next + 1;
        buffer[7] = temp.W.length;

        if(fwrite(buffer, 4, 9, out) != 9)
            return -1;
    }

    return 0;
}


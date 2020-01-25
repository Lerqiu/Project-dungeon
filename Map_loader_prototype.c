#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Map_loader_prototype.h"

Prototype_map *prototype_load_map(char name[])
{

    FILE *map = fopen(name, "r");
    if (map == NULL)
    {
        printf("Error: Brak pliku %s\n", name);
        return NULL;
    }

    int X, Y;
    fscanf(map, "%i %i", &X, &Y);

    Prototype_map *board = (Prototype_map *)malloc(sizeof(Prototype_map));
    board->X = X;
    board->Y = Y;

    board->map = (Prototype_map_element *)malloc(sizeof(Prototype_map_element) * X * Y);

    char buffer[100];
    int index = 0;

    while (fscanf(map, "%s", buffer) != EOF)
    {
        char buffor_temporary[100];
        int index_of_buffer = 0;
        while (index_of_buffer < (int)(strlen(buffer)) && buffer[index_of_buffer] != ':' && buffer[index_of_buffer] != '\0' && buffer[index_of_buffer] != '\n' && buffer[index_of_buffer] != EOF)
        {
            buffor_temporary[index_of_buffer] = buffer[index_of_buffer];
            index_of_buffer++;
        }
        buffor_temporary[index_of_buffer] = '\0';
        index_of_buffer++;

        (board->map + index)->type_of_object = (char *)malloc(sizeof(char) * (strlen(buffor_temporary + 1)));
        (board->map + index)->type_of_object = (char *)malloc(sizeof(char) * 100);
        strcpy((board->map + index)->type_of_object, buffor_temporary);
        strcpy(buffor_temporary, "");
        int index_of_buffor_temporary = 0;
        while (index_of_buffer < (int)(strlen(buffer)) && buffer[index_of_buffer] != '\0' && buffer[index_of_buffer] != '\n' && buffer[index_of_buffer] != EOF)
        {
            buffor_temporary[index_of_buffor_temporary] = buffer[index_of_buffer];
            index_of_buffer++;
            index_of_buffor_temporary++;
        }
        buffor_temporary[index_of_buffor_temporary] = '\0';
        (board->map + index)->v1 = -1;
        (board->map + index)->v2 = -1;
        sscanf(buffor_temporary, "%i:%i", &((board->map + index)->v1), &((board->map + index)->v2));

        index++;
    }

    fclose(map);
    return board;
}
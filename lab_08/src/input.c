#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "create.h"
#include "input.h"


bool read_string(FILE *file, char **str)
{
    size_t len = 0;
    *str = NULL;
    if (getline(str, &len, file) == -1)
    {
        free(*str);
        return false;
    }

    len = strcspn(*str, "\n");
    if (len < 1)
    {
        free(*str);
        return false;
    }

    (*str)[len] = '\0';
    *str = realloc(*str, len + 1);
    return true;
}


int parse_int(const char *str, int max)
{
    long long tmp_int;
    char *endptr;

    if (!isdigit(str[0]))
        return -1;
    tmp_int = strtoll(str, &endptr, 10);
    if (*endptr != '\0')
        return -1;
    if (tmp_int >= max || tmp_int < 0)
        return -1;
    return (int)tmp_int;
}

bool read_int(FILE *file, int *num, int max)
{
    char *buf;

    if (!read_string(file, &buf))
        return false;

    *num = parse_int(buf, max);
    free(buf);
    if (*num < 0)
        return false;
    return true;
}

bool read_graph(graph_t *graph, FILE *file)
{
    int vertex_1, vertex_2;
    
    if (!read_int(file, &graph->number_vertices, 1000000))
    {
        puts("Ошибка чтения файла");
        return false;
    }
    if (graph->number_vertices < 2)
    {
        puts("Слишком мало вершин в графе!");
        return false;
    }
    if (!read_int(file, &graph->number_edges, 1000000))
    {
        puts("Ошибка чтения файла!");
        return false;
    }
    
    if (!create_graph(graph))
    {
        puts("Ошибка выделения памяти!");
        return false;
    }
    
    for (int i = 0; i < graph->number_edges; i++)
    {
        if (fgetc(file) != '\n')
        {
            free_graph(graph->node_list);
            puts("Ошибка чтения файла!");
            return false;
        }
        if (!read_int(file, &vertex_1, graph->number_vertices))
        {
            free_graph(graph->node_list);
            puts("Ошибка чтения файла!");
            return false;
        }
        if (!read_int(file, &vertex_2, graph->number_vertices))
        {
            free_graph(graph->node_list);
            puts("Ошибка чтения файла!");
            return false;
        }
        if (vertex_1 == vertex_2)
        {
            free_graph(graph->node_list);
            puts("Петля невозможна в неорграфе!");
            return false;
        }
        if (!add_edge(graph->node_list, vertex_1, vertex_2))
        {
            free_graph(graph->node_list);
            puts("Ребро уже есть в графе!");
            return false;
        }
    }
    return true;
}

bool input_graph(graph_t *graph)
{
    char *filename;
    FILE *file;

    puts("Введите имя файла с описанием графа:");
    if (!read_string(stdin, &filename))
    {
        puts("Ошибка ввода имени файла!");
        return false;
    }

    if (!(file = fopen(filename, "r")))
    {
        puts("Ошибка чтения файла!");
        free(filename);
        return false;
    }
    free(filename);

    if (!read_graph(graph, file))
    {
        fclose(file);
        return false;
    }
    fclose(file);
    puts("Граф успешно считан");
    return true;
}

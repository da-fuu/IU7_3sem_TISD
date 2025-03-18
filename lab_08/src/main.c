#include <stdio.h>

#include "graph.h"
#include "input.h"
#include "dot.h"


int main(void)
{
    graph_t graph;

    puts("Программа для работы с графом");
    
    if (!input_graph(&graph))
        return 1;

    show_graph(graph, "graph_before.png");
    
    if (is_tree(graph, -1))
    {
        puts("Граф уже является деревом!");
        free_graph(graph.node_list);
        return 5;
    }

    for (int i = 0; i < graph.number_vertices; i++)
        if (is_tree(graph, i))
        {
            printf("Граф можно превратить в дерево удалением вершины %d\n", i);
            delete_node(&graph, i);
            show_graph(graph, "graph_after.png");
            free_graph(graph.node_list);
            return 0;
        }

    printf("Граф нельзя превратить в дерево удалением одной вершины\n");
    free_graph(graph.node_list);
    return 0;
}

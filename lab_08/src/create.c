#include <stdlib.h>

#include "create.h"


static bool add_adj_node(adj_node_t **head, graph_node_t *graph_node)
{
    adj_node_t *new;

    new = malloc(sizeof(adj_node_t));
    if (new == NULL)
        return false;

    new->node = graph_node;
    new->next = *head;
    *head = new;
    return true;
}

static bool is_adjacent(adj_node_t *head, graph_node_t *graph_node)
{
    while (head != NULL)
    {
        if (head->node == graph_node)
            return true;
        head = head->next;
    }
    return false;
}

bool add_edge(graph_node_t *graph, int number_1, int number_2)
{
    graph_node_t *graph_node_1, *graph_node_2;

    while (graph != NULL)
    {
        if (graph->number == number_1)
            graph_node_1 = graph;
        else if (graph->number == number_2)
            graph_node_2 = graph;

        graph = graph->next;
    }

    if (is_adjacent(graph_node_1->head, graph_node_2))
        return false;
    if (is_adjacent(graph_node_2->head, graph_node_1))
        return false;

    if (!add_adj_node(&graph_node_1->head, graph_node_2))
        return false;
    if (!add_adj_node(&graph_node_2->head, graph_node_1))
        return false;

    return true;
}

bool create_graph(graph_t *graph)
{
    graph_node_t *end = NULL;
    graph->node_list = NULL;
    
    for (int i = 0; i < graph->number_vertices; i++)
        if (!push_end_graph(&graph->node_list, &end, i))
            return false;
    
    return true;
}

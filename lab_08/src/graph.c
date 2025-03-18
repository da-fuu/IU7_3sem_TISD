#include <stdlib.h>

#include "graph.h"


static void init_visited(graph_node_t *graph)
{
    while (graph != NULL)
    {
        graph->visited = false;
        graph = graph->next;
    }
}

static void depth_search(graph_node_t *graph, int vertex_to_del)
{
    adj_node_t *adj_node;
    graph->visited = true;
    
    adj_node = graph->head;
    while (adj_node != NULL)
    {
        if (!adj_node->node->visited && adj_node->node->number != vertex_to_del)
            depth_search(adj_node->node, vertex_to_del);
        adj_node = adj_node->next;
    }   
}

bool is_tree(graph_t graph, int vertex_to_del)
{
    graph_node_t *curr;
    adj_node_t *curr_adj;
    
    if (vertex_to_del != 0)
        depth_search(graph.node_list, vertex_to_del);
    else
        depth_search(graph.node_list->next, vertex_to_del);
        
    curr = graph.node_list;
    while (curr != NULL)
    {
        if (curr->number != vertex_to_del)
        {
            if (!curr->visited)
            {
                init_visited(graph.node_list);
                return false;
            }
        }
        else
        {
            graph.number_vertices--;
            curr_adj = curr->head;
            while (curr_adj != NULL)
            {
                graph.number_edges--;
                curr_adj = curr_adj->next;
            }
        }
        curr = curr->next;
    }
    init_visited(graph.node_list);
    if (graph.number_edges + 1 != graph.number_vertices)
        return false;
    
    return true;
}

static void free_node(graph_node_t *graph_node)
{
    adj_node_t *tmp_adj;
    
    while (graph_node->head != NULL)
    {
        tmp_adj = graph_node->head->next;
        free(graph_node->head);
        graph_node->head = tmp_adj;
    }
    free(graph_node);
}

void free_graph(graph_node_t *graph)
{
    graph_node_t *tmp_graph;
    
    while (graph != NULL)
    {
        tmp_graph = graph->next;
        free_node(graph);
        graph = tmp_graph;
    }
}

bool push_end_graph(graph_node_t **head, graph_node_t **end, int data)
{
    if (*head == NULL)
    {
        *head = malloc(sizeof(graph_node_t));
        *end = *head;
    }
    else
    {
        (*end)->next = malloc(sizeof(graph_node_t));
        *end = (*end)->next;
    }
    if (*end == NULL)
        return false;
    
    (*end)->number = data;
    (*end)->visited = false;
    (*end)->head = NULL;
    (*end)->next = NULL;
    return true;
}

void delete_node(graph_t *graph, int number)
{
    graph_node_t *curr;
    graph_node_t *tmp;
    adj_node_t *curr_adj, *tmp_adj;
    
    curr = graph->node_list;
    while (curr != NULL)
    {
        if (curr->head != NULL)
        {
            if (curr->head->node->number == number)
            {
                tmp_adj = curr->head->next;
                free(curr->head);
                curr->head = tmp_adj;
            }
            else
            {
                curr_adj = curr->head;
                while (curr_adj->next != NULL)
                {
                    if (curr_adj->next->node->number == number)
                    {
                        tmp_adj = curr_adj->next->next;
                        free(curr_adj->next);
                        curr_adj->next = tmp_adj;
                        graph->number_edges--;
                        break;
                    }
                    curr_adj = curr_adj->next;
                }
            }
        }
        curr = curr->next;
    }

    if (graph->node_list->number == number)
    {
        tmp = graph->node_list->next;
        free_node(graph->node_list);
        graph->node_list = tmp;
        graph->number_vertices--;
        return;
    }
    
    curr = graph->node_list;
    while (curr != NULL)
    {
        if (curr->next != NULL && curr->next->number == number)
        {
            tmp = curr->next->next;
            free_node(curr->next);
            curr->next = tmp;
            graph->number_vertices--;
            return;
        }
        curr = curr->next;
    }
}

#ifndef GRAPH_H__
#define GRAPH_H__

#include <stdbool.h>


typedef struct adj_node_t adj_node_t;

typedef struct graph_node_t graph_node_t;

struct adj_node_t
{
    graph_node_t *node;
    adj_node_t *next;
};

struct graph_node_t
{
    int number;
    bool visited;
    adj_node_t *head;
    graph_node_t *next;
};

typedef struct
{
    graph_node_t *node_list;
    int number_vertices;
    int number_edges;
} graph_t;


bool push_end_graph(graph_node_t **head, graph_node_t **end, int data);

bool is_tree(graph_t graph, int vertex_to_del);

void delete_node(graph_t *graph, int number);

void free_graph(graph_node_t *graph);


#endif // #ifndef GRAPH_H__

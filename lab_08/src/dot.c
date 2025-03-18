#include <stdio.h>
#include <stdlib.h>

#include "dot.h"


static void graph_print_edge(FILE *f, int vert_1, int vert_2)
{
    fprintf(f, "  %d -- %d;\n", vert_1, vert_2);
}

void graph_export_to_dot(FILE *f, const char *graph_name, graph_t graph)
{
    graph_node_t *curr;
    adj_node_t *curr_adj;
    
    fprintf(f, "strict graph \"%s\" {\n", graph_name);
    fprintf(f, "  node [fontname=\"Arial\"];\n");

    curr = graph.node_list;
    while (curr != NULL)
    {
        fprintf(f, "  %d;\n", curr->number);
        curr_adj = curr->head;
        while (curr_adj != NULL)
        {
            graph_print_edge(f, curr->number, curr_adj->node->number);
            curr_adj = curr_adj->next;
        }
        curr = curr->next;
    }

    fprintf(f, "}\n");
}

void show_graph(graph_t graph, const char *graph_name)
{
    char cmd[100];
    
    FILE *file = fopen("./graph.gv", "w");
    graph_export_to_dot(file, graph_name, graph);
    fclose(file);
    
    sprintf(cmd, "dot -Tpng -o \"%s\" ./graph.gv", graph_name);
    system(cmd);
    sprintf(cmd, "open \"%s\"", graph_name);
    system(cmd);
}

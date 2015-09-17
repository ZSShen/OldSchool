#include "cds.h"


#define SIZE_BUF    (1024)
#define GRAY        (0)
#define BLACK       (1)


typedef struct Graph_ {
    int count_node;
    Vector *adj_list;
    bool *visit;
    int *order;
} Graph;

/*-----------------------------------------------------------------------------*
 * In the program, I assume that all the system calls can be called normally.  *
 * Also, I apply some custom data structure libraries for implementation.      *
 * The library is in: https://github.com/ZSShen/C-Common-Data-Structures       *
 *-----------------------------------------------------------------------------*/

void clean_adjacency_list(Item item)
{
    Vector *vec = (Vector*)item;
    VectorDeinit(&vec);
}


int compare_component_size(Item item_src, Item item_tge)
{
    if (item_src == item_tge)
        return 0;
    return (item_src < item_tge)? (-1) : 1;
}


void construct_graph(char *sz_input, Graph *p_graph)
{
    FILE *fp_input = fopen(sz_input, "r");

    /* Determine the node count. */
    char buf[SIZE_BUF];
    int count_node = 0;
    int node_src, node_tge;
    while (fgets(buf, SIZE_BUF, fp_input) != NULL) {
        sscanf(buf, "%d%d", &node_src, &node_tge);
        if (node_src > count_node)
            count_node = node_src;
        if (node_tge > count_node)
            count_node = node_tge;
    }
    p_graph->count_node = count_node;

    /* Construct the adjacency list. */
    VectorInit(&(p_graph->adj_list), count_node);
    Vector *adj_list = p_graph->adj_list;
    adj_list->set_destroy(adj_list, clean_adjacency_list);

    Vector *neighbor;
    int idx;
    for (idx = 0 ; idx < count_node ; idx++) {
        VectorInit(&neighbor, 0);
        adj_list->push_back(adj_list, (Item)neighbor);
    }

    Item item;
    fseek(fp_input, 0, SEEK_SET);
    while (fgets(buf, SIZE_BUF, fp_input) != NULL) {
        sscanf(buf, "%d%d", &node_src, &node_tge);
        node_src--;
        node_tge--;
        adj_list->get(adj_list, &item, node_src);
        neighbor = (Vector*)item;
        neighbor->push_back(neighbor, (Item)(long)node_tge);
    }

    /* Initialize the visiting and order records. */
    p_graph->visit = (bool*)malloc(sizeof(bool) * count_node);
    p_graph->order = (int*)malloc(sizeof(int) * count_node);
    for (idx = 0 ; idx < count_node ; idx++) {
        p_graph->order[idx] = -1;
        p_graph->visit[idx] = false;
    }

    fclose(fp_input);
}


void destruct_graph(Graph *p_graph)
{
    VectorDeinit(&(p_graph->adj_list));
    free(p_graph->visit);
    free(p_graph->order);
}


void transpose_graph(Graph *p_graph)
{
    int count_node = p_graph->count_node;

    /* Initialize the adjacency list for transposed graph. */
    Vector *adj_copy;
    VectorInit(&adj_copy, count_node);
    adj_copy->set_destroy(adj_copy, clean_adjacency_list);
    int idx;
    for (idx = 0 ; idx < count_node ; idx++) {
        Vector *neighbor;
        VectorInit(&neighbor, 0);
        adj_copy->push_back(adj_copy, (Item)neighbor);
    }

    /* Reverse all the edges. */
    Vector *adj_origin = p_graph->adj_list;
    int node_src;
    for (node_src = 0 ; node_src < count_node ; node_src++) {
        Item item;
        Vector *nbr_src;
        adj_origin->get(adj_origin, &item, node_src);
        nbr_src = (Vector*)item;

        int count_nbr = nbr_src->size(nbr_src);
        int idx_nbr;
        for (idx_nbr = 0 ; idx_nbr < count_nbr ; idx_nbr++) {
            nbr_src->get(nbr_src, &item, idx_nbr);
            int node_tge = (int)(long)item;

            Vector *nbr_tge;
            adj_copy->get(adj_copy, &item, node_tge);
            nbr_tge = (Vector*)item;
            nbr_tge->push_back(nbr_tge, (Item)(long)node_src);
        }
    }

    /* Release the original adjacency list */
    VectorDeinit(&(p_graph->adj_list));
    p_graph->adj_list = adj_copy;

    Vector *adj_list = p_graph->adj_list;
    for (node_src = 0 ; node_src < count_node ; node_src++) {
        Item item;
        adj_list->get(adj_list, &item, node_src);
        Vector *neighbor = (Vector*)item;
    }

    /* Reset the visiting record. */
    for (idx = 0 ; idx < count_node ; idx++)
        p_graph->visit[idx] = false;
}


void dfs_first_explore(Graph *p_graph, int node_src, int *p_time)
{
    p_graph->visit[node_src] = true;

    Item item;
    p_graph->adj_list->get(p_graph->adj_list, &item, node_src);
    Vector *neighbor = (Vector*)item;

    neighbor->iterate(neighbor, true, NULL);
    while (neighbor->iterate(neighbor, false, &item) != END) {
        int node_tge = (int)(long)item;
        if (p_graph->visit[node_tge])
            continue;
        dfs_first_explore(p_graph, node_tge, p_time);
    }

    int time = *p_time;
    p_graph->order[time] = node_src;
    (*p_time)++;
}


void dfs_first_drive(Graph *p_graph)
{
    int node_src;
    int time = 0;
    for (node_src = 0 ; node_src < p_graph->count_node ; node_src++) {
        if (p_graph->visit[node_src])
            continue;
        dfs_first_explore(p_graph, node_src, &time);
    }
}


void dfs_second_explore(Graph *p_graph, int node_src, int *p_size_comp)
{
    (*p_size_comp)++;
    p_graph->visit[node_src] = true;

    Item item;
    p_graph->adj_list->get(p_graph->adj_list, &item, node_src);
    Vector *neighbor = (Vector*)item;

    neighbor->iterate(neighbor, true, NULL);
    while (neighbor->iterate(neighbor, false, &item) != END) {
        int node_tge = (int)(long)item;
        if (p_graph->visit[node_tge])
            continue;
        dfs_second_explore(p_graph, node_tge, p_size_comp);
    }
}


void dfs_second_drive(Graph *p_graph)
{
    PriorityQueue *queue;
    PriorityQueueInit(&queue);
    queue->set_compare(queue, compare_component_size);

    int idx;
    for (idx = p_graph->count_node - 1 ; idx >= 0 ; idx--) {
        int node_src = p_graph->order[idx];
        if (p_graph->visit[node_src])
            continue;

        int size_comp = 0;
        dfs_second_explore(p_graph, node_src, &size_comp);
        queue->push(queue, (Item)(long)size_comp);
    }

    idx = 0;
    while ((idx < 5) && (queue->size(queue) > 0)) {
        Item item;
        queue->top(queue, &item);
        int size_comp = (int)(long)item;
        queue->pop(queue);
        printf("The #%dth SCC with size %d.\n", idx + 1, size_comp);
        idx++;
    }

    PriorityQueueDeinit(&queue);
}


int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Please specify the input test case.\n");
        return 0;
    }

    Graph graph;
    construct_graph(argv[1], &graph);

    dfs_first_drive(&graph);
    transpose_graph(&graph);
    dfs_second_drive(&graph);

    destruct_graph(&graph);

    return 0;
}

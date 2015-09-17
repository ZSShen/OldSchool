#include "cds.h"


#define SIZE_BUF        (1024)
#define NIL             (-1)
#define INFINITE        (2147483647)


typedef struct NodeInfo_ {
    int node_id;
    int dist;
} NodeInfo;

typedef struct Graph_ {
    int count_node;
    bool *visit;
    int *short_dist;
    int *short_pred;
    Vector *adj_list;
} Graph;

/*-----------------------------------------------------------------------------*
 * In the program, I assume that all the system calls can be called normally.  *
 * Also, I apply some custom data structure libraries for implementation.      *
 * The library is in: https://github.com/ZSShen/C-Common-Data-Structures       *
 *-----------------------------------------------------------------------------*/

void clean_adjacency_list(Item item)
{
    LinkedList *list = (LinkedList*)item;
    LinkedListDeinit(&list);
}

void clean_node_info(Item item)
{
    free((NodeInfo*)item);
}

/* This comparator will cause the item with the smallest distance to appear at
   the top of the queue. And we aim to create the minimum priority queue. */
int compare_queue_node(Item item_src, Item item_tge)
{
    NodeInfo *info_src = (NodeInfo*)item_src;
    NodeInfo *info_tge = (NodeInfo*)item_tge;
    if (info_src->dist == info_tge->dist)
        return 0;
    return (info_src->dist < info_tge->dist)? 1 : (-1);
}


void construct_graph(Graph **pp_graph, char *sz_input)
{
    *pp_graph = (Graph*)malloc(sizeof(Graph));
    Graph *p_graph = *pp_graph;

    VectorInit(&(p_graph->adj_list), 0);
    Vector *adj_list = p_graph->adj_list;
    adj_list->set_destroy(adj_list, clean_adjacency_list);

    /* Determine the number of nodes in the case. */
    int count_node = 0;
    char buf[SIZE_BUF];
    FILE *fp_input = fopen(sz_input, "r");
    while (fgets(buf, SIZE_BUF, fp_input) != NULL) {
        LinkedList *list;
        LinkedListInit(&list);
        list->set_destroy(list, clean_node_info);
        adj_list->push_back(adj_list, (Item)list);
        count_node++;
    }
    p_graph->count_node = count_node;

    /* Parse the adjacency information. */
    fseek(fp_input, 0, SEEK_SET);
    while (fgets(buf, SIZE_BUF, fp_input) != NULL) {
        char *token = strtok(buf, "\t\r\n ");
        int node_src = atoi(token) - 1;

        Item item;
        adj_list->get(adj_list, &item, node_src);
        LinkedList *list = (LinkedList*)item;

        while ((token = strtok(NULL, "\t\r\n ")) != NULL) {
            int node_tge, dist;
            sscanf(token, "%d,%d", &node_tge, &dist);
            node_tge--;

            NodeInfo *info = (NodeInfo*)malloc(sizeof(NodeInfo));
            info->node_id = node_tge;
            info->dist = dist;
            list->push_back(list, (Item)info);
        }
    }

    p_graph->visit = (bool*)malloc(sizeof(bool) * count_node);
    p_graph->short_dist = (int*)malloc(sizeof(int) * count_node);
    p_graph->short_pred = (int*)malloc(sizeof(int) * count_node);

    int i;
    for (i = 0 ; i < count_node ; i++) {
        Item item;
        adj_list->get(adj_list, &item, i);
        LinkedList *list = (LinkedList*)item;
    }

    fclose(fp_input);
}


void destruct_graph(Graph *p_graph)
{
    free(p_graph->visit);
    free(p_graph->short_dist);
    free(p_graph->short_pred);
    VectorDeinit(&(p_graph->adj_list));
    free(p_graph);
}


void run_dijkstra(Graph *p_graph)
{
    int count_node = p_graph->count_node;

    bool *visit = p_graph->visit;
    int *short_dist = p_graph->short_dist;
    int *short_pred = p_graph->short_pred;
    Vector *adj_list = p_graph->adj_list;

    PriorityQueue *queue;
    PriorityQueueInit(&queue);
    queue->set_compare(queue, compare_queue_node);
    queue->set_destroy(queue, clean_node_info);

    visit[0] = false;
    short_dist[0] = 0;
    short_pred[0] = 0;
    int idx;
    for (idx = 1 ; idx < count_node ; idx++) {
        visit[idx] = false;
        short_dist[idx] = INFINITE;
        short_pred[idx] = NIL;
    }
    NodeInfo *info = (NodeInfo*)malloc(sizeof(NodeInfo));
    info->node_id = 0;
    info->dist = 0;
    queue->push(queue, (Item)info);

    while (queue->size(queue) > 0) {
        Item item;
        queue->top(queue, &item);
        NodeInfo *info_src = (NodeInfo*)item;
        int node_src = info_src->node_id;
        int optimal = info_src->dist;
        queue->pop(queue);
        if (visit[node_src])
            continue;
        visit[node_src] = true;

        adj_list->get(adj_list, &item, node_src);
        LinkedList *list = (LinkedList*)item;
        list->iterate(list, true, NULL);
        while (list->iterate(list, false, &item) != END) {
            NodeInfo *info_tge = (NodeInfo*)item;
            int node_tge = info_tge->node_id;
            int dist = info_tge->dist;
            int new_dist = optimal + dist;

            if (new_dist < short_dist[node_tge]) {
                short_dist[node_tge] = new_dist;
                short_pred[node_tge] = node_src;
                NodeInfo *info_new = (NodeInfo*)malloc(sizeof(NodeInfo));
                info_new->node_id = node_tge;
                info_new->dist = new_dist;
                queue->push(queue, (Item)info_new);
            }
        }
    }

    PriorityQueueDeinit(&queue);
}


void show_shortest_path(Graph *p_graph)
{
    int *short_dist = p_graph->short_dist;
    int *short_pred = p_graph->short_pred;
    int idx;
    for (idx = 0 ; idx < p_graph->count_node ; idx++)
        printf("[%3d] distance=%6d\tpredecessor=%3d\n", idx + 1,
                short_dist[idx], short_pred[idx]);

    printf("\nFor homework answer\n");
    printf("%d\n", short_dist[6]);
    printf("%d\n", short_dist[36]);
    printf("%d\n", short_dist[58]);
    printf("%d\n", short_dist[81]);
    printf("%d\n", short_dist[98]);
    printf("%d\n", short_dist[114]);
    printf("%d\n", short_dist[132]);
    printf("%d\n", short_dist[164]);
    printf("%d\n", short_dist[187]);
    printf("%d\n", short_dist[196]);
}


int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Please specify the input test case.\n");
        return 0;
    }

    Graph *p_graph;
    construct_graph(&p_graph, argv[1]);

    run_dijkstra(p_graph);
    show_shortest_path(p_graph);

    destruct_graph(p_graph);

    return 0;
}
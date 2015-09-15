#include <time.h>
#include <math.h>
#include "cds.h"


#define SIZE_BUF (1024)


typedef struct Graph_ {
    int count_node;
    int **matrix;
    Vector *arr_node;
} Graph;

/*-----------------------------------------------------------------------------*
 * In the program, I assume that all the system calls can be called normally.  *
 * Also, I apply some custom data structure libraries for implementation.      *
 * The library is in: https://github.com/ZSShen/C-Common-Data-Structures       *
 *-----------------------------------------------------------------------------*/

void construct_graph(char *sz_name, Graph *p_graph)
{
    VectorInit(&(p_graph->arr_node), 0);
    Vector *arr_node = p_graph->arr_node;

    /* Check how many nodes are specified in the test case. */
    char buf[SIZE_BUF];
    FILE *fp_input = fopen(sz_name, "r");
    int count_node = 0;
    while (fgets(buf, SIZE_BUF, fp_input) != NULL) {
        count_node++;
        arr_node->push_back(arr_node, (Item)(long)count_node);
    }

    p_graph->matrix = (int**)malloc(sizeof(int*) * count_node);
    int **matrix = p_graph->matrix;
    int idx_node = 0;
    while (idx_node < count_node) {
        matrix[idx_node] = (int*)malloc(sizeof(int) * count_node);
        memset(matrix[idx_node], 0, sizeof(int) * count_node);
        idx_node++;
    }
    p_graph->count_node = count_node;

    /* Link the adjacency relations between the source and target nodes. */
    fseek(fp_input, 0, SEEK_SET);
    while (fgets(buf, SIZE_BUF, fp_input) != NULL) {
        char *token = strtok(buf, "\t\r\n ");
        int node_src = atoi(token) - 1;

        while ((token = strtok(NULL, "\t\r\n ")) != NULL) {
            int node_tge = atoi(token) - 1;
            matrix[node_src][node_tge] = 1;
            matrix[node_tge][node_src] = 1;
        }
    }

    fclose(fp_input);
}


void destruct_graph(Graph *p_graph)
{
    VectorDeinit(&(p_graph->arr_node));

    int count_node = p_graph->count_node;
    int idx_node = 0;
    int **matrix = p_graph->matrix;
    while (idx_node < count_node) {
        free(matrix[idx_node]);
        idx_node++;
    }
    free(matrix);
}


void replicate_graph(Graph *p_graph_src, Graph *p_graph_tge)
{
    /* Replicate the adjacency matrix. */
    int **matrix_src = p_graph_src->matrix;
    int count_node = p_graph_src->count_node;

    p_graph_tge->matrix = (int**)malloc(sizeof(int*) * count_node);
    int **matrix_tge = p_graph_tge->matrix;

    int node_src = 0;
    while (node_src < count_node) {
        matrix_tge[node_src] = (int*)malloc(sizeof(int) * count_node);
        int node_tge = 0;
        while (node_tge < count_node) {
            matrix_tge[node_src][node_tge] = matrix_src[node_src][node_tge];
            node_tge++;
        }
        node_src++;
    }

    /* Replicate the node vector. */
    VectorInit(&(p_graph_tge->arr_node), 0);
    Vector *arr_node = p_graph_tge->arr_node;
    int idx_node = 0;
    while (idx_node < p_graph_src->count_node) {
        arr_node->push_back(arr_node, (Item)(long)idx_node);
        idx_node++;
    }

    p_graph_tge->count_node = p_graph_src->count_node;
}


int contract_graph(Graph *p_graph)
{
    int count_node = p_graph->count_node;
    int iter_node = count_node;
    int **matrix = p_graph->matrix;
    Vector *arr_node = p_graph->arr_node;

    while (iter_node > 2) {
        int node_src, node_tge, idx_rand, idx_remove;
        Item item;

        /* Randomly pick up an edge. */
        do {
            idx_rand = rand() % iter_node;
            idx_remove = idx_rand;
            arr_node->get(arr_node, &item, idx_rand);
            node_src = (int)(long)item;

            idx_rand = rand() % iter_node;
            arr_node->get(arr_node, &item, idx_rand);
            node_tge = (int)(long)item;
        } while (node_src == node_tge || matrix[node_src][node_tge] == 0);

        /* Merge the source node with the target node:
           1. The source node is removed.
           2. The edges linked to source node should be diverted to target node.
        */
        arr_node->delete(arr_node, idx_remove);

        int node_adj;
        for (node_adj = 0 ; node_adj < count_node ; node_adj++) {
            if (matrix[node_src][node_adj] == 0)
                continue;
            matrix[node_src][node_adj] = 0;
            matrix[node_adj][node_tge] += matrix[node_adj][node_src];
            matrix[node_tge][node_adj] += matrix[node_adj][node_src];
            matrix[node_adj][node_src] = 0;
        }

        /* Eliminate the self loops. */
        for (node_src = 0 ; node_src < count_node ; node_src++)
            matrix[node_src][node_src] = 0;

        iter_node--;
    }

    Item item;
    arr_node->get(arr_node, &item, 0);
    int node_src = (int)(long)item;
    arr_node->get(arr_node, &item, 1);
    int node_tge = (int)(long)item;

    return matrix[node_src][node_tge];
}


int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Please specify the input test case.\n");
        return 0;
    }

    /* Initialize the random seed. */
    srand(time(NULL));

    Graph graph_origin;
    construct_graph(argv[1], &graph_origin);

    /* Need at least O(n^4logn) iterations to acquire the minimum cut with the
       1/n failure probability. */
    int count_node = graph_origin.count_node;
    int min_cut = count_node;
    int count_trial = count_node * count_node * log(count_node);
    int idx_trial = 0;
    while (idx_trial < count_trial) {
        Graph graph_copy;
        replicate_graph(&graph_origin, &graph_copy);

        int cut = contract_graph(&graph_copy);
        //printf("The [%5d/%5d] cut: %d\n", idx_trial + 1, count_trial, cut);
        if (cut < min_cut)
            min_cut = cut;

        destruct_graph(&graph_copy);
        idx_trial++;
    }

    destruct_graph(&graph_origin);
    printf("The final minimum cut: %d\n", min_cut);

    return 0;
}

#include "cds.h"

/*-----------------------------------------------------------------------------*
 * In the program, I assume that all the system calls can be called normally.  *
 * Also, I apply some custom data structure libraries for implementation.      *
 * The library is in: https://github.com/ZSShen/C-Common-Data-Structures       *
 *-----------------------------------------------------------------------------*/

int compare_min(Item item_src, Item item_tge)
{
    int src = (int)(long)item_src;
    int tge = (int)(long)item_tge;
    if (src == tge)
        return 0;
    return (src < tge)? 1 : (-1);
}

int compare_max(Item item_src, Item item_tge)
{
    int src = (int)(long)item_src;
    int tge = (int)(long)item_tge;
    if (src == tge)
        return 0;
    return (src > tge)? 1 : (-1);
}


void process_input(char *sz_input, int **p_arr_num, int *p_count_num)
{
    FILE *fp_input = fopen(sz_input, "r");

    int num;
    int count_num = 0;
    while (fscanf(fp_input, "%d", &num) != EOF)
        count_num++;

    fseek(fp_input, 0, SEEK_SET);
    *p_arr_num = (int*)malloc(sizeof(int) * count_num);
    int *arr_num = *p_arr_num;
    int idx = 0;
    while (fscanf(fp_input, "%d", &arr_num[idx]) != EOF)
        idx++;
    *p_count_num = count_num;

    fclose(fp_input);
    return;
}


void maintain_medians(int count_num, int *arr_num, int *p_ans)
{
    PriorityQueue *min_queue;
    PriorityQueue *max_queue;
    PriorityQueueInit(&min_queue);
    PriorityQueueInit(&max_queue);
    PriorityQueueSetCompare(min_queue, compare_min);
    PriorityQueueSetCompare(max_queue, compare_max);

    int sum = arr_num[0];

    if (arr_num[0] > arr_num[1]) {
        PriorityQueuePush(min_queue, (Item)(long)arr_num[0]);
        PriorityQueuePush(max_queue, (Item)(long)arr_num[1]);
        sum += arr_num[1];
    } else {
        PriorityQueuePush(min_queue, (Item)(long)arr_num[1]);
        PriorityQueuePush(max_queue, (Item)(long)arr_num[0]);
        sum += arr_num[0];
    }

    int idx;
    for (idx = 2 ; idx < count_num ; idx++) {
        Item item;

        /* Determine which queue the new item should be inserted to.*/
        PriorityQueueTop(max_queue, &item);
        if (arr_num[idx] < (int)(long)item)
            PriorityQueuePush(max_queue, (Item)(long)arr_num[idx]);
        else
            PriorityQueuePush(min_queue, (Item)(long)arr_num[idx]);

        /* Maintain the queue balanece. */
        int size_max_queue = PriorityQueueSize(max_queue);
        int size_min_queue = PriorityQueueSize(min_queue);

        if ((size_max_queue - size_min_queue) > 1) {
            PriorityQueueTop(max_queue, &item);
            PriorityQueuePush(min_queue, item);
            PriorityQueuePop(max_queue);
        } else if ((size_min_queue - size_max_queue) > 1) {
            PriorityQueueTop(min_queue, &item);
            PriorityQueuePush(max_queue, item);
            PriorityQueuePop(min_queue);
        }

        /* Calculate the median. */
        size_max_queue = PriorityQueueSize(max_queue);
        size_min_queue = PriorityQueueSize(min_queue);
        if (size_max_queue == size_min_queue) {
            PriorityQueueTop(max_queue, &item);
            sum += (int)(long)item;
        } else {
            if (size_max_queue > size_min_queue) {
                PriorityQueueTop(max_queue, &item);
                sum += (int)(long)item;
            } else {
                PriorityQueueTop(min_queue, &item);
                sum += (int)(long)item;
            }
        }
    }

    *p_ans = sum % 10000;
    if (*p_ans < 0)
        *p_ans += 10000;

    PriorityQueueDeinit(&min_queue);
    PriorityQueueDeinit(&max_queue);
    return;
}


int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Please specify the input test case.\n");
        return 0;
    }

    int *arr_num;
    int count_num;
    process_input(argv[1], &arr_num, &count_num);

    int ans;
    maintain_medians(count_num, arr_num, &ans);
    printf("The answer is: %d.\n", ans);

    free(arr_num);
    return 0;
}
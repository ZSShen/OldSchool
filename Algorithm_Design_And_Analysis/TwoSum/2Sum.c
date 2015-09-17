#include "cds.h"
#include <time.h>

#define RGE_BGN (-10000)
#define RGE_END (10000)

/*-----------------------------------------------------------------------------*
 * In the program, I assume that all the system calls can be called normally.  *
 * Also, I apply some custom data structure libraries for implementation.      *
 * The library is in: https://github.com/ZSShen/C-Common-Data-Structures       *
 *-----------------------------------------------------------------------------*/

int compare_num(const void *p_src, const void *p_tge)
{
    long src = *(long*)p_src;
    long tge = *(long*)p_tge;
    if (src == tge)
        return 0;
    return (src > tge)? 1 : (-1);
}

void destroy_pair(Pair *p_pair)
{
    free((long*)p_pair->key);
    free(p_pair);
}


void process_input(char *sz_input, long **p_arr_num, int *p_count_num)
{
    FILE *fp_input = fopen(sz_input, "r");

    /* Count the total numbers. */
    long num;
    int count_num = 0;
    while (fscanf(fp_input, "%ld", &num) != EOF)
        count_num++;

    /* Read in the numbers. */
    fseek(fp_input, 0, SEEK_SET);
    *p_arr_num = (long*)malloc(sizeof(long) * count_num);
    long *arr_num = *p_arr_num;
    *p_count_num = count_num;

    int idx = 0;
    while (fscanf(fp_input, "%ld", &(arr_num[idx])) != EOF)
        idx++;

    fclose(fp_input);
    return;
}

void collect_2_sum_in_range(int count_num, long *arr_num, int *p_ans)
{
    qsort(arr_num, count_num, sizeof(long), compare_num);

    HashMap *pMap;
    HashMapInit(&pMap);
    HashMapSetDestroy(pMap, destroy_pair);

    int ans = 0;
    int idx;
    for (idx = 0 ; idx < count_num ; idx++) {
        long src = arr_num[idx];
        int pivot_bgn, pivot_end;

        /* Approximate the left boundary. */
        long tge = RGE_BGN - src;
        int left = idx + 1;
        int right = count_num - 1;
        int mid;
        while (left <= right) {
            mid = (left + right) >> 1;
            if (arr_num[mid] > tge)
                right = mid - 1;
            else if (arr_num[mid] < tge)
                left = mid + 1;
            else
                break;
        }
        if (arr_num[mid] >= tge)
            pivot_bgn = mid;
        else {
            if (mid == (count_num - 1))
                continue;
            pivot_bgn = mid + 1;
        }

        /* Approximate the right boundary. */
        tge = RGE_END - src;
        left = idx + 1;
        right = count_num - 1;
        while (left <= right) {
            mid = (left + right) >> 1;
            if (arr_num[mid] > tge)
                right = mid - 1;
            else if (arr_num[mid] < tge)
                left = mid + 1;
            else
                break;
        }
        if (arr_num[mid] <= tge)
            pivot_end = mid;
        else {
            if (mid == (idx + 1))
                continue;
            pivot_end = mid - 1;
        }

        /* Calculate the unique sums. */
        long sum;
        long *p_key;
        Pair *p_pair;
        for (mid = pivot_bgn ; mid <= pivot_end ; mid++) {
            sum = src + arr_num[mid];
            p_pair = (Pair*)malloc(sizeof(Pair));
            p_key = (long*)malloc(sizeof(long));
            *p_key = sum;
            p_pair->key = (Key)p_key;
            p_pair->value = NULL;
            HashMapPut(pMap, p_pair, sizeof(long));
        }
    }

    *p_ans = HashMapSize(pMap);
    HashMapDeinit(&pMap);

    return;
}


int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Please specify the input test case.");
        return 0;
    }

    long *arr_num;
    int count_num;
    process_input(argv[1], &arr_num, &count_num);

    int ans;
    collect_2_sum_in_range(count_num, arr_num, &ans);
    printf("The number of 2-sum pairs with the range: %d.\n", ans);

    free(arr_num);
    return 0;
}
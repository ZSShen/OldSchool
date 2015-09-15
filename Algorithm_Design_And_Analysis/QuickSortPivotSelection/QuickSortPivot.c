#include <stdio.h>
#include <stdlib.h>


#define PIVOT_HEAD          (0)
#define PIVOT_TAIL          (1)
#define PIVOT_MIDDLE        (2)


/*-----------------------------------------------------------------------------*
 * In the program, I assume that all the system calls can be called normally.  *
 *-----------------------------------------------------------------------------*/

int read_input(const char *sz_input, int **p_a_elem)
{
    FILE *fp_input = fopen(sz_input, "r");

    /* Count the number of elements. */
    int elem;
    int count_elem = 0;
    while (fscanf(fp_input, "%d", &elem) != EOF)
        count_elem++;

    *p_a_elem = (int*)malloc(sizeof(int) * count_elem);
    fseek(fp_input, 0, SEEK_SET);

    /* Read the elements into unsorted array. */
    int *a_elem = *p_a_elem;
    int idx_elem = 0;
    while (fscanf(fp_input, "%d", &(a_elem[idx_elem])) != EOF)
        idx_elem++;

    fclose(fp_input);

    return count_elem;
}


void count_qsort_comparison(int *a_elem, int idx_head, int idx_tail,
                            int *p_count, char mode)
{
    if (idx_head < idx_tail) {
        /* Increase the comparison count. */
        *p_count += idx_tail - idx_head;

        /* Adjust the pivot. */
        int temp, idx_mid, idx_sec;
        switch (mode) {
            case PIVOT_HEAD:
                break;
            case PIVOT_TAIL:
                temp = a_elem[idx_head];
                a_elem[idx_head] = a_elem[idx_tail];
                a_elem[idx_tail] = temp;
                break;
            case PIVOT_MIDDLE:
                idx_mid = (idx_tail + idx_head) >> 1;
                if (a_elem[idx_head] > a_elem[idx_mid]) {
                    if (a_elem[idx_mid] > a_elem[idx_tail])
                        idx_sec = idx_mid;
                    else {
                        if (a_elem[idx_head] > a_elem[idx_tail])
                            idx_sec = idx_tail;
                        else
                            idx_sec = idx_head;
                    }
                } else {
                    if (a_elem[idx_tail] > a_elem[idx_mid])
                        idx_sec = idx_mid;
                    else {
                        if (a_elem[idx_tail] > a_elem[idx_head])
                            idx_sec = idx_tail;
                        else
                            idx_sec = idx_head;
                    }
                }

                temp = a_elem[idx_head];
                a_elem[idx_head] = a_elem[idx_sec];
                a_elem[idx_sec] = temp;
                break;
        }

        /* Split the elements into two partitions via the pivot. */
        int pivot = a_elem[idx_head];
        int idx_pivot = idx_head + 1;
        int idx_cur;
        for (idx_cur = idx_head + 1 ; idx_cur <= idx_tail ; idx_cur++) {
            if (a_elem[idx_cur] >= pivot)
                continue;

            temp = a_elem[idx_cur];
            a_elem[idx_cur] = a_elem[idx_pivot];
            a_elem[idx_pivot] = temp;
            idx_pivot++;
        }
        idx_pivot--;
        temp = a_elem[idx_head];
        a_elem[idx_head] = a_elem[idx_pivot];
        a_elem[idx_pivot] = temp;

        count_qsort_comparison(a_elem, idx_head, idx_pivot - 1, p_count, mode);
        count_qsort_comparison(a_elem, idx_pivot + 1, idx_tail, p_count, mode);
    }
}


int main(int argc, char **argv)
{
    if (argc != 3) {
        printf("Please specify the input filename.\n");
        printf("Please specify the pivot selection method.\n");
        return 0;
    }

    /* Prepare the array to store input elements. */
    const char *sz_input = argv[1];
    int *a_elem;
    int count_elem = read_input(sz_input, &a_elem);

    char mode = atoi(argv[2]);
    int count = 0;
    count_qsort_comparison(a_elem, 0, count_elem - 1, &count, mode);
    printf("The pivot selection mode: %d.\n", mode);
    printf("The number of comparisons: %d.\n", count);

    free(a_elem);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>

/*-----------------------------------------------------------------------------*
 * In the program, I assume that all the system calls can be called normally.  *
 *-----------------------------------------------------------------------------*/

int read_input(const char *sz_input, int **p_arr_elem, int **p_arr_copy)
{
    FILE *fp_input = fopen(sz_input, "r");

    /* Count the number of elements. */
    int elem;
    int count_elem = 0;
    while (fscanf(fp_input, "%d", &elem) != EOF)
        count_elem++;

    *p_arr_elem = (int*)malloc(sizeof(int) * count_elem);
    *p_arr_copy = (int*)malloc(sizeof(int) * count_elem);
    fseek(fp_input, 0, SEEK_SET);

    /* Read the elements into unsorted array. */
    int *arr_elem = *p_arr_elem;
    int idx_elem = 0;
    while (fscanf(fp_input, "%d", &(arr_elem[idx_elem])) != EOF)
        idx_elem++;

    fclose(fp_input);

    return count_elem;
}


void merge_data(int idx_left, int idx_mid, int idx_right, int *arr_elem, int *arr_copy,
                long *p_invert)
{
    int bgn_left = idx_left;
    int end_left = idx_mid;
    int bgn_right = idx_mid + 1;
    int end_right = idx_right;
    int idx_merge = idx_left;

    /* Merge the elements with increasing order and put them into shadow array. */
    while (bgn_left <= end_left && bgn_right <= end_right) {
        if (arr_elem[bgn_left] <= arr_elem[bgn_right])
            arr_copy[idx_merge++] = arr_elem[bgn_left++];
        else {
            arr_copy[idx_merge++] = arr_elem[bgn_right++];
            *p_invert += end_left - bgn_left + 1;
        }
    }

    while (bgn_left <= end_left)
        arr_copy[idx_merge++] = arr_elem[bgn_left++];
    while (bgn_right <= end_right)
        arr_copy[idx_merge++] = arr_elem[bgn_right++];

    /* Put back the sorted elements. */
    idx_merge = idx_left;
    while (idx_merge <= idx_right)
        arr_elem[idx_merge] = arr_copy[idx_merge++];
}


void find_inversion_pairs(int idx_left, int idx_right, int *arr_elem, int *arr_copy,
                          long *p_invert)
{
    if (idx_left < idx_right) {
        int idx_mid = (idx_left + idx_right) >> 1;
        find_inversion_pairs(idx_left, idx_mid, arr_elem, arr_copy, p_invert);
        find_inversion_pairs(idx_mid + 1, idx_right, arr_elem, arr_copy, p_invert);
        merge_data(idx_left, idx_mid, idx_right, arr_elem, arr_copy, p_invert);
    }
}


int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Please specify the input filename.\n");
        return 0;
    }

    /* Prepare the arrays to store input elements and shadow copying. */
    const char *sz_input = argv[1];
    int *arr_elem, *arr_copy;
    int count_elem = read_input(sz_input, &arr_elem, &arr_copy);

    /* Count the number of inversion pairs. */
    long invert = 0;
    find_inversion_pairs(0, count_elem - 1, arr_elem, arr_copy, &invert);
    printf("The number of inversion pairs: %ld\n", invert);

    free(arr_elem);
    free(arr_copy);

    return 0;
}
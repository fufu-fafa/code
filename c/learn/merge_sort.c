#include <stdio.h>

void merge_sorted(int arr[], int l, int m, int r) {
    int l_len = m - l + 1;
    int r_len = r - m;

    int l_arr[l_len];
    int r_arr[r_len];

    int i, j, k;

    for (int x = 0; x < l_len; x++) {
        l_arr[x] = arr[l + x];
    }
    for (int x = 0; x < r_len; x++) {
        r_arr[x] = arr[m + 1 + x];
    }

    for (i = 0, j = 0, k = l; k <= r; k++) {
        if ((i < l_len) && (j >= r_len || l_arr[i] <= r_arr[j])) {
            arr[k] = l_arr[i];
            i++;
        } else {
            arr[k] = r_arr[j];
            j++;
        }
    }
}

void sort_recurring(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        sort_recurring(arr, l, m);
        sort_recurring(arr, m + 1, r);
        merge_sorted(arr, l, m, r);
    }
}

void merge_sort(int arr[], const int length) {
    sort_recurring(arr, 0, length - 1);
}

void print_arr(const int arr[], const int length) {
    int loop_num = 0;
    for (int x = 0; x < length; x++) {
        if (((loop_num % 8) == 0) && loop_num != 0) printf("\n");
        printf("%d, ", arr[x]);
        loop_num++;
    }
    printf("\n");
}

int main() {
    const int sumn_arr[] = {55, 55, 43, 14, 17, 0, 94, 85, 6, 74, 28, 15, 79,
                            34, 15, 32, 43, 32, 40, 14, 93, 69, 67, 8, 20, 53,
                            85, 54, 26, 83, 60, 18, 10, 30, 9, 95, 57, 4, 69, 
                            31, 99, 43, 48, 20, 84, 15, 92, 85, 24, 81, 63, 99,
                            72, 26, 17, 78, 93, 71, 94, 100, 11, 68, 96, 23};
    const int length = sizeof(sumn_arr) / sizeof(int);
    printf("unsorted:\n");
    print_arr(sumn_arr, length);
    printf("\n\n");

    int result[length];
    for (int n = 0; n < length; n++) result[n] = sumn_arr[n];

    merge_sort(result, length);
    printf("sorted:\n");
    print_arr(result, length);
    return 0;
}

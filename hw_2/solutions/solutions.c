#include "solutions.h"
#include <stdlib.h>
#include <string.h>

int running_total(int val) {
    static int current_sum = 0;
    current_sum += val;
    return current_sum;
}

int* reverse(int* arr, int len) {
    if (!arr || len <= 0) return NULL;
    
    int* res = (int*)calloc(len, sizeof(int));
    for (int i = 0; i < len; i++) {
        res[i] = arr[len - 1 - i];
    }
    return res;
}

void reverse_in_place(int* arr, int len) {
    if (!arr || len <= 0) return;
    
    for (int i = 0; i < len / 2; i++) {
        int temp = arr[i];
        arr[i] = arr[len - 1 - i];
        arr[len - 1 - i] = temp;
    }
}

int num_occurences(int* arr, int len, int val) {
    if (!arr) return 0;
    
    int count = 0;
    for (int i = 0; i < len; i++) {
        if (arr[i] == val) {
            count++;
        }
    }
    return count;
}

int* remove_duplicates(int* arr, int len, int* new_len) {
    if (!arr || len <= 0) {
        *new_len = 0;
        return NULL;
    }
    
    // just malloc enough space
    int* res = (int*)malloc(len * sizeof(int));
    int k = 0;

    for (int i = 0; i < len; i++) {
        int found = 0;
        for (int j = 0; j < k; j++) {
            if (res[j] == arr[i]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            res[k] = arr[i];
            k++;
        }
    }

    *new_len = k;
    return res;
}

char* string_reverse(const char* str) {
    if (!str) return NULL;
    
    int len = strlen(str);
    char* res = (char*)malloc((len + 1) * sizeof(char));
    
    if (res) {
        for (int i = 0; i < len; i++) {
            res[i] = str[len - 1 - i];
        }
        res[len] = '\0';
    }
    return res;
}

int* transpose(int* matrix, int rows, int cols) {
    if (!matrix || rows <= 0 || cols <= 0) return NULL;

    int* res = (int*)malloc(rows * cols * sizeof(int));
    
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            res[c * rows + r] = matrix[r * cols + c];
        }
    }
    return res;
}

void free_string_array(char** arr, int count) {
    if (!arr) return;
    for (int i = 0; i < count; i++) {
        free(arr[i]);
    }
    free(arr);
}

char** split_string(const char* str, char delimiter, int* count) {
    *count = 0;
    if (!str) return NULL;
    
    int len = strlen(str);
    if (len == 0) return NULL;

    int num_tokens = 0;
    int flag = 0; // standard flag logic
    for (int i = 0; i < len; i++) {
        if (str[i] != delimiter) {
            if (!flag) {
                num_tokens++;
                flag = 1;
            }
        } else {
            flag = 0;
        }
    }

    if (num_tokens == 0) return NULL;

    char** res = (char**)malloc(num_tokens * sizeof(char*));
    
    int idx = 0;
    flag = 0;
    int start = -1;

    for (int i = 0; i <= len; i++) {
        char c = (i < len) ? str[i] : delimiter;

        if (c != delimiter) {
            if (!flag) {
                start = i;
                flag = 1;
            }
        } else {
            if (flag) {
                int size = i - start;
                res[idx] = (char*)malloc((size + 1) * sizeof(char));
                strncpy(res[idx], str + start, size);
                res[idx][size] = '\0';
                idx++;
                flag = 0;
            }
        }
    }
    
    *count = num_tokens;
    return res;
}
#include "dynamic_array.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EPSILON 0.0001

// for comparing doubles since == doesnt work well
static int doubles_equal(double x, double y) {
  double diff = fabs(x - y);
  // printf("diff = %f\n", diff);
  if (diff < EPSILON)
    return 1;
  return 0;
}

static int index_to_offset(const DynamicArray *da, int index) {
  return index + da->origin;
}

static int offset_to_index(const DynamicArray *da, int offset) {
  return offset - da->origin;
}

static int out_of_buffer(DynamicArray *da, int offset) {
  return offset < 0 || offset >= da->capacity;
}

static void extend_buffer(DynamicArray *da) {
  double *temp = (double *)calloc(2 * da->capacity, sizeof(double));
  int new_origin = da->capacity - (da->end - da->origin) / 2,
      new_end = new_origin + (da->end - da->origin);

  for (int i = 0; i < DynamicArray_size(da); i++) {
    temp[new_origin + i] = DynamicArray_get(da, i);
  }

  free(da->buffer);
  da->buffer = temp;

  da->capacity = 2 * da->capacity;
  da->origin = new_origin;
  da->end = new_end;
}

DynamicArray *DynamicArray_new(void) {
  DynamicArray *da = (DynamicArray *)malloc(sizeof(DynamicArray));
  da->capacity = DYNAMIC_ARRAY_INITIAL_CAPACITY;
  da->buffer = (double *)calloc(da->capacity, sizeof(double));
  da->origin = da->capacity / 2;
  da->end = da->origin;
  return da;
}

void DynamicArray_destroy(DynamicArray *da) {
  if (da == NULL)
    return;

  if (da->buffer) {
    free(da->buffer);
    da->buffer = NULL;
  }
}

int DynamicArray_size(const DynamicArray *da) {
  assert(da->buffer != NULL);
  return da->end - da->origin;
}

char *DynamicArray_to_string(const DynamicArray *da) {
  assert(da->buffer != NULL);
  char *str = (char *)calloc(20, DynamicArray_size(da) + 1), // +1 for safety
      temp[20];
  int j = 1;
  str[0] = '[';
  for (int i = 0; i < DynamicArray_size(da); i++) {
    if (DynamicArray_get(da, i) == 0) {
      snprintf(temp, 20, "0");
    } else {
      snprintf(temp, 20, "%.5lf", DynamicArray_get(da, i));
    }
    if (i < DynamicArray_size(da) - 1) {
      snprintf(str + j, 20, "%s,", temp);
      j += strlen(temp) + 1;
    } else {
      snprintf(str + j, 20, "%s", temp);
      j += strlen(temp);
    }
  }
  str[j] = ']';
  return str;
}

void DynamicArray_print_debug_info(const DynamicArray *da) {
  char *s = DynamicArray_to_string(da);
  printf("  %s\n", s);
  printf("  capacity: %d\n  origin: %d\n  end: %d\n  size: %d\n\n",
         da->capacity, da->origin, da->end, DynamicArray_size(da));
  free(s);
}

void DynamicArray_set(DynamicArray *da, int index, double value) {
  assert(da->buffer != NULL);
  assert(index >= 0);
  while (out_of_buffer(da, index_to_offset(da, index))) {
    extend_buffer(da);
  }
  da->buffer[index_to_offset(da, index)] = value;
  if (index >= DynamicArray_size(da)) {
    da->end = index_to_offset(da, index + 1);
  }
}

double DynamicArray_get(const DynamicArray *da, int index) {
  assert(da->buffer != NULL);
  assert(index >= 0);
  if (index >= DynamicArray_size(da)) {
    return 0;
  } else {
    return da->buffer[index_to_offset(da, index)];
  }
}

void DynamicArray_push(DynamicArray *da, double value) {
  DynamicArray_set(da, DynamicArray_size(da), value);
}

void DynamicArray_push_front(DynamicArray *da, double value) {
  assert(da->buffer != NULL);
  while (da->origin == 0) {
    extend_buffer(da);
  }
  da->origin--;
  DynamicArray_set(da, 0, value);
}

double DynamicArray_pop(DynamicArray *da) {
  assert(DynamicArray_size(da) > 0);
  double value = DynamicArray_get(da, DynamicArray_size(da) - 1);
  DynamicArray_set(da, DynamicArray_size(da) - 1, 0.0);
  da->end--;
  return value;
}

double DynamicArray_pop_front(DynamicArray *da) {
  assert(DynamicArray_size(da) > 0);
  double value = DynamicArray_get(da, 0);
  da->origin++;
  return value;
}

DynamicArray *DynamicArray_map(const DynamicArray *da, double (*f)(double)) {
  assert(da->buffer != NULL);
  DynamicArray *result = DynamicArray_new();
  for (int i = 0; i < DynamicArray_size(da); i++) {
    DynamicArray_set(result, i, f(DynamicArray_get(da, i)));
  }
  return result;
}

DynamicArray *DynamicArray_subarray(DynamicArray *da, int a, int b) {
  assert(da->buffer != NULL);
  assert(b >= a);

  DynamicArray *result = DynamicArray_new();

  for (int i = a; i < b; i++) {
    DynamicArray_push(result, DynamicArray_get(da, i));
  }
  return result;
}

double DynamicArray_min(const DynamicArray *da) {
  assert(DynamicArray_size(da) > 0);
  double minVal = DynamicArray_get(da, 0);
  int i;
  for (i = 1; i < DynamicArray_size(da); i++) {
    if (DynamicArray_get(da, i) < minVal) {
      minVal = DynamicArray_get(da, i);
    }
  }
  return minVal;
}

double DynamicArray_max(const DynamicArray *da) {
  assert(DynamicArray_size(da) > 0);
  double maxVal = DynamicArray_get(da, 0);
  int i;
  for (i = 1; i < DynamicArray_size(da); i++) {
    if (DynamicArray_get(da, i) > maxVal) {
      maxVal = DynamicArray_get(da, i);
    }
  }
  return maxVal;
}

double DynamicArray_mean(const DynamicArray *da) {
  assert(DynamicArray_size(da) > 0);
  return DynamicArray_sum(da) / DynamicArray_size(da);
}

// bubble sort helper for median
void sort_array(double arr[], int len) {
  int i, j;
  double temp;
  for (i = 0; i < len - 1; i++) {
    for (j = 0; j < len - i - 1; j++) {
      if (arr[j] > arr[j + 1]) {
        // swap them
        temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }
    }
  }
}

double DynamicArray_median(const DynamicArray *da) {
  assert(DynamicArray_size(da) > 0);
  int sz = DynamicArray_size(da);
  double *sorted = (double *)malloc(sz * sizeof(double));

  // copy into temp array first
  int i;
  for (i = 0; i < sz; i++) {
    sorted[i] = DynamicArray_get(da, i);
  }

  // sort the array
  sort_array(sorted, sz);

  double result;
  if (sz % 2 == 0) {
    // even number of elements, take average of middle two
    result = (sorted[sz / 2 - 1] + sorted[sz / 2]) / 2.0;
  } else {
    // odd, just take the middle one
    result = sorted[sz / 2];
  }
  free(sorted);
  return result;
}

double DynamicArray_sum(const DynamicArray *da) {
  double total = 0.0;
  int i;
  for (i = 0; i < DynamicArray_size(da); i++) {
    total = total + DynamicArray_get(da, i);
  }
  return total;
}

double DynamicArray_last(const DynamicArray *da) {
  assert(DynamicArray_size(da) > 0);
  return DynamicArray_get(da, DynamicArray_size(da) - 1);
}

double DynamicArray_first(const DynamicArray *da) {
  assert(DynamicArray_size(da) > 0);
  return DynamicArray_get(da, 0);
}

DynamicArray *DynamicArray_copy(const DynamicArray *da) {
  DynamicArray *newArr = DynamicArray_new();
  int i;
  int len = DynamicArray_size(da); // store size first
  for (i = 0; i < len; i++) {
    DynamicArray_push(newArr, DynamicArray_get(da, i));
  }
  return newArr;
}

DynamicArray *DynamicArray_range(double a, double b, double step) {
  DynamicArray *arr = DynamicArray_new();
  double curr = a;
  // keep adding until we pass b
  // had to use doubles_equal here because of floating point issues
  while (curr < b || doubles_equal(curr, b)) {
    DynamicArray_push(arr, curr);
    curr = curr + step;
  }
  // printf("range created %d elements\n", DynamicArray_size(arr));
  return arr;
}

DynamicArray *DynamicArray_concat(const DynamicArray *a,
                                  const DynamicArray *b) {
  DynamicArray *res = DynamicArray_new();
  for (int i = 0; i < DynamicArray_size(a); i++) {
    DynamicArray_push(res, DynamicArray_get(a, i));
  }
  for (int i = 0; i < DynamicArray_size(b); i++) {
    DynamicArray_push(res, DynamicArray_get(b, i));
  }
  return res;
}

DynamicArray *DynamicArray_take(const DynamicArray *da, int n) {
  DynamicArray *res = DynamicArray_new();
  int size = DynamicArray_size(da);
  int i;

  // if n is 0 just return empty
  if (n == 0) {
    return res;
  }

  // positive n means take from beginning
  if (n > 0) {
    for (i = 0; i < n; i++) {
      if (i < size) {
        DynamicArray_push(res, DynamicArray_get(da, i));
      } else {
        // not enough elements, pad with 0
        DynamicArray_push(res, 0.0);
      }
    }
  } else {
    // negative n means take from end
    // first figure out where to start
    int start = size + n;
    if (start < 0) {
      start = 0;
    }

    // copy the elements from start to end
    for (i = start; i < size; i++) {
      DynamicArray_push(res, DynamicArray_get(da, i));
    }

    // check if we need to add zeros
    int howMany = -n;
    int gotSoFar = DynamicArray_size(res);
    int needMore = howMany - gotSoFar;
    if (needMore > 0) {
      for (i = 0; i < needMore; i++) {
        DynamicArray_push(res, 0.0);
      }
    }
  }
  return res;
}

DynamicArray *DynamicArray_filter(const DynamicArray *da, Predicate p) {
  DynamicArray *res = DynamicArray_new();
  for (int i = 0; i < DynamicArray_size(da); i++) {
    double val = DynamicArray_get(da, i);
    if (p(val)) {
      DynamicArray_push(res, val);
    }
  }
  return res;
}

DynamicArray *DynamicArray_unique(const DynamicArray *da) {
  DynamicArray *result = DynamicArray_new();
  int n = DynamicArray_size(da);
  int i, j;

  for (i = 0; i < n; i++) {
    double current = DynamicArray_get(da, i);
    int found = 0;

    for (j = 0; j < DynamicArray_size(result); j++) {
      if (doubles_equal(DynamicArray_get(result, j), current)) {
        found = 1;
        break;
      }
    }

    if (found == 0) {
      DynamicArray_push(result, current);
    }
  }
  return result;
}

DynamicArray **DynamicArray_split(const DynamicArray *da, int num_chunks,
                                  int *actual_chunks) {
  if (num_chunks <= 0) {
    *actual_chunks = 0;
    return NULL;
  }

  int size = DynamicArray_size(da);

  if (size == 0) {
    *actual_chunks = 0;
    return NULL;
  }

  // figure out how big each chunk should be
  int chunk_size = size / num_chunks;
  if (size % num_chunks != 0) {
    chunk_size = chunk_size + 1;
  }

  DynamicArray **chunks =
      (DynamicArray **)malloc(num_chunks * sizeof(DynamicArray *));
  int created = 0;
  int idx = 0;
  int i, j;

  for (i = 0; i < num_chunks; i++) {
    if (idx >= size)
      break;

    chunks[i] = DynamicArray_new();
    for (j = 0; j < chunk_size; j++) {
      if (idx < size) {
        DynamicArray_push(chunks[i], DynamicArray_get(da, idx));
        idx = idx + 1;
      }
    }
    created = created + 1;
  }

  *actual_chunks = created;
  return chunks;
}

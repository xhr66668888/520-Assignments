#ifndef _DYNAMIC_ARRAY
#define _DYNAMIC_ARRAY

#define DYNAMIC_ARRAY_INITIAL_CAPACITY 10

typedef struct {
  int capacity, origin, end;
  double *buffer;
} DynamicArray;

DynamicArray *DynamicArray_new(void);
void DynamicArray_destroy(DynamicArray *);

void DynamicArray_set(DynamicArray *, int, double);
double DynamicArray_get(const DynamicArray *, int);
int DynamicArray_size(const DynamicArray *);

char *DynamicArray_to_string(const DynamicArray *);
void DynamicArray_print_debug_info(const DynamicArray *);

void DynamicArray_push(DynamicArray *, double);
void DynamicArray_push_front(DynamicArray *, double);

double DynamicArray_pop(DynamicArray *);
double DynamicArray_pop_front(DynamicArray *);

DynamicArray *DynamicArray_map(const DynamicArray *, double (*)(double));

/*! Return the last value in the given array.
 */
double DynamicArray_last(const DynamicArray *da);

/*! Return the first value in the given array.
 */
double DynamicArray_first(const DynamicArray *da);

/*! Return a copy of the array.
 */
DynamicArray *DynamicArray_copy(const DynamicArray *da);

/*! Return a new array whose elements span the given range.
 */
DynamicArray *DynamicArray_range(double a, double b, double step);

/*! Return a new array that is the concatenation of the given arrays.
 */
DynamicArray *DynamicArray_concat(const DynamicArray *a, const DynamicArray *b);

/*! Returns a sub-array of n elements.
 * Positive n: from beginning. Negative n: from end.
 */
DynamicArray *DynamicArray_take(const DynamicArray *da, int n);

/* Filter Predicate type definition */
typedef int (*Predicate)(double);

/* Keeps only elements that satisfy the predicate */
DynamicArray *DynamicArray_filter(const DynamicArray *da, Predicate p);

/* Removes duplicates */
DynamicArray *DynamicArray_unique(const DynamicArray *da);

/* Splits into chunks */
DynamicArray **DynamicArray_split(const DynamicArray *da, int num_chunks,
                                  int *actual_chunks);

/*! Mathematical operations
 */
double DynamicArray_min(const DynamicArray *da);
double DynamicArray_max(const DynamicArray *da);
double DynamicArray_mean(const DynamicArray *da);
double DynamicArray_median(const DynamicArray *da);
double DynamicArray_sum(const DynamicArray *da);

/*! Extra Credit
 */
int DynamicArray_is_valid(const DynamicArray *da);
int DynamicArray_num_arrays();
void DynamicArray_destroy_all();

DynamicArray *DynamicArray_subarray(DynamicArray *, int, int);

#endif
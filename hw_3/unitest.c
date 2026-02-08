#include "dynamic_array.h"
#include "gtest/gtest.h"
#include <float.h> /* defines DBL_EPSILON */
#include <math.h>

#define X 1.2345
#define EPSILON 0.0001

namespace {

TEST(DynamicArray, CreateAndDestroy) {
  DynamicArray *a = DynamicArray_new();
  DynamicArray_destroy(a);
}

TEST(DynamicArray, DeathTests) {
  DynamicArray *a = DynamicArray_new();
  ASSERT_DEATH(DynamicArray_pop(a), ".*Assertion.*");
  ASSERT_DEATH(DynamicArray_pop_front(a), ".*Assertion.*");
  DynamicArray_destroy(a);
  ASSERT_DEATH(DynamicArray_size(a), ".*Assertion.*");
}

TEST(DynamicArray, SmallIndex) {
  DynamicArray *da = DynamicArray_new();
  ASSERT_EQ(DynamicArray_size(da), 0);
  DynamicArray_set(da, 0, -X);
  DynamicArray_set(da, 3, X);
  ASSERT_EQ(DynamicArray_size(da), 4);
  ASSERT_EQ(DynamicArray_get(da, 0), -X);
  ASSERT_EQ(DynamicArray_get(da, 3), X);
  DynamicArray_destroy(da);
}

TEST(DynamicArray, BigIndex) {
  DynamicArray *da = DynamicArray_new();
  DynamicArray_set(da, 8, X);
  ASSERT_EQ(DynamicArray_get(da, 8), X);
  DynamicArray_destroy(da);
}

TEST(DynamicArray, ReallyBig) {
  DynamicArray *da = DynamicArray_new();
  DynamicArray_set(da, 400, X);
  DynamicArray_set(da, 200, X / 2);
  ASSERT_EQ(DynamicArray_get(da, 200), X / 2);
  ASSERT_EQ(DynamicArray_get(da, 400), X);
  DynamicArray_destroy(da);
}

TEST(DynamicArray, Push) {
  DynamicArray *da = DynamicArray_new();
  double x = 0;
  while (x < 10) {
    DynamicArray_push(da, x);
    x += 0.25;
  }
  ASSERT_EQ(DynamicArray_size(da), 40);
  while (DynamicArray_size(da) > 0) {
    DynamicArray_pop(da);
  }
  ASSERT_EQ(DynamicArray_size(da), 0);
  DynamicArray_destroy(da);
}

TEST(DynamicArray, PushFront) {
  DynamicArray *da = DynamicArray_new();
  double x = 0;
  while (x < 10) {
    DynamicArray_push_front(da, x);
    x += 0.25;
  }
  ASSERT_EQ(DynamicArray_size(da), 40);
  while (DynamicArray_size(da) > 0) {
    DynamicArray_pop_front(da);
  }
  ASSERT_EQ(DynamicArray_size(da), 0);
  DynamicArray_destroy(da);
}

TEST(DynamnicArray, ToString) {
  DynamicArray *da = DynamicArray_new();
  double x = 1.0;
  while (x <= 5) {
    DynamicArray_push(da, x);
    x += 1.0;
  }
  char *str = DynamicArray_to_string(da);
  printf("ToString Example: %s\n", str);
  ASSERT_STREQ(str, "[1.00000,2.00000,3.00000,4.00000,5.00000]");
  DynamicArray_destroy(da);
  free(str);
}

TEST(DynamicArray, Pop) {
  DynamicArray *da = DynamicArray_new();
  double x;
  DynamicArray_push(da, X);
  DynamicArray_push(da, X);
  x = DynamicArray_pop(da);
  ASSERT_EQ(DynamicArray_size(da), 1);
  ASSERT_EQ(x, X);
  ASSERT_EQ(DynamicArray_get(da, 1), 0.0);
  DynamicArray_destroy(da);
}

TEST(DynamicArray, Map) {
  DynamicArray *t = DynamicArray_new(), *y;
  double s = 0.0;
  for (int i = 0; i < 628; i++) {
    DynamicArray_set(t, i, s);
    s = s + 0.1;
  }
  y = DynamicArray_map(t, sin);
  for (int i = 0; i < DynamicArray_size(t); i++) {
    ASSERT_NEAR(DynamicArray_get(y, i), sin(0.1 * i), 0.0001);
  }
  DynamicArray_destroy(t);
  DynamicArray_destroy(y);
}

/* HW3 TESTS START HERE */

TEST(DynamicArray, MathOperations) {
  DynamicArray *da = DynamicArray_new();
  DynamicArray_push(da, 1.0);
  DynamicArray_push(da, 5.0);
  DynamicArray_push(da, 2.0);
  DynamicArray_push(da, 4.0);
  DynamicArray_push(da, 3.0);

  ASSERT_NEAR(DynamicArray_min(da), 1.0, EPSILON);
  ASSERT_NEAR(DynamicArray_max(da), 5.0, EPSILON);
  ASSERT_NEAR(DynamicArray_mean(da), 3.0, EPSILON);
  ASSERT_NEAR(DynamicArray_median(da), 3.0, EPSILON);
  ASSERT_NEAR(DynamicArray_sum(da), 15.0, EPSILON);

  ASSERT_NEAR(DynamicArray_first(da), 1.0, EPSILON);
  ASSERT_NEAR(DynamicArray_last(da), 3.0, EPSILON);

  DynamicArray_destroy(da);
}

TEST(DynamicArray, RangeAndCopy) {
  DynamicArray *da = DynamicArray_range(0, 1, 0.2);
  // Should be [0, 0.2, 0.4, 0.6, 0.8, 1.0] -> 6 elements
  ASSERT_EQ(DynamicArray_size(da), 6);
  ASSERT_NEAR(DynamicArray_get(da, 0), 0.0, EPSILON);
  ASSERT_NEAR(DynamicArray_get(da, 5), 1.0, EPSILON);

  DynamicArray *copy = DynamicArray_copy(da);
  ASSERT_EQ(DynamicArray_size(copy), 6);
  ASSERT_NEAR(DynamicArray_get(copy, 2), 0.4, EPSILON);

  DynamicArray_destroy(da);
  DynamicArray_destroy(copy);
}

TEST(DynamicArray, Concat) {
  DynamicArray *a = DynamicArray_range(1, 2, 1); // [1, 2]
  DynamicArray *b = DynamicArray_range(3, 4, 1); // [3, 4]
  DynamicArray *c = DynamicArray_concat(a, b);

  ASSERT_EQ(DynamicArray_size(c), 4);
  ASSERT_NEAR(DynamicArray_get(c, 0), 1.0, EPSILON);
  ASSERT_NEAR(DynamicArray_get(c, 3), 4.0, EPSILON);

  DynamicArray_destroy(a);
  DynamicArray_destroy(b);
  DynamicArray_destroy(c);
}

TEST(DynamicArray, Take) {
  DynamicArray *a = DynamicArray_range(1, 5, 1); // [1, 2, 3, 4, 5]

  DynamicArray *b = DynamicArray_take(a, 2); // [1, 2]
  ASSERT_EQ(DynamicArray_size(b), 2);
  ASSERT_NEAR(DynamicArray_get(b, 0), 1.0, EPSILON);

  DynamicArray *c = DynamicArray_take(a, -2); // [4, 5]
  ASSERT_EQ(DynamicArray_size(c), 2);
  ASSERT_NEAR(DynamicArray_get(c, 0), 4.0, EPSILON);

  DynamicArray *d = DynamicArray_take(a, 7); // [1,2,3,4,5,0,0]
  ASSERT_EQ(DynamicArray_size(d), 7);
  ASSERT_NEAR(DynamicArray_get(d, 5), 0.0, EPSILON);

  DynamicArray_destroy(a);
  DynamicArray_destroy(b);
  DynamicArray_destroy(c);
  DynamicArray_destroy(d);
}

int is_pos(double x) { return x > 0; }

TEST(DynamicArray, Filter) {
  DynamicArray *a = DynamicArray_new();
  DynamicArray_push(a, 1);
  DynamicArray_push(a, -2);
  DynamicArray_push(a, 3);

  DynamicArray *f = DynamicArray_filter(a, is_pos);
  ASSERT_EQ(DynamicArray_size(f), 2);
  ASSERT_NEAR(DynamicArray_get(f, 0), 1.0, EPSILON);
  ASSERT_NEAR(DynamicArray_get(f, 1), 3.0, EPSILON);

  DynamicArray_destroy(a);
  DynamicArray_destroy(f);
}

TEST(DynamicArray, Unique) {
  DynamicArray *a = DynamicArray_new();
  DynamicArray_push(a, 1);
  DynamicArray_push(a, 2);
  DynamicArray_push(a, 2);
  DynamicArray_push(a, 3);
  DynamicArray_push(a, 1);

  DynamicArray *u = DynamicArray_unique(a);
  ASSERT_EQ(DynamicArray_size(u), 3); // [1, 2, 3]
  ASSERT_NEAR(DynamicArray_get(u, 1), 2.0, EPSILON);

  DynamicArray_destroy(a);
  DynamicArray_destroy(u);
}

TEST(DynamicArray, Split) {
  DynamicArray *a = DynamicArray_range(1, 10, 1); // 10 elements
  int count = 0;
  DynamicArray **chunks = DynamicArray_split(a, 3, &count);

  // 10 / 3 -> chunk size 4 (ceil)
  // [1,2,3,4], [5,6,7,8], [9,10]
  ASSERT_EQ(count, 3);
  ASSERT_EQ(DynamicArray_size(chunks[0]), 4);
  ASSERT_EQ(DynamicArray_size(chunks[1]), 4);
  ASSERT_EQ(DynamicArray_size(chunks[2]), 2);

  DynamicArray_destroy(a);
  for (int i = 0; i < count; i++)
    DynamicArray_destroy(chunks[i]);
  free(chunks);
}

} // namespace
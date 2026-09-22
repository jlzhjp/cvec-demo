#include <gtest/gtest.h>

extern "C" {
#include "cvec.h"
}

TEST(CVecTest, InitCreatesEmptyVector) {
  cvec v;

  ASSERT_EQ(cvec_init(&v, sizeof(int), alignof(int)), 0);

  EXPECT_EQ(v.data, nullptr);
  EXPECT_EQ(v.len, 0);
  EXPECT_EQ(v.cap, 0);
  EXPECT_EQ(v.elem_size, sizeof(int));
  EXPECT_EQ(v.elem_align, alignof(int));

  cvec_destroy(&v);
}

TEST(CvecTest, PushIncreaseLength) {
  cvec v;

  ASSERT_EQ(cvec_init(&v, sizeof(int), alignof(int)), 0);

  int x = 42;

  ASSERT_EQ(cvec_push(&v, &x), 0);
  EXPECT_EQ(cvec_len(&v), 1);
  EXPECT_GE(v.cap, 1);

  cvec_destroy(&v);
}

TEST(CVecTest, PushCopiesElement) {
  cvec v;

  ASSERT_EQ(cvec_init(&v, sizeof(int), alignof(int)), 0);

  int x = 42;

  ASSERT_EQ(cvec_push(&v, &x), 0);

  auto *data = static_cast<int *>(v.data);

  EXPECT_EQ(data[0], 42);

  cvec_destroy(&v);
}

TEST(CVecTest, PushManyElements) {
  cvec v;
  ASSERT_EQ(cvec_init(&v, sizeof(int), alignof(int)), 0);

  for (int i = 0; i < 100; ++i) {
    ASSERT_EQ(cvec_push(&v, &i), 0);
  }

  ASSERT_EQ(cvec_len(&v), 100);

  auto *data = static_cast<int *>(v.data);

  for (int i = 0; i < 100; ++i) {
    EXPECT_EQ(data[i], i);
  }

  cvec_destroy(&v);
}

struct alignas(64) AlignedValue {
  int value;
};

TEST(CVecTest, RespectElementAlignment) {
  cvec v;

  ASSERT_EQ(cvec_init(&v, sizeof(AlignedValue), alignof(AlignedValue)), 0);

  AlignedValue x{42};

  ASSERT_EQ(cvec_push(&v, &x), 0);

  auto addr = reinterpret_cast<std::uintptr_t>(v.data);

  EXPECT_EQ(addr % alignof(AlignedValue), 0);

  cvec_destroy(&v);
}

TEST(CVecTest, GetReturnsElement) {
  cvec v;

  ASSERT_EQ(cvec_init(&v, sizeof(int), alignof(int)), 0);

  int a = 10;
  int b = 20;
  int c = 30;

  ASSERT_EQ(cvec_push(&v, &a), 0);
  ASSERT_EQ(cvec_push(&v, &b), 0);
  ASSERT_EQ(cvec_push(&v, &c), 0);

  auto *p = static_cast<const int *>(cvec_get(&v, 1));

  ASSERT_NE(p, nullptr);
  EXPECT_EQ(*p, 20);

  cvec_destroy(&v);
}

TEST(CVecTest, GetReturnsNullWhenOutOfBounds) {
  cvec v;

  ASSERT_EQ(cvec_init(&v, sizeof(int), alignof(int)), 0);

  EXPECT_EQ(cvec_get(&v, 0), nullptr);

  int x = 42;
  ASSERT_EQ(cvec_push(&v, &x), 0);

  EXPECT_EQ(cvec_get(&v, 1), nullptr);
  EXPECT_EQ(cvec_get(&v, 100), nullptr);

  cvec_destroy(&v);
}

TEST(CVecTest, GetMutCanModifyElement) {
  cvec v;

  ASSERT_EQ(cvec_init(&v, sizeof(int), alignof(int)), 0);

  int x = 42;
  ASSERT_EQ(cvec_push(&v, &x), 0);

  auto *p = static_cast<int *>(cvec_get_mut(&v, 0));

  ASSERT_NE(p, nullptr);

  *p = 100;

  auto *q = static_cast<const int *>(cvec_get(&v, 0));

  ASSERT_NE(q, nullptr);
  EXPECT_EQ(*q, 100);

  cvec_destroy(&v);
}

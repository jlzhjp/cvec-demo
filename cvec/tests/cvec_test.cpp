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

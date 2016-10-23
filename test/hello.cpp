#include <gtest/gtest.h>

TEST(HelloTest, Test1) {
  EXPECT_EQ(1, 1);
}

TEST(HelloTest, Test2) {
  EXPECT_NE(1, 1);
}
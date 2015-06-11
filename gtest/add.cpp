#include <gtest/gtest.h>
#include <stdio.h>

int add(int a, int b)
{
    return a + b;
}


TEST(ADD, pos)
{
    EXPECT_EQ(add(3, 6), 9);
    EXPECT_EQ(add(3, 6), 6);
    EXPECT_EQ(add(6, 6), 12);
}

TEST(ADD, pos1)
{
    EXPECT_EQ(add(-6, -6), -12);
}

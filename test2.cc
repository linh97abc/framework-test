#include <unittest.h>

TEST(suit1, 001) {
    EXPECT_EQ(1, 1);
    EXPECT_DOUBLE_EQ(2,2);
    EXPECT_EQ_PTR(nullptr, NULL);
    EXPECT_FALSE(0);
}

TEST(suit1, 002) {
    EXPECT_EQ(1, 1);
    EXPECT_DOUBLE_EQ(2,2);
    EXPECT_EQ_PTR(nullptr, NULL);
    EXPECT_FALSE(0);
}

TEST(suit1, 003) {
    EXPECT_EQ(1, 1);
    EXPECT_DOUBLE_EQ(2,2);
    EXPECT_EQ_PTR(nullptr, NULL);
    EXPECT_TRUE(1);
}
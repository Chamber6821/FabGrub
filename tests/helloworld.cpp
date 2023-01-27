//
// Created by Ivan on 26.01.2023.
//

#include "storage/Storage.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

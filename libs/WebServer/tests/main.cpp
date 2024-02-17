#include <gtest/gtest.h>

#include "../include/web_server.hpp"

TEST(WebServer, main) {
    EXPECT_EQ(true, true);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
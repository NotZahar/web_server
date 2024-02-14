#include <gtest/gtest.h>

#include "../include/web_server.hpp"

TEST(WebServer, main) {
    EXPECT_EQ(ws::mime_type("path/to/file.txt"), "text/plain");
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#pragma once

#include <string>
#include <chrono>

namespace ws {
    struct config {
        // options
        inline static const std::string defaultAddress = "127.0.0.1";
        inline static const unsigned short defaultPort = 3000;
        inline static const std::string defaultRoot = "./test/html/";
        inline static const int defaultThreads = 1;

        // session
        inline static const std::chrono::milliseconds defaultSessionTimeout{ 10'000 };
        inline static std::chrono::milliseconds sessionTimeout = defaultSessionTimeout;

        // http
        inline static const std::string fieldServer = "WebServer (Linux)";
    };
}
#pragma once

#include <string>
#include <chrono>

#include "types.hpp"

namespace ws {
    struct config {
        // options
        inline static const std::string defaultAddress = "127.0.0.1";
        inline static const unsigned short defaultPort = 3000;
        inline static const int defaultThreads = 1;

        inline static ip::address address = ip::make_address(defaultAddress);
        inline static unsigned short port = defaultPort;
        inline static int threads = defaultThreads;

        // session
        inline static const std::chrono::milliseconds defaultSessionTimeout{ 10'000 };
        inline static std::chrono::milliseconds sessionTimeout = defaultSessionTimeout;
        inline static unsigned httpVersion = ;
        inline static bool keepAlive = ;

        // http headers
        inline static const std::string fieldServer = "WebServer (Linux)";
    };
}
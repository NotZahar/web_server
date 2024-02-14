#pragma once

#include <string_view>

namespace ws {
    class WSLogger {
    public:
        WSLogger() = default;
        ~WSLogger() = default;

        static void log(std::string_view log);
        static void logHelp();
    };
}
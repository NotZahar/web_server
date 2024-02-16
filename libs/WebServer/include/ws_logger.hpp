#pragma once

#include <string_view>

namespace ws {
    class WSLogger {
    public:
        WSLogger() = default;
        
        ~WSLogger() = default;

        static void log(std::string_view log);
        static void err(std::string_view err);

        static void logHelp();
    };
}
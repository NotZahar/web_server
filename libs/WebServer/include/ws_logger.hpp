#pragma once

#include <string_view>

#include "../src/utility/types.hpp"

namespace ws {
    class WSLogger final {
    public:
        WS_NCM(WSLogger)

        static WSLogger& instance();

        void log(std::string_view log);
        void err(std::string_view err);

        void logHelp();

    private:
        WSLogger() = default;
        
        ~WSLogger() = default;
    };
}
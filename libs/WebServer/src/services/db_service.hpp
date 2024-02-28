#pragma once

#include <string_view>

namespace ws {
    class DBService {
    public:
        DBService() = default;

        ~DBService() = default;
        
        static bool selectQuery(std::string_view query) noexcept;
    };
}
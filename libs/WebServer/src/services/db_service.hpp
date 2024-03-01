#pragma once

#include <vector>
#include <list>
#include <string_view>

namespace ws {
    class DBService {
    public:
        enum class errorCode {
            noError,
            readError
        };

        DBService() = default;

        ~DBService() = default;
        
        static std::list<std::string> select(
            std::string_view rawQuery, 
            std::vector<std::string> values,
            const int schemaSize,
            errorCode& error) noexcept;
    };
}
#pragma once

#include <string>
#include <regex>
#include <string_view>

namespace ws {
    struct sanitizer {
        static bool check(std::string_view value) {
            static const std::regex queryRegex("^[a-zA-Z0-9.@ ]+$");    
            return std::regex_match(value.cbegin(), value.cend(), queryRegex);
        }
    };

    struct query {
        inline static const int getUserSizeResult = 2;
        inline static const std::string getUser = 
            R"(
                SELECT 
                    email, password 
                FROM 
                    user
                WHERE
                    email IS ?
            )";
    };
}
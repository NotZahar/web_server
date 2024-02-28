#pragma once

#include <string>

namespace ws {
    struct query {
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
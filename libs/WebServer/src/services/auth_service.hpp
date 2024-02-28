#pragma once

#include <string>
#include <unordered_map>

namespace ws {
    class AuthService {
    public:
        enum class errorCode {
            noError,
            noData,
            badData
        };

        AuthService() = default;

        ~AuthService() = default;

        static void authenticate(
            const std::unordered_map<std::string, std::string>& params,
            errorCode& code); 
    };
}
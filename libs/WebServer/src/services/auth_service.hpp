#pragma once

#include <string>
#include <unordered_map>

namespace ws {
    class AuthService {
    public:
        AuthService() = default;

        ~AuthService() = default;

        static bool authDataExists(
            const std::unordered_map<std::string, std::string>& params) noexcept;
        static bool authenticate(
            const std::unordered_map<std::string, std::string>& params) noexcept; 
    };
}
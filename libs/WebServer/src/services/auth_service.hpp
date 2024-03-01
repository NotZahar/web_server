#pragma once

#include <string>
#include <unordered_map>
#include <chrono>

namespace ws {
    class AuthService {
    public:
        enum class errorCode {
            noError,
            noData,
            badData
        };

        struct AuthData {
            std::string_view email;
            std::string_view password;
        };

        AuthService() = default;

        ~AuthService() = default;

        static std::string createToken(AuthData authData, errorCode& error);

        static void authenticate(
            const std::unordered_map<std::string, std::string>& params,
            errorCode& code); 

    private:
        inline static const std::string emailPayloadHeader = "email";
        inline static const std::chrono::hours tokenExpirationTime = std::chrono::hours{ 24 };
        inline static const std::string iatClaim = "iat";
        inline static const std::string expClaim = "exp";
    };
}
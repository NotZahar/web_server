#pragma once

#include "response.hpp"

namespace ws {
    class AuthResponse final : public Response {
    public:
        struct AuthData {
            std::string_view email;
            std::string_view password;
        };

        AuthResponse() = delete;
        explicit AuthResponse(RequestInfo request, AuthData authData);
        
        ~AuthResponse() override = default;

        http::message_generator create() const override;

    private:
        AuthData _authData;
    };
}
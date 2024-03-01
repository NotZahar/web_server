#pragma once

#include "response.hpp"
#include "../services/auth_service.hpp"

namespace ws {
    class AuthResponse final : public Response {
    public:
        AuthResponse() = delete;
        explicit AuthResponse(RequestInfo request, AuthService::AuthData authData);
        
        ~AuthResponse() override = default;

        http::message_generator create() const override;

    private:
        AuthService::AuthData _authData;
    };
}
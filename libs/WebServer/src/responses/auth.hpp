#pragma once

#include "response.hpp"

namespace ws {
    class AuthResponse final : public Response {
    public:
        AuthResponse() = delete;
        explicit AuthResponse(RequestInfo request);
        
        ~AuthResponse() override = default;

        http::message_generator create() const override;
    };
}
#pragma once

#include "response.hpp"

namespace ws {
    class LoginResponse final : public Response {
    public:
        LoginResponse() = delete;
        explicit LoginResponse(RequestInfo request);
        
        ~LoginResponse() override = default;

        http::message_generator create() const override;
    };
}
#pragma once

#include "../response.hpp"

namespace ws {
    class ServerErrorResponse final : public Response {
    public:
        ServerErrorResponse() = delete;
        ServerErrorResponse(unsigned httpVersion, bool keepAlive, std::string what);
        ~ServerErrorResponse() override = default;

        http::message_generator create() const override;
    
    private:
        std::string _what;
    };
}
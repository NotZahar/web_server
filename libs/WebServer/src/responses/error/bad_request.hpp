#pragma once

#include "../response.hpp"

namespace ws {
    class BadRequestResponse final : public Response {
    public:
        BadRequestResponse() = delete;
        BadRequestResponse(RequestInfo request, std::string_view reason);
        
        ~BadRequestResponse() override = default;

        http::message_generator create() const override;
    
    private:
        std::string _reason;
    };
}
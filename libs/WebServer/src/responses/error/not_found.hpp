#pragma once

#include "../response.hpp"

namespace ws {
    class NotFoundResponse final : public Response {
    public:
        NotFoundResponse() = delete;
        NotFoundResponse(RequestInfo request, std::string_view target);
        
        ~NotFoundResponse() override = default;

        http::message_generator create() const override;
    
    private:
        std::string _target;
    };
}
#pragma once

#include "response_handler.hpp"

namespace ws {
    class GetResponse final : public ResponseHandler {
    public:
        GetResponse() = delete;
        explicit GetResponse(
            const http::verb requestMethod, std::unique_ptr<Response> response) noexcept;
        
        ~GetResponse() override = default;

    protected:
        bool isValid() const noexcept override;
    };
}
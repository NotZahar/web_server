#pragma once

#include "response_handler.hpp"

namespace ws {
    class Get final : public ResponseHandler {
    public:
        Get() = delete;
        explicit Get(
            const http::verb requestMethod, std::unique_ptr<Response> response) noexcept;
        
        ~Get() override = default;

    protected:
        bool methodIsValid() const noexcept override;
    };
}
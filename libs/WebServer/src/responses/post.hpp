#pragma once

#include "response_handler.hpp"

namespace ws {
    class Post final : public ResponseHandler {
    public:
        Post() = delete;
        explicit Post(
            const http::verb requestMethod, std::unique_ptr<Response> response) noexcept;
        
        ~Post() override = default;

    protected:
        bool methodIsValid() const noexcept override;
    };
}
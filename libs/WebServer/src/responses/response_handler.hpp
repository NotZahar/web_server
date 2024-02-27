#pragma once

#include <memory>

#include "response.hpp"

namespace ws {
    class ResponseHandler {
    public:
        ResponseHandler() = delete;
        explicit ResponseHandler(
            const http::verb requestMethod, std::unique_ptr<Response> response) noexcept;

        virtual ~ResponseHandler() = default;

        http::message_generator response() const;

    protected:
        virtual bool isValid() const noexcept = 0;

        const http::verb _requestMethod;
        std::unique_ptr<Response> _response;
    };
}
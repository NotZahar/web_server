#pragma once

#include <memory>

#include "response.hpp"

namespace ws {
    /*!
        \brief Декоратор-родитель для проверки валидности http-запроса
    */
    class ResponseHandler {
    public:
        ResponseHandler() = delete;
        ResponseHandler(
            const http::verb requestMethod, std::unique_ptr<Response> response) noexcept;

        virtual ~ResponseHandler() = default;

        http::message_generator response() const;
        Response::RequestInfo getRequestInfo() const noexcept;

    protected:
        virtual bool methodIsValid() const noexcept = 0;

        const http::verb _requestMethod;
        std::unique_ptr<Response> _response;
    };
}
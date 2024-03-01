#pragma once

#include "response_handler.hpp"

namespace ws {
    /*!
        \brief Авторизующий декоратор

        Если http-ответ не должен получить неавторизованный пользователь
    */
    class Authorized {
    public:
        Authorized() = delete;
        Authorized(
            const std::unordered_map<std::string, std::string>& requestParams, 
            std::unique_ptr<ResponseHandler> handler) noexcept;

        virtual ~Authorized() = default;

        http::message_generator response() const;

    protected:
        const std::unordered_map<std::string, std::string>& _requestParams;
        std::unique_ptr<ResponseHandler> _handler;
    };
}
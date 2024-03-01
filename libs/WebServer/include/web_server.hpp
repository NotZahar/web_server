#pragma once

#include "ws_options.hpp"
#include "../src/utility/types.hpp"

namespace ws {
    /*!
        \brief Отвечает за запуск сервера

        Точка входа в приложение, отвечает за создание сетевых сессий
    */
    class WebServer {
    public:
        WebServer() = delete;
        explicit WebServer(WSOptions wsOption) noexcept;
        
        ~WebServer() = default;

        void run();
    
    private:
        asio::awaitable<void> makeListener(ssl::context& sslContext) const;
        asio::awaitable<void> makeSession(beast::ssl_stream<tcp_stream> stream) const;
    };
}

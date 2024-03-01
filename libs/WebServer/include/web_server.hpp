#pragma once

#include "ws_options.hpp"
#include "../src/utility/types.hpp"

namespace ws {
    /*!
        \brief Родительский класс, не несущий никакой смысловой нагрузки

        Данный класс имеет только одну простую цель: проиллюстрировать то,
        как Doxygen документирует наследование 
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

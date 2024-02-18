#pragma once

#include "ws_options.hpp"
#include "../src/utility/types.hpp"

namespace ws {
    class WebServer {
    public:
        WebServer() = delete;
        explicit WebServer(WSOptions wsOption) noexcept;
        
        ~WebServer() = default;

        void run();
    
    private:
        // Accepts incoming connections and launches the sessions
        asio::awaitable<void> startListen(ssl::context& sslContext) const;
        
        // Handles an HTTP server connection
        asio::awaitable<void> makeSession(beast::ssl_stream<tcp_stream> stream) const;
    };
}

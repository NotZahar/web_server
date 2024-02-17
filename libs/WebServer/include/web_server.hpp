#pragma once

#include "../src/utility/types.hpp"

namespace ws {
    class WebServer {
    public:
        WebServer() = delete;
        WebServer(
            std::string address, 
            unsigned short port, 
            std::string staticRootPath, 
            int threads);
        
        ~WebServer() = default;

        void run();
    
    private:
        // Accepts incoming connections and launches the sessions
        asio::awaitable<void> makeListener();
        
        // Handles an HTTP server connection
        asio::awaitable<void> makeSession(tcp_stream stream);

        const ip::address _address;
        const unsigned short _port;
        const fs::path _staticRootPath;
        const int _threads;
    };
}

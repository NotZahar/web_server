#pragma once

#include "../src/utility/types.hpp"

namespace ws {
    class WebServer {
    public:
        WebServer() = delete;
        WebServer(
            std::string address, 
            unsigned short port, 
            std::string root, 
            int threads);
        
        ~WebServer() = default;

        void run();
    
    private:
        asio::awaitable<void> makeListener();
        asio::awaitable<void> makeSession(tcp_stream stream);

        const ip::address _address;
        const unsigned short _port;
        const fs::path _root;
        const int _threads;
    };
}

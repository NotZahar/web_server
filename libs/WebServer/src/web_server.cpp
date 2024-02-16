#include "web_server.hpp"

#include <algorithm>
#include <boost/asio/this_coro.hpp>
#include <boost/filesystem/file_status.hpp>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <system_error>
#include <thread>
#include <vector>

#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/config.hpp>

#include "ws_logger.hpp"
#include "request_handler.hpp"
#include "utility/config.hpp"

namespace ws {
    WebServer::WebServer(
        std::string address, 
        unsigned short port, 
        std::string root, 
        int threads) 
        : _address{ ip::make_address(address) },
          _port{ port },
          _root{ root },
          _threads{ threads }
    {}

    void WebServer::run() {
        if (std::error_code code; !fs::is_directory(_root, code))
            throw code;

        asio::io_context context{ _threads };
        asio::co_spawn(
            context,
            makeListener(),
            [](std::exception_ptr exceptionPtr) {
                if (!exceptionPtr)
                    return;
                
                try {
                    std::rethrow_exception(exceptionPtr);
                } catch(const std::exception& exception) {
                    WSLogger::err(exception.what());
                }
            }
        );

        asio::detail::thread_group contextRunners;
        for (int i = 0; i < _threads; ++i)
            contextRunners.create_thread([&context] { context.run(); });
        context.run();
    }

    // Handles an HTTP server connection
    asio::awaitable<void> WebServer::makeSession(tcp_stream stream) {
        // Need to be persist across reads
        beast::flat_buffer buffer;

        try {
            while (true) {
                stream.expires_after(config::sessionTimeout);
                
                http::request<http::string_body> request;
                co_await http::async_read(stream, buffer, request);
                
                RequestHandler handler{ std::move(request) };
                auto response = handler.makeResponse(_root);

                co_await beast::async_write(stream, std::move(response), asio::use_awaitable);

                if(!response.keep_alive())
                    break;
            }
        } catch (const sm::system_error& error) {
            if (error.code() != http::error::end_of_stream)
                throw;
        }

        beast::error_code errorCode;
        stream.socket().shutdown(ip::tcp::socket::shutdown_send, errorCode);
    
        // Ignore error
    }

    // Accepts incoming connections and launches the sessions
    asio::awaitable<void> WebServer::makeListener() {
        auto executor = co_await asio::this_coro::executor;
        auto acceptor = 
            asio::use_awaitable.as_default_on(ip::tcp::acceptor{ executor });
        const auto endpoint = ip::tcp::endpoint{ _address, _port };
        
        acceptor.open(endpoint.protocol());
        acceptor.set_option(asio::socket_base::reuse_address(true));
        acceptor.bind(endpoint);
        acceptor.listen();

        while (true)
            asio::co_spawn(
                acceptor.get_executor(),
                makeSession(tcp_stream(co_await acceptor.async_accept())),
                [](std::exception_ptr exceptionPtr) {
                    if (!exceptionPtr)
                        return;

                    try {
                        std::rethrow_exception(exceptionPtr);
                    } catch (const std::exception& exception) {
                        WSLogger::err(exception.what());
                    }
                }
            );

    }
}
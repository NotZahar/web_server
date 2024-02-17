#include "web_server.hpp"

#include <stdexcept>

#include <boost/asio/co_spawn.hpp>

#include "ws_logger.hpp"
#include "request_handler.hpp"
#include "utility/config.hpp"
#include "utility/messages.hpp"

namespace ws {
    WebServer::WebServer(
        std::string address, 
        unsigned short port, 
        std::string staticRootPath, 
        int threads) 
        : _address{ ip::make_address(address) },
          _port{ port },
          _staticRootPath{ staticRootPath },
          _threads{ threads }
    {}

    void WebServer::run() {
        if (std::error_code code; !fs::is_directory(_staticRootPath, code))
            throw code;
        if (_staticRootPath.string().ends_with('/'))
            throw std::runtime_error{ messages::general::INVALID_STATIC_ROOT };

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
                    WSLogger::instance().err(exception.what());
                }
            }
        );

        asio::detail::thread_group contextRunners;
        for (int i = 0; i < _threads - 1; ++i)
            contextRunners.create_thread([&context] { context.run(); });
        context.run();
    }

    asio::awaitable<void> WebServer::makeSession(tcp_stream stream) {
        // Need to be persist across reads
        beast::flat_buffer buffer;

        try {
            while (true) {
                stream.expires_after(config::sessionTimeout);
                
                http::request<http::string_body> request;
                co_await http::async_read(stream, buffer, request);
                
                RequestHandler handler{ std::move(request) };
                auto response = handler.makeResponse(_staticRootPath);
                const auto keepAlive = response.keep_alive();

                co_await beast::async_write(stream, std::move(response), asio::use_awaitable);
                if(!keepAlive)
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
                        WSLogger::instance().err(exception.what());
                    }
                }
            );

    }
}
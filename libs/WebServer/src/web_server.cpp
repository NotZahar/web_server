#include "web_server.hpp"

#include <stdexcept>

#include <boost/asio/co_spawn.hpp>
#include <boost/asio/as_tuple.hpp>

#include "ws_logger.hpp"
#include "ssl.hpp"
#include "router.hpp"
#include "utility/config.hpp"
#include "utility/messages.hpp"
#include "utility/files.hpp"

namespace ws {
    WebServer::WebServer(WSOptions wsOption) noexcept
    {
        const auto options = wsOption.getOptions();
        config::address = ip::make_address(options.address);
        config::port = options.port;
        config::threads = options.threads;
    }

    void WebServer::run() {
        if (std::error_code code; !fs::is_directory(paths::resourcesPath, code))
            throw code;
        if (std::error_code code; !fs::is_directory(paths::publicPath, code))
            throw std::runtime_error{ messages::server::INVALID_PATH };

        asio::io_context ioContext{ config::threads };
        ssl::context sslContext{ ssl::context::tlsv12_server };
        SSL::configure(sslContext);

        asio::co_spawn(
            ioContext,
            makeListener(sslContext),
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
        for (int i = 0; i < config::threads - 1; ++i)
            contextRunners.create_thread([&ioContext] { ioContext.run(); });
        ioContext.run();
    }

    asio::awaitable<void> WebServer::makeListener(ssl::context& sslContext) const {
        auto executor = co_await asio::this_coro::executor;
        auto acceptor = 
            asio::use_awaitable.as_default_on(ip::tcp::acceptor{ executor });
        const auto endpoint = ip::tcp::endpoint{ config::address, config::port };
        
        acceptor.open(endpoint.protocol());
        acceptor.set_option(asio::socket_base::reuse_address(true));
        acceptor.bind(endpoint);
        acceptor.listen();

        co_await acceptor.async_accept();
        while (true) {
            asio::co_spawn(
                acceptor.get_executor(),
                makeSession(beast::ssl_stream<tcp_stream>(
                    co_await acceptor.async_accept(), sslContext)),
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

    asio::awaitable<void> WebServer::makeSession(beast::ssl_stream<tcp_stream> stream) const {
        try {
            co_await stream.async_handshake(ssl::stream_base::server);
        } catch (const std::exception& exception) {
            WSLogger::instance().err(exception.what());
        }
        
        beast::flat_buffer buffer;

        try {
            while (true) {
                beast::get_lowest_layer(stream).expires_after(config::sessionTimeout);
                
                http::request<http::string_body> request;
                co_await http::async_read(stream, buffer, request);
                
                Router router{ std::move(request) };
                auto response = router.makeResponse();
                const bool keepAlive = response.keep_alive();

                co_await beast::async_write(stream, std::move(response), asio::use_awaitable);
                if(!keepAlive)
                    break;
            }
        } catch (const sm::system_error& error) {
            if (error.code() != http::error::end_of_stream)
                throw;
        }

        beast::get_lowest_layer(stream).expires_after(config::sessionTimeout);
        auto [ errorCode ] = 
            co_await stream.async_shutdown(asio::as_tuple(asio::use_awaitable));
        if (errorCode == asio::error::eof)
            errorCode = {};

        if (errorCode)
            throw boost::system::system_error(errorCode);
    }
}
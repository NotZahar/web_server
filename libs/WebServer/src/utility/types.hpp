#pragma once

#include <filesystem>

#include <boost/program_options.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/use_awaitable.hpp>

namespace ws {
    namespace sm = boost::system;
    namespace core = boost::core;
    namespace asio = boost::asio;
    namespace ip = asio::ip;
    namespace fs = std::filesystem;
    namespace beast = boost::beast;
    namespace http = beast::http;

    using options_description = boost::program_options::options_description;
    using tcp_stream = typename beast::tcp_stream::rebind_executor<
        asio::use_awaitable_t<>::executor_with_default<asio::any_io_executor>>::other;
}
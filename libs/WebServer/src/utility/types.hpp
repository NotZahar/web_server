#pragma once

#include <filesystem>
#include <unordered_map>

#include <boost/program_options.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/url.hpp>

namespace ws {
    namespace sm = boost::system;
    namespace core = boost::core;
    namespace asio = boost::asio;
    namespace ip = asio::ip;
    namespace fs = std::filesystem;
    namespace beast = boost::beast;
    namespace http = beast::http;
    namespace ssl = asio::ssl;
    namespace urls = boost::urls;

    using options_description = boost::program_options::options_description;
    using tcp_stream = typename beast::tcp_stream::rebind_executor<
        asio::use_awaitable_t<>::executor_with_default<asio::any_io_executor>>::other;

    #define WS_NCM(T) T(const T&) = delete; \
        T& operator=(const T&) = delete;    \
        T(T&&) = delete;                    \
        T& operator=(T&&) = delete;
}
#pragma once

#include "utility/types.hpp"

namespace ws {
    class Router {
    public:
        Router() = delete;
        Router(http::request<http::string_body> request);

        // Return a response for the given request
        http::message_generator makeResponse();

    private:
        bool allowedMethod(const http::verb method) const;

        // TODO: make
        http::response<http::empty_body> headResponse(
            const std::uint64_t targetSize,
            const fs::path& targetPath) const;
        // TODO: make
        http::response<http::file_body> getResponse(
            const std::uint64_t targetSize,
            const fs::path& targetPath,
            http::file_body::value_type targetBody) const;

        http::response<http::string_body> badRequestResponse(std::string_view reason) const; // TODO: make
        http::response<http::string_body> notFoundResponse(std::string_view target) const; // TODO: make
        http::response<http::string_body> serverErrorResponse(std::string_view what) const; // TODO: make

        http::request<http::string_body> _request;
    };
}
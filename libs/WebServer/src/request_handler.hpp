#pragma once

#include "utility/types.hpp"

namespace ws {
    class RequestHandler {
    public:
        RequestHandler() = delete;
        RequestHandler(http::request<http::string_body> request);

        // Return a response for the given request
        http::message_generator makeResponse(const fs::path& docsRoot);

    private:
        bool allowedMethod(const http::verb method) const;
        bool allowedPath(std::string_view targetPath) const;

        http::response<http::string_body> badRequestResponse(std::string_view reason) const;
        http::response<http::string_body> notFoundResponse(std::string_view target) const;
        http::response<http::string_body> serverErrorResponse(std::string_view what) const;

        http::request<http::string_body> _request;
    };
}
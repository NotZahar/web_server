#pragma once

#include "utility/types.hpp"

namespace ws {
    class RequestHandler {
    public:
        RequestHandler() = delete;
        RequestHandler(http::request<http::string_body> request);

        // Return a response for the given request
        http::message_generator makeResponse(const fs::path& staticRootPath);

    private:
        bool allowedMethod(const http::verb method) const;
        bool allowedPath(std::string_view targetPath) const;
        
        // Append an HTTP rel-path to a local filesystem path.
        // The returned path is normalized for the platform.
        fs::path makeTargetPath(
            const fs::path& staticRootPath, 
            std::string_view requestedPath) const;

        http::response<http::empty_body> headResponse(
            const std::uint64_t targetSize,
            const fs::path& targetPath) const;
        http::response<http::file_body> getResponse(
            const std::uint64_t targetSize,
            const fs::path& targetPath,
            http::file_body::value_type targetBody) const;

        http::response<http::string_body> badRequestResponse(std::string_view reason) const;
        http::response<http::string_body> notFoundResponse(std::string_view target) const;
        http::response<http::string_body> serverErrorResponse(std::string_view what) const;

        http::request<http::string_body> _request;
    };
}
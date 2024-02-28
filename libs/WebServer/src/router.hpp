#pragma once

#include "utility/types.hpp"

namespace ws {
    class Router {
    public:
        Router() = default;

        ~Router() = default;

        // Return a response for the given request
        static http::message_generator makeResponse(http::request<http::string_body> request);
    };
}
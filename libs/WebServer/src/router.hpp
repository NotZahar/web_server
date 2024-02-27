#pragma once

#include <unordered_map>

#include "responses/response.hpp"

namespace ws {
    class Router {
    public:
        Router() = delete;
        Router(http::request<http::string_body> request);

        ~Router() = default;

        // Return a response for the given request
        http::message_generator makeResponse();

    private:
        http::request<http::string_body> _request;
        Response::RequestInfo _requestInfo;
        std::unordered_map<std::string, std::string> _requestParams;
    };
}
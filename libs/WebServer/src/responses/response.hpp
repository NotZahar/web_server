#pragma once

#include "../utility/types.hpp"

namespace ws {
    class Response {
    public:
        struct RequestInfo {
            const unsigned httpVersion;
            const bool keepAlive;
        };

        Response() = delete;
        explicit Response(RequestInfo request);

        virtual ~Response() = default;

        virtual http::message_generator create() const = 0;
    
    protected:
        RequestInfo _request;
    };
}
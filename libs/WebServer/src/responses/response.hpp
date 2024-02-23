#pragma once

#include "../utility/types.hpp"

namespace ws {
    class Response {
    public:
        Response() = delete;
        explicit Response(unsigned httpVersion, bool keepAlive);
        virtual ~Response() = default;

        virtual http::message_generator create() const;
    
    protected:
        const unsigned _httpVersion;
        const bool _keepAlive;
    };
}
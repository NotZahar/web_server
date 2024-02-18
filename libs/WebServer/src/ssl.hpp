#pragma once

#include "utility/types.hpp"

namespace ws {
    class SSL {
    public:
        SSL() = default;
        ~SSL() = default;

        // This holds the self-signed certificate used by the server
        static void configure(ssl::context& sslContext);
    
    private:
        inline static const std::string _password = "pass";
    };
}
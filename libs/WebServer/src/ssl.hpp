#pragma once

#include "utility/types.hpp"

namespace ws {
    /*!
        \brief Утилитарный класс для настройки сертифкатов TLS
    */
    class SSL {
    public:
        SSL() = default;
        
        ~SSL() = default;

        static void configure(ssl::context& sslContext);
    
    private:
        inline static const std::string _password = "pass";
    };
}
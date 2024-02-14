#pragma once

#include <string>
#include <format>

namespace ws {
    struct messages {
        struct options {
            inline static const std::string HELP_F = "help";
            inline static const std::string HELP_S = "h";
            inline static const std::string HELP = std::format("{},{}", HELP_F, HELP_S);
            inline static const std::string HELP_DESCR = "печать описания параметров";
            
            inline static const std::string ADDRESS_F = "address";
            inline static const std::string ADDRESS_S = "a";
            inline static const std::string ADDRESS = std::format("{},{}", ADDRESS_F, ADDRESS_S);
            inline static const std::string ADDRESS_DESCR = "ipv4 адрес";

            inline static const std::string PORT_F = "port";
            inline static const std::string PORT_S = "p";
            inline static const std::string PORT = std::format("{},{}", PORT_F, PORT_S);
            inline static const std::string PORT_DESCR = "порт";

            inline static const std::string ROOT_F = "root";
            inline static const std::string ROOT_S = "r";
            inline static const std::string ROOT = std::format("{},{}", ROOT_F, ROOT_S);
            inline static const std::string ROOT_DESCR = "путь к папке с контентом";

            inline static const std::string THREADS_F = "threads";
            inline static const std::string THREADS_S = "t";
            inline static const std::string THREADS = std::format("{},{}", THREADS_F, THREADS_S);
            inline static const std::string THREADS_DESCR = "кол-во потоков";
        };

        struct general {
            inline static const std::string HELP_HEADER = "Парамеры";
            inline static const std::string HELP_MESSAGE = std::format(
    "Парамеры:\n \
    -{} [ --{:8} ]        {}\n \
    -{} [ --{:8} ] arg    {}\n \
    -{} [ --{:8} ] arg    {}\n \
    -{} [ --{:8} ] arg    {}\n \
    -{} [ --{:8} ] arg    {}\n \
                ",
                options::HELP_S, options::HELP_F, options::HELP_DESCR,
                options::ADDRESS_S, options::ADDRESS_F, options::ADDRESS_DESCR,
                options::PORT_S, options::PORT_F, options::PORT_DESCR,
                options::ROOT_S, options::ROOT_F, options::ROOT_DESCR,
                options::THREADS_S, options::THREADS_F, options::THREADS_DESCR
            );
        };
    };
}
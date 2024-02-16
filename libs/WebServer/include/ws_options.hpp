#pragma once

#include "../src/utility/types.hpp"
#include "../src/utility/config.hpp"

namespace ws {
    class WSOptions {
    public:
        struct Options {
            bool help = false;
            std::string address = config::defaultAddress;
            unsigned short port = config::defaultPort;
            std::string root = config::defaultRoot;
            int threads = config::defaultThreads;
        };

        WSOptions() = delete;
        WSOptions(int argc, char** argv);
        
        ~WSOptions() = default;

        Options getOptions() const;

    private:
        void initOptions();
        Options parseOptions(int argc, char** argv) const;

        options_description _description;
        Options _options;
    };
}

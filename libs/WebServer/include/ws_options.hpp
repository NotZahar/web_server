#pragma once

#include <string>

#include <boost/program_options.hpp>

namespace ws {
    class WSOptions {
    public:
        using options_description = boost::program_options::options_description;

        struct Options {
            bool help = false;
            std::string address = "127.0.0.1";
            unsigned int port = 3000;
            std::string root = ".";
            unsigned int threads = 1;
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

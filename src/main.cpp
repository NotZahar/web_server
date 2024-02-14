#include <web_server.hpp>
#include <ws_options.hpp>
#include <ws_logger.hpp>

int main(int argc, char** argv) {
    ws::WSOptions wsOptions(argc, argv);
    const auto options = wsOptions.getOptions();
    if (options.help) {
        ws::WSLogger::logHelp();
        return 0;
    }

    ws::run(argc, argv);
    return 0;
}

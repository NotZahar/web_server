#include <web_server.hpp>
#include <ws_options.hpp>
#include <ws_logger.hpp>

int main(int argc, char** argv) {
    ws::WSOptions wsOptions(argc, argv);
    const auto options = wsOptions.getOptions();
    if (options.help) {
        ws::WSLogger::instance().logHelp();
        return 0;
    }

    ws::WebServer webServer{ std::move(wsOptions) };
    webServer.run();

    return 0;
}

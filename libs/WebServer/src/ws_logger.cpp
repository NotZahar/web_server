#include "ws_logger.hpp"

#include <iostream>

#include "utility/helper.hpp"

namespace ws {
    void WSLogger::log(std::string_view log) {
        std::cout << log << '\n';
    }

    void WSLogger::err(std::string_view err) {
        std::cerr << err << '\n';
    }

    void WSLogger::logHelp() {
        log(messages::general::HELP_MESSAGE);
    }
}
#include "response.hpp"

namespace ws {
    Response::Response(unsigned httpVersion, bool keepAlive) 
        : _httpVersion(httpVersion),
          _keepAlive(keepAlive)
    {}
}
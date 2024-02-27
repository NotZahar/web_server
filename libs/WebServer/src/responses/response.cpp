#include "response.hpp"

namespace ws {
    Response::Response(RequestInfo request) noexcept
        : _request(request)
    {}

    Response::RequestInfo Response::getRequestInfo() const noexcept {
        return _request;
    }
}
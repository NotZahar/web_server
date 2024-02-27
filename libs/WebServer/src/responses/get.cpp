#include "get.hpp"

namespace ws {
    GetResponse::GetResponse(
        const http::verb requestMethod, std::unique_ptr<Response> response) noexcept
        : ResponseHandler(requestMethod, std::move(response))
    {}

    bool GetResponse::isValid() const noexcept {
        return _requestMethod == http::verb::get;
    }
}
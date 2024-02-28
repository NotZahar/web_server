#include "get.hpp"

namespace ws {
    Get::Get(
        const http::verb requestMethod, std::unique_ptr<Response> response) noexcept
        : ResponseHandler(requestMethod, std::move(response))
    {}

    bool Get::methodIsValid() const noexcept {
        return _requestMethod == http::verb::get;
    }
}
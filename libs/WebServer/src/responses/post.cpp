#include "post.hpp"

namespace ws {
    Post::Post(
        const http::verb requestMethod, std::unique_ptr<Response> response) noexcept
        : ResponseHandler(requestMethod, std::move(response))
    {}

    bool Post::methodIsValid() const noexcept {
        return _requestMethod == http::verb::post;
    }
}
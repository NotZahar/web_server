#include "response_handler.hpp"

#include "error/bad_request.hpp"
#include "utility/messages.hpp"

namespace ws {
    ResponseHandler::ResponseHandler(
        const http::verb requestMethod, std::unique_ptr<Response> response) noexcept 
        : _requestMethod(requestMethod),
          _response(std::move(response))
    {}

    http::message_generator ResponseHandler::response() const {
        if (!methodIsValid()) {
            return BadRequestResponse{ 
                _response->getRequestInfo(), 
                messages::errors::INVALID_METHOD 
            }.create();
        }

        return _response->create();
    }

    Response::RequestInfo ResponseHandler::getRequestInfo() const noexcept {
        return _response->getRequestInfo();
    }  
}
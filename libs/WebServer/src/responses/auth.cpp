#include "auth.hpp"

#include "error/bad_request.hpp"
#include "../utility/net_helper.hpp"
#include "../utility/config.hpp"

namespace ws {
    AuthResponse::AuthResponse(RequestInfo request, AuthService::AuthData authData)
        : Response(request),
          _authData(authData)
    {}

    http::message_generator AuthResponse::create() const {
        using namespace netHelper;

        AuthService::errorCode error{};
        const auto token = AuthService::createToken(_authData, error);
        if (error != AuthService::errorCode::noError)
            return BadRequestResponse{ _request, messages::errors::INVALID_AUTH }.create();

        http::response<http::string_body> response{ 
            http::status::created, 
            _request.httpVersion 
        };

        response.set(http::field::server, config::fieldServer);
        response.set(
            http::field::content_type, 
            inUTF8(MIMEType.left.find(MIME::text_plain)->second)
        );

        response.keep_alive(_request.keepAlive);
        response.body() = token;
        response.prepare_payload();

        return response;
    }
}
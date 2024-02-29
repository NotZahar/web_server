#include "auth.hpp"

#include <chrono>

#include <jwt/jwt.hpp>
#include <bcrypt.h>

#include "../utility/net_helper.hpp"
#include "../utility/config.hpp"

#include <iostream> // TODO: delete

namespace ws {
    AuthResponse::AuthResponse(RequestInfo request, AuthData authData)
        : Response(request),
          _authData(authData)
    {}

    http::message_generator AuthResponse::create() const {
        using namespace netHelper;
        
        // TODO: [here]
    std::string password = "top_secret";

    std::string hash = bcrypt::generateHash(password);

    std::cout << "Hash: " << hash << std::endl;

    std::cout << "\"" << password << "\" : " << bcrypt::validatePassword(password,hash) << std::endl;
    std::cout << "\"wrong\" : " << bcrypt::validatePassword("wrong",hash) << std::endl;

        jwt::jwt_object token{
            jwt::params::algorithm(config::authAlgorithm), 
            jwt::params::secret(config::authSecret), 
            jwt::params::payload({
                { "email", "example@example.com" },
                { "password", "1234" }
            })
        };

        const auto now = std::chrono::system_clock::now(); 
        token.add_claim("iat", now)
            .add_claim("exp", now + std::chrono::hours{ 24 });

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
        response.body() = token.signature();
        response.prepare_payload();

        return response;
    }
}
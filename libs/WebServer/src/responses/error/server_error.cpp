#include "server_error.hpp"

#include "../../utility/net_helper.hpp"
#include "../../utility/config.hpp"
#include "../../utility/messages.hpp"

namespace ws {
    ServerErrorResponse::ServerErrorResponse(
        unsigned httpVersion, 
        bool keepAlive, 
        std::string what)
        : Response(httpVersion, keepAlive),
          _what(std::move(what))
    {}

    http::message_generator ServerErrorResponse::create() const {
        using namespace netHelper;

        http::response<http::string_body> response{
            http::status::internal_server_error, 
            _httpVersion
        };
        
        response.set(http::field::server, config::fieldServer);
        response.set(
            http::field::content_type, 
            inUTF8(MIMEType.left.find(MIME::text_html)->second)
        );
        
        response.keep_alive(_keepAlive);
        response.body() = messages::errors::INTERNAL_ERROR + _what;
        response.prepare_payload();
        return response;
    }
}
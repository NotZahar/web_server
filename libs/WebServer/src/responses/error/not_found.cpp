#include "not_found.hpp"

#include "../../utility/net_helper.hpp"
#include "../../utility/config.hpp"
#include "../../utility/messages.hpp"

namespace ws {
    NotFoundResponse::NotFoundResponse(
        RequestInfo request,
        std::string_view target)
        : Response(request),
          _target(target)
    {}

    http::message_generator NotFoundResponse::create() const {
        using namespace netHelper;
        
        http::response<http::string_body> response{
            http::status::not_found, 
            _request.httpVersion
        };

        response.set(http::field::server, config::fieldServer);
        response.set(
            http::field::content_type, 
            inUTF8(MIMEType.left.find(MIME::text_html)->second)
        );

        response.keep_alive(_request.keepAlive);
        response.body() = messages::errors::INVALID_TARGET + _target;
        response.prepare_payload();
        
        return response;
    }
}
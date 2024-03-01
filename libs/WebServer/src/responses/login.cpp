#include "login.hpp"

#include <cassert>

#include "error/server_error.hpp"
#include "../utility/config.hpp"
#include "../utility/net_helper.hpp"
#include "../utility/files.hpp"

namespace ws {
    LoginResponse::LoginResponse(RequestInfo request)
        : Response(request)
    {}

    http::message_generator LoginResponse::create() const {
        using namespace netHelper;

        assert(fs::is_regular_file(paths::pages::auth));

        beast::error_code authErrorCode;
        http::file_body::value_type authBody;
        authBody.open(paths::pages::auth.c_str(), beast::file_mode::scan, authErrorCode);
        if (authErrorCode)
            return ServerErrorResponse{ _request, authErrorCode.message() }.create();
        
        const auto contentSize = authBody.size();

        http::response<http::file_body> response{
            std::piecewise_construct,
            std::make_tuple(std::move(authBody)),
            std::make_tuple(http::status::ok, _request.httpVersion)
        };

        response.set(http::field::server, config::fieldServer);
        response.set(
            http::field::content_type, 
            MIMEType.left.find(extensionToMIME.at(paths::pages::auth.extension()))->second
        );
        
        response.content_length(contentSize);
        response.keep_alive(_request.keepAlive);

        return response;
    }
}
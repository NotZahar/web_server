#include "auth.hpp"

#include <cassert>

#include "error/server_error.hpp"
#include "../utility/net_helper.hpp"
#include "../utility/files.hpp"

namespace ws {
    http::message_generator AuthResponse::create() const {
        using namespace netHelper;

        assert(fs::is_regular_file(paths::pages::auth));

        // beast::error_code targetErrorCode;
        // http::file_body::value_type targetBody;
        // targetBody.open(targetPath.c_str(), beast::file_mode::scan, targetErrorCode);
        
        // if (targetErrorCode == beast::errc::no_such_file_or_directory)
        //     return notFoundResponse(requestTarget);
        // if (targetErrorCode)
        //     return serverErrorResponse(targetErrorCode.message());


        http::response<http::file_body> response{
            std::piecewise_construct,
            std::make_tuple(std::move(targetBody)),
            std::make_tuple(http::status::ok, _request.version())
        };

        response.set(http::field::server, config::fieldServer);
        response.set(
            http::field::content_type, 
            MIMEType.left.find(extensionToMIME.at(targetPath.extension()))->second
        );
        
        response.content_length(targetSize);
        response.keep_alive(_request.keep_alive());
        return response;
    }
}
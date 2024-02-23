#include "router.hpp"

#include "responses/auth.hpp"
#include "utility/messages.hpp"
#include "utility/config.hpp"
#include "utility/net_helper.hpp"
#include "utility/url.hpp"
#include "utility/params.hpp"

namespace ws {
    Router::Router(http::request<http::string_body> request)
        : _request(std::move(request))
    {}

    http::message_generator Router::makeResponse() {
        // std::cout << "base: " << _request.base() << '\n';
        // std::cout << "body: " << _request.body() << '\n';
        // std::cout << "method: " << _request.method() << '\n';
        // std::cout << "target: " << _request.target() << '\n';

        const auto requestMethod{ _request.method() };
        const url requestURL{ _request.target() };

        if (!allowedMethod(requestMethod))
            return badRequestResponse(messages::errors::INVALID_METHOD);
        if (requestURL.isEmpty())
            return badRequestResponse(messages::errors::INVALID_REQUEST);

        const auto requestParams = requestURL.getParams();
        if (!requestParams.contains(params.left.find(param::auth)->second))
            return AuthResponse().create();
        
        // fs::path targetPath = makeTargetPath(config::staticRootPath, requestTarget);
        // if (!fs::is_regular_file(targetPath))
        //     return notFoundResponse(requestTarget);

        // beast::error_code targetErrorCode;
        // http::file_body::value_type targetBody;
        // targetBody.open(targetPath.c_str(), beast::file_mode::scan, targetErrorCode);
        
        // if (targetErrorCode == beast::errc::no_such_file_or_directory)
        //     return notFoundResponse(requestTarget);
        // if (targetErrorCode)
        //     return serverErrorResponse(targetErrorCode.message());
    
        switch (requestMethod) {
            // case http::verb::head:
            //     return headResponse(targetBody.size(), targetPath);
            // case http::verb::get: 
            //     return getResponse(targetBody.size(), targetPath, std::move(targetBody));    
            default:
                return serverErrorResponse(messages::errors::INTERNAL_ERROR_GENERAL);
        }
    }

    bool Router::allowedMethod(const http::verb method) const {
        switch (method) {
            case http::verb::get:
                [[fallthrough]];
            case http::verb::head:
                [[fallthrough]];
            case http::verb::post:
                return true;
            default:
                return false;
        }
    }

    http::response<http::empty_body> Router::headResponse(
        const std::uint64_t targetSize,
        const fs::path& targetPath) const {
        using namespace netHelper;
        
        http::response<http::empty_body> response{
            http::status::ok, 
            _request.version()
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

    http::response<http::file_body> Router::getResponse(
        const std::uint64_t targetSize,
        const fs::path& targetPath,
        http::file_body::value_type targetBody) const {
        using namespace netHelper;
        
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

    http::response<http::string_body> Router::badRequestResponse(
            std::string_view reason) const {
        using namespace netHelper;
        
        http::response<http::string_body> response{ 
            http::status::bad_request, 
            _request.version() 
        };

        response.set(http::field::server, config::fieldServer);
        response.set(
            http::field::content_type, 
            appendCharset(MIMEType.left.find(MIME::text_html)->second)
        );

        response.keep_alive(_request.keep_alive());
        response.body() = std::string{ reason };
        response.prepare_payload();
        return response;
    }

    http::response<http::string_body> Router::notFoundResponse(
        std::string_view target) const {
        using namespace netHelper;
        
        http::response<http::string_body> response{
            http::status::not_found, 
            _request.version()
        };

        response.set(http::field::server, config::fieldServer);
        response.set(
            http::field::content_type, 
            appendCharset(MIMEType.left.find(MIME::text_html)->second)
        );

        response.keep_alive(_request.keep_alive());
        response.body() = messages::errors::INVALID_TARGET + std::string{ target };
        response.prepare_payload();
        return response;
    }
    
    http::response<http::string_body> Router::serverErrorResponse(
        std::string_view what) const {
        using namespace netHelper;

        http::response<http::string_body> response{
            http::status::internal_server_error, 
            _request.version()
        };
        
        response.set(http::field::server, config::fieldServer);
        response.set(
            http::field::content_type, 
            appendCharset(MIMEType.left.find(MIME::text_html)->second)
        );
        
        response.keep_alive(_request.keep_alive());
        response.body() = messages::errors::INTERNAL_ERROR + std::string{ what };
        response.prepare_payload();
        return response;
    }
}
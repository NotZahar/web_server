#include "request_handler.hpp"

#include "utility/messages.hpp"
#include "utility/config.hpp"
#include "utility/net_helper.hpp"

namespace ws {
    RequestHandler::RequestHandler(http::request<http::string_body> request)
        : _request(std::move(request))
    {}

    http::message_generator RequestHandler::makeResponse(const fs::path& staticRootPath) {
        const auto requestMethod = _request.method();
        const std::string requestTarget = _request.target();

        if (!allowedMethod(requestMethod))
            return badRequestResponse(messages::server::INVALID_METHOD);
        if (!allowedPath(requestTarget))
            return badRequestResponse(messages::server::INVALID_PATH);
        
        fs::path targetPath = makeTargetPath(staticRootPath, requestTarget);
        if (!fs::is_regular_file(targetPath))
            return notFoundResponse(requestTarget);

        beast::error_code targetErrorCode;
        http::file_body::value_type targetBody;
        targetBody.open(targetPath.c_str(), beast::file_mode::scan, targetErrorCode);
        
        if (targetErrorCode == beast::errc::no_such_file_or_directory)
            return notFoundResponse(requestTarget);
        if (targetErrorCode)
            return serverErrorResponse(targetErrorCode.message());
    
        switch (requestMethod) {
            case http::verb::head:
                return headResponse(targetBody.size(), targetPath);
            case http::verb::get: 
                return getResponse(targetBody.size(), targetPath, std::move(targetBody));    
            default:
                return serverErrorResponse(messages::server::INTERNAL_ERROR_GENERAL);
        }
    }

    bool RequestHandler::allowedMethod(const http::verb method) const {
        return method == http::verb::get 
            || method == http::verb::head;
    }

    bool RequestHandler::allowedPath(std::string_view targetPath) const {
        return !targetPath.empty()
            && targetPath[0] == '/'
            && targetPath.find("..") == std::string_view::npos;
    }

    fs::path RequestHandler::makeTargetPath(
            const fs::path& staticRootPath, 
            std::string_view requestedPath) const {
        fs::path target(staticRootPath);
        target /= std::string_view{
            requestedPath.cbegin() + 1, 
            requestedPath.cend() 
        };
        return target;
    }

    http::response<http::empty_body> RequestHandler::headResponse(
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

    http::response<http::file_body> RequestHandler::getResponse(
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

    http::response<http::string_body> RequestHandler::badRequestResponse(
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

    http::response<http::string_body> RequestHandler::notFoundResponse(
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
        response.body() = messages::server::INVALID_TARGET + std::string{ target };
        response.prepare_payload();
        return response;
    }
    
    http::response<http::string_body> RequestHandler::serverErrorResponse(
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
        response.body() = messages::server::INTERNAL_ERROR + std::string{ what };
        response.prepare_payload();
        return response;
    }
}
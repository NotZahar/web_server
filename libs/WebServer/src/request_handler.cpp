#include "request_handler.hpp"

#include "../utility/messages.hpp"

namespace ws {
    RequestHandler::RequestHandler(
        const fs::path& docsRoot, 
        http::request<http::string_body> request
    )
        : _docsRoot(docsRoot),
          _request(std::move(request))
    {}

    // Return a reasonable mime type based on the extension of a file.
    inline beast::string_view mime_type(beast::string_view path)
    {
        using beast::iequals;
        auto const ext = [&path]
        {
            auto const pos = path.rfind(".");
            if(pos == beast::string_view::npos)
                return beast::string_view{};
            return path.substr(pos);
        }();
        if(iequals(ext, ".htm"))  return "text/html";
        if(iequals(ext, ".html")) return "text/html";
        if(iequals(ext, ".php"))  return "text/html";
        if(iequals(ext, ".css"))  return "text/css";
        if(iequals(ext, ".txt"))  return "text/plain";
        if(iequals(ext, ".js"))   return "application/javascript";
        if(iequals(ext, ".json")) return "application/json";
        if(iequals(ext, ".xml"))  return "application/xml";
        if(iequals(ext, ".swf"))  return "application/x-shockwave-flash";
        if(iequals(ext, ".flv"))  return "video/x-flv";
        if(iequals(ext, ".png"))  return "image/png";
        if(iequals(ext, ".jpe"))  return "image/jpeg";
        if(iequals(ext, ".jpeg")) return "image/jpeg";
        if(iequals(ext, ".jpg"))  return "image/jpeg";
        if(iequals(ext, ".gif"))  return "image/gif";
        if(iequals(ext, ".bmp"))  return "image/bmp";
        if(iequals(ext, ".ico"))  return "image/vnd.microsoft.icon";
        if(iequals(ext, ".tiff")) return "image/tiff";
        if(iequals(ext, ".tif"))  return "image/tiff";
        if(iequals(ext, ".svg"))  return "image/svg+xml";
        if(iequals(ext, ".svgz")) return "image/svg+xml";
        return "application/text";
    }

    // Append an HTTP rel-path to a local filesystem path.
    // The returned path is normalized for the platform.
    inline std::string path_cat(
        beast::string_view base,
        beast::string_view path)
    {
        if(base.empty())
            return std::string(path);
        std::string result(base);
        char constexpr path_separator = '/';
        if(result.back() == path_separator)
            result.resize(result.size() - 1);
        result.append(path.data(), path.size());
        return result;
    }

    http::message_generator RequestHandler::makeResponse(const fs::path& docsRoot) {
        if(!allowedMethod(_request.method()))
            return badRequestResponse(messages::server::INVALID_METHOD);

        if(!allowedPath(_request.target()))
            return badRequestResponse(messages::server::INVALID_PATH);

        // Build the path to the requested file
        fs::path targetPath = path_cat(docsRoot, _request.target());
        if(_request.target().back() == '/')
            targetPath.append("index.html");

        // Attempt to open the file
        beast::error_code ec;
        http::file_body::value_type body;
        body.open(targetPath.c_str(), beast::file_mode::scan, ec);

        // Handle the case where the file doesn't exist
        if(ec == beast::errc::no_such_file_or_directory)
            return notFoundResponse(_request.target());

        // Handle an unknown error
        if(ec)
            return serverErrorResponse(ec.message());

        // Cache the size since we need it after the move
        auto const size = body.size();

        // Respond to HEAD request
        if(_request.method() == http::verb::head)
        {
            http::response<http::empty_body> res{http::status::ok, req.version()};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, mime_type(targetPath));
            res.content_length(size);
            res.keep_alive(_request.keep_alive());
            return res;
        }

        // Respond to GET request
        http::response<http::file_body> res{
            std::piecewise_construct,
            std::make_tuple(std::move(body)),
            std::make_tuple(http::status::ok, req.version())};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, mime_type(path));
        res.content_length(size);
        res.keep_alive(req.keep_alive());
        return res;
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

    http::response<http::string_body> RequestHandler::badRequestResponse(std::string_view reason) const {
        http::response<http::string_body> res{http::status::bad_request, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = std::string(why);
        res.prepare_payload();
        return res;
    }

    http::response<http::string_body> RequestHandler::notFoundResponse(std::string_view target) const {
        http::response<http::string_body> res{http::status::not_found, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "The resource '" + std::string(target) + "' was not found.";
        res.prepare_payload();
        return res;
    }
    
    http::response<http::string_body> RequestHandler::serverErrorResponse(std::string_view what) const {
        http::response<http::string_body> res{http::status::internal_server_error, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "An error occurred: '" + std::string(what) + "'";
        res.prepare_payload();
        return res;
    }
}
#include "file.hpp"

#include "error/not_found.hpp"
#include "error/bad_request.hpp"
#include "../utility/net_helper.hpp"
#include "../utility/files.hpp"
#include "../utility/messages.hpp"
#include "../utility/config.hpp"

namespace ws {
    FileResponse::FileResponse(RequestInfo request, fs::path path)
        : Response(request),
          _relativePath(std::move(path))
    {}

    http::message_generator FileResponse::create() const {
        using namespace netHelper;

        const auto filePath = paths::storagePath / _relativePath;

        beast::error_code fileErrorCode;
        http::file_body::value_type fileBody;
        fileBody.open(filePath.c_str(), beast::file_mode::scan, fileErrorCode);
        if (fileErrorCode)
            return NotFoundResponse{ _request, fileErrorCode.message() }.create();
        
        const auto contentSize = fileBody.size();

        http::response<http::file_body> response{
            std::piecewise_construct,
            std::make_tuple(std::move(fileBody)),
            std::make_tuple(http::status::ok, _request.httpVersion)
        };

        const auto extension = filePath.extension();
        if (!extensionToMIME.contains(extension))
            return BadRequestResponse{ _request, messages::errors::INVALID_REQUEST }.create();

        response.set(http::field::server, config::fieldServer);
        response.set(
            http::field::content_type, 
            MIMEType.left.find(extensionToMIME.at(extension))->second
        );
        
        response.content_length(contentSize);
        response.keep_alive(_request.keepAlive);

        return response;
    }
}
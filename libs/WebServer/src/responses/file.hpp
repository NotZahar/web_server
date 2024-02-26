#pragma once

#include "response.hpp"

namespace ws {
    class FileResponse final : public Response {
    public:
        FileResponse() = delete;
        explicit FileResponse(RequestInfo request, fs::path path);
        
        ~FileResponse() override = default;

        http::message_generator create() const override;

    private:
        const fs::path _relativePath;
    };
}
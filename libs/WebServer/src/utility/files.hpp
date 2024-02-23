#pragma once

#include <filesystem>
#include <fstream>
#include <cassert>
#include <cstdio>

#include "types.hpp"
#include "config.hpp"

namespace ws {
    struct paths {
        inline static const fs::path resourcesPath = fs::current_path() / "resources";
        inline static const fs::path publicPath = resourcesPath / "public";

        struct pages {
            inline static const fs::path auth = publicPath / "auth.html";
        };

        struct ssl {
            inline static const fs::path cert = resourcesPath / "ssl/cert.pem";
            inline static const fs::path key = resourcesPath / "ssl/key.pem";
            inline static const fs::path dh = resourcesPath / "ssl/dh.pem";
        };
    };

    struct files {
        static std::string getContent(const fs::path& path) {
            auto filePtr = std::fopen(path.c_str(), "rb");
            if (filePtr == nullptr)
                return {};

            std::fseek(filePtr, 0u, SEEK_END);
            const auto size = std::ftell(filePtr);
            std::fseek(filePtr, 0u, SEEK_SET);

            std::string content;
            content.resize(size);

            [[maybe_unused]] const auto read 
                = std::fread(&content[0], 1u, size, filePtr);
            std::fclose(filePtr);
            return content;
        }
    };
}
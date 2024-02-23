#pragma once

#include "types.hpp"

namespace ws {
    struct url {
        std::unordered_map<std::string, std::string> getParams() const {
            auto urlView = urls::url_view{ rawUrl };
            std::unordered_map<std::string, std::string> params;
            for (auto param : urlView.params())
                params.insert({ param.key, param.value });
            return params;
        }

        bool isEmpty() const {
            return urls::url_view{ rawUrl }.empty();
        }

        std::string rawUrl;
    };
}
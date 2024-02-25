#pragma once

#include <set>

#include "types.hpp"

namespace ws {
    struct url {
        std::multiset<std::string> getSegments() const {
            auto urlView = urls::url_view{ relativeUrl };
            std::multiset<std::string> segments;
            for (auto segment : urlView.segments())
                segments.insert(segment);
            return segments;
        }

        std::unordered_map<std::string, std::string> getParams() const {
            auto urlView = urls::url_view{ relativeUrl };
            std::unordered_map<std::string, std::string> params;
            for (auto param : urlView.params())
                params.insert({ param.key, param.value });
            return params;
        }

        bool isEmpty() const {
            return urls::url_view{ relativeUrl }.empty();
        }

        std::string relativeUrl;
    };
}
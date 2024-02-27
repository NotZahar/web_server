#include "router.hpp"

#include "responses/get.hpp"
#include "responses/auth.hpp"
#include "responses/file.hpp"
#include "responses/error/bad_request.hpp"
#include "services/auth_service.hpp"
#include "utility/messages.hpp"
#include "utility/net_helper.hpp"
#include "utility/url.hpp"
#include "utility/params.hpp"
#include "utility/segments.hpp"

namespace ws {
    Router::Router(http::request<http::string_body> request)
        : _request(std::move(request))
    {}

    http::message_generator Router::makeResponse() {
        const auto requestMethod{ _request.method() };
        const url requestURL{ _request.target() };
        _requestInfo = { _request.version(), _request.keep_alive() };
        _requestParams = requestURL.getParams();
        
        if (!AuthService::authDataExists(_requestParams))
            return AuthResponse{ _requestInfo }.create();
        if (!AuthService::authenticate(_requestParams))
            return BadRequestResponse{ 
                _requestInfo, 
                messages::errors::INVALID_AUTH 
            }.create();

        const auto requestSegments = requestURL.getSegments();
        if (requestSegments.empty() || routeSegments.right.count(requestSegments.front()) == 0)
            return BadRequestResponse{ _requestInfo }.create();

        const routeSegment route = routeSegments.right.find(requestSegments.front())->second;
        switch (route) {
            case routeSegment::file: {
                const auto& pathParam = urlParams.left.find(urlParam::path)->second;
                if (!_requestParams.contains(pathParam))
                    return BadRequestResponse{ _requestInfo }.create();
                
                return GetResponse{ 
                    requestMethod, 
                    std::make_unique<FileResponse>(
                        _requestInfo, 
                        _requestParams.at(pathParam)) 
                    }.response();
            }
            default: 
                return BadRequestResponse{ _requestInfo }.create();
        }
    }
}
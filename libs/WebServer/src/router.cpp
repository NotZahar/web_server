#include "router.hpp"

#include "responses/authorized.hpp"
#include "responses/get.hpp"
#include "responses/post.hpp"
#include "responses/file.hpp"
#include "responses/auth.hpp"
#include "responses/error/bad_request.hpp"
#include "utility/net_helper.hpp"
#include "utility/url.hpp"
#include "utility/params.hpp"
#include "utility/segments.hpp"

namespace ws {
    http::message_generator Router::makeResponse(http::request<http::string_body> request) {
        const auto requestMethod{ request.method() };
        const url requestURL{ request.target() };
        const Response::RequestInfo requestInfo = { request.version(), request.keep_alive() };
        const std::unordered_map<std::string, std::string> requestParams = requestURL.getParams();

        const auto requestSegments = requestURL.getSegments();
        if (requestSegments.empty() || routeSegments.right.count(requestSegments.front()) == 0)
            return BadRequestResponse{ requestInfo }.create();

        const routeSegment route = routeSegments.right.find(requestSegments.front())->second;
        switch (route) {
            case routeSegment::file: {
                if (!url::paramsExists(requestParams, { urlParam::path }))
                    return BadRequestResponse{ requestInfo }.create();
                
                return Authorized{ 
                    requestParams,
                    std::make_unique<Get>(
                        requestMethod, 
                        std::make_unique<FileResponse>(
                            requestInfo, 
                            requestParams.at(urlParams.left.find(urlParam::path)->second))
                    )
                }.response();
            } case routeSegment::auth: {
                if (!url::paramsExists(requestParams, { urlParam::email, urlParam::password }))
                    return BadRequestResponse{ requestInfo }.create();
                
                return Post{
                    requestMethod, 
                    std::make_unique<AuthResponse>(
                        requestInfo,
                        AuthResponse::AuthData{
                            requestParams.at(urlParams.left.find(urlParam::email)->second),
                            requestParams.at(urlParams.left.find(urlParam::password)->second)
                        }
                    )
                }.response();
            } default: { 
                return BadRequestResponse{ requestInfo }.create();
            }
        }
    }
}
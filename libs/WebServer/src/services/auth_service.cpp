#include "auth_service.hpp"

#include <jwt/jwt.hpp>

#include "db_service.hpp"
#include "../utility/params.hpp"
#include "../utility/db.hpp"
#include "../utility/config.hpp"

namespace ws {
    void AuthService::authenticate(
        const std::unordered_map<std::string, std::string>& params,
        errorCode& code) {
        if (!params.contains(urlParams.left.find(urlParam::token)->second)) {
            code = errorCode::noData;
            return;
        }

        std::error_code jwtErrorCode;
        const auto encodedToken = params.at(urlParams.left.find(urlParam::token)->second);
        const auto token = jwt::decode(
            encodedToken,
            jwt::params::algorithms({ config::authAlgorithm }), 
            jwtErrorCode,
            jwt::params::secret(config::authSecret)
        );

        if (jwtErrorCode) {
            code = errorCode::badData;
            return;
        }

        // TODO: [here]
        
        // jwt::jwt_object jwtToken{ 
        //     jwt::params::algorithm(config::authAlgorithm), 
        //     payload({{"some", "payload"}}), 
        //     secret(key) 
        // };

        // const bool asd = DBService::selectQuery(query::getUser);
    } 
}
#include "auth_service.hpp"

#include <iterator>

#include <jwt/jwt.hpp>
#include <bcrypt.h>

#include "db_service.hpp"
#include "../utility/params.hpp"
#include "../utility/db.hpp"
#include "../utility/config.hpp"

namespace ws {
    std::string AuthService::createToken(AuthData authData, errorCode& error) {
        const auto email = authData.email;
        const auto password = authData.password;

        if (!sanitizer::check(email)) {
            error = errorCode::badData;
            return {};
        }

        DBService::errorCode selectError{};
        std::vector<std::string> values;
        values.emplace_back(email);
        const auto user = DBService::select(
            query::getUser, 
            std::move(values),
            query::getUserSizeResult,
            selectError
        );
        
        if (selectError != DBService::errorCode::noError
            || user.size() != query::getUserSizeResult) {
            error = errorCode::badData;
            return {};
        }

        auto userIt = user.cbegin();
        std::string dbEmail = *userIt;
        std::advance(userIt, 1);
        std::string dbPassword = *(userIt);

        if (!bcrypt::validatePassword(std::string{ password }, dbPassword)) {
            error = errorCode::badData;
            return {};
        }

        jwt::jwt_object token{
            jwt::params::algorithm(config::authAlgorithm), 
            jwt::params::secret(config::authSecret), 
            jwt::params::payload({
                { emailPayloadHeader, email }
            })
        };

        const auto now = std::chrono::system_clock::now(); 
        token.add_claim(iatClaim, now)
            .add_claim(expClaim, now + tokenExpirationTime);

        return token.signature();
    }

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
    } 
}
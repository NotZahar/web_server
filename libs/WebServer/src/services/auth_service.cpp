#include "auth_service.hpp"

#include "../utility/params.hpp"

namespace ws {
    bool AuthService::authDataExists(
        const std::unordered_map<std::string, std::string>& params) noexcept {
        const bool emailExists = 
            params.contains(urlParams.left.find(urlParam::email)->second);
        const bool passwordExists = 
            params.contains(urlParams.left.find(urlParam::password)->second);

        return emailExists && passwordExists;
    }

    bool AuthService::authenticate(const std::unordered_map<std::string, std::string>&) noexcept {
        // TODO: [here]
        return true;
    } 
}
#include "ssl.hpp"

#include "utility/config.hpp"
#include "utility/files.hpp"

namespace ws {
    void SSL::configure(ssl::context& sslContext) {
        const std::string cert = files::getContent(paths::ssl::cert);
        const std::string key = files::getContent(paths::ssl::key);
        const std::string dh = files::getContent(paths::ssl::dh);

        sslContext.set_password_callback(
            [](
                std::size_t, 
                ssl::context_base::password_purpose) {
            return _password;
        });

        sslContext.set_options(
            ssl::context::default_workarounds |
            ssl::context::no_sslv2 |
            ssl::context::single_dh_use
        );

        sslContext.use_certificate_chain({ cert.data(), cert.size() });
        sslContext.use_private_key(
            { key.data(), key.size() }, 
            ssl::context::file_format::pem
        );
        sslContext.use_tmp_dh({ dh.data(), dh.size() });
    }
}

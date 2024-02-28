#pragma once

#include <string>

#include <boost/bimap.hpp>
#include <boost/assign.hpp>

namespace ws {
    enum class urlParam {
        token,
        email,
        password,
        path
    };

    inline static const boost::bimap<urlParam, std::string> urlParams = 
        boost::assign::list_of<boost::bimap<urlParam, std::string>::relation>
        ( urlParam::token, "token" )
        ( urlParam::email, "email" )
        ( urlParam::password, "pass" )
        ( urlParam::path, "path" );
}
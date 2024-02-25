#pragma once

#include <string>

#include <boost/bimap.hpp>
#include <boost/assign.hpp>

namespace ws {
    enum class param {
        email,
        password
    };

    inline static const boost::bimap<param, std::string> params = 
        boost::assign::list_of<boost::bimap<param, std::string>::relation>
        ( param::email, "email" )
        ( param::password, "password" );
}
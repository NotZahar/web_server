#pragma once

#include <string>

#include <boost/bimap.hpp>
#include <boost/assign.hpp>

namespace ws {
    enum class segment {
        auth
    };

    inline static const boost::bimap<segment, std::string> segments = 
        boost::assign::list_of<boost::bimap<segment, std::string>::relation>
        ( segment::auth, "auth" );
}
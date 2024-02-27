#pragma once

#include <string>

#include <boost/bimap.hpp>
#include <boost/assign.hpp>

namespace ws {
    enum class routeSegment {
        file,
        auth
    };

    inline static const boost::bimap<routeSegment, std::string> urlSegments = 
        boost::assign::list_of<boost::bimap<routeSegment, std::string>::relation>
        ( routeSegment::file, "file" )
        ( routeSegment::auth, "auth" );
}
#pragma once

#include <string>

#include <boost/bimap.hpp>
#include <boost/assign.hpp>

namespace ws {
    enum class urlKeySegment {
        file,
        auth
    };

    inline static const boost::bimap<urlKeySegment, std::string> urlSegments = 
        boost::assign::list_of<boost::bimap<urlKeySegment, std::string>::relation>
        ( urlKeySegment::file, "file" )
        ( urlKeySegment::auth, "auth" );
}
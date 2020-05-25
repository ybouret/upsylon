
#include "y/ios/scribe.hpp"
#include <typeinfo>

namespace upsylon {

    namespace ios {

        scribe::~scribe() throw()
        {
        }

        scribe:: scribe(const std::type_info &t) throw() :
        info(t)
        {
        }


    }

}


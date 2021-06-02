
#include "y/ios/indent.hpp"

namespace upsylon
{

    namespace ios
    {
        indent:: ~indent() throw()
        {
        }

        indent:: indent(const indent &other) throw() :
        width(other.width),
        space(other.space)
        {
        }

        indent:: indent(const size_t w, const char c) throw() :
        width(w),
        space(c)
        {
        }
        
    }
}


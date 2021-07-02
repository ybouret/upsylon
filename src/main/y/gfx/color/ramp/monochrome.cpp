
#include "y/gfx/color/ramp/monochrome.hpp"
#include <cstring>


namespace upsylon
{
    namespace graphic
    {
        monochrome:: monochrome(const rgba &C) throw() : data()
        {
            aliasing::_(data[0]) = rgba(0,0,0);
            aliasing::_(data[1]) = C;
        }

        monochrome:: monochrome(const monochrome &other) throw() : data()
        {
            memcpy( (void*) data, other.data, sizeof(data) );
        }

        monochrome:: ~monochrome() throw()
        {
            memset( (void*) data, 0, sizeof(data) );
        }

    }

}



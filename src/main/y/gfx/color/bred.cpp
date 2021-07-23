

#include "y/gfx/color/bred.hpp"

namespace upsylon {

    namespace graphic
    {

        byte_divider:: ~byte_divider() throw()
        {}

        byte_divider::byte_divider(const uint8_t *v, const size_t n) throw():
        value(v),
        count(n)
        {
            assert(!(NULL==value&&count>0));
        }

        byte_divider::byte_divider(const byte_divider &_) throw() :
        value( _.value ),
        count( _.count )
        {
        }
        
#include "bred-inc.hxx"

    }

}


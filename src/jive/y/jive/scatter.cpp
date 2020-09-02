

#include "y/jive/scatter.hpp"

namespace upsylon {

    namespace Jive
    {
        Scatter:: Node:: ~Node() throw()
        {
            assert(!next);
            assert(!prev);
        }

        Scatter:: Node:: Node(const Motif &m,const uint8_t c) throw() :
        motif( m.content() ),
        next(0),
        prev(0),
        code(c)
        {
        }


        Scatter:: ~Scatter() throw()
        {
            
        }


    }

}

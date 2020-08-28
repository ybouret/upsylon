
#include "y/jive/pattern/logic/seq.hpp"

namespace upsylon {

    namespace Jive
    {

        Sequential:: ~Sequential() throw()
        {
        }

        Sequential:: Sequential(const uint32_t t) throw() :
        Logical(t)
        {
        }

        Sequential:: Sequential(const Sequential &other) :
        Logical(other)
        {
        }

        void Sequential:: append(Pattern *p) throw()
        {
            operands.push_back(p);
        }

        void Sequential:: harden() throw()
        {
        }
        
    }

}


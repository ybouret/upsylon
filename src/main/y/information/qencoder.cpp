
#include "y/information/qencoder.hpp"

namespace upsylon {

    namespace information {

        qencoder:: qencoder() throw() : qbits::broker()
        {
        }

        qencoder:: ~qencoder() throw()
        {
        }
        
    }

}

namespace upsylon {
    
    namespace information {

        qwriter:: qwriter() throw()
        {
        }
        
        qwriter:: ~qwriter() throw()
        {
            
        }
        
        void qwriter:: writeBits(qbits &io, const char C)
        {
            io.push(C);
        }
        
        void qwriter:: flushBits(qbits &)
        {
        }
        
    }
}

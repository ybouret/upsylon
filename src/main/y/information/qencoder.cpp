
#include "y/information/qencoder.hpp"

namespace upsylon {

    namespace Information {

        qencoder:: qencoder() throw() : IOBits::Broker()
        {
        }

        qencoder:: ~qencoder() throw()
        {
        }
        
    }

}

namespace upsylon {
    
    namespace Information {

        qwriter:: qwriter() throw()
        {
        }
        
        qwriter:: ~qwriter() throw()
        {
            
        }
        
        void qwriter::  startBits() throw()
        {
        }
        
        void qwriter:: writeBits(IOBits &io, const char C)
        {
            io.push(C);
        }
        
        void qwriter:: flushBits(IOBits &)
        {
        }
        
    }
}

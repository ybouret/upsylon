
#include "y/information/qencoder.hpp"

namespace upsylon {

    namespace Information {

        QEncoder:: QEncoder() throw() : IOBits::Broker()
        {
        }

        QEncoder:: ~QEncoder() throw()
        {
        }
        
    }

}

namespace upsylon {
    
    namespace Information {

        QWriter:: QWriter() throw()
        {
        }
        
        QWriter:: ~QWriter() throw()
        {
            
        }
        
        void QWriter::  startBits() throw()
        {
        }
        
        void QWriter:: writeBits(IOBits &io, const char C)
        {
            io.push(C);
        }
        
        void QWriter:: flushBits(IOBits &)
        {
        }
        
    }
}

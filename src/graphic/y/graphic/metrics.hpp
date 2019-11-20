
//! \file

#ifndef Y_GRAPHIC_METRICS_INCLUDED
#define Y_GRAPHIC_METRICS_INCLUDED 1

#include "y/graphic/area.hpp"


namespace upsylon {

    namespace Graphic {


        //! 
        struct Metrics
        {
            static const unsigned GlobalMemory = 0x01;
            static const unsigned StaticMemory = 0x02;
            static const unsigned MemoryMask   = GlobalMemory|StaticMemory;
            static const unsigned ReadWrite    = 0x04;
            static const unsigned ReadOnly     = 0x08;
            static const unsigned AccessMask   = ReadWrite|ReadOnly;
            
            static void *Acquire( size_t & );
            static void  Release( void * &, size_t & ) throw();

            const void *entry;
            size_t      count;
            unsigned    flags;
            size_t      width;
            size_t      height;
            size_t      depth;
            size_t      stride;
            
        };

    }

}

#endif


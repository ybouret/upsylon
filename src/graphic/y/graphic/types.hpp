
//! \file

#ifndef Y_GRAPHIC_TYPES_INCLUDED
#define Y_GRAPHIC_TYPES_INCLUDED 1

#include "y/ptr/counted.hpp"
#include "y/type/point2d.hpp"
#include "y/core/zero-flux-index.hpp"

namespace upsylon {

    namespace Graphic {

        typedef counted_object           Object;    //!< base type for dynamic object
        typedef point2d<unit_t>          Point;     //!< logical point
        typedef core::zero_flux_index<0> ZeroFlux;  //!< for indexing

        void *MemoryAcquire( size_t &n );
        void  MemoryRelease( void * &p, size_t &n ) throw();

        template <typename T>
        T *MemoryAcquireAs( size_t &required )
        {
            required *= sizeof(T);
            return static_cast<T*>( MemoryAcquire(required) );
        }

        template <typename T>
        void MemoryReleaseAs( T * &p, size_t &allocated ) throw()
        {
            void * &address = *(void **)&p;
            MemoryRelease(address,allocated);
        }


    }
}

#endif



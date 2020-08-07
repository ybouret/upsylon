
//! \file

#ifndef Y_GRAPHIC_TYPES_INCLUDED
#define Y_GRAPHIC_TYPES_INCLUDED 1

#include "y/ptr/counted.hpp"
#include "y/type/point2d.hpp"
#include "y/core/zero-flux-index.hpp"

namespace upsylon {

    namespace Graphic {

        typedef counted_object           Object;     //!< base type for dynamic object
        typedef point2d<unit_t>          Point;     //!< logical point
        typedef core::zero_flux_index<0> ZeroFlux;  //!< for indexing
        typedef point2d<float>           Vertex;    //!< for floating point vertices
        
        //! memory description
        struct Memory
        {
            //! allocated or not
            enum Kind
            {
                Global, //!< use of memory::global
                Static  //!< user's memory
            };

            //! access node
            enum Mode
            {
                ReadWrite, //!< full access
                ReadOnly   //!< read only
            };

            //! dedicated acquire>=n bytes
            static void *Acquire( size_t &n );

            //! release previously allocated memory
            static void  Release( void * &p, size_t &n ) throw();

            //! special allocation function
            template <typename T> static inline
            T *AcquireAs( size_t &required )
            {
                required *= sizeof(T);
                return static_cast<T*>( Acquire(required) );
            }

            //! special release function
            template <typename T> static inline
            void ReleaseAs( T * &p, size_t &allocated ) throw()
            {
                void * &address = *(void **)&p;
                Release(address,allocated);
            }
        };
        
    }
}

#endif



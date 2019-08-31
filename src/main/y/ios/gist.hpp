//!\file

#ifndef Y_IOS_GIST_INCLUDED
#define Y_IOS_GIST_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    namespace ios
    {
        //! utility function for I/O
        struct gist
        {
            //! right shift 8 bits sizeof(T)>1
            template <typename T>
            static inline void shr8( T &x, int2type<true> ) throw()
            {
                assert(sizeof(T)>1);
                x >>= 8;
            }

            //! right shift 8 bits sizeof(T)<=1
            template <typename T>
            static inline void shr8( T &x, int2type<false> ) throw()
            {
                assert(sizeof(T)<=1);
                x=0;
            }

            //! left shift 8 bits sizeof(T)>1
            template <typename T>
            static inline void shl8( T &x, int2type<true> ) throw()
            {
                assert(sizeof(T)>1);
                (x <<= 8);
            }

            //! left shift 8 bits sizeof(T)<=1
            template <typename T>
            static inline void shl8( T &x, int2type<false> ) throw()
            {
                assert(sizeof(T)<=1);
                x=0;
            }

            //! conditional add extra to count address
            template <typename T, typename U>
            static inline void add_to( T *count, const U extra ) throw()
            {
                if(count) { *count += extra; }
            }

            //! conditional assign value to count address
            template <typename T, typename U>
            static inline void assign( T *count, const U value ) throw()
            {
                if(count) { *count = value; }
            }


        };

    }
}

#endif


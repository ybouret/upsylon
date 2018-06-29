//! \file
#ifndef Y_HASHING_HASH64_INCLUDED
#define Y_HASHING_HASH64_INCLUDED 1

#include "y/code/round.hpp"

namespace upsylon
{
    namespace hashing
    {
        //! hashing of 64 bits unsigned integers
        struct hash64
        {
            //! mix two 32 bits
            typedef void (*proc)( uint32_t *lword, uint32_t *rword );

            static void BJ( uint32_t *lword, uint32_t *rword ) throw(); //!< Bob Jenkin's
            static void NR( uint32_t *lword, uint32_t *rword ) throw(); //!< NR's

            //! mixing any integral type
            template <typename T> static inline
            T mix(const T x, hash64::proc h) throw()
            {
                assert(h);
                assert(sizeof(T)<=sizeof(uint64_t));
                union
                {
                    uint32_t u[2];
                    T        t;
                } q = { {0,0} };
                q.t = x;
                h(&q.u[0],&q.u[1]);
                return q.t;
            }

        };
    }

}


#endif


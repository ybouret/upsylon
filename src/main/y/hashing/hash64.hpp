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

            static void IBJ( uint32_t *lword, uint32_t *rword ) throw(); //!< based on Bob Jenkins's integer hash, 4-round
            static void DES( uint32_t *lword, uint32_t *rword ) throw(); //!< based on DES code

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

            //! wrapper
            template <typename T> static inline
            T mixIBJ(const T x) throw() { return mix<T>(x,IBJ); }

            //! wrapper
            template <typename T> static inline
            T mixDES(const T x) throw() { return mix<T>(x,DES); }

            //! exchange bits from mask bwtween two dwords
            template <const uint32_t MASK>
            static inline void swap_bits(uint32_t &lhs,
                                         uint32_t &rhs) throw()
            {
                const uint32_t l_bits = lhs & MASK;
                const uint32_t r_bits = rhs & MASK;
                lhs &= ~MASK;
                rhs &= ~MASK;
                lhs |= r_bits;
                rhs |= l_bits;
            }

            template <typename T> static inline
            size_t key(const      T x,
                       hash64::proc h) throw()
            {

                assert(h);
                assert(sizeof(T)<=sizeof(uint64_t));
                union
                {
                    uint32_t u[2];
                    T        t;
                    size_t   k;
                } q = { {0,0} };
                q.t = x;
                h(&q.u[0],&q.u[1]);
                return q.k;
            }

            template <typename T> static inline size_t keyIBJ(const T x ) throw() { return key<T>(x,IBJ); }
            template <typename T> static inline size_t keyDES(const T x ) throw() { return key<T>(x,DES); }

        };
    }

}


#endif


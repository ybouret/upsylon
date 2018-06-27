
// \file
#ifndef Y_TYPE_BSWAP_INCLUDED
#define Y_TYPE_BSWAP_INCLUDED 1

#include "y/type/cswap.hpp"

namespace upsylon
{
    namespace core
    {
        template <size_t N>
        inline void __bswap(void *a, void *b) throw();

        template <>
        inline void __bswap<0>(void*,void*) throw() {}

        template <>
        inline void __bswap<1>(void *a,void *b) throw()
        {
            uint8_t &A = *static_cast<uint8_t *>(a);
            uint8_t &B = *static_cast<uint8_t *>(b);
            const uint8_t temp(A);
            A = B;
            B = temp;
        }

        template <>
        inline void __bswap<2>(void *a,void *b) throw()
        {
            uint16_t &A = *static_cast<uint16_t *>(a);
            uint16_t &B = *static_cast<uint16_t *>(b);
            const uint16_t temp(A);
            A = B;
            B = temp;
        }

        template <>
        inline void __bswap<3>(void *a,void *b) throw()
        {
            uint16_t *A = static_cast<uint16_t *>(a);
            uint16_t *B = static_cast<uint16_t *>(b);
            {
                const uint16_t temp(A[0]);
                A[0] = B[0];
                B[0] = temp;
            }
            __bswap<1>(&A[1],&B[1]);
        }


        template <>
        inline void __bswap<4>(void *a,void *b) throw()
        {
            uint32_t &A = *static_cast<uint32_t *>(a);
            uint32_t &B = *static_cast<uint32_t *>(b);
            const uint32_t temp(A);
            A = B;
            B = temp;
        }

        template <>
        inline void __bswap<5>(void *a,void *b) throw()
        {
            uint32_t *A = static_cast<uint32_t *>(a);
            uint32_t *B = static_cast<uint32_t *>(b);
            {
                const uint32_t temp(A[0]);
                A[0] = B[0];
                B[0] = temp;
            }
            __bswap<1>(&A[1],&B[1]);
        }

        template <>
        inline void __bswap<6>(void *a,void *b) throw()
        {
            uint32_t *A = static_cast<uint32_t *>(a);
            uint32_t *B = static_cast<uint32_t *>(b);
            {
                const uint32_t temp(A[0]);
                A[0] = B[0];
                B[0] = temp;
            }
            __bswap<2>(&A[1],&B[1]);
        }

        template <>
        inline void __bswap<7>(void *a,void *b) throw()
        {
            uint32_t *A = static_cast<uint32_t *>(a);
            uint32_t *B = static_cast<uint32_t *>(b);
            {
                const uint32_t temp(A[0]);
                A[0] = B[0];
                B[0] = temp;
            }
            __bswap<3>(&A[1],&B[1]);
        }

        template <size_t N>
        inline void _bswap(void *a,void *b) throw()
        {
            static const size_t Q = N>>3; // quotient
            uint64_t *A = static_cast<uint64_t *>(a);
            uint64_t *B = static_cast<uint64_t *>(b);
            for(size_t i=0;i<Q;++i)
            {
                const uint64_t temp(A[i]);
                A[i] = B[i];
                B[i] = temp;
            }
            __bswap<N&7>(&A[Q],&B[Q]);
        }

    }

    template <typename T>
    inline void bswap( T &a, T &b ) throw()
    {
        core::_bswap<sizeof(T)>(&a,&b);
    }

}


#endif


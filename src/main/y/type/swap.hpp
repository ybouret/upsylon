// \file
#ifndef Y_TYPE_SWAP_INCLUDED
#define Y_TYPE_SWAP_INCLUDED 1

namespace upsylon
{
    template <typename T>
    inline void cswap( T &a, T &b )
    {
        const T tmp(a);
        a=b;
        b=tmp;
    }

    namespace core
    {
        template <size_t N>
        inline void __bswap(void *a, void *b) throw()
        {
            uint8_t *A = static_cast<uint8_t *>(a);
            uint8_t *B = static_cast<uint8_t *>(b);
            for(size_t i=0;i<N;++i)
            {
                const uint8_t temp(A[i]);
                B[i] = A[i];
                A[i] = temp;
            }
        }

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
        inline void __bswap<4>(void *a,void *b) throw()
        {
            uint32_t &A = *static_cast<uint32_t *>(a);
            uint32_t &B = *static_cast<uint32_t *>(b);
            const uint32_t temp(A);
            A = B;
            B = temp;
        }

    }
}

#endif


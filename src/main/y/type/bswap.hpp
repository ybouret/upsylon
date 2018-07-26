//! \file
#ifndef Y_TYPE_BSWAP_INCLUDED
#define Y_TYPE_BSWAP_INCLUDED 1

#include "y/type/cswap.hpp"
#include <iostream>
namespace upsylon
{
    namespace core
    {
        //! generic bytes swap
        template <size_t N>
        inline void bswap(void *a, void *b) throw()
        {
            assert(a); assert(b);
            uint8_t *p = (uint8_t *)a;
            uint8_t *q = (uint8_t *)b;
            for(size_t i=0;i<N;++i)
            {
                const uint8_t tmp = p[i];
                p[i] = q[i];
                q[i] = tmp;
            }
        }

        //! no byte swap
        template <>
        inline void bswap<0>(void *,void*) throw() {}

        //! 1 byte swap
        template <>
        inline void bswap<1>(void *a, void *b) throw()
        {
            uint8_t &p = *(uint8_t *)a;
            uint8_t &q = *(uint8_t *)b;
            const uint8_t tmp = p;
            p=q;
            q=tmp;
        }

        //! swapping 16 bits
#define Y_BSWAP16(P,Q) \
uint16_t      &p16   = *(uint16_t *)(P);\
uint16_t      &q16   = *(uint16_t *)(Q);\
const uint16_t tmp16 = p16; p16=q16; q16=tmp16

        //! 2 bytes swap
        template <>
        inline void bswap<2>(void *a, void *b) throw()
        {
            Y_BSWAP16(a,b);
        }

        //! swapping 32 bits
#define Y_BSWAP32(P,Q) \
volatile uint32_t      &p32   = *(uint32_t *)(P);\
volatile uint32_t      &q32   = *(uint32_t *)(Q);\
volatile const uint32_t tmp32 = p32; p32=q32; q32=tmp32

        //! 4 bytes swap
        template <>
        inline void bswap<4>(void *a, void *b) throw()
        {
            //std::cerr << "bswap<4>" << std::endl;
            Y_BSWAP32(a,b);
        }


        //! 6 bytes swap
        template <>
        inline void bswap<6>(void *a, void *b) throw()
        {
            uint32_t      *p = (uint32_t *)a;
            uint32_t      *q = (uint32_t *)b;
            { Y_BSWAP32(p,q);     }
            { Y_BSWAP16(p+1,q+1); }
        }


        //! 8 bytes swap
        template <>
        inline void bswap<8>(void *a, void *b) throw()
        {
            uint64_t &p = *(uint64_t *)a;
            uint64_t &q = *(uint64_t *)b;
            const uint64_t tmp = p;
            p=q;
            q=tmp;
        }


        //! local uint64_t swap
#define Y_BSWAP64(IDX) const uint64_t tmp = p[IDX]; p[IDX]=q[IDX]; q[IDX]=tmp

        //! 10 bytes swap
        template <>
        inline void bswap<10>(void *a, void *b) throw()
        {
            uint64_t *p = (uint64_t *)a;
            uint64_t *q = (uint64_t *)b;
            { Y_BSWAP64(0); }
            { Y_BSWAP16(p+1,q+1); }
        }


        //! 16 bytes swap
        template <>
        inline void bswap<16>(void *a, void *b) throw()
        {
            uint64_t *p = (uint64_t *)a;
            uint64_t *q = (uint64_t *)b;
            { Y_BSWAP64(0); }
            { Y_BSWAP64(1); }
        }

        //! 24 bytes swap
        template <>
        inline void bswap<24>(void *a, void *b) throw()
        {
            uint64_t *p = (uint64_t *)a;
            uint64_t *q = (uint64_t *)b;
            { Y_BSWAP64(0); }
            { Y_BSWAP64(1); }
            { Y_BSWAP64(2); }
        }

        //! 32 bytes swap
        template <>
        inline void bswap<32>(void *a, void *b) throw()
        {
            uint64_t *p = (uint64_t *)a;
            uint64_t *q = (uint64_t *)b;
            { Y_BSWAP64(0); }
            { Y_BSWAP64(1); }
            { Y_BSWAP64(2); }
            { Y_BSWAP64(3); }
        }

        //! 40 bytes swap
        template <>
        inline void bswap<40>(void *a, void *b) throw()
        {
            uint64_t *p = (uint64_t *)a;
            uint64_t *q = (uint64_t *)b;
            { Y_BSWAP64(0); }
            { Y_BSWAP64(1); }
            { Y_BSWAP64(2); }
            { Y_BSWAP64(3); }
            { Y_BSWAP64(4); }
        }

        //! 48 bytes swap
        template <>
        inline void bswap<48>(void *a, void *b) throw()
        {
            uint64_t *p = (uint64_t *)a;
            uint64_t *q = (uint64_t *)b;
            { Y_BSWAP64(0); }
            { Y_BSWAP64(1); }
            { Y_BSWAP64(2); }
            { Y_BSWAP64(3); }
            { Y_BSWAP64(4); }
            { Y_BSWAP64(5); }
        }

        //! 56 bytes swap
        template <>
        inline void bswap<56>(void *a, void *b) throw()
        {
            uint64_t *p = (uint64_t *)a;
            uint64_t *q = (uint64_t *)b;
            { Y_BSWAP64(0); }
            { Y_BSWAP64(1); }
            { Y_BSWAP64(2); }
            { Y_BSWAP64(3); }
            { Y_BSWAP64(4); }
            { Y_BSWAP64(5); }
            { Y_BSWAP64(6); }
        }

        //! 64 bytes swap
        template <>
        inline void bswap<64>(void *a, void *b) throw()
        {
            uint64_t *p = (uint64_t *)a;
            uint64_t *q = (uint64_t *)b;
            { Y_BSWAP64(0); }
            { Y_BSWAP64(1); }
            { Y_BSWAP64(2); }
            { Y_BSWAP64(3); }
            { Y_BSWAP64(4); }
            { Y_BSWAP64(5); }
            { Y_BSWAP64(6); }
            { Y_BSWAP64(7); }
        }

    }

    //! bytes swapping for any type
    template <typename T>
    inline void bswap( T &a, T &b ) throw()
    {
        core::bswap<sizeof(T)>(&a,&b);
    }

    //! memory reversal
    inline void mreverse( void *addr, const size_t size ) throw()
    {
        assert(!(addr==0&&size>0));
        uint8_t *p = static_cast<uint8_t*>(addr);
        uint8_t *q = static_cast<uint8_t*>(addr)+size;
        for(size_t i=(size>>1);i>0;--i)
        {
            cswap(*(p++), *(--q));
        }
    }

}


#endif


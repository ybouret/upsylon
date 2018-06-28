
// \file
#ifndef Y_TYPE_BSWAP_INCLUDED
#define Y_TYPE_BSWAP_INCLUDED 1

#include "y/type/cswap.hpp"

namespace upsylon
{
    namespace core
    {
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

        template <>
        inline void bswap<0>(void *,void*) throw() {}
        
        template <>
        inline void bswap<1>(void *a, void *b) throw()
        {
            uint8_t &p = *(uint8_t *)a;
            uint8_t &q = *(uint8_t *)b;
            const uint8_t tmp = p;
            p=q;
            q=tmp;
        }

        template <>
        inline void bswap<2>(void *a, void *b) throw()
        {
            uint16_t &p = *(uint16_t *)a;
            uint16_t &q = *(uint16_t *)b;
            const uint16_t tmp = p;
            p=q;
            q=tmp;
        }

        template <>
        inline void bswap<4>(void *a, void *b) throw()
        {
            uint32_t &p = *(uint32_t *)a;
            uint32_t &q = *(uint32_t *)b;
            const uint32_t tmp = p;
            p=q;
            q=tmp;
        }

        template <>
        inline void bswap<8>(void *a, void *b) throw()
        {
            uint64_t &p = *(uint64_t *)a;
            uint64_t &q = *(uint64_t *)b;
            const uint64_t tmp = p;
            p=q;
            q=tmp;
        }

#define Y_BSWAP64(IDX) const uint64_t tmp = p[IDX]; p[IDX]=q[IDX]; q[IDX]=tmp

        template <>
        inline void bswap<16>(void *a, void *b) throw()
        {
            uint64_t *p = (uint64_t *)a;
            uint64_t *q = (uint64_t *)b;
            { Y_BSWAP64(0); }
            { Y_BSWAP64(1); }
        }

        template <>
        inline void bswap<24>(void *a, void *b) throw()
        {
            uint64_t *p = (uint64_t *)a;
            uint64_t *q = (uint64_t *)b;
            { Y_BSWAP64(0); }
            { Y_BSWAP64(1); }
            { Y_BSWAP64(2); }
        }

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


    }

    template <typename T>
    inline void bswap( T &a, T &b ) throw()
    {
        core::bswap<sizeof(T)>(&a,&b);
    }

}


#endif


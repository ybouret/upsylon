//! \file
#ifndef Y_TYPE_BZSET_INCLUDED
#define Y_TYPE_BZSET_INCLUDED 1

#include "y/os/platform.hpp"
#include <cstring>

namespace upsylon
{
    namespace core
    {
        template <size_t N>
        inline void bzset(void *addr) throw()
        {
            memset(addr,0,N);
        }

        template <>
        inline void bzset<1>(void *addr) throw()
        {
            *(uint8_t *)addr = 0;
        }

        template <>
        inline void bzset<2>(void *addr) throw()
        {
            *(uint16_t *)addr = 0;
        }

        template <>
        inline void bzset<4>(void *addr) throw()
        {
            *(uint32_t *)addr = 0;
        }

        template <>
        inline void bzset<8>(void *addr) throw()
        {
            *(uint64_t *)addr = 0;
        }

        template <>
        inline void bzset<16>(void *addr) throw()
        {
            uint64_t *p = (uint64_t *)addr;
            p[0] = 0;
            p[1] = 0;
        }

        template <>
        inline void bzset<24>(void *addr) throw()
        {
            uint64_t *p = (uint64_t *)addr;
            p[0] = 0;
            p[1] = 0;
            p[2] = 0;
        }

        template <>
        inline void bzset<32>(void *addr) throw()
        {
            uint64_t *p = (uint64_t *)addr;
            p[0] = 0;
            p[1] = 0;
            p[2] = 0;
            p[3] = 0;
        }

    }

    template <typename T>
    inline void bzset( T &args ) throw()
    {
        core::bzset<sizeof(T)>(&args);
    }

}

#endif


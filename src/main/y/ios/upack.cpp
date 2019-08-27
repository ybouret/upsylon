
#include "y/ios/upack.hpp"
#include <cstring>

#include "y/ios/omstream.hpp"

namespace upsylon
{

    namespace ios
    {
        upack_::  upack_() throw() : size(0) {}
        upack_:: ~upack_() throw() {}


        void upack_:: clear(void *p,const size_t n) throw()
        {
            (size_t &)size = 0;
            memset(p,0,n);
        }

#define Y_IOS_UPACK_ENCODE(TYPE)                     \
static const size_t length = upack<TYPE>::requested; \
clear(addr,length);                                  \
ios::omstream fp( addr, length );                    \
(void) fp.emit_upack<TYPE>( *static_cast<const TYPE*>(u) , (size_t *)&size );


        void upack_:: encode (void       *addr,
                              const void *u,
                              int2type<1> ) throw()
        {
            Y_IOS_UPACK_ENCODE(uint8_t)
        }

        void upack_:: encode (void       *addr,
                              const void *u,
                              int2type<2> ) throw()
        {
            Y_IOS_UPACK_ENCODE(uint16_t)
        }

        void upack_:: encode (void       *addr,
                              const void *u,
                              int2type<4> ) throw()
        {
            Y_IOS_UPACK_ENCODE(uint32_t)
        }

        void upack_:: encode (void       *addr,
                              const void *u,
                              int2type<8> ) throw()
        {
            Y_IOS_UPACK_ENCODE(uint64_t)
        }

    }

}


#include "y/ios/upack.hpp"

#include "y/ios/omstream.hpp"
#include "y/ios/imstream.hpp"
#include "y/type/aliasing.hpp"
#include "y/exception.hpp"

#include <cstring>

namespace upsylon {

    namespace ios {

        upack_::  upack_() throw() : size(0) {}
        upack_:: ~upack_() throw() {}


        void upack_:: clear(const size_t sz, void *p, const size_t n) throw()
        {
            aliasing::_(size) = sz;
            memset(p,0,n);
        }

#define Y_IOS_UPACK_ENCODE(TYPE) \
ios::omstream fp(p,n);           \
aliasing::_(size) = fp.write_upack<TYPE>( *static_cast<const TYPE *>(addr) );\
assert(size<=n)

        void upack_:: encode_( Y_IOS_UPACK_ENCODE_PROTO(1) ) throw()
        {
            Y_IOS_UPACK_ENCODE(uint8_t);
        }

        void upack_:: encode_( Y_IOS_UPACK_ENCODE_PROTO(2) ) throw()
        {
            Y_IOS_UPACK_ENCODE(uint16_t);
        }

        void upack_:: encode_( Y_IOS_UPACK_ENCODE_PROTO(4) ) throw()
        {
            Y_IOS_UPACK_ENCODE(uint32_t);
        }

        void upack_:: encode_( Y_IOS_UPACK_ENCODE_PROTO(8) ) throw()
        {
            Y_IOS_UPACK_ENCODE(uint64_t);
        }

        static const char fmt[] = "upack::decode_(missing %s)";

#define Y_IOS_UPACK_DECODE(TYPE) \
ios::imstream fp(p,size);\
if( !fp.query_upack( *static_cast<TYPE*>(addr) ) ) throw exception(fmt,#TYPE)

        void upack_:: decode_( Y_IOS_UPACK_DECODE_PROTO(1) ) const
        {
            Y_IOS_UPACK_DECODE(uint8_t);
        }

        void upack_:: decode_( Y_IOS_UPACK_DECODE_PROTO(2) ) const
        {
            Y_IOS_UPACK_DECODE(uint16_t);
        }

        void upack_:: decode_( Y_IOS_UPACK_DECODE_PROTO(4) ) const
        {
            Y_IOS_UPACK_DECODE(uint32_t);
        }

        void upack_:: decode_( Y_IOS_UPACK_DECODE_PROTO(8) ) const
        {
            Y_IOS_UPACK_DECODE(uint64_t);
        }
    }

}

namespace upsylon
{

    namespace ios
    {
        upack_size:: upack_size() throw() : upack_size_type() {}

        upack_size:: ~upack_size() throw() {}

        void upack_size:: operator()(const size_t count) throw()
        {
            encode(count);
        }

        size_t upack_size:: operator()(void) const
        {
            return decode<size_t>();
        }

        upack_size & upack_size:: get() throw()
        {
            reset(); assert(requested==size);
            return *this;
        }

    }

}



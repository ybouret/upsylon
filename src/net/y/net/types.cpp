#include "y/net/types.hpp"
#include "y/type/bswap.hpp"

#include <cstring>
#include <iostream>

namespace upsylon
{

    net128_t:: net128_t() throw() : h()
    {
        memset( h, 0, sizeof(h) );
    }

    net128_t:: ~net128_t() throw()
    {
    }

    net128_t:: net128_t( const net128_t &other ) throw() : h()
    {
        memcpy( h, other.h, sizeof(h) );
    }

    net128_t & net128_t:: operator=( const net128_t &other ) throw()
    {
        if( this != &other )
        {
            memcpy( h, other.h, sizeof(h) );
        }
        return *this;
    }

    net128_t:: net128_t( const uint8_t buf[16] ) throw() :
    h()
    {
        assert( buf );
        memcpy( h, buf, sizeof(h) );
    }
    
    void net128_t:: reverse() throw()
    {
        cswap(h[0x0],h[0xF]);
        cswap(h[0x1],h[0xE]);
        cswap(h[0x2],h[0xD]);
        cswap(h[0x3],h[0xC]);
        cswap(h[0x4],h[0xB]);
        cswap(h[0x5],h[0xA]);
        cswap(h[0x6],h[0x9]);
        cswap(h[0x7],h[0x8]);
    }

    uint8_t & net128_t:: operator[](const size_t indx) throw()
    {
        assert(indx<16);
        return h[indx];
    }

    const uint8_t & net128_t:: operator[](const size_t indx) const throw()
    {
        assert(indx<16);
        return h[indx];
    }


    std::ostream & operator<< ( std::ostream &os, const net128_t &x )
    {
        for( size_t i=0; i < 16; ++i )
        {
            os << int(x.h[i]);
        }
        return os;
    }
}

namespace upsylon
{

    net_object::  net_object() throw() {}
    net_object:: ~net_object() throw() {}

}


#include "y/os/error.hpp"

#include <cstdarg>
#include <cstring>
#include <cerrno>

namespace upsylon
{

    namespace net
    {
        
        exception:: ~exception() throw()
        {
            memset(what_,0,sizeof(what_));
        }

        exception:: exception( const error_code err, const char *fmt, ... ) throw() :
        upsylon::exception(),
        code_( err ),
        what_(     )
        {
            va_list ap;
            va_start(ap,fmt);
            format(fmt,&ap);
            format_error(what_, sizeof(what_),code_);
        }

        exception:: exception( const exception &other ) throw() :
        upsylon::exception(other),
        code_( other.code_ ),
        what_()
        {
            memcpy(what_, other.what_, sizeof(what_) );
        }

        error_code exception:: code() const throw() { return code_; }

        const char * exception:: what() const throw()
        {
            return what_;
        }

    }
}



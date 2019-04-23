#include "y/net/types.hpp"

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

    std::ostream & operator<< ( std::ostream &os, const net128_t &x )
    {
        for( size_t i=0; i < 16; ++i )
        {
            os << int(x.h[i]);
        }
        return os;
    }
}

#include <cstdarg>
#include <cstring>

#if defined(Y_BSD)
#include <cerrno>
#endif

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
            memset( what_, 0, sizeof(what_) );

#if defined(Y_BSD)
            strncpy(what_, strerror(code_), sizeof(what_)-1 );
#endif

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



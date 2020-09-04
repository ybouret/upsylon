#include "y/exception.hpp"

#include <cstring>
#include <cstdarg>
#include <cstdio>

namespace upsylon
{

    
    exception:: exception() throw() : when_()
    {
        memset( when_, 0, sizeof(when_) );
    }

    exception:: exception( const exception &other ) throw() :
    std::exception(other),
    when_()
    {
        memcpy( when_, other.when_, sizeof(when_) );
    }

    exception:: ~exception() throw()
    {
        memset( when_, 0, sizeof(when_) );
    }

    void exception:: format( const char *fmt, void *va_list_ptr ) throw()
    {

        assert(fmt);
        assert(va_list_ptr);
        va_list &ap = *static_cast<va_list *>(va_list_ptr);
        vsnprintf( when_, sizeof(when_)-1, fmt, ap);
        va_end (ap);
    }

    exception:: exception( const char *fmt, ... ) throw() :
    when_()
    {
        memset( when_, 0, sizeof(when_) );
        va_list ap;
        va_start(ap, fmt);
        format( fmt, &ap );
    }

    void exception:: cat( const char *fmt,...) throw()
    {
        va_list   ap;
        va_start (ap, fmt);
        const size_t wlen = strlen( when_ );
        if( wlen < (sizeof(when_)-1) )
        {
            vsnprintf( when_+wlen,( (sizeof(when_)-1) - wlen ),fmt,ap );
        }
        va_end(ap);
    }

    void exception:: set( const char *fmt,...) throw()
    {
        memset( when_, 0, sizeof(when_) );
        va_list ap;
        va_start(ap, fmt);
        format( fmt, &ap );
    }

    void exception:: hdr( const char *fmt,...) throw()
    {
        char buff[ sizeof(when_) ];
        memcpy( buff, when_, sizeof(when_) );
        va_list   ap;
        va_start (ap, fmt);
        format( fmt, &ap );
        const size_t blen = strlen(buff);
        size_t       wlen = strlen( when_ );
        for( size_t i=0; i < blen && wlen < ( sizeof(when_) - 1 ); ++i )
        {
            when_[ wlen++ ] = buff[i];
        }

    }

    void exception:: add(const char *buffer, const size_t buflen) throw()
    {
        assert( !(NULL==buffer && buflen>0) );
        size_t pos = strlen(when_);
        if(pos<max_length)
        {
            const size_t m = max_length-pos;
            const size_t n = (m<buflen) ? m : buflen;
            for(size_t i=0;i<n;)
            {
                when_[pos++] = buffer[i++];
            }
            when_[pos] = 0;
        }

    }

    void exception:: add(const char C) throw()
    {
        add(&C,1);
    }

    void exception:: add(const char *text) throw()
    {
        if(text)
        {
            add(text,strlen(text));
        }
    }


    const char *exception:: what() const throw()
    {
        return "exception";
    }

    const char *exception:: when() const throw()
    {
        return when_;
    }

    exception & exception:: operator<<(const char  C) throw()
    {
        add(C);
        return *this;
    }

    exception & exception:: operator<<(const char *C) throw()
    {
        add(C);
        return *this;
    }

}

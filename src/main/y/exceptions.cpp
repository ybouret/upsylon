#include "y/exceptions.hpp"
#include "y/os/error.hpp"

#include <cstdarg>
#include <cstring>


namespace upsylon
{

    namespace libc
    {


        exception:: exception( int err, const char *fmt, ... ) throw() :
        upsylon::exception(),
        code_( err ),
        what_(     )
        {
            va_list ap;
            va_start(ap,fmt);
            format(fmt,&ap);
            libc::format_error( what_, sizeof(what_), code_ );
        }

        exception:: ~exception() throw()
        {
            memset( what_, 0, sizeof(what_) );
        }

        exception:: exception( const exception &other ) throw() :
        upsylon::exception( other ),
        code_( other.code_ ),
        what_( )
        {
            memcpy( what_, other.what_, sizeof(what_) );
        }

        int exception:: code() const throw()
        {
            return code_;
        }

        const char *exception:: what() const throw()
        {
            return what_;
        }

    }

    namespace win32 {

        exception:: exception( uint32_t err, const char *fmt, ... ) throw() :
        upsylon::exception(),
        code_( err ),
        what_(     )
        {
            va_list ap;
            va_start(ap,fmt);
            format(fmt,&ap);
            win32::format_error( what_, sizeof(what_), code_ );
        }

        exception:: ~exception() throw()
        {
            memset(what_,0,sizeof(what_));
        }

        exception:: exception( const exception &other ) throw() :
        upsylon::exception( other ),
        code_( other.code_ ),
        what_( )
        {
            memcpy( what_, other.what_, sizeof(what_) );
        }

        uint32_t exception:: code() const throw()
        {
            return code_;
        }

        const char *exception:: what() const throw()
        {
            return what_;
        }

    }


    namespace imported {

        exception:: exception( const char *reason, const char *fmt,...) throw() :
        upsylon::exception(),
        what_()
        {
            memset( what_, 0, sizeof(what_) );
            if( reason )
                strncpy( what_, reason, sizeof(what_)-1 );
            va_list ap;
            va_start(ap,fmt);
            format(fmt,&ap);
        }

        exception:: ~exception() throw()
        {
            memset( what_, 0, sizeof(what_) );
        }

        exception:: exception( const exception &other ) throw() :
        upsylon::exception( other ),
        what_()
        {
            memcpy( what_, other.what_, sizeof(what_) );
        }

        const char *exception:: what() const throw()
        {
            return what_;
        }

        exception & exception:: operator=( const upsylon::exception &other ) throw()
        {
            if( this != &other )
            {

                memset( what_, 0, sizeof(what_) );
                strncpy( what_, other.what(), sizeof(what_)-1 );
                set( "%s", other.when() );
            }
            return *this;
        }

        exception & exception:: operator=( const exception &other ) throw()
        {
            if( this != &other )
            {
                memcpy( what_, other.what_, sizeof(what_) );
                memcpy( when_, other.when_, sizeof(when_) );
            }
            return *this;
        }

    }


}

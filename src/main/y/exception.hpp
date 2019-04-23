//! \file
#ifndef Y_EXCEPTION_INCLUDED
#define Y_EXCEPTION_INCLUDED 1

#include "y/os/platform.hpp"
#include "y/os/printf-check.hpp"
#include <exception>

namespace upsylon
{
    //! base class for exception
    class exception : public std::exception
    {
    public:
        //! aligned internal space
        static const size_t max_length = 512-sizeof(std::exception);


        virtual ~exception() throw();                //!< destructor
        exception( const exception &other ) throw(); //!< copy constructor
        virtual const char *what() const throw(); //!< overrides std::exception::what()
        const char         *when() const throw(); //!< exception context

        //! format when using printf syntax
        explicit exception( const char *fmt,...) throw() Y_PRINTF_CHECK(2,3);
        
        //! append to when using printf syntax
        void     cat( const char *fmt,...)       throw() Y_PRINTF_CHECK(2,3);
        
        //! reset when using printf syntax
        void     set( const char *fmt,...)       throw() Y_PRINTF_CHECK(2,3);
        
        //! prepend to when using printf syntax
        void     hdr( const char *fmt,...)       throw() Y_PRINTF_CHECK(2,3);

    protected:
        explicit exception() throw(); //!< ""
        void     format( const char *fmt, void *va_list_ptr) throw(); //!< common formatting
        char     when_[ max_length ]; //!< user's formatted string

    private:
        Y_DISABLE_ASSIGN(exception);

    };
}

#endif


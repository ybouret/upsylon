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
        static const size_t max_length = 120;

        virtual ~exception() throw();
        exception( const exception &other ) throw();

        virtual const char *what() const throw(); //!< overrides std::exception::what()
        const char *when() const throw();         //!< exception context

        explicit exception( const char *fmt,...) throw() Y_PRINTF_CHECK(2,3); //!< format when
        void     cat( const char *fmt,...)       throw() Y_PRINTF_CHECK(2,3); //!< append to when
        void     set( const char *fmt,...)       throw() Y_PRINTF_CHECK(2,3); //!< reset when
        void     hdr( const char *fmt,...)       throw() Y_PRINTF_CHECK(2,3); //!< prepend to when

    protected:
        explicit exception() throw();
        void     format( const char *fmt, void *va_list_ptr) throw();
        char     when_[ max_length ];

    private:
        Y_DISABLE_ASSIGN(exception);

    };
}

#endif


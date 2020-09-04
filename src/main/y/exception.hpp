//! \file
#ifndef Y_EXCEPTION_INCLUDED
#define Y_EXCEPTION_INCLUDED 1

#include "y/os/platform.hpp"
#include "y/os/printf-check.hpp"
#include <exception>

namespace upsylon
{
    //__________________________________________________________________________
    //
    //
    //! base class for exception
    //
    //__________________________________________________________________________
    class exception : public std::exception
    {
    public:
        //! aligned internal space
        static const size_t alignement = 512-sizeof(std::exception); //!< alignment size
        static const size_t max_length = alignement-1;               //!< max message length
        
        explicit exception() throw();                //!< setup empty
        exception( const exception &other ) throw(); //!< copy constructor
        virtual            ~exception()  throw();    //!< destructor
        virtual const char *what() const throw();    //!< overrides std::exception::what()
        const char         *when() const throw();    //!< exception context

        //! format when using printf syntax
        explicit exception(const char *fmt,...) throw() Y_PRINTF_CHECK(2,3);
        
        //! append to when using printf syntax
        void     cat(const char *fmt,...)       throw() Y_PRINTF_CHECK(2,3);
        
        //! reset when using printf syntax
        void     set(const char *fmt,...)       throw() Y_PRINTF_CHECK(2,3);
        
        //! prepend to when using printf syntax
        void     hdr(const char *fmt,...)       throw() Y_PRINTF_CHECK(2,3);


        void add(const char *, const size_t) throw();  //!< add some chars
        void add(const char C) throw();                //!< add one char
        void add(const char *text) throw();            //!< add text
        exception & operator<<(const char  C) throw(); //!< add(C)
        exception & operator<<(const char *C) throw(); //!< add(C)


    protected:
        void     format(const char *, void *) throw(); //!< common formatting
        char     when_[ alignement ];                  //!< user's formatted string

    private:
        Y_DISABLE_ASSIGN(exception);

        
    };
}

#endif


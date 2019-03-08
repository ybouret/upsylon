//! \file
#ifndef Y_CODE_UTILS_INCLUDED
#define Y_CODE_UTILS_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{

    //! return "s" if count>1
    inline const char * plural_s(const size_t count) throw()
    {
        return (count<=1) ? "" : "s";
    }

    //! hexadecimal helpers
    struct hexadecimal
    {
        static const char *lowercase_word[16]; //!< "0".."f"
        static const char *uppercase_word[16]; //!< "0".."F"
        static const char *lowercase[256];     //!< "00".."ff"
        static const char *uppercase[256];     //!< "00".."FF"

        //! internal formatting of an address to a C-string
        static const char *address( const void *addr ) throw();

        //! return the decimal value, -1 on error
        static int  to_decimal(const char h) throw();
        //! get the xdigit from last fourbits
        static char digit(const unsigned fourBits);
    };

    //! convert a byte into a visible char
    extern const char *visible_char[256];

    //! convert a byte into a printable char
    extern const char *printable_char[256];


}

#endif


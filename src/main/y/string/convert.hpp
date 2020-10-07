//! \file
#ifndef Y_STRING_CONVERT_INCLUDED
#define Y_STRING_CONVERT_INCLUDED 1

#include "y/string.hpp"

namespace upsylon
{
    //! string to numeric
    struct string_convert
    {
        //! size_t (decimal/hexadecimal),unit_t,float,double \todo complexes?
        template <typename T> static
        T to( const string &data, const char *name=NULL);

        //! wrapper
        template <typename T> static inline
        T to( const char   *text, const char *name=NULL)
        {
            const string data(text); return to<T>(data,name);
        }

        static void   to_lower( string &s );         //!< each char tolower
        static void   to_upper( string &s );         //!< each char toupper
        static string to_visible(const string &s);   //!< each char to visible
        static string to_printable(const string &s); //!< each char to printable
        static string to_encoded(const string &s);   //!< each char to encoded

    };
}

#endif


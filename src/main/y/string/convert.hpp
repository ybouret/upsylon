//! \file
#ifndef Y_STRING_CONVERT_INCLUDED
#define Y_STRING_CONVERT_INCLUDED 1

#include "y/string.hpp"

namespace upsylon
{

    //! inline wrapper for C-strings
#define Y_STRING_CONVERT_(METHOD)                          \
/**/    static inline string to_##METHOD(const char *text) \
/**/    {                                                  \
/**/        const string _(text); return to_##METHOD(_);   \
/**/    }

    //! string to numeric/other
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

        static void   make_lower( string &s );       //!< each char tolower
        static void   make_upper( string &s );       //!< each char toupper

        static string to_lower(const string &s);     //!< lowercase
        static string to_upper(const string &s);     //!< uppercase
        static string to_visible(const string &s);   //!< each char to visible
        static string to_printable(const string &s); //!< each char to printable
        static string to_encoded(const string &s);   //!< each char to encoded

        Y_STRING_CONVERT_(lower)
        Y_STRING_CONVERT_(upper)
        Y_STRING_CONVERT_(visible)
        Y_STRING_CONVERT_(printable)
        Y_STRING_CONVERT_(encoded)




    };
}

#endif


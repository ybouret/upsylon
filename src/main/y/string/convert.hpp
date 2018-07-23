//! \file
#ifndef Y_STRING_CONVERT_INCLUDED
#define Y_STRING_CONVERT_INCLUDED 1

#include "y/string.hpp"

namespace upsylon
{
    //! string to numeric
    struct string_convert
    {
        //! size_t,unit_t,float,double \todo complexes?
        template <typename T> static
        T to( const string &data, const char *name=NULL);
        template <typename T> static inline

        //! wrapper
        T to( const char   *text, const char *name=NULL)
        {
            const string data(text); return to<T>(data,name);
        }
    };
}

#endif


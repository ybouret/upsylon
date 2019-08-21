//!\file

#ifndef Y_STRFWD_INCLUDED
#define Y_STRFWD_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    namespace core
    {
        template <typename T> class string; //!< forward declaration
    }
    typedef core::string<char> string;      //!< forward declaration

    typedef type2type<string> as_string_t;  //!< type information selector
    extern const as_string_t  as_string;    //!< external instance
}

#endif



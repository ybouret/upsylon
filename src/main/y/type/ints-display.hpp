//! \file
#ifndef Y_INTS_DISPLAY_INCLUDED
#define Y_INTS_DISPLAY_INCLUDED 1

#include "y/type/ints.hpp"
#include <iosfwd>

namespace upsylon
{

    namespace core
    {
        //! display [un]singed integer
        std::ostream & display_any_int(std::ostream &os,
                                       const void  *addr,
                                       const size_t size,
                                       const bool   sgnd);

        //! wrapper to display any int
        template <typename T>
        inline std::ostream & display_int(std::ostream &os, const T x)
        {
            return display_any_int(os, &x, sizeof(T), is_signed<T>::value );
        }

    }

}


#endif


//! \file

#ifndef Y_UTEST_SIZEOF_INCLUDED
#define Y_UTEST_SIZEOF_INCLUDED 1

#include "y/os/platform.hpp"
#include <iostream>


namespace upsylon {

    //! helpers to nicely display sizeof(something)
    struct __sizeof
    {
        static size_t         width;//!< default aligment width

        //! display aligned info
        static std::ostream & display( std::ostream &os, const char *type, const size_t value);
    };

    //! display aligned sizeof(TYPE)
#define Y_UTEST_SIZEOF(TYPE)          do { upsylon::__sizeof::display( std::cerr, #TYPE, sizeof(TYPE) ) << std::endl; } while(false)

    //! display aligned sizeof(TYPE) with PFX before
#define Y_UTEST_SIZEOF_WITH(PFX,TYPE) do { upsylon::__sizeof::display( std::cerr << PFX, #TYPE, sizeof(TYPE) ) << std::endl; } while(false)

}

#endif

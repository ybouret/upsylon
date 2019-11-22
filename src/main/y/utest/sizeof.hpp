//! \file

#ifndef Y_UTEST_SIZEOF_INCLUDED
#define Y_UTEST_SIZEOF_INCLUDED 1

#include "y/os/platform.hpp"
#include <iostream>


namespace upsylon {

    struct __sizeof
    {
        static size_t width;
        static std::ostream & display( std::ostream &os, const char *type, const size_t value);
    };

#define Y_UTEST_SIZEOF(TYPE)          do { upsylon::__sizeof::display( std::cerr, #TYPE, sizeof(TYPE) ) << std::endl; } while(false)
#define Y_UTEST_SIZEOF_WITH(PFX,TYPE) do { upsylon::__sizeof::display( std::cerr << PFX, #TYPE, sizeof(TYPE) ) << std::endl; } while(false)

}

#endif

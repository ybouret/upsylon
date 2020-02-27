//! \file

#ifndef Y_INFORMATION_BASE64_INCLUDED
#define Y_INFORMATION_BASE64_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    namespace Information {

        struct Base64
        {
            static const char  padding = '=';
            static const char  std_table[64];
            static const char  url_table[64];
            static const short inv_table[256];

            static void Encode3to4(void *target, const void *source, const char *table) throw();

        };

    }

}

#endif

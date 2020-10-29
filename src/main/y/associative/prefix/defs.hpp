
//! \file
#ifndef Y_PREFIX_DEFS_INCLUDED
#define Y_PREFIX_DEFS_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    struct prefix_
    {
    public:
        static const char *byte_to_text(const uint8_t &) throw();

        template <typename T> static inline const char *code_to_text(const T &x) throw()
        {
            const union {
                T       _;
                uint8_t b[ sizeof(T) ];
            } alias = { x };
            clr();
            for(size_t i=0;i<sizeof(T);++i) cat( byte_to_text(alias.b[i]));
            return text;
        }

    private:
        static char text[];
        static void clr() throw();
        static void cat(const char *) throw();
    };

}

#endif

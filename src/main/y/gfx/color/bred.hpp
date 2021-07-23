//! \file

#ifndef Y_GFX_COLOR_BRED_INCLUDED
#define Y_GFX_COLOR_BRED_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    namespace graphic
    {

        class byte_divider
        {
        public:
            byte_divider(const uint8_t *, const size_t) throw();
            byte_divider(const byte_divider &) throw();
            ~byte_divider() throw();
            
            const uint8_t *value;
            const size_t   count;

            static const byte_divider table[256];
        private:
            Y_DISABLE_ASSIGN(byte_divider);
        };

    }

}

#endif

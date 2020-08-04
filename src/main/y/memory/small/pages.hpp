//! \file

#ifndef Y_MEMORY_SMALL_PAGES_INCLUDED
#define Y_MEMORY_SMALL_PAGES_INCLUDED 1

#include "y/code/ilog2.hpp"

namespace upsylon {

    namespace memory {

        namespace small {

            struct page
            {
                page *next;
                page *prev;
            };

            class pages
            {
            public:
                static const size_t one           = 1;
                static const size_t min_page_iln2 = ilog2_of<page>::value;
                static const size_t min_page_size = one << min_page_iln2;

            private:

            };


        }

    }

}

#endif

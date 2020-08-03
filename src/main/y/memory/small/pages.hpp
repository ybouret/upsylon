
//! \file

#ifndef Y_MEMORY_SMALL_PAGES_INCLUDED
#define Y_MEMORY_SMALL_PAGES_INCLUDED 1

 #include "y/memory/small/piece.hpp"

namespace upsylon {

    namespace memory {

        namespace small {


            class pages
            {
            public:
                pages(const size_t usr_chunk_size) throw();
                ~pages() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(pages);
            };

        }

    }

}

#endif

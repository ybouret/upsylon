
//! \file

#ifndef Y_MEMORY_SMALL_STONES_INCLUDED
#define Y_MEMORY_SMALL_STONES_INCLUDED 1

#include "y/memory/small/stone.hpp"
#include "y/code/ilog2.hpp"
#include "y/core/list.hpp"

namespace upsylon {

    namespace memory {

        namespace small {


            class stones
            {
            public:
                typedef core::list_of<stone> slist_type;
                static const size_t one      = 1;
                static const size_t min_shift = ilog2_of<stone>::value;
                static const size_t max_shift = (sizeof(size_t)<<3)-1;

                const size_t shift;
                const size_t bytes;
                slist_type   slist;

                stones(const size_t usr_shift) throw();
                ~stones() throw();

                //! get/create a stone, memory may be garbage
                void *query();

                //! store a previous stone, keep memory ordered
                void  store(void *) throw();
                void  release(stone *) const throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(stones);
            };

        }
    }
}
#endif

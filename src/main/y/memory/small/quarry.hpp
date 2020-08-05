
//! \file

#ifndef Y_MEMORY_SMALL_QUARRY_INCLUDED
#define Y_MEMORY_SMALL_QUARRY_INCLUDED 1

#include "y/memory/small/stones.hpp"
#include "y/code/round.hpp"

namespace upsylon {

    namespace memory {

        namespace small {


            class quarry
            {
            public:
                static const size_t max_stones = stones::max_shift+1;
                static const size_t wksp_bytes = max_stones * sizeof(stones);

                explicit quarry() throw();
                virtual ~quarry() throw();

                void *pull(const size_t shift);
                void  push(void *, const size_t shift) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(quarry);
                stones  *ore;
                uint64_t wksp[ Y_U64_FOR_SIZE(wksp_bytes) ];

            };

        }

    }

}

#endif

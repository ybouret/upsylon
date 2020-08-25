//! \file

#ifndef Y_MEMORY_TIGHT_TWAIN_ALLOCATOR_INCLUDED
#define Y_MEMORY_TIGHT_TWAIN_ALLOCATOR_INCLUDED 1

#include "y/memory/tight/arena.hpp"
#include "y/memory/tight/quarry.hpp"
#include "y/lockable.hpp"

namespace upsylon
{
    namespace memory
    {
        namespace tight
        {

            class twain_allocator
            {
            public:

                explicit twain_allocator(lockable    &sync,
                                         const size_t usr_chunk_size,
                                         const size_t usr_limit_size);

                virtual ~twain_allocator() throw();

                const size_t limit_size;
                const size_t limit_exp2;
                const size_t num_arenas;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(twain_allocator);
                lockable &L;
                quarry    Q;
                void     *A;
                void     *Z;
                char        *workspace;
                const size_t work_exp2;

                void release_A(size_t) throw();
                void release_W() throw();
                void release_Z() throw();
            };

        }
    }
}


#endif

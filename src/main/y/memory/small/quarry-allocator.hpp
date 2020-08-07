
//! \file

#ifndef Y_MEMORY_SMALL_QUARRY_ALLOCATOR_INCLUDED
#define Y_MEMORY_SMALL_QUARRY_ALLOCATOR_INCLUDED 1

#include "y/lockable.hpp"

namespace upsylon {

    namespace memory {

        namespace small {

            class quarry;

            class quarry_allocator
            {
            public:
                explicit quarry_allocator(lockable &usr_access,
                                          quarry   &usr_quarry) throw();

                virtual ~quarry_allocator() throw();

                void *acquire(size_t &bytes, size_t &shift);
                void  release(void *&addr, size_t &bytes, size_t &shift) throw();

            private:
                lockable &Access;
                quarry   &Quarry;

                Y_DISABLE_COPY_AND_ASSIGN(quarry_allocator);
                void *fetch_locked(const size_t);
            };

        }

    }
}

#endif

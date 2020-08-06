
//! \file

#ifndef Y_MEMORY_SMALL_OBJECTS_INCLUDED
#define Y_MEMORY_SMALL_OBJECTS_INCLUDED 1

#include "y/memory/small/blocks.hpp"
#include "y/lockable.hpp"

namespace upsylon {

    namespace memory {

        namespace small {


            class objects
            {
            public:
                explicit objects(lockable &sync, const size_t chunk_size, const size_t limit_size);
                virtual ~objects() throw();

                lockable &Access;
                quarry    Quarry;
                blocks    Blocks;

                void *acquire(const size_t block_size);
                void  release(void *addr, const size_t block_size) throw();
                void *global_acquire(const size_t global_bytes);
                void  global_release(void *addr, const size_t global_bytes) throw();


            private:
                Y_DISABLE_COPY_AND_ASSIGN(objects);

            };

        }

    }

}
#endif

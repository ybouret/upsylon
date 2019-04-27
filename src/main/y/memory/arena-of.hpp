//! \file
#ifndef Y_MEMORY_ARENA_OF_INCLUDED
#define Y_MEMORY_ARENA_OF_INCLUDED 1

#include "y/memory/arena.hpp"

namespace upsylon
{
    namespace memory
    {

        template <typename T>
        class arena_of
        {
        public:
            static const size_t BLOCK_SIZE = sizeof(T);

            inline  arena_of(const size_t the_chunk_size) throw() : A(BLOCK_SIZE,the_chunk_size) {}
            inline ~arena_of() throw() {}

            inline T   *acquire()               { return static_cast<T*>(A.acquire()); }
            inline void release( T *p ) throw() { A.release(p); }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(arena_of);
            arena A;
        };

    }
}

#endif




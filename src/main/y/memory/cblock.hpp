//! \file
#ifndef Y_MEMORY_CBLOCK_INCLUDED
#define Y_MEMORY_CBLOCK_INCLUDED 1

#include "y/memory/global.hpp"

namespace upsylon
{
    namespace memory
    {
        template <typename T>
        class cblock_of
        {
        public:
            const size_t size;
            const size_t bytes;
            T           *data;
            inline explicit cblock_of(const size_t n) :
            size(n),
            bytes(0),
            data( global::instance().acquire_as<T>((size_t&)size,(size_t&)bytes))
            {
            }

            inline virtual ~cblock_of() throw()
            {
                global::location().release_as<T>(data,(size_t&)size,(size_t&)bytes);
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(cblock_of);
        };
    }

}

#endif


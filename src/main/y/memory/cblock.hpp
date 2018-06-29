//! \file
#ifndef Y_MEMORY_CBLOCK_INCLUDED
#define Y_MEMORY_CBLOCK_INCLUDED 1

#include "y/memory/global.hpp"

namespace upsylon
{
    namespace memory
    {
        //! C-style block of data
        template <typename T>
        class cblock_of
        {
        public:
            const size_t size;  //!< number of items
            const size_t bytes; //!< allocated bytes
            T           *data;  //!< data[0..size-1]

            //! acquire all
            inline explicit cblock_of(const size_t n) :
            size(n),
            bytes(0),
            data( global::instance().acquire_as<T>((size_t&)size,(size_t&)bytes))
            {
            }

            //! release all
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


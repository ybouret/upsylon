//! \file
#ifndef Y_MEMORY_CBLOCK_INCLUDED
#define Y_MEMORY_CBLOCK_INCLUDED 1

#include "y/memory/global.hpp"
#include "y/type/args.hpp"

namespace upsylon
{
    namespace memory
    {
        //! C-style block of data
        template <typename T, typename ALLOCATOR = global >
        class cblock_of
        {
        public:
            Y_DECL_ARGS(T,type); //!< aliases
            const size_t size;   //!< number of items
            const size_t bytes;  //!< allocated bytes
            T           *data;   //!< data[0..size-1]

            //! acquire all
            inline explicit cblock_of(const size_t n) :
            size(n),
            bytes(0),
            data( ALLOCATOR::instance().template acquire_as<T>((size_t&)size,(size_t&)bytes))
            {
            }

            //! release all
            inline virtual ~cblock_of() throw()
            {
                ALLOCATOR::location().template release_as<T>(data,(size_t&)size,(size_t&)bytes);
            }

            //! access [0..size-1]
            inline type       & operator[](size_t indx) throw()       { assert(indx<size); return data[indx]; }
            //! const access[0..size-1]
            inline const type & operator[](size_t indx) const throw() { assert(indx<size); return data[indx]; }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(cblock_of);
        };
    }

}

#endif


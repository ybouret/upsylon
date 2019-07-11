//! \file
#ifndef Y_MEMORY_CPPBLOCK_INCLUDED
#define Y_MEMORY_CPPBLOCK_INCLUDED 1

#include "y/memory/global.hpp"
#include "y/type/args.hpp"
#include <cstring>

namespace upsylon
{
    namespace memory
    {
        //! C++-style block of data
        template <typename T, typename ALLOCATOR = global >
        class cppblock_of
        {
        public:
            Y_DECL_ARGS(T,type); //!< aliases
            const size_t size;   //!< number of items
            const size_t bytes;  //!< allocated bytes
        private:
            mutable_type *wksp;  //!< for memory
        public:
            T           *data;   //!< data[1..size]

            //! acquire all
            inline explicit cppblock_of(const size_t n) :
            size(n),
            bytes(0),
            wksp(ALLOCATOR::instance().template acquire_as<mutable_type>((size_t&)size,(size_t&)bytes) ),
            data(wksp-1)
            {

            }

            //! release all
            inline virtual ~cppblock_of() throw()
            {
                clear();
                ALLOCATOR::location().template release_as<mutable_type>(wksp,(size_t&)size,(size_t&)bytes);
                data = 0;
            }

            //! access [1..size]
            inline type       & operator[](size_t indx) throw()       { assert(indx>0); assert(indx<=size); return data[indx]; }

            //! const access[1..size]
            inline const type & operator[](size_t indx) const throw() { assert(indx>0); assert(indx<=size); return data[indx]; }

            //! reset memory content
            inline void clear() throw()
            {
                memset((void*)wksp,0,bytes);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(cppblock_of);
        };
        
    }

}

#endif


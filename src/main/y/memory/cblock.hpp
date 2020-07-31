//! \file
#ifndef Y_MEMORY_CBLOCK_INCLUDED
#define Y_MEMORY_CBLOCK_INCLUDED 1

#include "y/memory/allocator/global.hpp"
#include "y/type/args.hpp"
#include "y/type/aliasing.hpp"
#include <cstring>

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
        private:
            mutable_type *wksp; //!< for memory
        public:
            T           *data;   //!< data[0..size-1]

            //! acquire all
            inline explicit cblock_of(const size_t n) :
            size(n),
            bytes(0),
            wksp( ALLOCATOR::instance().
                 template acquire_as<mutable_type>(aliasing::_(size),aliasing::_(bytes))),
            data(wksp)
            {
            }

            //! release all
            inline virtual ~cblock_of() throw()
            {
                clear();
                ALLOCATOR::location().
                template release_as<mutable_type>(wksp,aliasing::_(size),aliasing::_(bytes));
                data = 0;
            }

            //! access [0..size-1]
            inline type       & operator[](size_t indx) throw()       { assert(indx<size); return data[indx]; }
            //! const access[0..size-1]
            inline const type & operator[](size_t indx) const throw() { assert(indx<size); return data[indx]; }

            //! clear memory
            inline void clear() throw()
            {
                memset((void*)wksp,0,bytes);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(cblock_of);
        };
        
    }

}

#endif


//! \file
#ifndef Y_MEMORY_CPPBLOCK_INCLUDED
#define Y_MEMORY_CPPBLOCK_INCLUDED 1

#include "y/memory/allocator/global.hpp"
#include "y/type/args.hpp"
#include "y/type/aliasing.hpp"
#include "y/sequence/addressable.hpp"
#include <cstring>

namespace upsylon
{
    namespace memory
    {
        //! C++-style block of data
        template <typename T, typename ALLOCATOR = global >
        class cppblock_of : public addressable<T>
        {
        public:
            Y_DECL_ARGS(T,type); //!< aliases
            const size_t size_;   //!< number of items
            const size_t bytes;  //!< allocated bytes
        private:
            mutable_type *wksp;  //!< for memory
        public:
            T           *data;   //!< data[1..size]

            //! acquire all
            inline explicit cppblock_of(const size_t n) :
            addressable<T>(),
            size_(n),
            bytes(0),
            wksp(ALLOCATOR::instance().template acquire_as<mutable_type>(aliasing::_(size_),aliasing::_(bytes) ) ),
            data(wksp-1)
            {

            }

            //! release all
            inline virtual ~cppblock_of() throw()
            {
                clear();
                ALLOCATOR::location().template release_as<mutable_type>(wksp,aliasing::_(size_),aliasing::_(bytes));
                data = 0;
            }

            inline virtual size_t size() const throw()
            {
                return size_;
            }

            //! access [1..size]
            inline virtual type       & operator[](const size_t indx) throw()       { assert(indx>0); assert(indx<=size_); return data[indx]; }

            //! const access[1..size]
            inline virtual const_type & operator[](const size_t indx) const throw() { assert(indx>0); assert(indx<=size_); return data[indx]; }

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


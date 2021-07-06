//! \file

#ifndef Y_MEMORY_BUFFER_OF_INCLUDED
#define Y_MEMORY_BUFFER_OF_INCLUDED 1

#include "y/type/args.hpp"
#include "y/type/aliasing.hpp"
#include "y/sequence/addressable.hpp"
#include "y/memory/allocator.hpp"

namespace upsylon
{
    namespace memory
    {

#define Y_MEMORY_BUFFER_OF_CTOR(N)  \
items(N),                           \
bytes(items*sizeof(T)),             \
hmem(ALLOCATOR::instance()),        \
addr( static_cast<mutable_type*>( hmem.acquire( aliasing::_(bytes) ) )-1 )

        template <typename T, typename ALLOCATOR>
        class buffer_of : public addressable<T>
        {
        public:
            Y_DECL_ARGS(T,type);

            inline explicit buffer_of(const size_t n) :
            Y_MEMORY_BUFFER_OF_CTOR(n)
            {
            }

            inline virtual ~buffer_of() throw()
            {
                hmem.release(*(void**)&addr,aliasing::_(bytes));
                aliasing::_(items)=0;
            }


            inline size_t allocated() const throw() { return bytes; }

            inline virtual size_t size() const throw() { return items; }
            
            inline virtual type  &operator[](const size_t indx) throw()
            {
                assert(indx>0);
                assert(indx<=size());
                return addr[indx];
            }

            inline virtual const_type & operator[](const size_t indx) const throw()
            {
                assert(indx>0);
                assert(indx<=size());
                return addr[indx];
            }


        private:
            const size_t  items;
            const size_t  bytes;
            ALLOCATOR    &hmem;
            mutable_type *addr;

            Y_DISABLE_ASSIGN(buffer_of);
        };

    }
}

#endif


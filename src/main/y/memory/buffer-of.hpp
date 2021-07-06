//! \file

#ifndef Y_MEMORY_BUFFER_OF_INCLUDED
#define Y_MEMORY_BUFFER_OF_INCLUDED 1

#include "y/type/args.hpp"
#include "y/type/aliasing.hpp"
#include "y/sequence/addressable.hpp"
#include "y/memory/allocator.hpp"
#include "y/memory/buffer.hpp"

#include <cstring>

namespace upsylon
{
    namespace memory
    {

#define Y_MEMORY_BUFFER_OF_CTOR(N)        \
items(N),                                 \
bytes(items*sizeof(T)),                   \
inMem(bytes),                             \
hmem(ALLOCATOR::instance()),              \
addr(hmem.acquire( aliasing::_(inMem) )), \
item(static_cast<mutable_type*>(addr)-1)

        template <typename T, typename ALLOCATOR>
        class buffer_of : public addressable<T>, public memory::ro_buffer
        {
        public:
            Y_DECL_ARGS(T,type);

            inline buffer_of(const size_t n) :
            Y_MEMORY_BUFFER_OF_CTOR(n)
            {
            }

            inline buffer_of(const buffer_of &other) :
            Y_MEMORY_BUFFER_OF_CTOR(other.items)
            {
                memcpy(addr,other.addr,bytes);
            }

            inline virtual ~buffer_of() throw()
            {
                hmem.release(*(void**)&addr,aliasing::_(inMem));
                aliasing::_(items)=0;
                aliasing::_(bytes)=0;
            }


            inline size_t allocated() const throw() { return inMem; }

            inline virtual size_t size() const throw() { return items; }
            
            inline virtual type  &operator[](const size_t indx) throw()
            {
                assert(indx>0);
                assert(indx<=size());
                return item[indx];
            }

            inline virtual const_type & operator[](const size_t indx) const throw()
            {
                assert(indx>0);
                assert(indx<=size());
                return item[indx];
            }

            inline virtual const void * ro()     const throw() { return addr;  }
            inline virtual size_t       length() const throw() { return bytes; }


        private:
            const size_t  items;
            const size_t  bytes;
            const size_t  inMem;
            ALLOCATOR    &hmem;
            void         *addr;
            mutable_type *item;

            Y_DISABLE_ASSIGN(buffer_of);
        };

    }
}

#endif


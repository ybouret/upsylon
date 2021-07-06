//! \file

#ifndef Y_MEMORY_CBUFFER_INCLUDED
#define Y_MEMORY_CBUFFER_INCLUDED 1

#include "y/memory/allocator.hpp"
#include "y/type/args.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    namespace memory
    {

        template <typename T, typename ALLOCATOR>
        class cbuffer
        {
        public:
            Y_DECL_ARGS(T,type);

            inline explicit cbuffer(const size_t n) :
            size(n),
            allocated(size*sizeof(T)),
            hmem( ALLOCATOR::instance() ),
            addr( static_cast<mutable_type*>( hmem.acquire( aliasing::_(allocated) ) ) )
            {
            }

            inline virtual ~cbuffer() throw()
            {
                hmem.release(*(void**)&addr,aliasing::_(allocated));
                aliasing::_(size)=0;
            }

            const size_t  size;
            const size_t  allocated;

        private:
            ALLOCATOR    &hmem;
            mutable_type *addr;

            Y_DISABLE_ASSIGN(cbuffer);
        };

    }
}

#endif


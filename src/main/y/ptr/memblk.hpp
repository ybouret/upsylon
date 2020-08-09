
//! \file
#ifndef Y_PTR_MEMBLK_INCLUDED
#define Y_PTR_MEMBLK_INCLUDED 1

#include "y/ptr/ptr.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    
    template <typename T, typename ALLOCATOR = memory::global >
    class memblk : public ptr<T>
    {
    public:
        Y_DECL_ARGS(T,type);
        
        virtual ~memblk() throw()
        {
            static ALLOCATOR &mgr = ALLOCATOR::location();
            mgr.release( *(void **)& (this->pointee), aliasing::_(bytes) );
            aliasing::_(count) = 0;
        }
        
        explicit memblk(const size_t required) :
        ptr<T>(0),
        count(required?required:1),
        bytes( count*sizeof(T) )
        {
            this->pointee = static_cast<type *>(ALLOCATOR::instance().acquire( aliasing::_(bytes) ) );
        }
        
        
        const size_t  count;
        const size_t  bytes;
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(memblk);
    
    };
}

#endif


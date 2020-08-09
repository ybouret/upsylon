
//! \file
#ifndef Y_PTR_CPPBLOCK_INCLUDED
#define Y_PTR_CPPBLOCK_INCLUDED 1

#include "y/ptr/memblk.hpp"
#include <cstring>

namespace upsylon
{
    
    template <typename T, typename ALLOCATOR = memory::global >
    class cblock : public memblk<T,ALLOCATOR>
    {
    public:
        Y_DECL_ARGS(T,type);
        typedef memblk<T,ALLOCATOR> memblk_type;
        
        virtual ~cblock() throw()
        {
            
        }
        
        explicit cblock(const size_t required) :
        memblk_type(required)
        {
        }
        
        inline type & operator[](const size_t indx) throw()
        {
            assert(indx<this->count);
            return this->pointee[indx];
        }
        
        
        inline const_type & operator[](const size_t indx) const throw()
        {
            assert(indx<this->count);
            return this->pointee[indx];
        }
        
        inline void clear() throw()
        {
            memset(this->pointee,0,this->bytes);
        }
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(cblock);
        
    };
}

#endif

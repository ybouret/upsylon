
//! \file
#ifndef Y_SORTED_VECTOR_INCLUDED
#define Y_SORTED_VECTOR_INCLUDED 1

#include "y/container/ordered.hpp"

namespace upsylon
{
#define Y_SORTED_VECTOR(N)

    template <typename T, typename ALLOCATOR = memory::global>
    class sorted_vector //: public ordered<T>
    {
    public:
        Y_DECL_ARGS(T,type);

        inline virtual ~sorted_vector() throw() { release__(); }

        inline sorted_vector() throw() : size_(0), maxi_(0), bytes(0), hmem( ALLOCATOR::instance() ), addr( hmem.acquire_as<mutable_type>(maxi_,bytes) ), item( addr-1 ) {}

        inline virtual size_t  size()     const throw() { return size_; }
        inline virtual size_t  capacity() const throw() { return maxi_; }
        
        inline virtual void    free()    throw() { free__(); }
        inline virtual void    release() throw() { release__(); }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(sorted_vector);
        size_t             size_;
        size_t             maxi_;
        size_t             bytes;
        memory::allocator &hmem;
        mutable_type      *addr;
        mutable_type      *item;

        inline void free__() throw()
        {
            while(this->size_>0)
            {
                self_destruct(addr[--(this->size_)]);
            }
        }

        inline void release__() throw()
        {
            free__();
            hmem.release_as(addr,maxi_,bytes);
            item = 0;
        }
    };
}

#endif


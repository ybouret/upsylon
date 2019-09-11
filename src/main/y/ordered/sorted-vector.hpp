
//! \file
#ifndef Y_SORTED_VECTOR_INCLUDED
#define Y_SORTED_VECTOR_INCLUDED 1

#include "y/container/ordered.hpp"
#include "y/comparator.hpp"

namespace upsylon
{

#define Y_SORTED_VECTOR(N) \
size_(0), maxi_(N), bytes(0), hmem( ALLOCATOR::instance() ), addr( hmem.acquire_as<mutable_type>(maxi_,bytes) ), item(addr-1)

    template <typename T,
    typename COMPARATOR = increasing_comparator<T>,
    typename ALLOCATOR = memory::global>
    class sorted_vector : public ordered<T>
    {
    public:
        Y_DECL_ARGS(T,type);

        inline virtual ~sorted_vector() throw() { release__(); }

        inline explicit sorted_vector() throw() :
        size_(0), maxi_(0), bytes(0), hmem( ALLOCATOR::instance() ), addr(0), item(addr-1)
        {}

        inline explicit sorted_vector(const size_t n, const as_capacity_t &) throw() : Y_SORTED_VECTOR(n) {}

        inline  sorted_vector( const sorted_vector &other ) : Y_SORTED_VECTOR( other.size_ ) { duplicate(other); }
        inline  sorted_vector( const sorted_vector &other, const size_t extra ) : Y_SORTED_VECTOR( other.size_+extra ) { duplicate(other); }

        // dynamic interface
        inline virtual size_t  size()     const throw() { return size_; }
        inline virtual size_t  capacity() const throw() { return maxi_; }

        // container interface
        inline virtual void    free()    throw() { free__();    }
        inline virtual void    release() throw() { release__(); }
        inline virtual void    reserve(const size_t n)
        {
            if(n>0)
            {
                sorted_vector temp( *this, n );
                swap_with(temp);
            }
        }

        //! no throw swap
        inline void swap_with( sorted_vector &_ ) throw()
        {
            cswap(size_,_.size_);
            cswap(maxi_,_.maxi_);
            cswap(bytes,_.bytes);
            cswap(addr,_.addr);
            cswap(item,_.item);
        }

    private:
        Y_DISABLE_ASSIGN(sorted_vector);
        size_t             size_;
        size_t             maxi_;
        size_t             bytes;
        memory::allocator &hmem;
        mutable_type      *addr;
        mutable_type      *item;
        COMPARATOR         compare;
        
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

        inline void duplicate( const sorted_vector &other )
        {
            assert(0==size_);
            assert(maxi_>=other.size_);
            while(size_<other.size_)
            {
                new ( &addr[size_] ) mutable_type( other.addr[size_] );
                ++size_;
            }
        }
    };
}

#endif


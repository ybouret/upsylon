#ifndef Y_VECTOR_INCLUDED
#define Y_VECTOR_INCLUDED 1

#include "y/container/sequence.hpp"
#include "y/sequence/array.hpp"

namespace upsylon
{
    template <typename T, typename ALLOCATOR = memory::global >
    class vector : public sequence<T>, public array<T>
    {
    public:
        Y_DECL_ARGS(T,type); //!< aliases

        inline vector() : sequence<T>(), array<T>(), maxi_(0), bytes(0), hmem_( ALLOCATOR::instance() ),addr_(0) {}
        inline virtual ~vector() throw()
        {
            __release();
        }

        inline vector(const size_t n, const as_capacity_t &) :
        sequence<T>(),
        array<T>(),
        maxi_(n),
        bytes(0),
        hmem_( ALLOCATOR::instance() ),
        addr_( hmem_.acquire_as<mutable_type>(maxi_,bytes) )
        {
            this->item_ = addr_-1;
        }



        //! dynamic interface: capacity
        inline virtual size_t capacity() const throw() { return maxi_; }

        //! container interface: free
        virtual void free() throw()
        {
        }

        //! container interface: release
        virtual void release() throw()
        {
        }

        //! container interface: reserve
        virtual void reserve(const size_t)
        {

        }

        //! sequence interface : push_back
        virtual void push_back( param_type )
        {
        }

        //! sequence interface : pop_front
        virtual void push_front( param_type )
        {
        }

        inline void swap_with( vector &other ) throw()
        {
            cswap(this->item_,other.item_);
            cswap(this->size_,other.size_);
            cswap(this->addr_,other.addr_);
            cswap(this->maxi_,other.maxi_);
            cswap(this->bytes,other.bytes);
        }

    private:
        size_t             maxi_;
        size_t             bytes;
        memory::allocator &hmem_;
        mutable_type      *addr_;

        inline void __release() throw()
        {
            hmem_.release_as(this->addr_,maxi_,bytes);
        }
    };

}
#endif


#ifndef Y_VECTOR_INCLUDED
#define Y_VECTOR_INCLUDED 1

#include "y/container/sequence.hpp"
#include "y/sequence/array.hpp"

namespace upsylon
{
    //! vector of memory movable objects...
    template <typename T, typename ALLOCATOR = memory::global >
    class vector : public sequence<T>, public array<T>
    {
    public:
        Y_DECL_ARGS(T,type); //!< aliases

        //! initialize fields
#define Y_VECTOR_CTOR(n) \
sequence<T>(), array<T>(), maxi_(n), bytes(0), hmem_( ALLOCATOR::instance() ),addr_( hmem_.acquire_as<mutable_type>(maxi_,bytes) )

        inline vector() : Y_VECTOR_CTOR(0) {}

        inline virtual ~vector() throw()
        {
            __release();
        }

        //! vector with empty memory
        inline vector(const size_t n, const as_capacity_t &) : Y_VECTOR_CTOR(n)
        {
            this->item_ = addr_-1;
        }

        //! vector with default memory
        inline vector(const size_t n) : Y_VECTOR_CTOR(n)
        {
            this->item_ = addr_-1;
            try
            {
                while(this->size_<n)
                {
                    new (addr_+this->size_) T();
                    ++(this->size_);
                }
            }
            catch(...)
            {
                __release();
                throw;
            }
        }

        //! dynamic interface: capacity
        inline virtual size_t capacity() const throw() { return maxi_; }

        //! container interface: free
        virtual void free() throw()
        {
            __free();
        }

        //! container interface: release
        virtual void release() throw()
        {
            __release();
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

        inline void __free() throw()
        {
            while(this->size_>0)
            {
                destruct(&addr_[--(this->size_)]);
            }
        }

        inline void __release() throw()
        {
            __free();
            hmem_.release_as(this->addr_,maxi_,bytes);
        }
    };

}
#endif


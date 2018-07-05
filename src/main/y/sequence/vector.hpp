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

        inline vector() : sequence<T>(), array<T>(), addr_(0), maxi_(0), bytes(0), hmem_( ALLOCATOR::instance() ) {}
        inline virtual ~vector() throw()
        {
            __release();
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


    private:
        mutable_type      *addr_;
        size_t             maxi_;
        size_t             bytes;
        memory::allocator &hmem_;

        inline void __release() throw()
        {
            hmem_.release_as(this->addr_,maxi_,bytes);
        }
    };

}
#endif


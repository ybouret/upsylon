//! \file
#ifndef Y_VECTOR_INCLUDED
#define Y_VECTOR_INCLUDED 1

#include "y/container/sequence.hpp"
#include "y/sequence/array.hpp"
#include "y/iterate/linear.hpp"
#include <cstring>

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

        //! copy constructor
        vector(const vector &other) : Y_VECTOR_CTOR(other.size_)
        {
            this->item_ = addr_-1;
            try
            {
                while(this->size_<other.size_)
                {
                    new (addr_+this->size_) T( other.addr[this->size_] );
                    ++(this->size_);
                }
            }
            catch(...)
            {
                __release();
                throw;
            }
        }

        //! assign another vector
        vector & operator=( const vector &other )
        {
            if(this != &other)
            {
                vector tmp(other);
                swap_with(tmp);
            }
            return *this;
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
        virtual void reserve(const size_t n)
        {
            if(n>0)
            {
                vector tmp(maxi_+n,as_capacity);
                memcpy((void*)(tmp.addr_),(void*)addr_,(tmp.size_=this->size_)*sizeof(T));
                this->size_ = 0;
                swap_with(tmp);
            }
        }

        //! push back with enough memory
        inline void push_back_(param_type args)
        {
            assert(this->has_space());
            // try to build in place
            new (addr_+this->size_) T(args);
            ++(this->size_);
        }

        //! sequence interface : push_back
        virtual void push_back( param_type args )
        {
            if(this->is_filled())
            {
                vector tmp( container::next_capacity(maxi_), as_capacity );
                // will keep memory in any case
                memcpy( (void*)(tmp.addr_), (void*)addr_,this->size_*sizeof(T));
                try
                {
                    new (tmp.addr_+this->size_) T(args); // put into position
                    tmp.size_ = this->size_ + 1;         // success
                    this->size_ = 0;
                    swap_with(tmp);
                }
                catch(...)
                {
                    tmp.size_   = this->size_; // failure
                    this->size_ = 0;
                    swap_with(tmp);
                    throw;
                }
            }
            else
            {
                push_back_(args);
            }
        }

        //! sequence interface : pop_front
        virtual void push_front( param_type args)
        {
            const size_t len = this->size_ * sizeof(T);
            if(this->is_filled())
            {
                vector tmp( container::next_capacity(maxi_), as_capacity );
                try
                {
                    new(tmp.addr_) T(args);
                    memcpy( (void*)(tmp.addr_+1), (void*)addr_,len);
                    tmp.size_   = this->size_ + 1;
                    this->size_ = 0;
                    swap_with(tmp);
                }
                catch(...)
                {
                    // keep memory
                    memcpy( (void*) tmp.addr_, (void *)addr_, len );
                    tmp.size_   = this->size_;
                    this->size_ = 0;
                    swap_with(tmp);
                    throw;
                }
            }
            else
            {
                memmove((void*)(addr_+1), (void*)(addr_), len);
                try
                {
                    new (addr_) T(args);
                }
                catch(...)
                {
                    memmove((void*)addr_,(void *)(addr_+1),len);
                    throw;
                }
                ++(this->size_);
            }

        }

        //! swap all descriptive data, array included
        inline void swap_with( vector &other ) throw()
        {
            cswap(this->item_,other.item_);
            cswap(this->size_,other.size_);
            cswap(this->addr_,other.addr_);
            cswap(this->maxi_,other.maxi_);
            cswap(this->bytes,other.bytes);
        }

        typedef iterate::linear<type,iterate::forward>        iterator;        //!< forward iterator
        typedef iterate::linear<const_type,iterate::forward>  const_iterator;  //!< forward iterator, const

        inline iterator begin() throw() { return iterator(addr_);             } //!< begin forward
        inline iterator end()   throw() { return iterator(addr_+this->size_); } //!< end forward

        inline const_iterator begin() const throw() { return const_iterator(addr_);             } //!< begin forward const
        inline const_iterator end()   const throw() { return const_iterator(addr_+this->size_); } //!< end forward const

        typedef iterate::linear<type,iterate::reverse>        reverse_iterator;        //!< reverse iterator
        typedef iterate::linear<const_type,iterate::reverse>  const_reverse_iterator;  //!< reverse iterator, const

        inline reverse_iterator rbegin() throw() { return reverse_iterator(this->item_+this->size_); } //!< begin reverse
        inline reverse_iterator rend()   throw() { return reverse_iterator(this->item_);             } //!< end reverse

        inline const_reverse_iterator rbegin() const throw() { return const_reverse_iterator(this->item_+this->size_); } //!< begin reverse const
        inline const_reverse_iterator rend()   const throw() { return const_reverse_iterator(this->item_);             } //!< end reverse const
        
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


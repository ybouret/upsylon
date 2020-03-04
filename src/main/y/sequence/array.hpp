//! \file
#ifndef Y_SEQUENCE_ARRAY_INCLUDED
#define Y_SEQUENCE_ARRAY_INCLUDED 1

#include "y/sequence/addressable.hpp"
#include "y/type/memops.hpp"
#include "y/memory/buffer.hpp"
#include "y/memory/io.hpp"
#include <iostream>

namespace upsylon
{
    //! array of contiguous objects
    template <typename T>
    class array : public memory::ro_buffer, public virtual addressable<T>
    {
    public:
        Y_DECL_ARGS(T,type); //!< aliases

        //======================================================================
        //
        //
        // virtual interfaces
        //
        //
        //======================================================================

        //! destructor
        inline virtual ~array() throw()
        {
            assert(0==item_);
            assert(0==size_);
        }
        
        //! cluster interface: access
        inline virtual type       & operator[](const size_t i)       throw() { assert(i>0);assert(i<=this->size()); assert(item_); return item_[i]; }

        //! cluster interface: access
        inline virtual const_type & operator[](const size_t i) const throw() { assert(i>0);assert(i<=this->size()); assert(item_); return item_[i]; }


        //! buffer interface: read only access
        inline virtual const void *ro() const throw() { return (size_>0) ? &item_[1] : 0; }

        //! buffer interface : length in bytes
        inline virtual size_t length() const throw() { return size_ * sizeof(T); }


        //======================================================================
        //
        //
        // specific code
        //
        //
        //======================================================================

        //! content
        inline type * operator*() throw() { return (size_>0) ? &item_[1] : 0; }

        //! const content
        inline const_type * operator*() const throw() { return (size_>0) ? &item_[1] : 0; }


        //! load same value
        inline void ld( param_type a )
        {
            for(size_t i=size_;i>0;--i)
            {
                item_[i] = a;
            }
        }

        //! reverse
        inline void reverse() throw()
        {
            size_t lo=1;
            size_t up=size_;
            for(size_t i=size_>>1;i>0;--i)
            {
                memops::swap(item_[lo++],item_[up--]);
            }
        }


    protected:
        mutable_type *item_; //!< item_[1..size()]
        size_t        size_; //!< size()

        //! default initialisation
        inline array() throw() :  item_(0), size_(0) {}

        //! default C-style init
        inline array(type *p, const size_t n) throw() :
        item_( memory::io::__prev( (mutable_type *)p) ),
        size_( n )
        {
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(array);
    };

    //! a simple wrapper
    template <typename T>
    class lightweight_array : public array<T>
    {
    public:
        //! default empty array
        inline explicit lightweight_array() throw() : collection(), array<T>()  {}

        //! C++ array on a C array
        inline explicit lightweight_array(T *p,const size_t n) throw() : collection(), array<T>(p,n) { assert(!(p==NULL&&n>0)); }

        //! destructor
        inline virtual ~lightweight_array() throw() { this->item_=0; this->size_=0; }

        //! dynamic interface: size
        inline virtual size_t size() const throw() { return this->size_; }

        //! copy by passing parameters
        inline lightweight_array( const lightweight_array &other ) throw() :
        collection(), addressable<T>(), array<T>(other.item_,other.size_)
        {
            
        }
        
    private:
        Y_DISABLE_ASSIGN(lightweight_array);
    };

}

#endif


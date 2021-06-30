
//! \file
#ifndef Y_SEQUENCE_CRATES_INCLUDED
#define Y_SEQUENCE_CRATES_INCLUDED 1

#include "y/container/sequence.hpp"
#include "y/type/collapse.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/aliasing.hpp"
#include <iostream>

namespace upsylon
{

    namespace core
    {
        //! common functions for crates
        struct crates
        {
            static void reserve_failure(); //!< throw exception
        };
    }

    //! sequence of fixed memory size
    template <typename T, typename ALLOCATOR = memory::global >
    class crates : public sequence<T>
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        Y_DECL_ARGS(T,type); //!< aliases

        //______________________________________________________________________
        //
        // interface : dynamic
        //______________________________________________________________________

        //! setup for n objects
        inline explicit crates(const size_t n) :
        count(n),
        bytes( n * sizeof(T) ),
        entry( allocate() ),
        shift( entry-1    ),
        size_(0)
        {
        }

        inline virtual ~crates() throw()
        {
            static ALLOCATOR &mgr = ALLOCATOR::location();
            free();
            mgr.release( *(void **)&entry, aliasing::_(bytes) );
            shift=0;
        }

        //______________________________________________________________________
        //
        // interface: addressable
        //______________________________________________________________________

        //! constructed objects
        inline virtual size_t size() const throw() { return size_; }

        //! indx in [1:size()]
        inline virtual type & operator[](const size_t indx) throw()
        {
            assert(indx>0);assert(indx<=size_); return shift[indx];
        }

        //! indx in [1:size()]
        virtual const_type & operator[](const size_t indx) const throw()
        {
            assert(indx>0);assert(indx<=size_); return shift[indx];
        }

        //______________________________________________________________________
        //
        // interface : dynamic
        //______________________________________________________________________

        //! intial capacity
        inline virtual size_t capacity() const throw() { return count; }

        //! free
        inline virtual void   free()           throw()
        {
            while(size_>0) zcollapse(entry[--size_]);
        }

        //! release would just free!
        virtual void release() throw() { free(); }

        //______________________________________________________________________
        //
        // interface : sequence
        //______________________________________________________________________
        //! front
        inline virtual type       & front()       throw() { assert(size_>0); return *entry; }

        //! front, const
        inline virtual const_type & front() const throw() { assert(size_>0); return *entry; }

        //! front
        inline virtual type       & back()       throw() { assert(size_>0); return shift[size_]; }

        //! front, const
        inline virtual const_type & back() const throw() { assert(size_>0); return shift[size_]; }

        //! failure!
        inline virtual void reserve(const size_t ) { core::crates::reserve_failure(); }

        //! pop back
        inline virtual void pop_back() throw()
        {
            assert(size_>0);
            zcollapse(entry[--size_]);
        }

        //! pop front
        inline virtual void pop_front() throw()
        {
            assert(size_>0);
            collapse(entry[0]);
            const size_t block_size = (--size_)*sizeof(T);
            out_of_reach::move(entry,entry+1,block_size);
        }

        //! push back
        inline virtual void push_back(param_type obj)
        {
            assert(size_<count);
            new (entry+size_) mutable_type(obj);
            ++size_;
        }

        //! push front
        inline virtual void push_front(param_type obj)
        {
            const size_t  block_size = size_ * sizeof(T);
            mutable_type *target     = entry+1;
            out_of_reach::move(target,entry,block_size);
            try
            {
                new (entry) mutable_type(obj);
                ++size_;
            }
            catch(...)
            {
                out_of_reach::move(entry,target,block_size);
                throw;
            }
        }

        //! adjust
        inline virtual void adjust(const size_t n, param_type obj)
        {
            assert(n<=count);
            while(size_>n) { zcollapse(entry[--size_]); }
            while(size_<n) { push_back(obj);            }
        }

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! access
        inline type * operator*() throw() { return entry; }

        //! access, const
        inline const_type * operator*() const throw() { return entry; }

        //! build emtpy object
        inline void build(void) { assert(this->has_space()); new (entry+size_) T(); ++size_; }

        //! build empty objects
        inline void vbuild(size_t n) { while(n-- > 0) build(); }

        //! build with 1 arg
        template <typename U>
        inline void build( typename type_traits<U>::parameter_type args )
        {
            assert(this->has_space()); new (entry+size_) T(args); ++size_;
        }

        //! build with 2 args
        template <typename U,typename V>
        inline void build(typename type_traits<U>::parameter_type argU,
                          typename type_traits<V>::parameter_type argV)
        {
            assert(this->has_space()); new (entry+size_) T(argU,argV); ++size_;
        }

        //! build with 3 args
        template <typename U,typename V,typename W>
        inline void build(typename type_traits<U>::parameter_type argU,
                          typename type_traits<V>::parameter_type argV,
                          typename type_traits<W>::parameter_type argW)
        {
            assert(this->has_space()); new (entry+size_) T(argU,argV,argW); ++size_;
        }

        //! build with 4 args
        template <typename U,typename V,typename W,typename X>
        inline void build(typename type_traits<U>::parameter_type argU,
                          typename type_traits<V>::parameter_type argV,
                          typename type_traits<W>::parameter_type argW,
                          typename type_traits<X>::parameter_type argX)
        {
            assert(this->has_space()); new (entry+size_) T(argU,argV,argW,argX); ++size_;
        }

        //! build with 5 args
        template <typename U,typename V,typename W,typename X,typename Y>
        inline void build(typename type_traits<U>::parameter_type argU,
                          typename type_traits<V>::parameter_type argV,
                          typename type_traits<W>::parameter_type argW,
                          typename type_traits<X>::parameter_type argX,
                          typename type_traits<Y>::parameter_type argY)
        {
            assert(this->has_space()); new (entry+size_) T(argU,argV,argW,argX,argY); ++size_;
        }

        //! display
        inline friend std::ostream & operator<<( std::ostream &os, const crates &s )
        {
            os << '<';
            for(size_t i=0;i<s.size_;++i)
            {
                if(i>0) os << ':';
                os << s.entry[i];
            }
            os << '>';
            return os;
        }



        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const size_t  count; //!< capacity
        const size_t  bytes; //!< allocated

    private:
        mutable_type *entry; //!< first item
        mutable_type *shift; //!< shifted
        size_t        size_; //!< internal counter

    private:
        Y_DISABLE_COPY_AND_ASSIGN(crates);
        inline mutable_type *allocate()
        {
            static ALLOCATOR &mgr = ALLOCATOR::instance();
            return static_cast<mutable_type *>( mgr.acquire( aliasing::_(bytes) ) );
        }

    };
    
}

#endif

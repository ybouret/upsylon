//! \file
#ifndef Y_SEQUENCE_SLOTS_INCLUDED
#define Y_SEQUENCE_SLOTS_INCLUDED 1

#include "y/dynamic.hpp"
 #include "y/type/args.hpp"
#include "y/type/self-destruct.hpp"
#include "y/memory/global.hpp"
#include <iostream>

namespace upsylon {

    //! C-style slots of [0..size()-1] C++ objects
    template <typename T, typename ALLOCATOR=memory::global>
    class slots :   public dynamic
    {
    public:
        Y_DECL_ARGS(T,type); //!< type aliases

        //! prepare n slots
        inline explicit slots(size_t n) :
        size_(0),
        count(n),
        bytes(n*sizeof(T)),
        addr( static_cast<mutable_type *>(ALLOCATOR::instance().acquire(bytes)) )
        {
        }

        //! release all
        inline virtual ~slots() throw()
        {
            release();
        }

        inline slots( const slots &other) :
        dynamic(),
        size_(0),
        count(other.count),
        bytes(count*sizeof(T)),
        addr( static_cast<mutable_type *>(ALLOCATOR::instance().acquire(bytes)) )
        {
            try
            {
                for(size_t i=0;i<other.size();++i)
                {
                    this->build<const_type&>( other[i] );
                }
            }
            catch(...)
            {
                release();
                throw;
            }
        }

        //! dynamic interface: initial and constant count
        inline virtual size_t capacity() const throw() { return count; }

        //! dynamic interface: current built slots
        inline virtual size_t size()     const throw() { return size_; }

        //! access
        inline   type       & operator[](size_t indx) throw()       { assert(indx<size_); assert(addr); return addr[indx]; }

        //! access, CONST
        inline   const_type & operator[](size_t indx) const throw() { assert(indx<size_); assert(addr); return addr[indx]; }

        //! push a new object using copy ctor
        inline void push( param_type args )
        {
            assert(this->has_space()); new (addr+size_) T(args); ++size_;

        }

        //! build emtpy object
        inline void build(void) { assert(this->has_space()); new (addr+size_) T(); ++size_; }

        //! build with 1 arg
        template <typename U>
        inline void build( typename type_traits<U>::parameter_type args )
        {
            assert(this->has_space()); new (addr+size_) T(args); ++size_;
        }

        //! build with 2 args
        template <typename U,typename V>
        inline void build(typename type_traits<U>::parameter_type argU,
                          typename type_traits<V>::parameter_type argV)
        {
            assert(this->has_space()); new (addr+size_) T(argU,argV); ++size_;
        }

        //! build with 3 args
        template <typename U,typename V,typename W>
        inline void build(typename type_traits<U>::parameter_type argU,
                          typename type_traits<V>::parameter_type argV,
                          typename type_traits<W>::parameter_type argW)
        {
            assert(this->has_space()); new (addr+size_) T(argU,argV,argW); ++size_;
        }

        //! build with 4 args
        template <typename U,typename V,typename W,typename X>
        inline void build(typename type_traits<U>::parameter_type argU,
                          typename type_traits<V>::parameter_type argV,
                          typename type_traits<W>::parameter_type argW,
                          typename type_traits<X>::parameter_type argX)
        {
            assert(this->has_space()); new (addr+size_) T(argU,argV,argW,argX); ++size_;
        }

        //! build with 5 args
        template <typename U,typename V,typename W,typename X,typename Y>
        inline void build(typename type_traits<U>::parameter_type argU,
                          typename type_traits<V>::parameter_type argV,
                          typename type_traits<W>::parameter_type argW,
                          typename type_traits<X>::parameter_type argX,
                          typename type_traits<Y>::parameter_type argY)
        {
            assert(this->has_space()); new (addr+size_) T(argU,argV,argW,argX,argY); ++size_;
        }

        //! display
        inline friend std::ostream & operator<<( std::ostream &os, const slots &s )
        {
            os << '<';
            for(size_t i=0;i<s.size_;++i)
            {
                if(i>0) os << ':';
                os << s.addr[i];
            }
            os << '>';
            return os;
        }

    private:
        Y_DISABLE_ASSIGN(slots);
        size_t        size_;
    public:
        const size_t  count; //!< constant capacity
    private:
        size_t        bytes;
        mutable_type *addr;
        inline void release() throw()
        {
            while(size_>0)
            {
                self_destruct( addr[--size_] );
            }
            void *entry = addr;
            ALLOCATOR::location().release(entry,bytes);
        }
    };
}

#endif


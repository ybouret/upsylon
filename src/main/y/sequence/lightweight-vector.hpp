//! \file

#ifndef Y_LW_VECTOR_INCLUDED
#define Y_LW_VECTOR_INCLUDED 1

#include "y/container/sequence.hpp"
#include "y/sequence/array.hpp"

namespace upsylon {

    namespace core {

        //! common ops for lightweight_vector
        struct lw_vector
        {
            static void throw_on_memory_ops(); //!< when not allowed!
        };

    }

    //! fixed decorating vector
    template <typename T >
    class lightweight_vector :  public sequence<T>, public lightweight_array<T>
    {
    public:
        Y_DECL_ARGS(T,type);                       //!< aliases
        typedef lightweight_array<type> base_type; //!< alias

        //! cleanup
        inline virtual ~lightweight_vector() throw() {}

        //! setup
        inline explicit lightweight_vector(type *p, const size_t n) throw() :
        base_type(p,n)
        {
        }

        //! sequence : capacity
        inline virtual size_t capacity() const throw() { return this->size(); }


        inline virtual void free()      throw() { } //!< do nothing
        inline virtual void release()   throw() { } //!< do nothing
        inline virtual void pop_back()  throw() {}  //!< do nothing
        inline virtual void pop_front() throw() {}  //!< do nothing

        inline void reserve(size_t)                 { core::lw_vector:: throw_on_memory_ops(); } //!< error
        inline void push_back(param_type)           { core::lw_vector:: throw_on_memory_ops(); } //!< error
        inline void push_front(param_type)          { core::lw_vector:: throw_on_memory_ops(); } //!< error
        inline void adjust(const size_t,param_type) { core::lw_vector:: throw_on_memory_ops(); } //!< error

        //! sequence: front
        inline virtual type & front() throw()
        {
            addressable<type> &self = *this;
            assert( self.size() > 0 );
            return self[1];
        }

        //! sequence : const front
        inline virtual const_type & front() const throw()
        {
            const accessible<type> &self = *this;
            assert( self.size() > 0 );
            return self[1];
        }

        //! sequence: back
        inline virtual type & back() throw()
        {
            addressable<type> &self = *this;
            assert( self.size() > 0 );
            return self[ self.size() ];
        }

        //! sequence: const back
        inline virtual const_type & back() const throw()
        {
            const accessible<type> &self = *this;
            assert( self.size() > 0 );
            return self[ self.size() ];
        }

    private:
        Y_DISABLE_ASSIGN(lightweight_vector);
    };

}
#endif


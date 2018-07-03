//! \file
#ifndef Y_PTR_INCLUDED
#define Y_PTR_INCLUDED 1

#include "y/type/args.hpp"

namespace upsylon
{

    //! base class for smart pointers
    template <typename T>
    class ptr
    {
    public:
        Y_DECL_ARGS(T);
        typedef mutable_type *pointee_type;
        
        inline virtual ~ptr() throw() { pointee=0; }
        
        inline type       * operator->() throw()       { assert(pointee); return  pointee; }
        inline const_type * operator->() const throw() { assert(pointee); return  pointee; }
        inline type       & operator*() throw()        { assert(pointee); return *pointee; }
        inline const_type & operator*() const throw()  { assert(pointee); return *pointee; }
        
        inline type *       __get() throw()       { return pointee; }
        inline const_type * __get() const throw() { return pointee; }
        
    protected:
        inline explicit ptr(T *addr) throw() : pointee( (pointee_type)addr ) {}
        inline ptr(const ptr &other) throw() : pointee( other.pointee ) {}
        pointee_type pointee;
        inline void swap_with( ptr<T> &other ) throw()
        {
            cswap(pointee,other.pointee);
        }
    private:
        Y_DISABLE_ASSIGN(ptr);
    };
    
}

#endif


//! \file
#ifndef Y_PTR_ARC_INCLUDED
#define Y_PTR_ARC_INCLUDED 1

#include "y/ptr/ptr.hpp"

namespace upsylon
{
    //! A Reference Counted
    /**
     T must have the following methods
     - void   withhold() throw()
     - bool   liberate() throw()
     - size_t refcount() const throw()
     */
    template <typename T>
    class arc_ptr : public ptr<T>
    {
    public:
        Y_DECL_ARGS(T);
        inline arc_ptr(T *addr) throw() : ptr<T>(addr)
        {
            assert(this->pointee||die("not accepting null address"));
            this->pointee->withhold();
        }
        
        inline arc_ptr(const arc_ptr &other) throw() :
        ptr<T>(other)
        {
            this->pointee->withhold();
        }
        
        inline arc_ptr & operator=(const arc_ptr &other) throw()
        {
            arc_ptr tmp(other);
            this->swap_with(tmp);
            return *this;
        }
        
        inline virtual ~arc_ptr() throw()
        {
            if(this->pointee->liberate())
            {
                delete (this->pointee);
            }
        }
        
    };
}

#endif




//! \file
#ifndef Y_PTR_SHARED_INCLUDED
#define Y_PTR_SHARED_INCLUDED 1

#include "y/ptr/ptr.hpp"
#include "y/object.hpp"

namespace upsylon
{
    template <typename T>
    class shared_ptr : public ptr<T>
    {
    public:
        inline shared_ptr(T*addr) : ptr<T>(addr), nref(0)
        {
            assert(this->pointee);
            try
            {
                *( nref = object::acquire1<size_t>() ) = 1;
            } catch(...) { delete this->pointee; throw; }
        }
        
        inline shared_ptr( const shared_ptr &other ) throw() :
        ptr<T>(other),
        nref(other.nref)
        {
            assert(this->pointee);
            assert(nref);
            assert((*nref>0));
            ++(*nref);
        }
        
        inline shared_ptr & operator=( const shared_ptr &other) throw()
        {
            shared_ptr tmp(other);
            this->swap_with(tmp);
            cswap(nref,tmp.nref);
            return *this;
        }
        
        inline virtual ~shared_ptr() throw()
        {
            assert(nref);
            assert(*nref>0);
            if( --(*nref)<=0 )
            {
                object::release1(nref);
                delete this->pointee;
            }
        }
        
        inline size_t refcount() const throw()
        {
            assert(nref); return *nref;
        }
        
    private:
        size_t *nref;
        
    };
}

#endif


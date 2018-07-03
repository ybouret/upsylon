//! \file
#ifndef Y_COUNTED_INCLUDED
#define Y_COUNTED_INCLUDED 1

#include "y/object.hpp"

namespace upsylon
{
    //! a self standing counted class for arc_ptr
    class counted
    {
    public:
        inline virtual ~counted() throw() { assert(nref_==0); }
        
        inline void   withhold() throw() { ++nref_; }
        inline bool   liberate() throw() { assert(nref_>0); return (--nref_ <= 0); }
        inline size_t refcount() const throw() { return nref_; }
        
        inline explicit counted() throw() : nref_(0) {}
        
    private:
        size_t nref_;
        Y_DISABLE_COPY_AND_ASSIGN(counted);
    };
    
    class counted_object : public counted, public virtual object
    {
    public:
        inline virtual ~counted_object() throw() {}
        inline explicit counted_object() throw() {}
    private:
        Y_DISABLE_COPY_AND_ASSIGN(counted_object);
    };
    
}

#endif

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
        //! destructor
        inline virtual ~counted() throw() { assert(nref_==0); }

        //! increase refcount
        inline void   withhold() throw() { ++nref_; }
        //! decrease refcount, return true if drops to 0
        inline bool   liberate() throw() { assert(nref_>0); return (--nref_ <= 0); }
        //! return refcount
        inline size_t refcount() const throw() { return nref_; }
        
        inline explicit counted() throw() : nref_(0) {}
        
    private:
        size_t nref_;
        Y_DISABLE_COPY_AND_ASSIGN(counted);
    };

    //! a counted object...
    class counted_object :  public virtual object, public counted
    {
    public:
        //! destructor
        inline virtual ~counted_object() throw() {}
        //! constructor
        inline explicit counted_object() throw() {}
    private:
        Y_DISABLE_COPY_AND_ASSIGN(counted_object);
    };
    
}

#endif

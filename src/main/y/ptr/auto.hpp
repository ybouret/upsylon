//! \file
#ifndef Y_POINTER_AUTO_INCLUDED
#define Y_POINTER_AUTO_INCLUDED 1

#include "y/ptr/ptr.hpp"

namespace upsylon
{
    //! automatic pointer for resources management
    template <typename T>
    class auto_ptr : public ptr<T>
    {
    public:
        //! attach any resource
        inline auto_ptr( T *addr ) throw() : ptr<T>(addr) {}

        //! delete any resource
        inline ~auto_ptr() throw() { if(this->pointee) delete this->pointee; }

        
    };
}

#endif


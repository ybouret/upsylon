
//! \file
#ifndef Y_PTR_CBLOCK_INCLUDED
#define Y_PTR_CBLOCK_INCLUDED 1

#include "y/ptr/memblk.hpp"
#include <cstring>

namespace upsylon
{

    //__________________________________________________________________________
    //
    //
    //! local "C" memory
    //
    //__________________________________________________________________________
    template <typename T, typename ALLOCATOR = memory::global >
    class cblock : public memblk<T,ALLOCATOR>
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        Y_DECL_ARGS(T,type);                     //!< aliases
        typedef memblk<T,ALLOCATOR> memblk_type; //!< base type

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        //! cleanup
        inline virtual ~cblock() throw() {}

        //! setup
        inline explicit cblock(const size_t required) : memblk_type(required) {}

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________

        //! access in [0..count-1]
        inline type & operator[](const size_t indx) throw()
        {
            assert(indx<this->count);
            return this->pointee[indx];
        }
        
        //! access in [0..count-1]
        inline const_type & operator[](const size_t indx) const throw()
        {
            assert(indx<this->count);
            return this->pointee[indx];
        }
        

    private:
        Y_DISABLE_COPY_AND_ASSIGN(cblock);
        
    };
}

#endif
